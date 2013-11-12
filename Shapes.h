#pragma once
#include "atltypes.h"


//структура RIB - описывает ребро полигона
struct SEdge
{
	CPoint ptVertex1; //координаты двух точек ребра
	CPoint ptVertex2;
	UINT nParent;  //номер многоугольника - родителя
};



// класс CShapes
// Класс, описывающий общие свойства фигур и методы работы с ними
class CShapes :public CObject
{
DECLARE_SERIAL(CShapes)
public:
	__declspec(nothrow) CShapes();
	__declspec(nothrow) virtual ~CShapes();

public:
	//рисует фигуру на экране
	__declspec(nothrow) virtual VOID Show(CDC *pDC, BOOL);

	//добавляет новую вершину фигуры в конец массива
	__declspec(nothrow) VOID AddPoint(const CPoint &);

	//удаляет одну из вершин фигуры
	__declspec(nothrow) VOID DeletePoint(const INT &);

	//устанавливает параметры пера
	__declspec(nothrow) BOOL SetPen(const COLORREF, const UINT, const UINT &);

	//устанавливает параметры заливки
	__declspec(nothrow) BOOL SetBrush(const COLORREF, const INT);

	//записывает имя фигуры
	__declspec(nothrow) VOID SetShapeName(const CString &);

	//сообщает область региона
	__declspec(nothrow) virtual VOID GetRegion(CRgn &Rgn);

	//возвращает текущий стиль пера
	__declspec(nothrow) UINT GetPenStyle()const;

	//возвращает текущую толщину пера
	__declspec(nothrow) UINT GetPenWidth()const;

	//возвращает текущий цвет пера
	__declspec(nothrow) COLORREF GetPenColor()const;

	//возвращает текущий стиль заливки
	__declspec(nothrow) UINT GetBrushStyle()const;

	//возвращает текущий цвет заливки
	__declspec(nothrow) COLORREF GetBrushColor()const;

	//возвращает вершину фигуры по её номеру
	__declspec(nothrow) CPoint GetPoint(const INT&)const;

	//возвращает количество вершин фигуры
	__declspec(nothrow) INT GetPointNumber()const;

	//возвращает имя фигуры
	__declspec(nothrow) CString GetShapeName()const;

protected:
	//выполняет сохранение в файл и восстановление фигур из файлов
	virtual VOID Serialize(CArchive& ar);                  //exception

	//выполняет подготовку контекста устройства для вывода изображения
	__declspec(nothrow) BOOL _PrepareDC(CDC *pDC);

	//выполняет восстановление контекста устройства
	__declspec(nothrow) BOOL _RestoreDC(CDC *pDC);

protected:
	CArray <CPoint> m_ShapePoints;    //координаты вершин фигур

private:
	CString m_sShapeName;
	CPen m_Pen;
	CBrush m_Brush;
	UINT m_nPenStyle;
	UINT m_nPenWidth;
	COLORREF m_rgbPenColor; 
	INT m_nBrushStyle;
	COLORREF m_rgbBrushColor;
};
