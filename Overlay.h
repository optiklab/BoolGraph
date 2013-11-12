#pragma once
#include "afx.h"
#include "Shapes.h"
#include "Polygon.h"
#include "atltypes.h"

//Класс COverlay
//Отвечает за создание оверлеев пересечения, разности или объединения двух фигур
class COverlay:public CObject
{
public:
	__declspec(nothrow) COverlay();
	__declspec(nothrow) ~COverlay();
	
	//функция управляет процессом прохождения операции над фигурами
	__declspec(nothrow) VOID MakeOperation(CShapes *, CShapes *, const UINT &);

	//функция копирует массив ребер фигур во внешний массив
	__declspec(nothrow) VOID GetResult(CArray <SEdge>*)const;

private:
	//функция выполняет разбиение фигуры на список непересекающихся ребер
	__declspec(nothrow) VOID ModelGenerator();

	//метод формирует список временных необработанных ребер
	__declspec(nothrow) VOID TempEdgesGenerator(const CShapes*, const UINT &);

	//метод проверяет, чтобы ребро не повторяло уже вставленное массив
	__declspec(nothrow) BOOL IsEdgeAlone(const SEdge &)const;

	//метод формирует список вершин
	__declspec(nothrow) VOID VertexGenerator();

	//функция выполняет поиск вершины полигона вокруг ребра на расстоянии Эпсилон
	__declspec(nothrow) BOOL FindAroundEdge(const UINT &);

	//метод возвращает номер самого длинного ребра в массиве
	__declspec(nothrow) INT GetMax()const;

	//метод определяет, есть ли указанное ребро в массиве ребер
	__declspec(nothrow) BOOL EdgeAlreadyInModel(const SEdge &)const;

	//функция выполняет поиск пересекающихся ребер, их удаление и замену
	__declspec(nothrow) BOOL FindIntersectInModel(const INT &);

	//метод находит точку пересечения двух отрезков
	__declspec(nothrow) BOOL IsLinesCross(const SEdge &, const SEdge &, CPoint *);

	//метод выделения рёбер, относящихся к фигуре-результату операции
	__declspec(nothrow) VOID SetResultEdges(const UINT &);

	//метод определяет, лежит ли точка внутри полигона (положение на границе - не учитывается)
	__declspec(nothrow) BOOL PointInPolygon(const CPoint &, const CArray <SEdge>*)const;

private:
	CShapes *pFirstShape;       //указатели на две фигуры
	CShapes *pSecondShape;      
	CArray <SEdge> Edges;       // начальное множество ребер двух фигур
	CArray <SEdge> ModelEdges;  //обработанное множество непересекающихся ребер, сокращенно - Модель
	CArray <CPoint> Points;     //список вершин Модели
};
