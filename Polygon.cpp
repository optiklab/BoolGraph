#include "StdAfx.h"
#include "Polygon.h"
#include "Shapes.h"
#include "math.h"


CPolygon::CPolygon():CShapes()
{}

CPolygon::~CPolygon()
{}

IMPLEMENT_SERIAL(CPolygon, CObject, 1)




//Переопределение метода сохранения и восстановления
VOID CPolygon::Serialize(CArchive &ar)
{
	CShapes::Serialize(ar);
}



//Переопределение метода рисования
//Входные данные: указатель на контекст устройства и флаг необходимости заливки фигур
VOID CPolygon::Show(CDC* pDC, BOOL bIsBrush)
{
	ASSERT(pDC);
	CShapes::_PrepareDC(pDC);
	if(bIsBrush)
		pDC->Polygon(m_ShapePoints.GetData(),(INT)m_ShapePoints.GetSize());
	else
	{
		INT nI=0;
		for(nI=0; nI<(INT)m_ShapePoints.GetSize()-1; nI++)
		{ 
			pDC->MoveTo(m_ShapePoints.GetAt(nI));
			pDC->LineTo(m_ShapePoints.GetAt(nI+1));
		}
		pDC->MoveTo(m_ShapePoints.GetAt(nI));
		pDC->LineTo(m_ShapePoints.GetAt(0));
	}
	CShapes::_RestoreDC(pDC);
}




//Метод взятия области
//создает полигональную область, занимаемую многоугольником, по его вершинам
//Входные данные: адрес на переменную-регион в которую производится запись данных
VOID CPolygon::GetRegion(CRgn &Rgn)
{
	Rgn.CreatePolygonRgn(m_ShapePoints.GetData(),(INT)m_ShapePoints.GetSize(), ALTERNATE);
}