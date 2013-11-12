#pragma once
#include "atltypes.h"


//��������� RIB - ��������� ����� ��������
struct SEdge
{
	CPoint ptVertex1; //���������� ���� ����� �����
	CPoint ptVertex2;
	UINT nParent;  //����� �������������� - ��������
};



// ����� CShapes
// �����, ����������� ����� �������� ����� � ������ ������ � ����
class CShapes :public CObject
{
DECLARE_SERIAL(CShapes)
public:
	__declspec(nothrow) CShapes();
	__declspec(nothrow) virtual ~CShapes();

public:
	//������ ������ �� ������
	__declspec(nothrow) virtual VOID Show(CDC *pDC, BOOL);

	//��������� ����� ������� ������ � ����� �������
	__declspec(nothrow) VOID AddPoint(const CPoint &);

	//������� ���� �� ������ ������
	__declspec(nothrow) VOID DeletePoint(const INT &);

	//������������� ��������� ����
	__declspec(nothrow) BOOL SetPen(const COLORREF, const UINT, const UINT &);

	//������������� ��������� �������
	__declspec(nothrow) BOOL SetBrush(const COLORREF, const INT);

	//���������� ��� ������
	__declspec(nothrow) VOID SetShapeName(const CString &);

	//�������� ������� �������
	__declspec(nothrow) virtual VOID GetRegion(CRgn &Rgn);

	//���������� ������� ����� ����
	__declspec(nothrow) UINT GetPenStyle()const;

	//���������� ������� ������� ����
	__declspec(nothrow) UINT GetPenWidth()const;

	//���������� ������� ���� ����
	__declspec(nothrow) COLORREF GetPenColor()const;

	//���������� ������� ����� �������
	__declspec(nothrow) UINT GetBrushStyle()const;

	//���������� ������� ���� �������
	__declspec(nothrow) COLORREF GetBrushColor()const;

	//���������� ������� ������ �� � ������
	__declspec(nothrow) CPoint GetPoint(const INT&)const;

	//���������� ���������� ������ ������
	__declspec(nothrow) INT GetPointNumber()const;

	//���������� ��� ������
	__declspec(nothrow) CString GetShapeName()const;

protected:
	//��������� ���������� � ���� � �������������� ����� �� ������
	virtual VOID Serialize(CArchive& ar);                  //exception

	//��������� ���������� ��������� ���������� ��� ������ �����������
	__declspec(nothrow) BOOL _PrepareDC(CDC *pDC);

	//��������� �������������� ��������� ����������
	__declspec(nothrow) BOOL _RestoreDC(CDC *pDC);

protected:
	CArray <CPoint> m_ShapePoints;    //���������� ������ �����

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
