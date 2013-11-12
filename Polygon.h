#pragma once
#include "Shapes.h"

//����� CPolygon ��������� ������������ ������������� � ������ ��� ���������
class CPolygon :public CShapes
{
DECLARE_SERIAL(CPolygon)

public:
	__declspec(nothrow) CPolygon();
	__declspec(nothrow) ~CPolygon();

protected:
	//���������������� �����
	//��������� ���������� � �������������� ������ � ��������
	__declspec(nothrow) VOID Serialize(CArchive& ar);

private:
	//���������������� ����� ��������� ��������
	__declspec(nothrow) VOID Show(CDC *pDC, BOOL);

	//���������������� �����: ���������� �������, ���������� ���������
	__declspec(nothrow) VOID GetRegion(CRgn &Rgn);
};