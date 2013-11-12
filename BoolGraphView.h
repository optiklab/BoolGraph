#pragma once

//����� CBoolGraphView - ���
//�������� �� ����������� ������ � �������� � ���� ������� ������-�����������, �������� 
//��������� ���������� � ������������� ��������� ����������
class CBoolGraphView : public CView
{
protected:
	__declspec(nothrow) CBoolGraphView();
	DECLARE_DYNCREATE(CBoolGraphView)

//���������������
public:
	__declspec(nothrow) virtual ~CBoolGraphView();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//������� ����������� ���� 
	virtual void OnDraw(CDC* pDC);

#ifdef _DEBUG
	virtual void AssertValid() const;
	__declspec(nothrow) virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	//����� ���������� ��������� �� ��������
	CBoolGraphDoc* GetDocument() const;

	//������� ��������� ����������� �����-�����
	__declspec(nothrow) VOID DrawMoveLine(const CPoint &, const CPoint &);

	//������� ��������� �����
	__declspec(nothrow) VOID DrawGrid(CDC* pDC);

	//----------- ������� ��� ������ � ��������� ��������� List Control -------------
	//������� ���������� ����� ������ � List Control
	VOID AddLayerAtList();      //exception

	//������� �������� ������ �� List Control
	__declspec(nothrow) VOID DeleteLayerFromList();

	//������� ������� List Control
	__declspec(nothrow) BOOL ClearListControl();
	
	//������� ������� ������� ��������� �����
	__declspec(nothrow) VOID ClearShapeDeque();

	//������� ��������� �����  POINT_SYMBOL(+) ����� ��������� ������ � List Control
	__declspec(nothrow) BOOL SetShapePoint(const INT &);

	//-------------------------------- ������� �������� -----------------------------
	//������� ���������� �����, ��������� �� ����� ��������� � List Control ������
	__declspec(nothrow) BOOL IsShapeNumberCorrect(const INT &)const;

	//������� ���������, �������� �� ��������� ���������� ������ � ������
	__declspec(nothrow) BOOL IsLegalShape();

	//������� ���������, ���������� �� ������, ��� ���������� ��������
	__declspec(nothrow) BOOL IsLegalOperation();

	//------------------------------ ������� �������� ����� -------------------------
	//������� ������� ������-���������� ��������
	VOID CreateShapeResult(CArray <SEdge>*);      //exception

	//����� ������������ ����� ������ ������������� ����� � �������� ������� � ������� �����
	__declspec(nothrow) INT SetIncedentEdges(const CPoint &, const CArray <SEdge>*);

	//��������� ����� ��������� ������, ���������� ����������� ��������, ����� ������� �����
	__declspec(nothrow) VOID DetermineShape(CArray <SEdge>*, CArray <CPoint>*);

	//�������� ������� ���� �������
	VOID CreatePolygon(const UINT &);            //exception
	
	//������� ��������� �������� ���������� �������� ��� ��������
	__declspec(nothrow) VOID StartOperation(const UINT &);

private:
	//------------------------ ��������� ������� ������ ���� -----------------------
	afx_msg VOID OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg VOID OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg VOID OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg VOID OnMouseMove(UINT nFlags, CPoint point);

	//---------------------- ��������� ������� ������ �� ����� ---------------------
	//������� �� ������ "��������" - ��������� ����� ������ ���� ��� ��������� ������
	afx_msg VOID OnBnClickedAddLayer();

	//������� �� ������ "�������" - ������� ��������� ������
	afx_msg VOID OnBnClickedDeleteLayer();

	//������� �� ������ "����� ������"
	afx_msg VOID OnBnClickedLayerOptions();

	//-------------- ��������� ������� ������ � ������ List Control ----------------
	//��������� ���������� ����� - ��������� ������
	afx_msg VOID OnNMClickListLayers(NMHDR* pNMHDR, LRESULT* pResult);

	//��������� �������� ����� - ����� ������ ��� ��������
	afx_msg VOID OnNMDblclkListLayers(NMHDR* pNMHDR, LRESULT* pResult);

	//---------------------- ��������� ������� ���� --------------------------------
	//�������� ������ �������
	afx_msg VOID OnProjCreate();  

	//��������� ������������ ������
	afx_msg VOID OnPolygon();

	//��������� ��������������
	afx_msg VOID OnPolyline();

	//�������� �����������
	afx_msg VOID OnUnion();  

	//�������� ��������
	afx_msg VOID OnDifference();  

	 //�������� �����������
	afx_msg VOID OnCross();      

	//------------------- ��������� ������� ������������ ���� --------------------
	//����� ������������ ����
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint ptPoint);
	//��������� �������� �������� � ����������� ����
	afx_msg void OnContextDelete();
	//��������� ������ ����� ������ � ����������� ����
	afx_msg void OnContextOptions();
	
	//������� �������� ����: ����������� ����������/�� ���������� �����
	afx_msg void OnShowTransparent();
	
	//������� �������� ����: ����������� ���� � ��������
	afx_msg void OnViewGrid();

private:
	UINT m_nCurrentOperation;   //����� ������� �������� ���������, ���� ������ ����������� � GlobalData.h
	INT m_nCurrentLayer;	    //����� ��������� ������ � ������ List Control
	CPoint m_ptFirstPoint;      //��������� ����� ������ ��� ���������
	CPoint m_ptCurMovePoint;    //������� �����, � ������� ��������� ������������
	CArray <INT> m_nShapesDeque;  //������� ������� �� ������ �����, ������� �������� ��� ������������ ��������
	UINT m_nShapesChoosed;      //������� ���������� �����, ������� ��� ������� ��� ���������� ��������
	BOOL bIfPaint;              //����� ������������� ���������� ����� ��� ���������
	BOOL bIfGrid;               //����� ������������� ��������� ������ �� ���� ���������
};

#ifndef _DEBUG  // debug version in BoolGraphView.cpp
inline CBoolGraphDoc* CBoolGraphView::GetDocument() const
   { return reinterpret_cast<CBoolGraphDoc*>(m_pDocument); }
#endif

