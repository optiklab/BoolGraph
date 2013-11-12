// BoolGraphDoc.cpp : implementation of the CBoolGraphDoc class
#include "stdafx.h"
#include "BoolGraph.h"
#include "BoolGraphDoc.h"
#include "Shapes.h"
#include "Polygon.h"
#include "GlobalData.h"
#include "MainFrm.h"
#include "atlsimpstr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBoolGraphDoc
IMPLEMENT_DYNCREATE(CBoolGraphDoc, CDocument)

BEGIN_MESSAGE_MAP(CBoolGraphDoc, CDocument)
END_MESSAGE_MAP()



CBoolGraphDoc::CBoolGraphDoc():	m_nVersion(FILE_VERSION),//Версия проекта
								m_PaintAreaIsClear(FALSE)//область рисования Занята
{
	ClearLayerList();  //Очищаем список фигур перед началом работы
}





CBoolGraphDoc::~CBoolGraphDoc()
{
	ClearLayerList();   //Очищаем список фигур по окончании работы
}



//Функция устанавливает начальные параметры при создании нового документа
BOOL CBoolGraphDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	CBoolGraphDoc::ClearLayerList();
	UpdateAllViews(NULL); 
	return TRUE;
}



//Метод загрузки данных проекта из файла или записи в файл
//Входные данные: адрес на переменную-архив
void CBoolGraphDoc::Serialize(CArchive& ar)    //exception
{
	if (ar.IsStoring())
	{    //запись в файл
		TRY
		{
			ar<<m_nVersion;
			ar<<m_nLoadLayers;
		}
		CATCH(CFileException, pEx)
		{
			pEx->ReportError();
		}
		END_CATCH
	}
	else
	{   //чтение из файла
		TRY
		{
			ClearLayerList();
			ar>>m_nVersion;
			ar>>m_nLoadLayers;
			m_PaintAreaIsClear=FALSE;
		}
		CATCH(CFileException, pEx)
		{
			pEx->ReportError();
		}
		END_CATCH
	}

	TRY
	{
		m_LayersList.Serialize(ar);  //Функция сохранения/загрузки списка слоев
	}
	CATCH_ALL(pEx)
	{
		pEx->ReportError();
		AfxAbort();
	}
	END_CATCH_ALL
	if(LoadLayersAtListControl()==FALSE)  //Добавляем все фигуры в List Control
		AfxMessageBox(MSG_FILE_CORRUPT);
}




//Стандартные диагностические функции
#ifdef _DEBUG
void CBoolGraphDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBoolGraphDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG




//Функция загружает все фигуры в List Control
//входными данными является список указателей на фигуры
BOOL CBoolGraphDoc::LoadLayersAtListControl()    
{
	CListCtrl* plstLayers = GET_LST_LAYERS;    //получаем указатель на элемент List Control
	plstLayers->DeleteAllItems();  //Очищаем List Control
	if(m_nLoadLayers<=0)
		return FALSE;
	else
	{
		if(m_nLoadLayers>MAX_LOADABLE_LAYERS)
			AfxMessageBox(MSG_TOO_MANY_LAYERS);
		CShapes *pShape = NULL;
		for(UINT nI=0; nI<m_nLoadLayers;nI++)   //загружаем слои в List Control
		{
			pShape=CBoolGraphDoc::GetLayerByNumber(nI); 
			INT nError=plstLayers->InsertItem(0, pShape->GetShapeName());
			if(nError==-1)
				AfxMessageBox(MSG_ADD_LAYER_ERROR);
		}
		UpdateAllViews(NULL);
	}
	return TRUE;
}



//Функция очистки памяти от объектов фигур, которые были созданы ранее
VOID CBoolGraphDoc::ClearLayerList()
{
	POSITION pos = NULL;
	while(m_LayersList.IsEmpty()==0)  //очистка списка указателей
		delete m_LayersList.RemoveHead();
	m_pSelectedLayer=NULL;    //фигура не выделена
	m_nLoadLayers=0;      //не загружено ни одной фигуры
	UpdateAllViews(NULL);
}






//---------------------- выбор и выделение иконок в List Control -------------------

//Функция устанавливает выбранную фигуру как активную
//Входные данные: указатель на выбранную фигуру
VOID CBoolGraphDoc::SetSelLayer(CShapes *pSelected)
{
	ASSERT(pSelected);
	if(pSelected != NULL)
		m_pSelectedLayer=pSelected;
	else
		m_pSelectedLayer=NULL;
}


//Возвращает выбранную фигуру по её номеру в списке указателей
//Входные данные: адрес номера необходимой фигуры
//Выходные данные: указатель на фигуру
CShapes* CBoolGraphDoc::GetLayerByNumber(const INT &nLayerNumber)const
{
	ASSERT(nLayerNumber >= NULL_LAYER);
	CShapes* pShape;
	POSITION pos=NULL;
	//проверяем, чтобы номер фигуры был в диапазоне числа загруженных слоев
	if(nLayerNumber>NULL_LAYER && nLayerNumber<(INT)m_nLoadLayers)
	{
		pos=m_LayersList.FindIndex(nLayerNumber);
		if(pos!=NULL)
			pShape = m_LayersList.GetAt(pos);
	}
	else
		pShape=NULL;
	return pShape;
}


//Функция возвращает указатель на выбранный слой
//Выходные даннные: указатель на объект типа Фигура
CShapes* CBoolGraphDoc::GetSelLayer()const
{
	return m_pSelectedLayer;
}


//Удалить выбранную фигуру из списка указателей
//Входные данные: номер выбранной фигуры
//Возвращает TRUE при успешном удалении, либо FALSE
BOOL CBoolGraphDoc::DeleteSelectedLayer(const INT &nLayerNumber)
{
	ASSERT(nLayerNumber > NULL_LAYER);
	if(m_pSelectedLayer==NULL) 
		return FALSE;
	CShapes *pShape=NULL;
	POSITION pos=NULL;
	pos=m_LayersList.FindIndex(nLayerNumber); //Найдем позицию объекта в списке 
	if(pos!=NULL) // удаляем объект в этой позиции
	{
		m_LayersList.RemoveAt(pos);
		delete m_pSelectedLayer;
		m_pSelectedLayer=NULL;
		return TRUE;
	}
	return FALSE;
}


//Функция получает область, занятую выделенной на текующий момент фигурой
//Входные данные: адрес на переменную-регион, в которую будут записаны данные
VOID CBoolGraphDoc::GetRegion(CRgn &Region)const
{
	m_pSelectedLayer->GetRegion(Region);
}





 //----------------------- число загруженных фигур ---------------------------------------------
//Функция возвращает число загруженных фигур 
//Выходные данные: число загруженных фигур
UINT CBoolGraphDoc::GetLoadedLayers()const
{
	return m_nLoadLayers;
}


//Функция устанавливает число загруженных фигур
//Входные данные: число, которое необходимо установить
VOID CBoolGraphDoc::SetLoadedLayers(const UINT &nLayersNumber)
{
	m_nLoadLayers = nLayersNumber;
}




//---------------------методы занятия и освобождения области рисования ------------------

//Устанавливает флаг занятости области рисования
VOID CBoolGraphDoc::SetPaintAreaClose()
{
	m_PaintAreaIsClear=FALSE;
}


//Устанавливает флаг свободной области рисования
VOID CBoolGraphDoc::SetPaintAreaOpen()
{
	m_PaintAreaIsClear=TRUE;
}


//Возвращает состояние области рисования (занята/свободна)
//Выходные данные: Да или Нет - свободна или нет область
BOOL CBoolGraphDoc::GetPaintArea()const
{
	return m_PaintAreaIsClear;
}

