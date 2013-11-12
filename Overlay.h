#pragma once
#include "afx.h"
#include "Shapes.h"
#include "Polygon.h"
#include "atltypes.h"

//����� COverlay
//�������� �� �������� �������� �����������, �������� ��� ����������� ���� �����
class COverlay:public CObject
{
public:
	__declspec(nothrow) COverlay();
	__declspec(nothrow) ~COverlay();
	
	//������� ��������� ��������� ����������� �������� ��� ��������
	__declspec(nothrow) VOID MakeOperation(CShapes *, CShapes *, const UINT &);

	//������� �������� ������ ����� ����� �� ������� ������
	__declspec(nothrow) VOID GetResult(CArray <SEdge>*)const;

private:
	//������� ��������� ��������� ������ �� ������ ���������������� �����
	__declspec(nothrow) VOID ModelGenerator();

	//����� ��������� ������ ��������� �������������� �����
	__declspec(nothrow) VOID TempEdgesGenerator(const CShapes*, const UINT &);

	//����� ���������, ����� ����� �� ��������� ��� ����������� ������
	__declspec(nothrow) BOOL IsEdgeAlone(const SEdge &)const;

	//����� ��������� ������ ������
	__declspec(nothrow) VOID VertexGenerator();

	//������� ��������� ����� ������� �������� ������ ����� �� ���������� �������
	__declspec(nothrow) BOOL FindAroundEdge(const UINT &);

	//����� ���������� ����� ������ �������� ����� � �������
	__declspec(nothrow) INT GetMax()const;

	//����� ����������, ���� �� ��������� ����� � ������� �����
	__declspec(nothrow) BOOL EdgeAlreadyInModel(const SEdge &)const;

	//������� ��������� ����� �������������� �����, �� �������� � ������
	__declspec(nothrow) BOOL FindIntersectInModel(const INT &);

	//����� ������� ����� ����������� ���� ��������
	__declspec(nothrow) BOOL IsLinesCross(const SEdge &, const SEdge &, CPoint *);

	//����� ��������� ����, ����������� � ������-���������� ��������
	__declspec(nothrow) VOID SetResultEdges(const UINT &);

	//����� ����������, ����� �� ����� ������ �������� (��������� �� ������� - �� �����������)
	__declspec(nothrow) BOOL PointInPolygon(const CPoint &, const CArray <SEdge>*)const;

private:
	CShapes *pFirstShape;       //��������� �� ��� ������
	CShapes *pSecondShape;      
	CArray <SEdge> Edges;       // ��������� ��������� ����� ���� �����
	CArray <SEdge> ModelEdges;  //������������ ��������� ���������������� �����, ���������� - ������
	CArray <CPoint> Points;     //������ ������ ������
};
