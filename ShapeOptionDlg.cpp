// ShapeOptionDlg.cpp : implementation file
#include "stdafx.h"
#include "BoolGraph.h"
#include "ShapeOptionDlg.h"
#include "GlobalData.h"

// CShapeOptionDlg dialog

IMPLEMENT_DYNAMIC(CShapeOptionDlg, CDialog)


//������������� ����� ������
CShapeOptionDlg::CShapeOptionDlg(CWnd* pParent):CDialog(CShapeOptionDlg::IDD, pParent), 
												m_ShapeName(DEFAULT_NAME),
												m_PenColor(RGB(0,0,0)),
												m_BrushColor(RGB(0,0,0))
{
	m_Brush.CreateStockObject(WHITE_BRUSH);	  //������� ����� �����
}



CShapeOptionDlg::~CShapeOptionDlg()
{}




//������� ������ ������� ����� ���������� ���������� � �������
void CShapeOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	ASSERT(pDX);
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SHAPE_NAME, m_ShapeName);
	DDV_MaxChars(pDX, m_ShapeName, 20);
	DDX_Control(pDX, IDC_BRUSH_STYLE_LIST, m_BrushStyleCtrl);
}



BEGIN_MESSAGE_MAP(CShapeOptionDlg, CDialog)
	ON_STN_CLICKED(IDC_COLOR_PEN, &CShapeOptionDlg::OnStnClickedColorPen)
	ON_STN_CLICKED(IDC_COLOR_BRUSH, &CShapeOptionDlg::OnStnClickedColorBrush)
	ON_LBN_SELCHANGE(IDC_BRUSH_STYLE_LIST, &CShapeOptionDlg::OnLbnSelchangeBrushStyleList)
END_MESSAGE_MAP()




//���������� ��� ������
//�������� ������: ������ � ������ ������
CString CShapeOptionDlg::GetShapeName()const
{
	return m_ShapeName;
}




//���������� ���� �������
//�������� ������: ������� ����� ��������������� ���� �����(�������)
COLORREF CShapeOptionDlg::GetBrushColor()const
{
	return m_BrushColor;
}




//���������� ���� ����
//�������� ������: ������� ����� ��������������� ���� ����
COLORREF CShapeOptionDlg::GetPenColor()const
{
	return m_PenColor;
}




//���������� ����� �������
//�������� ������: ����� ����� - ����� ���� �������
INT CShapeOptionDlg::GetBrushStyle()const
{
	return m_BrushStyle;
}




//������������� ��� ������
//������� ������: ����� �� ������ � ������ ������
VOID CShapeOptionDlg::SetShapeName(const CString &sShapeName)
{
	ASSERT(sShapeName);
	m_ShapeName = sShapeName;
}



//������������� ���� �������
//������� ������: ������� ����� ���������� ���� �����(�������)
VOID CShapeOptionDlg::SetBrushColor(const COLORREF rgbBrushColor)
{	
	m_BrushColor = rgbBrushColor;
}



//������������� ���� ����
//������� ������: ������� ����� ���������� ���� ����
VOID CShapeOptionDlg::SetPenColor(const COLORREF rgbPenColor)
{
	m_PenColor = rgbPenColor;
}


//������������� ��� �������
//������� ������: ����� �� ����� ����� - ����� ����� �������
VOID CShapeOptionDlg::SetBrushStyle(const INT &iBrushStyle)   
{
	m_BrushStyle = iBrushStyle;
}



//������������� ����������� ����
//�������� ������: TRUE ��� FALSE - ������� ��� ��� ����������� ��������
BOOL CShapeOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//��������� �������� ������ � ������� ListBox
	m_BrushStyleCtrl.AddString(_T("��������"));
	m_BrushStyleCtrl.AddString(_T("��-�� ���������"));
	m_BrushStyleCtrl.AddString(_T("��-�� ���������"));
	m_BrushStyleCtrl.AddString(_T("������"));
	m_BrushStyleCtrl.AddString(_T("������������ ������"));
	m_BrushStyleCtrl.AddString(_T("��������������"));
	m_BrushStyleCtrl.AddString(_T("������������"));
	m_BrushStyleCtrl.SetCurSel(0);
	return TRUE;
}



//��������� ����� �� ����������� ������ ����� ����
//���������� ����������� ������ ������ ����� Windows,
//����� ������� �� �������� �����������
VOID CShapeOptionDlg::OnStnClickedColorPen()
{
	CColorDialog dlg(m_PenColor); 
	if(dlg.DoModal()==IDOK)
	{
		m_PenColor=dlg.GetColor(); //��������� �����
		CBrush swatch;     
		swatch.CreateSolidBrush(m_PenColor);  //�������� �����
		CClientDC dc(this);
		//����������� �������� ����� ����...
		CWnd* pPenColor=GetDlgItem(IDC_COLOR_PEN); //�������� ��������� �� ������� ����������
		pPenColor->GetWindowRect(&m_PenColorSwatch);
		ScreenToClient(&m_PenColorSwatch); 	//�������� ���������� ������������ ���������� �������
		dc.FillRect(&m_PenColorSwatch, &swatch);    //����������� ��������� ��������� ������
	}
}



//��������� ����� �� ����������� ������ ����� �������
//� ����������� ������� ������ ����� ������������ ������������ ������� ���� ������� ������
VOID CShapeOptionDlg::OnStnClickedColorBrush()
{
	CColorDialog dlg(m_BrushColor); 

	if(dlg.DoModal()==IDOK)
	{
		m_BrushColor=dlg.GetColor(); //��������� �����
		CBrush swatch;
		swatch.CreateSolidBrush(m_BrushColor);
		CClientDC dc(this);
		//����������� �������� ����� �����...
		CWnd* pBrushColor=GetDlgItem(IDC_COLOR_BRUSH);
		pBrushColor->GetWindowRect(&m_BrushColorSwatch);
		ScreenToClient(&m_BrushColorSwatch);
		dc.FillRect(&m_BrushColorSwatch, &swatch);
	}
	OnLbnSelchangeBrushStyleList();
}


//����� ����� ������� � ListBox
//������� � ������ ListBox �� ������������ ��������, ���������� ��������������� �����
VOID CShapeOptionDlg::OnLbnSelchangeBrushStyleList()
{
	m_Brush.DeleteObject();
	int style = m_BrushStyleCtrl.GetCurSel();
	switch(style)
	{
		case 0:
			m_BrushStyle=-1;
			m_Brush.CreateSolidBrush(RGB(255,255,255));
			break; 
		case 1:
			m_BrushStyle=3;
			m_Brush.CreateHatchBrush(HS_BDIAGONAL, m_BrushColor);
			break; 
		case 2:
			m_BrushStyle=2;
			m_Brush.CreateHatchBrush(HS_FDIAGONAL, m_BrushColor);
			break; 
		case 3:
			m_BrushStyle=4;
			m_Brush.CreateHatchBrush(HS_CROSS, m_BrushColor);
			break; 
		case 4:
			m_BrushStyle=5;
			m_Brush.CreateHatchBrush(HS_DIAGCROSS, m_BrushColor);
			break; 
		case 5:
			m_BrushStyle=0;
			m_Brush.CreateHatchBrush(HS_HORIZONTAL, m_BrushColor);
			break; 
		case 6:
			m_BrushStyle=1;
			m_Brush.CreateHatchBrush(HS_VERTICAL, m_BrushColor);
			break; 
		default:
			m_BrushStyle=-1;
			m_Brush.CreateSolidBrush(RGB(255,255,255));
			break;
	}
	PaintBrushPreview(); //��������� ������� ��������� ����� �������
}




//������� ����������� ������� ��������� ����� �������
VOID CShapeOptionDlg::PaintBrushPreview()
{
	CClientDC dc(this);
	CWnd* pBrushStyle=GetDlgItem(IDC_BRUSH_STYLE);
	pBrushStyle->GetWindowRect(&m_BrushStyleSwatch);
	ScreenToClient(&m_BrushStyleSwatch);
	m_BrushStyleSwatch.DeflateRect(2,2,1,1); 
	dc.FillRect(&m_BrushStyleSwatch,&m_Brush);
}
