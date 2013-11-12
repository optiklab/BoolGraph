#include "StdAfx.h"
#include "Shapes.h"
#include "GlobalData.h"



//������������� ����� ������
CShapes::CShapes(): m_sShapeName(DEFAULT_NAME),  //��� �� ���������
					m_nPenStyle(PS_SOLID),       //����� ��������
					m_nPenWidth(THEME_WIDTH_1),  //�������
					m_rgbPenColor(RGB(0,0,0)),   //���� - ������
					m_nBrushStyle(SOLID_BRUSH),  //��� ���������
					m_rgbBrushColor(RGB(255,255,255)) //������� �����
{
	m_ShapePoints.SetSize(ZERO_SIZE,SINGLE_STEP);
}



CShapes::~CShapes()
{
	m_ShapePoints.RemoveAll();
}


IMPLEMENT_SERIAL(CShapes,CObject,VERSIONABLE_SCHEMA|1)





//����� ������������ ������
//���������� � �������-�������� �����
VOID CShapes::Show(CDC *pDC, BOOL bIfPaint)
{}





//��������� ����� ������� ������ � ����� �������
//������� ���������: ����� �� ����������, ���������� �������
VOID CShapes::AddPoint(const CPoint &ptPoint)
{
	ASSERT(ptPoint.x >= 0);
	ASSERT(ptPoint.y >= 0);
	m_ShapePoints.Add(ptPoint);
}







//������� ���� �� ������ ������
//������� ������: ������ �������� ��� ��������
//�������� ������:  TRUE ��� FALSE - ������� ��� ��� ��������� ��������
VOID CShapes::DeletePoint(const INT &iIndex)
{
	ASSERT(iIndex >= 0);
	m_ShapePoints.RemoveAt(iIndex,1);
}





//����� ������������� ���� � ������������ �����������
//������� ���������: ���� � ������ �����, ����� ���������� �����
//�������� ��������: TRUE - ���� �������� ���������� ��� ������, ����� FALSE
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






//����� ������������� �������
//������� ���������: ����, ����� ���������� �����
//�������� ��������: TRUE - ���� �������� ���������� ��� ������, ����� FALSE
BOOL CShapes::SetBrush(const COLORREF rgbColor, const INT iStyle)
{
	m_nBrushStyle=iStyle;
	m_rgbBrushColor=rgbColor;
	if(HBRUSH(m_Brush)!=NULL)
		if(!m_Brush.DeleteObject())
			return FALSE;
	if(m_nBrushStyle>=0)      //���� ������ ����� ���������
		return m_Brush.CreateHatchBrush(m_nBrushStyle, m_rgbBrushColor);
	return m_Brush.CreateSolidBrush(m_rgbBrushColor);
}







//���������� ��� ������
//������� ������: ����� �� ������ � ������ ������
VOID CShapes::SetShapeName(const CString &sName)
{
	ASSERT(sName);
	m_sShapeName=sName;
}
	


//����� ���������� �������, ������� �������
//���������� � �������-�������� �����
VOID CShapes::GetRegion(CRgn &Region)
{}


//���������� ������� ����� ����
//�������� ������: ����������� ����� ����� - ����� ����� 
UINT CShapes::GetPenStyle()const
{
	return m_nPenStyle;
}


//���������� ������� ������ ����
//�������� ������: ����������� ����� ����� - ������
UINT CShapes::GetPenWidth()const
{
	return m_nPenWidth;
}


//���������� ������� ���� ����
//�������� ������: ������� �����, ���������� �������� ���������� RGB - ���� ����
COLORREF CShapes::GetPenColor()const
{
	return m_rgbPenColor;
}


//���������� ������� ����� �������
//�������� ������: ����������� ����� ����� - ����� ����� 
UINT CShapes::GetBrushStyle()const
{
	return m_nBrushStyle;
}


//���������� ������� ���� �������
//�������� ������: ������� �����, ���������� �������� ���������� RGB - ���� �������
COLORREF CShapes::GetBrushColor()const
{
	return m_rgbBrushColor;
}


//���������� ������� ������ �� � ������
//������� ������: ������ �������� ������� ������
//�������� ������: ����� (��� 32-������ �����) 
CPoint CShapes::GetPoint(const INT &iPointIndex)const
{
	ASSERT(iPointIndex >= 0);
	return m_ShapePoints.GetAt(iPointIndex);
}


//���������� ���������� ������ ������
//�������� ������: ����� ����� - ���������� ��������� ������� ������
INT CShapes::GetPointNumber()const
{
	return (INT)m_ShapePoints.GetSize();
}


//���������� ��� ������
//�������� ������: ������ � ������ ������
CString CShapes::GetShapeName()const
{
	return m_sShapeName;
}



//����� �������� �� ���������� � ���� � �������� ������ �� �����
//������� ������: ����� �� ����������-�����
VOID CShapes::Serialize(CArchive& ar)      //exception
{
	if(ar.IsStoring())
	{   //���������� ������
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
	{   //������ ������
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
	//��������� ������ ����� ������
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






//����� �������������� �������� ���������� � ������ �����������
//������� ������: ��������� �� �������� ���������� ������
//�������� ������: TRUE ��� FALSE � ����������� �� ���������� ��������
BOOL CShapes::_PrepareDC(CDC *pDC)
{
	ASSERT(pDC);
	//��������� ��������� ��������� ����������
	if(!pDC->SaveDC())
		return FALSE;
	//������������� ��������� ���� � �����
	if(HPEN(m_Pen)!=NULL)
		pDC->SelectObject(&m_Pen);
	if(HBRUSH(m_Brush)!=NULL)
		pDC->SelectObject(&m_Brush);
	return TRUE;	
}



//����� ��������������� �������� ����������
//������� ������: ��������� �� �������� ���������� ������
//�������� ������: TRUE ��� FALSE � ����������� �� ���������� ��������
BOOL CShapes::_RestoreDC(CDC *pDC)
{
	ASSERT(pDC);
	return pDC->RestoreDC(STEP_BACK); //��������������� �������� �� ���� ��������� �����
}