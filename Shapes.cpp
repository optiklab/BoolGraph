#include "StdAfx.h"
#include "Shapes.h"
#include "GlobalData.h"



//Инициализация полей класса
CShapes::CShapes(): m_sShapeName(DEFAULT_NAME),  //имя по умолчанию
					m_nPenStyle(PS_SOLID),       //кисть сплошная
					m_nPenWidth(THEME_WIDTH_1),  //толщина
					m_rgbPenColor(RGB(0,0,0)),   //цвет - черный
					m_nBrushStyle(SOLID_BRUSH),  //без штриховки
					m_rgbBrushColor(RGB(255,255,255)) //заливка белая
{
	m_ShapePoints.SetSize(ZERO_SIZE,SINGLE_STEP);
}



CShapes::~CShapes()
{
	m_ShapePoints.RemoveAll();
}


IMPLEMENT_SERIAL(CShapes,CObject,VERSIONABLE_SCHEMA|1)





//Метод вырисовывает фигуру
//перегружен в классах-потомках фигур
VOID CShapes::Show(CDC *pDC, BOOL bIfPaint)
{}





//Добавляет новую вершину фигуры в конец массива
//Входные параметры: адрес на переменную, содержащую вершину
VOID CShapes::AddPoint(const CPoint &ptPoint)
{
	ASSERT(ptPoint.x >= 0);
	ASSERT(ptPoint.y >= 0);
	m_ShapePoints.Add(ptPoint);
}







//Удаляет одну из вершин фигуры
//Входные данные: индекс элемента для удаления
//Выходные данные:  TRUE или FALSE - успешно или нет выполнена операция
VOID CShapes::DeletePoint(const INT &iIndex)
{
	ASSERT(iIndex >= 0);
	m_ShapePoints.RemoveAt(iIndex,1);
}





//Метод устанавливает перо с необходимыми параметрами
//Входные параметры: цвет и ширина кисти, стиль заполнения фигур
//Выходной параметр: TRUE - если операция окончилась без ошибок, иначе FALSE
BOOL CShapes::SetPen(const COLORREF rgbColor, const UINT iWidth, const UINT &iStyle)
{
	ASSERT(iWidth);
	m_nPenStyle=iStyle;
	m_nPenWidth=iWidth;
	m_rgbPenColor=rgbColor;
	if(HPEN(m_Pen)!=NULL)
		if(!m_Pen.DeleteObject())
			return FALSE;
	return m_Pen.CreatePen(m_nPenStyle,m_nPenWidth,m_rgbPenColor);
}






//Метод устанавливает заливку
//Входные параметры: цвет, стиль заполнения фигур
//Выходной параметр: TRUE - если операция окончилась без ошибок, иначе FALSE
BOOL CShapes::SetBrush(const COLORREF rgbColor, const INT iStyle)
{
	m_nBrushStyle=iStyle;
	m_rgbBrushColor=rgbColor;
	if(HBRUSH(m_Brush)!=NULL)
		if(!m_Brush.DeleteObject())
			return FALSE;
	if(m_nBrushStyle>=0)      //если указан стиль штриховки
		return m_Brush.CreateHatchBrush(m_nBrushStyle, m_rgbBrushColor);
	return m_Brush.CreateSolidBrush(m_rgbBrushColor);
}







//Записывает имя фигуры
//Входные данные: адрес на строку с именем фигуры
VOID CShapes::SetShapeName(const CString &sName)
{
	ASSERT(sName);
	m_sShapeName=sName;
}
	


//Метод возвращает область, занятую фигурой
//перегружен в классах-потомках фигур
VOID CShapes::GetRegion(CRgn &Region)
{}


//Возвращает текущий стиль пера
//Выходные данные: беззнаковое целое число - номер стиля 
UINT CShapes::GetPenStyle()const
{
	return m_nPenStyle;
}


//Возвращает текущую ширину пера
//Выходные данные: беззнаковое целое число - ширина
UINT CShapes::GetPenWidth()const
{
	return m_nPenWidth;
}


//Возвращает текущий цвет пера
//Выходные данные: длинное целое, содержащее цветовую комбинацию RGB - цвет пера
COLORREF CShapes::GetPenColor()const
{
	return m_rgbPenColor;
}


//Возвращает текущий стиль заливки
//Выходные данные: беззнаковое целое число - номер стиля 
UINT CShapes::GetBrushStyle()const
{
	return m_nBrushStyle;
}


//Возвращает текущий цвет заливки
//Выходные данные: длинное целое, содержащее цветовую комбинацию RGB - цвет заливки
COLORREF CShapes::GetBrushColor()const
{
	return m_rgbBrushColor;
}


//Возвращает вершину фигуры по её номеру
//Входные данные: индекс элемента массива вершин
//Выходные данные: точка (два 32-битных целых) 
CPoint CShapes::GetPoint(const INT &iPointIndex)const
{
	ASSERT(iPointIndex >= 0);
	return m_ShapePoints.GetAt(iPointIndex);
}


//Возвращает количество вершин фигуры
//Выходные данные: целое число - количество элементов массива вершин
INT CShapes::GetPointNumber()const
{
	return (INT)m_ShapePoints.GetSize();
}


//Возвращает имя фигуры
//Выходные данные: строка с именем фигуры
CString CShapes::GetShapeName()const
{
	return m_sShapeName;
}



//Метод отвечает за сохранение в файл и открытие данных из файла
//Входные данные: адрес на переменную-архив
VOID CShapes::Serialize(CArchive& ar)      //exception
{
	if(ar.IsStoring())
	{   //сохранение данных
		TRY
		{
			ar<<m_sShapeName;
			ar<<m_nPenStyle;
			ar<<m_nPenWidth;
			ar<<m_rgbPenColor;
			ar<<m_nBrushStyle;
			ar<<m_rgbBrushColor;
		}
		CATCH(CFileException, pEx)
		{
			pEx->ReportError();
		}
		END_CATCH
	}
	else
	{   //чтение данных
		TRY
		{
			ar>>m_sShapeName;
			ar>>m_nPenStyle;
			ar>>m_nPenWidth;
			ar>>m_rgbPenColor;
			ar>>m_nBrushStyle;
			ar>>m_rgbBrushColor;
			SetPen(m_rgbPenColor,m_nPenWidth,m_nPenStyle);
			SetBrush(m_rgbBrushColor,m_nBrushStyle);
		}
		CATCH(CFileException, pEx)
		{
			pEx->ReportError();
		}
		END_CATCH
	}
	//сохраняем массив точек фигуры
	TRY
	{
		m_ShapePoints.Serialize(ar);
	}
	CATCH_ALL(pEx)
	{
		pEx->ReportError();
		AfxAbort();
	}
	END_CATCH_ALL
}






//Метод подготавливает контекст устройства к выводу изображения
//Входные данные: указатель на контекст устройства вывода
//Выходные данные: TRUE или FALSE в зависимости от успешности операции
BOOL CShapes::_PrepareDC(CDC *pDC)
{
	ASSERT(pDC);
	//сохраняем состояние контекста устройства
	if(!pDC->SaveDC())
		return FALSE;
	//устанавливаем созданные перо и кисть
	if(HPEN(m_Pen)!=NULL)
		pDC->SelectObject(&m_Pen);
	if(HBRUSH(m_Brush)!=NULL)
		pDC->SelectObject(&m_Brush);
	return TRUE;	
}



//Метод восстанавливает контекст устройства
//Входные данные: указатель на контекст устройства вывода
//Выходные данные: TRUE или FALSE в зависимости от успешности операции
BOOL CShapes::_RestoreDC(CDC *pDC)
{
	ASSERT(pDC);
	return pDC->RestoreDC(STEP_BACK); //восстанавливаем контекст на одно состояние назад
}