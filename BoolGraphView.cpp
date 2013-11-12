#include "stdafx.h"
#include "BoolGraph.h"
#include "GlobalData.h"
#include "MainFrm.h"
#include "BoolGraphDoc.h"
#include "BoolGraphView.h"
#include "Shapes.h"
#include "Polygon.h"
#include "ShapeOptionDlg.h"
#include <math.h>
#include "Overlay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Класс CBoolGraphView - Вид
IMPLEMENT_DYNCREATE(CBoolGraphView, CView)

//------------------------------Карта сообщений---------------------------------------------------
BEGIN_MESSAGE_MAP(CBoolGraphView, CView)
	ON_COMMAND(ID_PROJ_CREATE, &CBoolGraphView::OnProjCreate)      //Создать проект
	ON_COMMAND(ID_POLYGON, &CBoolGraphView::OnPolygon)             //элемент рисования Многоугольник
	ON_COMMAND(ID_POLYLINE, &CBoolGraphView::OnPolyline)           //элемент рисования Свободное рисование
	//Обработчики нажатий клавиш мыши и движения мыши
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_DELETE_LAYER, &CBoolGraphView::OnBnClickedDeleteLayer)     //Удалить фигуру
	ON_BN_CLICKED(IDC_LAYER_OPTIONS, &CBoolGraphView::OnBnClickedLayerOptions)   //Вызвать окно опций фигуры
	ON_NOTIFY(NM_CLICK, IDC_LIST_LAYERS, &CBoolGraphView::OnNMClickListLayers)   // Нажатие левой кнопки мыши в List Control
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LAYERS, &CBoolGraphView::OnNMDblclkListLayers) // Двойной клик мышью в List Control
	ON_COMMAND(ID_UNION, &CBoolGraphView::OnUnion)       //Операция Объединение
	ON_COMMAND(ID_DIFFERENCE, &CBoolGraphView::OnDifference)    //Операция Разность
	ON_COMMAND(ID_CROSS, &CBoolGraphView::OnCross)       //Операция Пересечение
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CONTEXT_DELETE, &CBoolGraphView::OnContextDelete)
	ON_COMMAND(ID_CONTEXT_OPTIONS, &CBoolGraphView::OnContextOptions)
	ON_COMMAND(ID_SHOW_TRANSPARENT, &CBoolGraphView::OnShowTransparent)
	ON_COMMAND(ID_VIEW_GRID, &CBoolGraphView::OnViewGrid)
END_MESSAGE_MAP()





// Инициализация полей класса
CBoolGraphView::CBoolGraphView():m_nCurrentOperation(OP_NO_OPER),
								 m_nCurrentLayer(NULL_LAYER),      
								 m_ptFirstPoint(NULL),
								 bIfPaint(FALSE),
								 bIfGrid(FALSE),
								 m_ptCurMovePoint(NULL),
								 m_nShapesChoosed(0)
{
	m_nShapesDeque.SetSize(MAX_WORKING_LAYERS,-1);
	//все элементы массива выбранных фигур для операции хранят значение -1
	for(UINT nI=0; nI<MAX_WORKING_LAYERS;nI++)
		m_nShapesDeque.SetAt(nI,-1);
}


CBoolGraphView::~CBoolGraphView()
{}


BOOL CBoolGraphView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}


//Функция перерисовки окна
//В поле рисования отрисовывает все фигуры, созданные пользователем или программой
//Входной параметр: указатель на контекст устройства вывода
void CBoolGraphView::OnDraw(CDC* pDC)
{
	ASSERT(pDC);
	CBoolGraphDoc* pDoc = GetDocument();
	POSITION pos=NULL;
	CShapes *pShape=NULL;
	DrawGrid(pDC);
	if(pDoc->m_LayersList.GetCount()>0)   //получаем первую позицию в списке указателей
		pos=pDoc->m_LayersList.GetHeadPosition();
	while(pos!=NULL)                      //по одной отрисовываем все фигуры
	{
		pShape = pDoc->m_LayersList.GetNext(pos);
		if(pShape!=NULL)
			pShape->Show(pDC,bIfPaint);
	}
	//проверка корректности номера выбранной фигуры и выделение её цветом
	//выражением (pDoc->GetLoadedLayers()-m_nCurrentLayer-1) получаем номер фигуры в списке указателей через номер в List Control
	if(IsShapeNumberCorrect((pDoc->GetLoadedLayers()-m_nCurrentLayer-1)) == TRUE) 
	{  
		pShape=pDoc->GetLayerByNumber((pDoc->GetLoadedLayers()-m_nCurrentLayer-1));
		if(pShape!=NULL)
		{
			UINT nStyle = pShape->GetPenStyle();
			UINT nWidth = pShape->GetPenWidth();
			COLORREF rgbColor = pShape->GetPenColor();
			pShape->SetPen(THEME_PEN_2,THEME_WIDTH_2,THEME_STYLE_2); //цветовая тема #2 - из GlobalData.h
			pShape->Show(pDC,bIfPaint);
			pShape->SetPen(rgbColor,nWidth,nStyle); //снимаем тему выделения, оставляем старую
		}
	}
}




//---------------------------- Функции дебаггера ----------------------
//Стандартные фукнции дигностики класса CBoolGraphView
#ifdef _DEBUG
void CBoolGraphView::AssertValid() const
{
	CView::AssertValid();
}


void CBoolGraphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
//---------------------------------------------------------------------







//Функция возвращает указатель на документ
CBoolGraphDoc* CBoolGraphView::GetDocument() const //в релиз-версии inline-функция
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBoolGraphDoc)));
	return (CBoolGraphDoc*)m_pDocument;

}
#endif //_DEBUG



//Функция рисования указывающей линии-метки
//Инверсным цветом рисует прямую между двумя выбранными пользователем точками
//Эти точки являются координатами отрисовки фигур
//Входные параметры: координаты начальной и конечной точек
VOID CBoolGraphView::DrawMoveLine(const CPoint &ptFirstPoint, const CPoint &ptSecondPoint)
{
	ASSERT(ptFirstPoint.x >= 0);
	ASSERT(ptFirstPoint.y >= 0);
	ASSERT(ptSecondPoint.x >= 0);
	ASSERT(ptSecondPoint.y >= 0);
	CClientDC dc(this);
	OnPrepareDC(&dc); 	
	INT OldMode=dc.SetROP2(R2_NOT); //Установим режим рисования инверсным цветом
   	dc.MoveTo(ptFirstPoint);
	dc.LineTo(ptSecondPoint);  
	dc.SetROP2(OldMode);    //Восстанавливаем прежний режим рисования
}





//функция рисования сетки
VOID CBoolGraphView::DrawGrid(CDC* pDC)
{
	if(bIfGrid)
	{
		//вычисление шага сетки - 5 миллиметров
		INT nPIMX, nPIMY;
		DOUBLE TempX = (DOUBLE)pDC->GetDeviceCaps(LOGPIXELSX)/MPI;
		DOUBLE TempY = (DOUBLE)pDC->GetDeviceCaps(LOGPIXELSY)/MPI;
		DOUBLE OstatokX = TempX - (DOUBLE)(INT)TempX;
		DOUBLE OstatokY = TempY - (DOUBLE)(INT)TempY;
		OstatokX > 0.5 ? nPIMX = (INT)TempX + 1 : nPIMX = (INT)TempX;
		OstatokY > 0.5 ? nPIMY = (INT)TempY + 1 : nPIMY = (INT)TempY;
		//рисование сетки
		CPen Pen(0, 1, GRID_COLOR);
		CPen *oldPen = pDC->SelectObject(&Pen);
		CRect Rect;
		pDC->GetClipBox(Rect);
		for(INT nY = 0; nY < Rect.Height(); nY+=5*nPIMX)
		{
			pDC->MoveTo(0,nY);
			pDC->LineTo(Rect.Width(),nY);
		}
		for(INT nX = 0; nX < Rect.Width(); nX+=5*nPIMY)
		{
			pDC->MoveTo(nX,0);
			pDC->LineTo(nX,Rect.Height());
		}
		pDC->SelectObject(oldPen);
	}
}


//---------------- функции для работы с элементом интерфеса List Control ----------------------------

//Добавить новую фигуру в список List Control
VOID CBoolGraphView::AddLayerAtList()   
{
	CListCtrl* plstLayers = GET_LST_LAYERS;
	INT iError=plstLayers->InsertItem(0, DEFAULT_NAME);
	if(iError==-1)
		AfxMessageBox(MSG_ADD_LAYER_ERROR);
}


//Функция удаления одной фигуры из списка List Control
VOID CBoolGraphView::DeleteLayerFromList()
{
	CBoolGraphDoc *pDoc=GetDocument();
	CListCtrl *plstLayers = GET_LST_LAYERS; 
	if(pDoc->GetPaintArea()==FALSE)
	{
		if(m_nCurrentLayer == NULL_LAYER)
		{
			AfxMessageBox(MSG_UNCHOOSED_ELEMENT);
			return;
		}
		if(IsShapeNumberCorrect((pDoc->GetLoadedLayers()-m_nCurrentLayer-1)) == TRUE)
		{
			pDoc->DeleteSelectedLayer((pDoc->GetLoadedLayers()-m_nCurrentLayer-1)); 
			plstLayers->DeleteItem(m_nCurrentLayer);  
			pDoc->SetLoadedLayers(pDoc->GetLoadedLayers()-1);  
		}
		m_nCurrentLayer=NULL_LAYER;
		pDoc->SetModifiedFlag();
		Invalidate();
	}
}




//Функция очищает список List Control
//Выходные данные: ДА или НЕТ - успешно ли выполнена
BOOL CBoolGraphView::ClearListControl()
{
	CListCtrl *plstLayers = GET_LST_LAYERS;
	plstLayers->DeleteAllItems();
	return TRUE;
}



//функция очистки очереди выбранных фигур
VOID CBoolGraphView::ClearShapeDeque()
{
	for(UINT nI=0; nI<MAX_WORKING_LAYERS; nI++)  //все элементы в начальном значении -1
		m_nShapesDeque.SetAt(nI,-1);
}



//Функция установки метки POINT_SYMBOL (+) перед иконкой фигуры в List Control
//Выходные данные: ДА или НЕТ - успешно ли выполнена
BOOL CBoolGraphView::SetShapePoint(const INT &nShapeIndex)
{
	ASSERT(nShapeIndex >= 0);
	//получаем указатель на элемент формы List Control
	CListCtrl* plstLayers = GET_LST_LAYERS;
	plstLayers->SetItemText(nShapeIndex,1,POINT_SYMBOL);
	return TRUE;
}





//---------------- функции проверки --------------------------------------------------

//функция возвращает ответ, корректен ли номер выбранной в List Control фигуры
//Входные данные: номер фигуры в списке указателей на фигуры
//Выходные данные: Да(true) или Нет(false) - кооректен номер или нет
BOOL CBoolGraphView::IsShapeNumberCorrect(const INT &nCurrentLayer)const
{
	ASSERT(nCurrentLayer >= 0);
	CBoolGraphDoc *pDoc=GetDocument();
	if(nCurrentLayer<FIRST_LAYER || nCurrentLayer>(INT)pDoc->GetLoadedLayers())
		return FALSE;
	else
		return TRUE;
}





//Функция проверяет, возможно ли следующее добавление фигуры в проект
//максимальное число фигур ограничено константой MAX_LOADABLE_LAYERS
//Выходные данные: ДА - разрешено добавлять фигуру, НЕТ - не разрешено
BOOL CBoolGraphView::IsLegalShape()
{
	CBoolGraphDoc *pDoc=GetDocument();
	if(pDoc->GetLoadedLayers()>=MAX_LOADABLE_LAYERS)
	{
		//выдаем сообщение о том, что больше добавлять фигуры нельзя
		AfxMessageBox(MSG_TOO_MANY_LAYERS);
		return FALSE;
	}
	return TRUE;
}




//Функция проверки, существуют ли фигуры, под выбранными номерами
//Выходные данные: ДА - операция разрешена, НЕТ - нет
BOOL CBoolGraphView::IsLegalOperation()
{
	CBoolGraphDoc *pDoc=GetDocument();
	for(UINT nI=0; nI<MAX_WORKING_LAYERS;nI++)
		if(m_nShapesDeque.GetAt(nI)<0 || m_nShapesDeque.GetAt(nI) >= (INT)(pDoc->GetLoadedLayers()))
		{
			AfxMessageBox(MSG_THERE_IS_NO_SHAPES);
			return FALSE;
		}
	return TRUE;
}






//------------------ Обработка нажатий кнопок мыши    -------------------------------------------

//Обработчик нажатия левой кнопки мыши
//при нажатии на левую кнопку мыши происходит:
//1. Сохранение точки нажатия, для определения начальной точки рисования фигуры
//2. Если выбрана операция рисования многоугольника или полигона, то добавление точки как очередной его вершины
VOID CBoolGraphView::OnLButtonDown(UINT nFlags, CPoint ptPoint)
{
	ASSERT(ptPoint.x >= 0);
	ASSERT(ptPoint.y >= 0);
	CBoolGraphDoc *pDoc=GetDocument();
	CDC *pDC=GetDC();  
	OnPrepareDC(pDC);  
	ReleaseDC(pDC);   
	m_ptCurMovePoint=ptPoint;
	switch(m_nCurrentOperation)
	{
		case OP_POLYLINE:
		case OP_POLYGON:
			//добавляем точку в список вершин многоугольника
			((CPolygon*)pDoc->m_LayersList.GetTail())->AddPoint(ptPoint);
			m_nCurrentLayer=NULL_LAYER;
			Invalidate();
			break;
	}
	m_ptFirstPoint=ptPoint;
	CView::OnLButtonDown(nFlags, ptPoint);
}




//Обработчик двойного клика мышью в поле рисования
//Завершение рисования многоугольника
VOID CBoolGraphView::OnLButtonDblClk(UINT nFlags, CPoint ptPoint)
{
	ASSERT(ptPoint.x >= 0);
	ASSERT(ptPoint.y >= 0);
	CBoolGraphDoc *pDoc=GetDocument();
	switch(m_nCurrentOperation)
	{
		case OP_POLYLINE:
			pDoc->SetPaintAreaClose();     //Закрываем область рисования
			m_nCurrentOperation=OP_NO_OPER; //Операция не выбрана
			break;
		case OP_NO_OPER:
			if(pDoc->GetPaintArea()==FALSE)
			{
				CListCtrl *plstLayers = GET_LST_LAYERS;
				CRgn UserRegion;
				CShapes *pShape = NULL;
				POSITION pos = NULL;
				INT nCounter = 0;
				if(pDoc->m_LayersList.GetCount()>0) 
					pos=pDoc->m_LayersList.GetTailPosition();
				while(pos != NULL) 
				{
					pShape=pDoc->m_LayersList.GetPrev(pos);
					UserRegion.DeleteObject();
					pShape->GetRegion(UserRegion);
					if(UserRegion.PtInRegion(ptPoint)) 
					{
						pDoc->SetSelLayer(pShape);
						m_nCurrentLayer = nCounter;
						pos = NULL;
						plstLayers->SetFocus();
						plstLayers->SetItemState(nCounter, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
						if(m_nShapesChoosed>=0 && m_nShapesChoosed<MAX_WORKING_LAYERS)
						{
							m_nShapesDeque.SetAt(m_nShapesChoosed, (pDoc->GetLoadedLayers()-m_nCurrentLayer-1));
							m_nShapesChoosed+=1;
							SetShapePoint(m_nCurrentLayer); //ставим метку + в списке List Control напротив выбранной фигуры
						}
						else
						{
							ClearShapeDeque();
							for(UINT nI=0; nI<pDoc->GetLoadedLayers();nI++)
								plstLayers->SetItemText(nI, 1,EMPTY_NAME);
							m_nShapesChoosed=0;
						}
					}
					nCounter++;
				}
				Invalidate();
			}
	}
	CView::OnLButtonDblClk(nFlags, ptPoint);
}




//Обработчик отжатия левой кнопки мыши
//При этом происходит:
//1. Или окончание рисования фигуры со свободным контуром
//2. Или добавление простой фигуры
VOID CBoolGraphView::OnLButtonUp(UINT nFlags, CPoint ptPoint)
{
	ASSERT(ptPoint.x >= 0);
	ASSERT(ptPoint.y >= 0);
	CBoolGraphDoc *pDoc=GetDocument();
	CDC *pDC=GetDC();
	OnPrepareDC(pDC);
	ReleaseDC(pDC);

	switch(m_nCurrentOperation)
	{
		case OP_POLYGON:
			pDoc->SetPaintAreaClose();
			m_nCurrentOperation=OP_NO_OPER;
			break;
		case OP_NO_OPER:
			if(pDoc->GetPaintArea()==FALSE)
			{
				CListCtrl *plstLayers = GET_LST_LAYERS;
				CRgn UserRegion;
				CShapes *pShape = NULL;
				POSITION pos = NULL;
				INT nCounter = 0;
				if(pDoc->m_LayersList.GetCount()>0) 
					pos=pDoc->m_LayersList.GetTailPosition();
				while(pos != NULL) 
				{
					pShape=pDoc->m_LayersList.GetPrev(pos);
					UserRegion.DeleteObject();
					pShape->GetRegion(UserRegion);
					if(UserRegion.PtInRegion(ptPoint)) 
					{
						pDoc->SetSelLayer(pShape);
						m_nCurrentLayer = nCounter;
						pos = NULL;
						plstLayers->SetFocus();
						plstLayers->SetItemState(nCounter, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
					}
					else
						m_nCurrentLayer = NULL_LAYER;
					nCounter++;
				}
				Invalidate();
			}
	}
	CView::OnLButtonUp(nFlags, ptPoint);
}




//Обработчик движения мыши при нажатой левой кнопке
//Рисуется линия-метка между точкой начала и точкой текущего положения
VOID CBoolGraphView::OnMouseMove(UINT nFlags, CPoint ptPoint)
{
	ASSERT(ptPoint.x >= 0);
	ASSERT(ptPoint.y >= 0);
	CBoolGraphDoc *pDoc=GetDocument();
	CDC *pDC=GetDC();
	OnPrepareDC(pDC); 
	ReleaseDC(pDC);
	
	switch(m_nCurrentOperation)
	{
		//если рисуем фигуру по точкам,здесь происходит перерисовка линии-метки
		case OP_POLYLINE:
			if(((CPolygon*)pDoc->m_LayersList.GetTail())->GetPointNumber()<=0) 
				break;
			DrawMoveLine(m_ptFirstPoint, m_ptCurMovePoint);
			m_ptCurMovePoint=ptPoint;
			DrawMoveLine(m_ptFirstPoint, m_ptCurMovePoint);
			break;
		//если рисуем произвольный контур,запоминаем каждую точку пока зажата левая кнопка мыши
		case OP_POLYGON:
			if(((CPolygon*)pDoc->m_LayersList.GetTail())->GetPointNumber()<=0) 
				break;
			if((nFlags & MK_LBUTTON) == MK_LBUTTON)
			{
				((CPolygon*)pDoc->m_LayersList.GetTail())->AddPoint(ptPoint);
				Invalidate();
			}
			break;
	}
	CView::OnMouseMove(nFlags, ptPoint);
}





//Нажатие кнопки "Delete"
VOID CBoolGraphView::OnBnClickedDeleteLayer()
{
	DeleteLayerFromList();
}



//Обработка вызова окна Опции фигуры
//При нажатии на кнопку Опции всплывает окно, в котором можно задать такие свойства фигуры, как
//имя, цвет и тип заливки, цвет пера
VOID CBoolGraphView::OnBnClickedLayerOptions()
{
	CShapeOptionDlg ShapeOptionsDialog;
	CBoolGraphDoc *pDoc=GetDocument();
	CShapes* pShape;
	if(pDoc->GetPaintArea()==FALSE)
	{
		if(m_nCurrentLayer == NULL_LAYER)
		{
			AfxMessageBox(MSG_UNCHOOSED_ELEMENT);
			return;
		}
		if(IsShapeNumberCorrect((pDoc->GetLoadedLayers()-m_nCurrentLayer-1))==TRUE)
		{
			//передаем в текстовое поле окна опций - имя текущего слоя
			pShape = pDoc->GetSelLayer();
			ShapeOptionsDialog.SetShapeName(pShape->GetShapeName());
			if(ShapeOptionsDialog.DoModal()==IDOK)
			{
				//сохраняем изменения
				pShape->SetPen(ShapeOptionsDialog.GetPenColor(),1,PS_SOLID);
				pShape->SetBrush(ShapeOptionsDialog.GetBrushColor(),ShapeOptionsDialog.GetBrushStyle());
				pShape->SetShapeName(ShapeOptionsDialog.GetShapeName());
				pDoc->LoadLayersAtListControl();
				ClearShapeDeque();
			}
		}
	}
}





//--------------------обработка нажатий мышкой в списке List Control-----------------------------

//Клик в списке List Control
//Происходит выделение соответствующей фигуры цветом
VOID CBoolGraphView::OnNMClickListLayers(NMHDR *pNMHDR, LRESULT *pResult)
{
	CBoolGraphDoc *pDoc=GetDocument();
	if(pDoc->GetPaintArea()==FALSE)
	{
		CShapes* pShape;
		CListCtrl *plstLayers = GET_LST_LAYERS;  //получаем указатель на список List Control
		if(plstLayers->GetSelectedCount() == 0) 
			return;
		//находим индекс выбранной фигуры
		m_nCurrentLayer=plstLayers->GetNextItem(-1, LVNI_SELECTED); 
		if(m_nCurrentLayer!=NULL_LAYER)
		{
			//фигура, на которой пользователь кликнул в списке, выделена
			pShape=pDoc->GetLayerByNumber((pDoc->GetLoadedLayers()-m_nCurrentLayer-1));
			pDoc->SetSelLayer(pShape);
		}
		Invalidate();
	}
}




//Обработка двойного клика мышью в списке List Control
//Фигура, на иконке которой кликнул пользователь, будет использована для операции
VOID CBoolGraphView::OnNMDblclkListLayers(NMHDR *pNMHDR, LRESULT *pResult)
{
	CBoolGraphDoc *pDoc=GetDocument();
	if(pDoc->GetPaintArea()==FALSE) //если поле рисования закрыто
	{
		CListCtrl *plstLayers = GET_LST_LAYERS;
		if(m_nCurrentLayer == NULL_LAYER)
			return;
		if(m_nShapesChoosed>=0 && m_nShapesChoosed<MAX_WORKING_LAYERS)
		{
			m_nShapesDeque.SetAt(m_nShapesChoosed, (pDoc->GetLoadedLayers()-m_nCurrentLayer-1));
			m_nShapesChoosed+=1;
			SetShapePoint(m_nCurrentLayer); //ставим метку + в списке List Control напротив выбранной фигуры
		}
		else
		{
			ClearShapeDeque();
			for(UINT nI=0; nI<pDoc->GetLoadedLayers();nI++)
				plstLayers->SetItemText(nI, 1,EMPTY_NAME);
			m_nShapesChoosed=0;
		}
	}
}







//-----------------------  Функции меню   ------------------------------
//Создание нового проекта "Новый"
VOID CBoolGraphView::OnProjCreate()
{
	CBoolGraphDoc *pDoc=GetDocument();
	//устанавливаем все параметры в исходное положение
	m_nCurrentOperation=OP_NO_OPER;   
	m_nCurrentLayer=NULL_LAYER;
	m_ptFirstPoint=NULL;
	m_ptCurMovePoint=NULL;
	if (!pDoc->OnNewDocument())
		AfxMessageBox(MSG_NEW_DOCUMENT_ERROR);
	ClearListControl();
}




//обработка команды меню Фигура->Тип фигуры->По контуру(не точно)
VOID CBoolGraphView::OnPolygon()
{
	CBoolGraphDoc *pDoc=GetDocument();
	if(pDoc->GetPaintArea() == FALSE)
		CreatePolygon(OP_POLYGON);
	else
		AfxMessageBox(MSG_PAINTING_CLOSE);
}




//обработка команды меню Фигура->Тип фигуры->По точкам
VOID CBoolGraphView::OnPolyline()
{
	CBoolGraphDoc *pDoc=GetDocument();
	if(pDoc->GetPaintArea() == FALSE)
		CreatePolygon(OP_POLYLINE);
	else
		AfxMessageBox(MSG_PAINTING_CLOSE);
}




//Выбор в меню операции Объединение
//Выполняется математическая операция дополнения к ранее выбранным фигурам
VOID CBoolGraphView::OnUnion()
{
	if(IsLegalOperation()==TRUE)
		StartOperation(OP_UNION);
}




//Выбор в меню операции Разность
//Выполняется математическая операция разности к ранее выбранным фигурам
VOID CBoolGraphView::OnDifference()
{
	if(IsLegalOperation()==TRUE)
		StartOperation(OP_DIFFERENCE);
}



//Выбор в меню операции Пересечение
//Выполняется математическая операция пересечения к ранее выбранным фигурам
VOID CBoolGraphView::OnCross()
{
	if(IsLegalOperation()==TRUE)
		StartOperation(OP_INTERSECT);
}



//------------------- обработка функций контекстного меню --------------------

//Функция вызова контекстного меню
void CBoolGraphView::OnContextMenu(CWnd *pWnd, CPoint ptPoint)
{
	CMenu ShapeContextMenu;
	ShapeContextMenu.LoadMenuW(IDR_CONTEXTMENU);
	ShapeContextMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, ptPoint.x, ptPoint.y, this);
}



//обработка операции удаления в контекстном меню
void CBoolGraphView::OnContextDelete()
{
	DeleteLayerFromList();
}




//обработка вызова опций фигуры в контекстном меню
void CBoolGraphView::OnContextOptions()
{
	OnBnClickedLayerOptions();
}


//Функция главного меню: отображения прозрачных/не прозрачных фигур
void CBoolGraphView::OnShowTransparent()
{
	if(bIfPaint)
		bIfPaint = FALSE;
	else
		bIfPaint = TRUE;
	CMenu *pMenu = AfxGetMainWnd()->GetMenu();
	UINT nState = pMenu->CheckMenuItem(ID_SHOW_TRANSPARENT, MF_BYCOMMAND);
	if(nState == MF_CHECKED)
		pMenu->CheckMenuItem(ID_SHOW_TRANSPARENT, MF_BYCOMMAND | MF_UNCHECKED);
	else
		pMenu->CheckMenuItem(ID_SHOW_TRANSPARENT, MF_BYCOMMAND | MF_CHECKED);
	Invalidate();
}




//Функция главного меню: отображение поля в клеточку
void CBoolGraphView::OnViewGrid()
{
	if(bIfGrid)
		bIfGrid = FALSE;
	else
		bIfGrid = TRUE;
	CMenu *pMenu = AfxGetMainWnd()->GetMenu();
	UINT nState = pMenu->CheckMenuItem(ID_VIEW_GRID, MF_BYCOMMAND);
	if(nState == MF_CHECKED)
		pMenu->CheckMenuItem(ID_VIEW_GRID, MF_BYCOMMAND | MF_UNCHECKED);
	else
		pMenu->CheckMenuItem(ID_VIEW_GRID, MF_BYCOMMAND | MF_CHECKED);
	Invalidate();
}




//------------------------------ функции создания фигур -------------------------

//функция создает фигуру(-ры)-результаты операции
//вход: массив ребер из которого можно выделить фигуры - результаты
//выход: создаются фигуры, сохраняются в список фигур, выводятся на экран
VOID CBoolGraphView::CreateShapeResult(CArray <SEdge>* pModel) //exception
{
	ASSERT(pModel);
	CArray <CPoint> ptPoints;
	ptPoints.SetSize(ZERO_SIZE, SINGLE_STEP);
	CBoolGraphDoc *pDoc=GetDocument();
	while(pModel->IsEmpty()==FALSE)
	{
		DetermineShape(pModel, &ptPoints); //находим очередную фигуру
		if(ptPoints.IsEmpty() == FALSE)   //и сохраняем её
		{
			CShapes *pShape = NULL;
			pDoc->SetPaintAreaOpen();

			TRY
			{
				pShape=new CPolygon;
			}
			CATCH(CMemoryException, pEx)
			{
				pEx->ReportError();
				AfxAbort();
			}
			END_CATCH
			pDoc->SetLoadedLayers(pDoc->GetLoadedLayers()+1);
			pShape->SetPen(THEME_PEN_3, THEME_WIDTH_3, THEME_STYLE_3);
			for(INT nJ=0; nJ<ptPoints.GetSize(); nJ++)
				pShape->AddPoint(ptPoints.GetAt(nJ));
			pDoc->m_LayersList.AddTail(pShape);  //добавляем фигуру в конец списка указателей
			AddLayerAtList();
			pDoc->SetPaintAreaClose();
			ptPoints.RemoveAll();  
			m_nCurrentLayer=NULL_LAYER;
			pDoc->SetModifiedFlag();
			Invalidate();
		}
		else
			pModel->RemoveAll();
	}
	ptPoints.RemoveAll();
}





//Метод осуществляет поиск номера инциндентного ребра к заданной вершине
//Входные данные: вершина и массив ребер, в котором производится поиск
//Выходные данные: номер ребра, инцидентного к вершине, либо -1 - если такого нет
//Пояснение: на самом деле, может быть случай - когда к вершине инцидентно несколько ребер,
//           но этом случай учитывается в функции выделения фигур
INT CBoolGraphView::SetIncedentEdges(const CPoint &ptPoint, const CArray <SEdge>* pSearchArray)
{
	ASSERT(ptPoint.x >= 0);
	ASSERT(ptPoint.y >= 0);
	ASSERT(pSearchArray);
	INT nNumber = -1;
	for(INT nI=0; nI<pSearchArray->GetSize(); nI++)
		if((ptPoint == pSearchArray->GetAt(nI).ptVertex1) || (ptPoint==pSearchArray->GetAt(nI).ptVertex2))
			nNumber=nI;
	return nNumber;
}





//Выполняет поиск очередной фигуры, являющейся результатом операции, среди массива ребер
//Входные данные: указатели на массивы ребер и вершин будущей фигуры
//Выходные данные: заполненный массив вершин фигуры
VOID CBoolGraphView::DetermineShape(CArray <SEdge>* pModel, CArray <CPoint>* pPoints)
{
	ASSERT(pModel);
	ASSERT(pPoints);
	CPoint ptStartPoint = pModel->GetAt(0).ptVertex2;//выбираем первое ребро(ЛЮБОЕ) и обозначаем его вершины
	CPoint ptEndPoint = pModel->GetAt(0).ptVertex1;
	pPoints->Add(ptEndPoint);   //запоминаем стартовую точку - первую в списке вершин фигуры
	SEdge TempEdge = pModel->GetAt(0);
	pModel->RemoveAt(0,1);
	INT nNumberOfIncedent = -1;
	while(ptStartPoint != pPoints->GetAt(0))  //выполняем пока фигура не замкнется
	{
		pPoints->Add(ptStartPoint);
		nNumberOfIncedent=SetIncedentEdges(ptStartPoint, pModel);  //ищем номер инцидентного ребра
		if(nNumberOfIncedent>=0)
		{
			TempEdge = pModel->GetAt(nNumberOfIncedent);
			//в массиве ребер направления векторов не отсортированы, поэтому 
			//необходимо определять крайнюю вершину - которой ещё нет в списке вершин:
			if(TempEdge.ptVertex1 != ptStartPoint && TempEdge.ptVertex2 == ptStartPoint) 
			{
				ptStartPoint = TempEdge.ptVertex1;  //запоминаем вершины нового найденного ребра
				ptEndPoint = TempEdge.ptVertex2;
			}
			else if(TempEdge.ptVertex2 != ptStartPoint && TempEdge.ptVertex1 == ptStartPoint)
			{
				ptStartPoint=TempEdge.ptVertex2;
				ptEndPoint = TempEdge.ptVertex1;
			}
			pModel->RemoveAt(nNumberOfIncedent);
			nNumberOfIncedent=-1;
		}
		else 
		{
			//из-за погрешности при расчетах инцидентные ребра могут быть необнаружены
			//тогда полученный уже оверлей замыкается, проходя по самому себе в обратном порядке
			CArray <CPoint> TempPoints;
			TempPoints.SetSize(ZERO_SIZE, SINGLE_STEP);
			for(INT nI = (INT)pPoints->GetSize()-2; nI > 0; nI--)
				TempPoints.Add(pPoints->GetAt(nI));
			pPoints->Append(TempPoints);
			break;
		}
	}
}





//создание объекта типа Полигон
//Исключение возможно при выделении памяти новому объекту 
VOID CBoolGraphView::CreatePolygon(const UINT &nOperation)         //exception
{
	CBoolGraphDoc *pDoc=GetDocument();
	if(IsLegalShape()==TRUE)
	{
		CShapes *pShape=NULL;
		TRY
		{
			pShape=new CPolygon;
		}
		CATCH(CMemoryException, pEx)
		{
			pEx->ReportError();
			AfxAbort();
		}
		END_CATCH
		pDoc->SetLoadedLayers(pDoc->GetLoadedLayers()+1);
		pShape->SetPen(THEME_PEN_1, THEME_WIDTH_1, THEME_STYLE_1);
		pDoc->m_LayersList.AddTail(pShape);
		(nOperation==OP_POLYGON)?m_nCurrentOperation=OP_POLYGON:m_nCurrentOperation=OP_POLYLINE;
		pDoc->SetPaintAreaOpen(); //разрешаем в области рисования создать фигуру
		pDoc->SetModifiedFlag();
		AddLayerAtList();
	}
	else
		m_nCurrentOperation=OP_NO_OPER;

}



//функция запускает механизм выполнения операций над фигурами,
//перед этим берет указатели на эти фигуры
//Входные данные: номер операции (объединение, пересечение или разность)
VOID CBoolGraphView::StartOperation(const UINT &nOperation)
{
	ASSERT(nOperation);
	CShapes* pFirstShape;
	CShapes* pSecondShape;     
	CBoolGraphDoc *pDoc=GetDocument();
	pFirstShape=pDoc->GetLayerByNumber(m_nShapesDeque.GetAt(FIRST_ELEMENT));
	pSecondShape=pDoc->GetLayerByNumber(m_nShapesDeque.GetAt(SECOND_ELEMENT));
	COverlay OverlayShape; //создаем объект класса Оверлей для вычисления операции
	CArray <SEdge> ModelEdges;
	ModelEdges.SetSize(ZERO_SIZE, SINGLE_STEP);
	switch(nOperation)
	{
		case OP_UNION:
			OverlayShape.MakeOperation(pFirstShape, pSecondShape, OP_UNION);
			break;
		case OP_INTERSECT:
			OverlayShape.MakeOperation(pFirstShape, pSecondShape, OP_INTERSECT);
			break;
		case OP_DIFFERENCE:
			OverlayShape.MakeOperation(pFirstShape, pSecondShape, OP_DIFFERENCE);
			break;
	}
	OverlayShape.GetResult(&ModelEdges);
	CreateShapeResult(&ModelEdges);
}
