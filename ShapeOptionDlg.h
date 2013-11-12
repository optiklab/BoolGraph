#pragma once
#include "afxwin.h"


//����� CShapeOptionDlg 
//�������� �� �������� ����������, ����������� � ���� ����� ������,
//��������� ��� ��������, ������� ����������� �������������
class CShapeOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(CShapeOptionDlg)
public:
	__declspec(nothrow) CShapeOptionDlg(CWnd* pParent = NULL);
	__declspec(nothrow) virtual ~CShapeOptionDlg();
	//��� ����������� ����
	enum { IDD = IDD_SHAPE_OPTIONS };

protected:
	//�������, ����������� ����� �������
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP() //��������� ����� ���������
public:
	//���������� ��� ������
	__declspec(nothrow) CString GetShapeName()const;

	//���������� ���� �������
	__declspec(nothrow) COLORREF GetBrushColor()const;

	//���������� ���� ����
	__declspec(nothrow) COLORREF GetPenColor()const;

	//���������� ��� �������
	__declspec(nothrow) INT GetBrushStyle()const;

	//������������� ��� ������
	__declspec(nothrow) VOID SetShapeName(const CString &);

	//������������� ���� �������
	__declspec(nothrow) VOID SetBrushColor(const COLORREF);

	//������������� ���� ����
	__declspec(nothrow) VOID SetPenColor(const COLORREF);

	//������������� ��� �������
	__declspec(nothrow) VOID SetBrushStyle(const INT &);


private:
	//������� ������������� ����������� ����
	virtual BOOL OnInitDialog();

	//������� �� ������ ���� ����
	afx_msg VOID OnStnClickedColorPen();

	//������� �� ������ ���� �������
	afx_msg VOID OnStnClickedColorBrush();

	//��������� ����� �������
	afx_msg VOID OnLbnSelchangeBrushStyleList();

	//���������� ������� ��������� ����� �������
	__declspec(nothrow) VOID PaintBrushPreview();

private:
	
	CString m_ShapeName;
	COLORREF m_PenColor;
	CRect m_PenColorSwatch;     //���������� ���������� ����� ����
	CBrush m_Brush;
	COLORREF m_BrushColor;
	CRect m_BrushColorSwatch;   //���������� ���������� ����� �����
	CRect m_BrushStyleSwatch;   //���������� ���������� ��������� ����� �������
	INT m_BrushStyle;
	CListBox m_BrushStyleCtrl;  //������� ���������� ListBox
};
