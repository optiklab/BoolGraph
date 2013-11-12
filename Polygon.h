#pragma once
#include "Shapes.h"

//Класс CPolygon описывает произвольный многоугольник и методы его рисования
class CPolygon :public CShapes
{
DECLARE_SERIAL(CPolygon)

public:
	__declspec(nothrow) CPolygon();
	__declspec(nothrow) ~CPolygon();

protected:
	//переопределенный метод
	//выполняет сохранение и восстановление данных о полигоне
	__declspec(nothrow) VOID Serialize(CArchive& ar);

private:
	//переопределенный метод рисования полигона
	__declspec(nothrow) VOID Show(CDC *pDC, BOOL);

	//переопределенный метод: возвращает область, занимаемую полигоном
	__declspec(nothrow) VOID GetRegion(CRgn &Rgn);
};