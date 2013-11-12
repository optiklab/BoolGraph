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

//инициализируем поля класса
COverlay::COverlay():pFirstShape(NULL),
					 pSecondShape(NULL)
{
	Edges.SetSize(ZERO_SIZE,SINGLE_STEP);  
	ModelEdges.SetSize(ZERO_SIZE,SINGLE_STEP);
	Points.SetSize(ZERO_SIZE,SINGLE_STEP);  
}



COverlay::~COverlay()
{
	Edges.RemoveAll();
	ModelEdges.RemoveAll();
	Points.RemoveAll();
}



//Функция управляет процессом прохождения операции над фигурами
//Входные данные: два указателя на фигуры и номер операции
VOID COverlay::MakeOperation(CShapes *pFirst, CShapes *pSecond, const UINT &nOperation)
{
	ASSERT(pFirst);
	ASSERT(pSecond);
	ASSERT(nOperation);
	pFirstShape = pFirst;
	pSecondShape = pSecond;
	ModelGenerator();  //создаем массив непересекающихся отрезков
	SetResultEdges(nOperation); //выделяем отрезки, соответствующие операции
}




//Функция копирует массив ребер фигур-результатов во внешний, по отношению к классу, массив
//Входные данные: указатель на массив, в который необходимо скопировать данные
VOID COverlay::GetResult(CArray <SEdge>* pModel)const
{
	ASSERT(pModel);
	for(INT nI = 0; nI < ModelEdges.GetSize(); nI++)
		pModel->Add(ModelEdges.GetAt(nI));
}



//Функция выполняет разбиение фигуры на список непересекающихся ребер
//Все данные берутся из перед этим инициализированных полей класса, 
//и туда же записывается результат
 VOID COverlay::ModelGenerator()
 {
	//в массив необработанных ребер записываем все ребра обеих фигур
	TempEdgesGenerator(pFirstShape, FIRST_SHAPE);   
	TempEdgesGenerator(pSecondShape, SECOND_SHAPE);
	VertexGenerator();
	INT nI = 0, nIntersectCounter = 0;
	while(Edges.IsEmpty() == FALSE)
	{
		nI = GetMax();    //находим номер самого длинного отрезка
		if(EdgeAlreadyInModel(Edges.GetAt(nI)) == FALSE) //если такого ребра в Модели нет
		{
			//И если ребро не пересекается с другими ребрами - вставляем его в Модель
			if(FindIntersectInModel(nI) == FALSE && FindAroundEdge(nI) == FALSE)
			{
				ModelEdges.Add(Edges.GetAt(nI));
				Edges.RemoveAt(nI, ONE_ELEMENT);
			}
			else
				nIntersectCounter += 1;
		}
		else
			Edges.RemoveAt(nI,ONE_ELEMENT);
	}
	if(nIntersectCounter == 0)
		AfxMessageBox(MSG_DIFFERENT_SHAPES);
}





//метод формирует список временных необработанных ребер
//Входные данные: указатель на фигуру, из которой берутся ребра и массив, куда необходимо записать ребра
//Выходные данные: сформированный массив ребер
VOID COverlay::TempEdgesGenerator(const CShapes* pShape, const UINT &nParent)
{
	ASSERT(nParent == FIRST_SHAPE || nParent == SECOND_SHAPE);
	ASSERT(pShape);
	SEdge TempEdge;
	for(INT nI = 1; nI < pShape->GetPointNumber(); nI++)
	{
		TempEdge.ptVertex1=pShape->GetPoint(nI - 1);
		TempEdge.ptVertex2=pShape->GetPoint(nI);
		TempEdge.nParent = nParent;
		if(IsEdgeAlone(TempEdge) == TRUE)
			Edges.Add(TempEdge);
		if(nI + 1 == pShape->GetPointNumber())
		{
			TempEdge.ptVertex1 = pShape->GetPoint(nI);
			TempEdge.ptVertex2 = pShape->GetPoint(0);
			TempEdge.nParent = nParent;
			if(IsEdgeAlone(TempEdge) == TRUE)
				Edges.Add(TempEdge);
		}
	}
}




//Метод проверяет, чтобы ребро не повторяло уже вставленное массив
//Входные данные: ребро, которое необходимо проверить
//Выходные данные: false - такое же ребро есть в массиве, true - нет
BOOL COverlay::IsEdgeAlone(const SEdge &TempEdge)const
{
	for(INT nI = 0; nI < Edges.GetSize(); nI++)
		if((TempEdge.ptVertex1 == Edges.GetAt(nI).ptVertex1 && TempEdge.ptVertex2 == Edges.GetAt(nI).ptVertex2) ||
			(TempEdge.ptVertex1 == Edges.GetAt(nI).ptVertex2 && TempEdge.ptVertex2 == Edges.GetAt(nI).ptVertex1))
			return FALSE;
	return TRUE;
}





//Метод формирует начальный список вершин двух фигур
VOID COverlay::VertexGenerator()
{
	for(INT nI = 0; nI < pFirstShape->GetPointNumber(); nI++)
	{
		//проверка на несовпадение вставляемой вершины с ранее вставленной
		for(INT nJ = 0; nJ < Points.GetSize(); nJ++)
			if(pFirstShape->GetPoint(nI) == Points.GetAt(nJ))
				break;
		Points.Add(pFirstShape->GetPoint(nI));
	}		

	for(INT nI = 0; nI < pSecondShape->GetPointNumber(); nI++)
	{
		for(INT nJ = 0; nJ < Points.GetSize(); nJ++)
			if(pSecondShape->GetPoint(nI)==Points.GetAt(nJ))
				break;
		Points.Add(pSecondShape->GetPoint(nI));
	}		
}





//Функция выполняет поиск вершины полигона вокруг ребра на расстоянии Эпсилон
//Входные данные: номер ребра, рядом с которые выполняется поиск
//Выходные данные: найдена или нет такая точка
BOOL COverlay::FindAroundEdge(const UINT &EdgeNumber)
{
	ASSERT(EdgeNumber >= 0);
	for(INT nI = 0; nI < Points.GetSize(); nI++)
	{
		CPoint TempPointOne = Edges.GetAt(EdgeNumber).ptVertex1;
		CPoint TempPointTwo = Edges.GetAt(EdgeNumber).ptVertex2;
		DOUBLE Length = 0.0;
		Length = abs((TempPointOne.y - TempPointTwo.y)*Points.GetAt(nI).x + (TempPointOne.x - TempPointTwo.x)*Points.GetAt(nI).y + 
			(TempPointOne.x * TempPointTwo.y - TempPointTwo.x * TempPointOne.y))/
			sqrt(pow((DOUBLE)(TempPointOne.x - TempPointTwo.x),2) + pow((DOUBLE)(TempPointOne.y - TempPointTwo.y),2));
		if(Length <= EPSILON)
		{
			if(Points.GetAt(nI).x > min(TempPointOne.x,TempPointTwo.x) && Points.GetAt(nI).x < max(TempPointOne.x,TempPointTwo.x))
			{
				if(Points.GetAt(nI).y > min(TempPointOne.y,TempPointTwo.y) && Points.GetAt(nI).y < max(TempPointOne.y,TempPointTwo.y))
				{
					SEdge Rib1 = {Points.GetAt(nI), TempPointOne, Edges.GetAt(EdgeNumber).nParent};
					Edges.Add(Rib1);
					SEdge Rib2 = {Points.GetAt(nI), TempPointTwo, Edges.GetAt(EdgeNumber).nParent};
					Edges.Add(Rib2);
					Points.Add(Points.GetAt(nI));
					Edges.RemoveAt(EdgeNumber,ONE_ELEMENT);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}



//Метод возвращает номер самого длинного ребра в массиве
//Входные данные: указатель на массив, в котором производится поиск
//Выходные данные: номер максимального элемента - ребра
INT COverlay::GetMax()const
{
	INT nMax=0;
	for(INT nI=1; nI<Edges.GetSize(); nI++)
	{
		FLOAT DMAX  = sqrt(pow((FLOAT)(Edges.GetAt(nMax).ptVertex1.x - Edges.GetAt(nMax).ptVertex2.x),2)+
		   pow((FLOAT)(Edges.GetAt(nMax).ptVertex1.y - Edges.GetAt(nMax).ptVertex2.y),2));
		FLOAT D = sqrt(pow((FLOAT)(Edges.GetAt(nI).ptVertex1.x - Edges.GetAt(nI).ptVertex2.x),2)+
		   pow((FLOAT)(Edges.GetAt(nI).ptVertex1.y - Edges.GetAt(nI).ptVertex2.y),2));
		if(DMAX<D)
		  nMax=nI;
	}
	return nMax;
}






//Метод определяет, есть ли указанное ребро в списке ребер Модели - среди уже обработанных ребер
//Входные данные: ребро, и указатель на массив ребер Модели
//Выходные данные: да - TRUE или нет - FALSE
BOOL COverlay::EdgeAlreadyInModel(const SEdge &SearchEdge)const
{
	ASSERT(SearchEdge.ptVertex1.x >= 0 && SearchEdge.ptVertex1.y >= 0);
	ASSERT(SearchEdge.ptVertex2.x >= 0 && SearchEdge.ptVertex2.y >= 0);
	for(INT nI=0; nI<ModelEdges.GetSize(); nI++)
	   if((SearchEdge.ptVertex1 == ModelEdges.GetAt(nI).ptVertex1 && SearchEdge.ptVertex2 == ModelEdges.GetAt(nI).ptVertex2) && 
		   (SearchEdge.ptVertex1 == ModelEdges.GetAt(nI).ptVertex2 && SearchEdge.ptVertex2 == ModelEdges.GetAt(nI).ptVertex1))
				return TRUE;
	return FALSE;
}






//Функция определяет наличие пересечения указанного ребра с ребрами в массиве модели ModelEdges
//Если это так, удаляет ребра из модели и вставляет новые ребра, образованные
//вокруг точки пересечения в список необработанных ребер Edges
//Входные данные: номер ребра из массива необработанных ребер
//Выходные данные: да(TRUE) - если есть пересечения, иначе - FALSE
BOOL COverlay::FindIntersectInModel(const INT &nNumber)
{
	ASSERT(nNumber >= 0);

	CPoint ptIntersect;
	for(INT nI=0; nI<ModelEdges.GetSize(); nI++)
		if(IsLinesCross(Edges.GetAt(nNumber), ModelEdges.GetAt(nI), &ptIntersect) == TRUE)
		{
			SEdge Rib1 = {ptIntersect, Edges.GetAt(nNumber).ptVertex1, Edges.GetAt(nNumber).nParent};
			Edges.Add(Rib1);
			SEdge Rib2 = {ptIntersect, Edges.GetAt(nNumber).ptVertex2, Edges.GetAt(nNumber).nParent};
			Edges.Add(Rib2);
			SEdge Rib3 = {ptIntersect, ModelEdges.GetAt(nI).ptVertex1, ModelEdges.GetAt(nI).nParent};
			Edges.Add(Rib3);
			SEdge Rib4 = {ptIntersect, ModelEdges.GetAt(nI).ptVertex2, ModelEdges.GetAt(nI).nParent};
			Edges.Add(Rib4);
			Points.Add(ptIntersect);
			Edges.RemoveAt(nNumber,ONE_ELEMENT);
			ModelEdges.RemoveAt(nI,ONE_ELEMENT);
			return TRUE;
		}
	   return FALSE;
}



//Метод находит точку пересечения двух отрезков
//Входные данные: два отрезка и указатель на точку, в которую надо записать координаты точки пересечения
//Выходные данные: в случае наличия факта пересечения,
//функция возвращает TRUE и заносит координаты точки пересечения отрезков в переменную
BOOL COverlay::IsLinesCross(const SEdge &Edge1, const SEdge &Edge2, CPoint *pIntersect)
{
	ASSERT(Edge1.ptVertex1.x >= 0 && Edge1.ptVertex1.y >= 0);
	ASSERT(Edge2.ptVertex1.x >= 0 && Edge1.ptVertex2.y >= 0);
	ASSERT(pIntersect);
	CPoint ptDir1 = Edge1.ptVertex2 - Edge1.ptVertex1;
	CPoint ptDir2 = Edge2.ptVertex2 - Edge2.ptVertex1;

	//считаем уравнения прямых проходящих через отрезки
    DOUBLE a1 = (-1)*(DOUBLE)ptDir1.y;
    DOUBLE b1 = (DOUBLE)ptDir1.x;
    DOUBLE d1 = (-1)*(a1*Edge1.ptVertex1.x + b1*Edge1.ptVertex1.y);

    DOUBLE a2 = (-1)*(DOUBLE)ptDir2.y;
    DOUBLE b2 = (DOUBLE)ptDir2.x;
    DOUBLE d2 = (-1)*(a2*Edge2.ptVertex1.x + b2*Edge2.ptVertex1.y);

    //подставляем концы отрезков, для выяснения в каких полуплоскотях они
    DOUBLE Seg1Start = a2*(DOUBLE)Edge1.ptVertex1.x + b2*(DOUBLE)Edge1.ptVertex1.y + d2;
    DOUBLE Seg1End = a2*(DOUBLE)Edge1.ptVertex2.x + b2*(DOUBLE)Edge1.ptVertex2.y + d2;

    DOUBLE Seg2Start = a1*(DOUBLE)Edge2.ptVertex1.x + b1*(DOUBLE)Edge2.ptVertex1.y + d1;
    DOUBLE Seg2End = a1*(DOUBLE)Edge2.ptVertex2.x + b1*(DOUBLE)Edge2.ptVertex2.y + d1;

    //если концы одного отрезка имеют один знак, значит он в одной полуплоскости и пересечения нет.
    if ((Seg1Start * Seg1End) > 0 || (Seg2Start * Seg2End) >= 0)    
        return FALSE;
	//Примечание: Если условие здесь будет такого вида ((Seg1Start * Seg1End) >= 0 || (Seg2Start * Seg2End) >= 0)
	//то два отрезка лежащие на одной прямой будут считаться не пересекающимися
    DOUBLE u = (Seg1Start / (Seg1Start - Seg1End));
    pIntersect->x = (INT) ((DOUBLE)Edge1.ptVertex1.x + u * (DOUBLE)ptDir1.x);
	pIntersect->y = (INT) ((DOUBLE)Edge1.ptVertex1.y + u * (DOUBLE)ptDir1.y);
    return TRUE;
}



//Метод выделения рёбер, относящихся к фигуре-результату операции
//Входные данные: номер операции
//Выходные данные: в массиве Модели остаются только ребра, удовлетворяющие условиям операции
VOID COverlay::SetResultEdges(const UINT &nOperation)
{
	ASSERT(nOperation);
	CArray <SEdge> ResultEdges;
	ResultEdges.SetSize(ZERO_SIZE, SINGLE_STEP);
	CArray <SEdge> FirstPolygon;
	FirstPolygon.SetSize(ZERO_SIZE, SINGLE_STEP);
	CArray <SEdge> SecondPolygon;
	SecondPolygon.SetSize(ZERO_SIZE, SINGLE_STEP);
	 //делим ребра по принадлежности к фигуре на два массива
	for(INT nJ=0; nJ<ModelEdges.GetSize(); nJ++)
	{
		if(ModelEdges.GetAt(nJ).nParent == FIRST_SHAPE)
			FirstPolygon.Add(ModelEdges.GetAt(nJ));
		else if(ModelEdges.GetAt(nJ).nParent == SECOND_SHAPE)
			SecondPolygon.Add(ModelEdges.GetAt(nJ));
		else 
			AfxMessageBox(MSG_PARENT_ERROR);
	}
	BOOL Vertex1, Vertex2, Vertex3;
	CPoint ptPoint;
	//выделяем подходящие по условиям отрезки из первой фигуры
	for(INT nJ=0; nJ<FirstPolygon.GetSize(); nJ++)  
	{
		//координаты средней точки между двумя вершинами ребра
		ptPoint.x = ((INT)(abs(FirstPolygon.GetAt(nJ).ptVertex2.x - FirstPolygon.GetAt(nJ).ptVertex1.x)/2)+
			min(FirstPolygon.GetAt(nJ).ptVertex2.x, FirstPolygon.GetAt(nJ).ptVertex1.x));
		ptPoint.y = ((INT)(abs(FirstPolygon.GetAt(nJ).ptVertex2.y - FirstPolygon.GetAt(nJ).ptVertex1.y)/2)+
			min(FirstPolygon.GetAt(nJ).ptVertex2.y, FirstPolygon.GetAt(nJ).ptVertex1.y));
		Vertex1 = PointInPolygon(FirstPolygon.GetAt(nJ).ptVertex1,&SecondPolygon);
		Vertex2 = PointInPolygon(FirstPolygon.GetAt(nJ).ptVertex2,&SecondPolygon);
		Vertex3 = PointInPolygon(ptPoint,&SecondPolygon);
		switch(nOperation)
		{
			case OP_UNION:
				if((Vertex1==FALSE) || (Vertex2==FALSE) || (Vertex3==FALSE))
					ResultEdges.Add(FirstPolygon.GetAt(nJ));
				break;
			case OP_INTERSECT:
				if((Vertex1==TRUE) && (Vertex2==TRUE) && (Vertex3==TRUE))
					ResultEdges.Add(FirstPolygon.GetAt(nJ));
				break;
			case OP_DIFFERENCE:
				if((Vertex1==FALSE) || (Vertex2==FALSE) || (Vertex3==FALSE))
					ResultEdges.Add(FirstPolygon.GetAt(nJ));
				break;
		}
	}
	//выделяем подходящие по условиям отрезки из второй фигуры
	for(INT nJ=0; nJ<SecondPolygon.GetSize(); nJ++)
	{
		ptPoint.x = ((INT)(abs(SecondPolygon.GetAt(nJ).ptVertex2.x - SecondPolygon.GetAt(nJ).ptVertex1.x)/2)+
			min(SecondPolygon.GetAt(nJ).ptVertex2.x,SecondPolygon.GetAt(nJ).ptVertex1.x));
		ptPoint.y = ((INT)(abs(SecondPolygon.GetAt(nJ).ptVertex2.y - SecondPolygon.GetAt(nJ).ptVertex1.y)/2)+
			min(SecondPolygon.GetAt(nJ).ptVertex2.y,SecondPolygon.GetAt(nJ).ptVertex1.y));
		Vertex1 = PointInPolygon(SecondPolygon.GetAt(nJ).ptVertex1,&FirstPolygon);
		Vertex2 = PointInPolygon(SecondPolygon.GetAt(nJ).ptVertex2,&FirstPolygon);
		Vertex3 = PointInPolygon(ptPoint,&FirstPolygon);
		switch(nOperation)
		{
			case OP_UNION:
				if((Vertex1==FALSE) || (Vertex2==FALSE) || (Vertex3==FALSE))
					ResultEdges.Add(SecondPolygon.GetAt(nJ));
				break;
			case OP_INTERSECT:
				if((Vertex1==TRUE) && (Vertex2==TRUE) && (Vertex3==TRUE))
					ResultEdges.Add(SecondPolygon.GetAt(nJ));
				break;
			case OP_DIFFERENCE:
				if((Vertex1==TRUE) && (Vertex2==TRUE) && (Vertex3==TRUE))
					ResultEdges.Add(SecondPolygon.GetAt(nJ));
				break;
		}
	}
	ModelEdges.RemoveAll();
	for(INT nJ=0; nJ<ResultEdges.GetSize(); nJ++)
		ModelEdges.Add(ResultEdges.GetAt(nJ));
	ResultEdges.RemoveAll();
	FirstPolygon.RemoveAll();
	SecondPolygon.RemoveAll();
}




//Функция определяет, лежит ли точка внутри полигона методом Линии
//Входные данные: координаты точки(переменная CPoint), и массив с ребрами многоугольника
//Выходные данные: ДА или НЕТ
BOOL COverlay::PointInPolygon(const CPoint &ptPoint, const CArray <SEdge>* pPolygon)const
{
	ASSERT(ptPoint.x >= 0);
	ASSERT(ptPoint.y >= 0);
	ASSERT(pPolygon);
	INT nParity = 0;
	INT nXCoord = ptPoint.x;
	INT nYCoord = ptPoint.y;
	INT nVertexY1 = 0, nVertexX1 = 0, nVertexY2 = 0, nVertexX2 = 0;
	DOUBLE YUp = 0.0, YLow = 0.0, WorkX = 0.0;
	//сразу проверяем не совпадает ли точка с какой-нибудь вершиной
	for (INT nI=0; nI<pPolygon->GetSize(); nI++)
		if(ptPoint == pPolygon->GetAt(nI).ptVertex1 || ptPoint == pPolygon->GetAt(nI).ptVertex2)
			return TRUE;
	//считаем количество пересечений луча из точки и ребер фигуры
	for (INT nI=0; nI<pPolygon->GetSize(); nI++)
	{
		nVertexY1=pPolygon->GetAt(nI).ptVertex1.y;
		nVertexX1=pPolygon->GetAt(nI).ptVertex1.x;
		nVertexY2=pPolygon->GetAt(nI).ptVertex2.y;
		nVertexX2=pPolygon->GetAt(nI).ptVertex2.x;
		YUp = nVertexY1>nVertexY2?nVertexY1:nVertexY2;
		YLow = nVertexY1<nVertexY2?nVertexY1:nVertexY2;
		if (nVertexY2 - nVertexY1)
			WorkX = nVertexX1 + (nVertexX2 - nVertexX1)*(nYCoord - nVertexY1)/(nVertexY2 - nVertexY1);
		else
			WorkX = nVertexX1;
		if (YUp >= nYCoord) 
			if (YLow < nYCoord)
			{
				if (nXCoord > WorkX)
					nParity++;
				if (fabs(nXCoord - WorkX) < 0.00001) return 1;
			}
		if ((fabs((DOUBLE)nYCoord-YLow)<0.00001) && (fabs(YUp-YLow)<0.00001) && 
			(fabs(fabs(WorkX-(DOUBLE)nVertexX1)+fabs(WorkX-(DOUBLE)nVertexX2)-fabs((DOUBLE)nVertexX1-(DOUBLE)nVertexX2))<0.0001))
			return TRUE;
	}
	if (nParity%2)
		return TRUE;
	else
		return FALSE;
}
