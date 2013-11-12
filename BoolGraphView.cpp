#include "stdafx.h"
#include "BoolGraph.h"
#include "GlobalData.h"
#include "MainFrm.h"
#include "BoolGraphDoc.h"
#include "BoolGraphView.h"
#include "Shapes.h"
#include "Polygon.h"
#include "ShapeOptionDlg.h"
#include <math.h>
#include "Overlay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//����� CBoolGraphView - ���
IMPLEMENT_DYNCREATE(CBoolGraphView, CView)

//------------------------------����� ���������---------------------------------------------------
BEGIN_MESSAGE_MAP(CBoolGraphView, CView)
	ON_COMMAND(ID_PROJ_CREATE, &CBoolGraphView::OnProjCreate)      //������� ������
	ON_COMMAND(ID_POLYGON, &CBoolGraphView::OnPolygon)             //������� ��������� �������������
	ON_COMMAND(ID_POLYLINE, &CBoolGraphView::OnPolyline)           //������� ��������� ��������� ���������
	//����������� ������� ������ ���� � �������� ����
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_DELETE_LAYER, &CBoolGraphView::OnBnClickedDeleteLayer)     //������� ������
	ON_BN_CLICKED(IDC_LAYER_OPTIONS, &CBoolGraphView::OnBnClickedLayerOptions)   //������� ���� ����� ������
	ON_NOTIFY(NM_CLICK, IDC_LIST_LAYERS, &CBoolGraphView::OnNMClickListLayers)   // ������� ����� ������ ���� � List Control
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LAYERS, &CBoolGraphView::OnNMDblclkListLayers) // ������� ���� ����� � List Control
	ON_COMMAND(ID_UNION, &CBoolGraphView::OnUnion)       //�������� �����������
	ON_COMMAND(ID_DIFFERENCE, &CBoolGraphView::OnDifference)    //�������� ��������
	ON_COMMAND(ID_CROSS, &CBoolGraphView::OnCross)       //�������� �����������
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CONTEXT_DELETE, &CBoolGraphView::OnContextDelete)
	ON_COMMAND(ID_CONTEXT_OPTIONS, &CBoolGraphView::OnContextOptions)
	ON_COMMAND(ID_SHOW_TRANSPARENT, &CBoolGraphView::OnShowTransparent)
	ON_COMMAND(ID_VIEW_GRID, &CBoolGraphView::OnViewGrid)
END_MESSAGE_MAP()





// ������������� ����� ������
CBoolGraphView::CBoolGraphView():m_nCurrentOperation(OP_NO_OPER),
								 m_nCurrentLayer(NULL_LAYER),      
								 m_ptFirstPoint(NULL),
								 bIfPaint(FALSE),
								 bIfGrid(FALSE),
								 m_ptCurMovePoint(NULL),
								 m_nShapesChoosed(0)
{
	m_nShapesDeque.SetSize(MAX_WORKING_LAYERS,-1);
	//��� �������� ������� ��������� ����� ��� �������� ������ �������� -1
	for(UINT nI=0; nI<MAX_WORKING_LAYERS;nI++)
		m_nShapesDeque.SetAt(nI,-1);
}


CBoolGraphView::~CBoolGraphView()
{}


BOOL CBoolGraphView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}


//������� ����������� ����
//� ���� ��������� ������������ ��� ������, ��������� ������������� ��� ����������
//������� ��������: ��������� �� �������� ���������� ������
void CBoolGraphView::OnDraw(CDC* pDC)
{
	ASSERT(pDC);
	CBoolGraphDoc* pDoc = GetDocument();
	POSITION pos=NULL;
	CShapes *pShape=NULL;
	DrawGrid(pDC);
	if(pDoc->m_LayersList.GetCount()>0)   //�������� ������ ������� � ������ ����������
		pos=pDoc->m_LayersList.GetHeadPosition();
	while(pos!=NULL)                      //�� ����� ������������ ��� ������
	{
		pShape = pDoc->m_LayersList.GetNext(pos);
		if(pShape!=NULL)
			pShape->Show(pDC,bIfPaint);
	}
	//�������� ������������ ������ ��������� ������ � ��������� � ������
	//���������� (pDoc->GetLoadedLayers()-m_nCurrentLayer-1) �������� ����� ������ � ������ ���������� ����� ����� � List Control
	if(IsShapeNumberCorrect((pDoc->GetLoadedLayers()-m_nCurrentLayer-1)) == TRUE) 
	{  
		pShape=pDoc->GetLayerByNumber((pDoc->GetLoadedLayers()-m_nCurrentLayer-1));
		if(pShape!=NULL)
		{
			UINT nStyle = pShape->GetPenStyle();
			UINT nWidth = pShape->GetPenWidth();
			COLORREF rgbColor = pShape->GetPenColor();
			pShape->SetPen(THEME_PEN_2,THEME_WIDTH_2,THEME_STYLE_2); //�������� ���� #2 - �� GlobalData.h
			pShape->Show(pDC,bIfPaint);
			pShape->SetPen(rgbColor,nWidth,nStyle); //������� ���� ���������, ��������� ������
		}
	}
}




//---------------------------- ������� ��������� ----------------------
//����������� ������� ���������� ������ CBoolGraphView
#ifdef _DEBUG
void CBoolGraphView::AssertValid() const
{
	CView::AssertValid();
}


void CBoolGraphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
//---------------------------------------------------------------------







//������� ���������� ��������� �� ��������
CBoolGraphDoc* CBoolGraphView::GetDocument() const //� �����-������ inline-�������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBoolGraphDoc)));
	return (CBoolGraphDoc*)m_pDocument;

}
#endif //_DEBUG



//������� ��������� ����������� �����-�����
//��������� ������ ������ ������ ����� ����� ���������� ������������� �������
//��� ����� �������� ������������ ��������� �����
//������� ���������: ���������� ��������� � �������� �����
VOID CBoolGraphView::DrawMoveLine(const CPoint &ptFirstPoint, const CPoint &ptSecondPoint)
{
	ASSERT(ptFirstPoint.x >= 0);
	ASSERT(ptFirstPoint.y >= 0);
	ASSERT(ptSecondPoint.x >= 0);
	ASSERT(ptSecondPoint.y >= 0);
	CClientDC dc(this);
	OnPrepareDC(&dc); 	
	INT OldMode=dc.SetROP2(R2_NOT); //��������� ����� ��������� ��������� ������
   	dc.MoveTo(ptFirstPoint);
	dc.LineTo(ptSecondPoint);  
	dc.SetROP2(OldMode);    //��������������� ������� ����� ���������
}





//������� ��������� �����
VOID CBoolGraphView::DrawGrid(CDC* pDC)
{
	if(bIfGrid)
	{
		//���������� ���� ����� - 5 �����������
		INT nPIMX, nPIMY;
		DOUBLE TempX = (DOUBLE)pDC->GetDeviceCaps(LOGPIXELSX)/MPI;
		DOUBLE TempY = (DOUBLE)pDC->GetDeviceCaps(LOGPIXELSY)/MPI;
		DOUBLE OstatokX = TempX - (DOUBLE)(INT)TempX;
		DOUBLE OstatokY = TempY - (DOUBLE)(INT)TempY;
		OstatokX > 0.5 ? nPIMX = (INT)TempX + 1 : nPIMX = (INT)TempX;
		OstatokY > 0.5 ? nPIMY = (INT)TempY + 1 : nPIMY = (INT)TempY;
		//��������� �����
		CPen Pen(0, 1, GRID_COLOR);
		CPen *oldPen = pDC->SelectObject(&Pen);
		CRect Rect;
		pDC->GetClipBox(Rect);
		for(INT nY = 0; nY < Rect.Height(); nY+=5*nPIMX)
		{
			pDC->MoveTo(0,nY);
			pDC->LineTo(Rect.Width(),nY);
		}
		for(INT nX = 0; nX < Rect.Width(); nX+=5*nPIMY)
		{
			pDC->MoveTo(nX,0);
			pDC->LineTo(nX,Rect.Height());
		}
		pDC->SelectObject(oldPen);
	}
}


//---------------- ������� ��� ������ � ��������� ��������� List Control ----------------------------

//�������� ����� ������ � ������ List Control
VOID CBoolGraphView::AddLayerAtList()   
{
	CListCtrl* plstLayers = GET_LST_LAYERS;
	INT iError=plstLayers->InsertItem(0, DEFAULT_NAME);
	if(iError==-1)
		AfxMessageBox(MSG_ADD_LAYER_ERROR);
}


//������� �������� ����� ������ �� ������ List Control
VOID CBoolGraphView::DeleteLayerFromList()
{
	CBoolGraphDoc *pDoc=GetDocument();
	CListCtrl *plstLayers = GET_LST_LAYERS; 
	if(pDoc->GetPaintArea()==FALSE)
	{
		if(m_nCurrentLayer == NULL_LAYER)
		{
			AfxMessageBox(MSG_UNCHOOSED_ELEMENT);
			return;
		}
		if(IsShapeNumberCorrect((pDoc->GetLoadedLayers()-m_nCurrentLayer-1)) == TRUE)
		{
			pDoc->DeleteSelectedLayer((pDoc->GetLoadedLayers()-m_nCurrentLayer-1)); 
			plstLayers->DeleteItem(m_nCurrentLayer);  
			pDoc->SetLoadedLayers(pDoc->GetLoadedLayers()-1);  
		}
		m_nCurrentLayer=NULL_LAYER;
		pDoc->SetModifiedFlag();
		Invalidate();
	}
}




//������� ������� ������ List Control
//�������� ������: �� ��� ��� - ������� �� ���������
BOOL CBoolGraphView::ClearListControl()
{
	CListCtrl *plstLayers = GET_LST_LAYERS;
	plstLayers->DeleteAllItems();
	return TRUE;
}



//������� ������� ������� ��������� �����
VOID CBoolGraphView::ClearShapeDeque()
{
	for(UINT nI=0; nI<MAX_WORKING_LAYERS; nI++)  //��� �������� � ��������� �������� -1
		m_nShapesDeque.SetAt(nI,-1);
}



//������� ��������� ����� POINT_SYMBOL (+) ����� ������� ������ � List Control
//�������� ������: �� ��� ��� - ������� �� ���������
BOOL CBoolGraphView::SetShapePoint(const INT &nShapeIndex)
{
	ASSERT(nShapeIndex >= 0);
	//�������� ��������� �� ������� ����� List Control
	CListCtrl* plstLayers = GET_LST_LAYERS;
	plstLayers->SetItemText(nShapeIndex,1,POINT_SYMBOL);
	return TRUE;
}





//---------------- ������� �������� --------------------------------------------------

//������� ���������� �����, ��������� �� ����� ��������� � List Control ������
//������� ������: ����� ������ � ������ ���������� �� ������
//�������� ������: ��(true) ��� ���(false) - ��������� ����� ��� ���
BOOL CBoolGraphView::IsShapeNumberCorrect(const INT &nCurrentLayer)const
{
	ASSERT(nCurrentLayer >= 0);
	CBoolGraphDoc *pDoc=GetDocument();
	if(nCurrentLayer<FIRST_LAYER || nCurrentLayer>(INT)pDoc->GetLoadedLayers())
		return FALSE;
	else
		return TRUE;
}





//������� ���������, �������� �� ��������� ���������� ������ � ������
//������������ ����� ����� ���������� ���������� MAX_LOADABLE_LAYERS
//�������� ������: �� - ��������� ��������� ������, ��� - �� ���������
BOOL CBoolGraphView::IsLegalShape()
{
	CBoolGraphDoc *pDoc=GetDocument();
	if(pDoc->GetLoadedLayers()>=MAX_LOADABLE_LAYERS)
	{
		//������ ��������� � ���, ��� ������ ��������� ������ ������
		AfxMessageBox(MSG_TOO_MANY_LAYERS);
		return FALSE;
	}
	return TRUE;
}




//������� ��������, ���������� �� ������, ��� ���������� ��������
//�������� ������: �� - �������� ���������, ��� - ���
BOOL CBoolGraphView::IsLegalOperation()
{
	CBoolGraphDoc *pDoc=GetDocument();
	for(UINT nI=0; nI<MAX_WORKING_LAYERS;nI++)
		if(m_nShapesDeque.GetAt(nI)<0 || m_nShapesDeque.GetAt(nI) >= (INT)(pDoc->GetLoadedLayers()))
		{
			AfxMessageBox(MSG_THERE_IS_NO_SHAPES);
			return FALSE;
		}
	return TRUE;
}






//------------------ ��������� ������� ������ ����    -------------------------------------------

//���������� ������� ����� ������ ����
//��� ������� �� ����� ������ ���� ����������:
//1. ���������� ����� �������, ��� ����������� ��������� ����� ��������� ������
//2. ���� ������� �������� ��������� �������������� ��� ��������, �� ���������� ����� ��� ��������� ��� �������
VOID CBoolGraphView::OnLButtonDown(UINT nFlags, CPoint ptPoint)
{
	ASSERT(ptPoint.x >= 0);
	ASSERT(ptPoint.y >= 0);
	CBoolGraphDoc *pDoc=GetDocument();
	CDC *pDC=GetDC();  
	OnPrepareDC(pDC);  
	ReleaseDC(pDC);   
	m_ptCurMovePoint=ptPoint;
	switch(m_nCurrentOperation)
	{
		case OP_POLYLINE:
		case OP_POLYGON:
			//��������� ����� � ������ ������ ��������������
			((CPolygon*)pDoc->m_LayersList.GetTail())->AddPoint(ptPoint);
			m_nCurrentLayer=NULL_LAYER;
			Invalidate();
			break;
	}
	m_ptFirstPoint=ptPoint;
	CView::OnLButtonDown(nFlags, ptPoint);
}




//���������� �������� ����� ����� � ���� ���������
//���������� ��������� ��������������
VOID CBoolGraphView::OnLButtonDblClk(UINT nFlags, CPoint ptPoint)
{
	ASSERT(ptPoint.x >= 0);
	ASSERT(ptPoint.y >= 0);
	CBoolGraphDoc *pDoc=GetDocument();
	switch(m_nCurrentOperation)
	{
		case OP_POLYLINE:
			pDoc->SetPaintAreaClose();     //��������� ������� ���������
			m_nCurrentOperation=OP_NO_OPER; //�������� �� �������
			break;
		case OP_NO_OPER:
			if(pDoc->GetPaintArea()==FALSE)
			{
				CListCtrl *plstLayers = GET_LST_LAYERS;
				CRgn UserRegion;
				CShapes *pShape = NULL;
				POSITION pos = NULL;
				INT nCounter = 0;
				if(pDoc->m_LayersList.GetCount()>0) 
					pos=pDoc->m_LayersList.GetTailPosition();
				while(pos != NULL) 
				{
					pShape=pDoc->m_LayersList.GetPrev(pos);
					UserRegion.DeleteObject();
					pShape->GetRegion(UserRegion);
					if(UserRegion.PtInRegion(ptPoint)) 
					{
						pDoc->SetSelLayer(pShape);
						m_nCurrentLayer = nCounter;
						pos = NULL;
						plstLayers->SetFocus();
						plstLayers->SetItemState(nCounter, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
						if(m_nShapesChoosed>=0 && m_nShapesChoosed<MAX_WORKING_LAYERS)
						{
							m_nShapesDeque.SetAt(m_nShapesChoosed, (pDoc->GetLoadedLayers()-m_nCurrentLayer-1));
							m_nShapesChoosed+=1;
							SetShapePoint(m_nCurrentLayer); //������ ����� + � ������ List Control �������� ��������� ������
						}
						else
						{
							ClearShapeDeque();
							for(UINT nI=0; nI<pDoc->GetLoadedLayers();nI++)
								plstLayers->SetItemText(nI, 1,EMPTY_NAME);
							m_nShapesChoosed=0;
						}
					}
					nCounter++;
				}
				Invalidate();
			}
	}
	CView::OnLButtonDblClk(nFlags, ptPoint);
}




//���������� ������� ����� ������ ����
//��� ���� ����������:
//1. ��� ��������� ��������� ������ �� ��������� ��������
//2. ��� ���������� ������� ������
VOID CBoolGraphView::OnLButtonUp(UINT nFlags, CPoint ptPoint)
{
	ASSERT(ptPoint.x >= 0);
	ASSERT(ptPoint.y >= 0);
	CBoolGraphDoc *pDoc=GetDocument();
	CDC *pDC=GetDC();
	OnPrepareDC(pDC);
	ReleaseDC(pDC);

	switch(m_nCurrentOperation)
	{
		case OP_POLYGON:
			pDoc->SetPaintAreaClose();
			m_nCurrentOperation=OP_NO_OPER;
			break;
		case OP_NO_OPER:
			if(pDoc->GetPaintArea()==FALSE)
			{
				CListCtrl *plstLayers = GET_LST_LAYERS;
				CRgn UserRegion;
				CShapes *pShape = NULL;
				POSITION pos = NULL;
				INT nCounter = 0;
				if(pDoc->m_LayersList.GetCount()>0) 
					pos=pDoc->m_LayersList.GetTailPosition();
				while(pos != NULL) 
				{
					pShape=pDoc->m_LayersList.GetPrev(pos);
					UserRegion.DeleteObject();
					pShape->GetRegion(UserRegion);
					if(UserRegion.PtInRegion(ptPoint)) 
					{
						pDoc->SetSelLayer(pShape);
						m_nCurrentLayer = nCounter;
						pos = NULL;
						plstLayers->SetFocus();
						plstLayers->SetItemState(nCounter, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
					}
					else
						m_nCurrentLayer = NULL_LAYER;
					nCounter++;
				}
				Invalidate();
			}
	}
	CView::OnLButtonUp(nFlags, ptPoint);
}




//���������� �������� ���� ��� ������� ����� ������
//�������� �����-����� ����� ������ ������ � ������ �������� ���������
VOID CBoolGraphView::OnMouseMove(UINT nFlags, CPoint ptPoint)
{
	ASSERT(ptPoint.x >= 0);
	ASSERT(ptPoint.y >= 0);
	CBoolGraphDoc *pDoc=GetDocument();
	CDC *pDC=GetDC();
	OnPrepareDC(pDC); 
	ReleaseDC(pDC);
	
	switch(m_nCurrentOperation)
	{
		//���� ������ ������ �� ������,����� ���������� ����������� �����-�����
		case OP_POLYLINE:
			if(((CPolygon*)pDoc->m_LayersList.GetTail())->GetPointNumber()<=0) 
				break;
			DrawMoveLine(m_ptFirstPoint, m_ptCurMovePoint);
			m_ptCurMovePoint=ptPoint;
			DrawMoveLine(m_ptFirstPoint, m_ptCurMovePoint);
			break;
		//���� ������ ������������ ������,���������� ������ ����� ���� ������ ����� ������ ����
		case OP_POLYGON:
			if(((CPolygon*)pDoc->m_LayersList.GetTail())->GetPointNumber()<=0) 
				break;
			if((nFlags & MK_LBUTTON) == MK_LBUTTON)
			{
				((CPolygon*)pDoc->m_LayersList.GetTail())->AddPoint(ptPoint);
				Invalidate();
			}
			break;
	}
	CView::OnMouseMove(nFlags, ptPoint);
}





//������� ������ "Delete"
VOID CBoolGraphView::OnBnClickedDeleteLayer()
{
	DeleteLayerFromList();
}



//��������� ������ ���� ����� ������
//��� ������� �� ������ ����� ��������� ����, � ������� ����� ������ ����� �������� ������, ���
//���, ���� � ��� �������, ���� ����
VOID CBoolGraphView::OnBnClickedLayerOptions()
{
	CShapeOptionDlg ShapeOptionsDialog;
	CBoolGraphDoc *pDoc=GetDocument();
	CShapes* pShape;
	if(pDoc->GetPaintArea()==FALSE)
	{
		if(m_nCurrentLayer == NULL_LAYER)
		{
			AfxMessageBox(MSG_UNCHOOSED_ELEMENT);
			return;
		}
		if(IsShapeNumberCorrect((pDoc->GetLoadedLayers()-m_nCurrentLayer-1))==TRUE)
		{
			//�������� � ��������� ���� ���� ����� - ��� �������� ����
			pShape = pDoc->GetSelLayer();
			ShapeOptionsDialog.SetShapeName(pShape->GetShapeName());
			if(ShapeOptionsDialog.DoModal()==IDOK)
			{
				//��������� ���������
				pShape->SetPen(ShapeOptionsDialog.GetPenColor(),1,PS_SOLID);
				pShape->SetBrush(ShapeOptionsDialog.GetBrushColor(),ShapeOptionsDialog.GetBrushStyle());
				pShape->SetShapeName(ShapeOptionsDialog.GetShapeName());
				pDoc->LoadLayersAtListControl();
				ClearShapeDeque();
			}
		}
	}
}





//--------------------��������� ������� ������ � ������ List Control-----------------------------

//���� � ������ List Control
//���������� ��������� ��������������� ������ ������
VOID CBoolGraphView::OnNMClickListLayers(NMHDR *pNMHDR, LRESULT *pResult)
{
	CBoolGraphDoc *pDoc=GetDocument();
	if(pDoc->GetPaintArea()==FALSE)
	{
		CShapes* pShape;
		CListCtrl *plstLayers = GET_LST_LAYERS;  //�������� ��������� �� ������ List Control
		if(plstLayers->GetSelectedCount() == 0) 
			return;
		//������� ������ ��������� ������
		m_nCurrentLayer=plstLayers->GetNextItem(-1, LVNI_SELECTED); 
		if(m_nCurrentLayer!=NULL_LAYER)
		{
			//������, �� ������� ������������ ������� � ������, ��������
			pShape=pDoc->GetLayerByNumber((pDoc->GetLoadedLayers()-m_nCurrentLayer-1));
			pDoc->SetSelLayer(pShape);
		}
		Invalidate();
	}
}




//��������� �������� ����� ����� � ������ List Control
//������, �� ������ ������� ������� ������������, ����� ������������ ��� ��������
VOID CBoolGraphView::OnNMDblclkListLayers(NMHDR *pNMHDR, LRESULT *pResult)
{
	CBoolGraphDoc *pDoc=GetDocument();
	if(pDoc->GetPaintArea()==FALSE) //���� ���� ��������� �������
	{
		CListCtrl *plstLayers = GET_LST_LAYERS;
		if(m_nCurrentLayer == NULL_LAYER)
			return;
		if(m_nShapesChoosed>=0 && m_nShapesChoosed<MAX_WORKING_LAYERS)
		{
			m_nShapesDeque.SetAt(m_nShapesChoosed, (pDoc->GetLoadedLayers()-m_nCurrentLayer-1));
			m_nShapesChoosed+=1;
			SetShapePoint(m_nCurrentLayer); //������ ����� + � ������ List Control �������� ��������� ������
		}
		else
		{
			ClearShapeDeque();
			for(UINT nI=0; nI<pDoc->GetLoadedLayers();nI++)
				plstLayers->SetItemText(nI, 1,EMPTY_NAME);
			m_nShapesChoosed=0;
		}
	}
}







//-----------------------  ������� ����   ------------------------------
//�������� ������ ������� "�����"
VOID CBoolGraphView::OnProjCreate()
{
	CBoolGraphDoc *pDoc=GetDocument();
	//������������� ��� ��������� � �������� ���������
	m_nCurrentOperation=OP_NO_OPER;   
	m_nCurrentLayer=NULL_LAYER;
	m_ptFirstPoint=NULL;
	m_ptCurMovePoint=NULL;
	if (!pDoc->OnNewDocument())
		AfxMessageBox(MSG_NEW_DOCUMENT_ERROR);
	ClearListControl();
}




//��������� ������� ���� ������->��� ������->�� �������(�� �����)
VOID CBoolGraphView::OnPolygon()
{
	CBoolGraphDoc *pDoc=GetDocument();
	if(pDoc->GetPaintArea() == FALSE)
		CreatePolygon(OP_POLYGON);
	else
		AfxMessageBox(MSG_PAINTING_CLOSE);
}




//��������� ������� ���� ������->��� ������->�� ������
VOID CBoolGraphView::OnPolyline()
{
	CBoolGraphDoc *pDoc=GetDocument();
	if(pDoc->GetPaintArea() == FALSE)
		CreatePolygon(OP_POLYLINE);
	else
		AfxMessageBox(MSG_PAINTING_CLOSE);
}




//����� � ���� �������� �����������
//����������� �������������� �������� ���������� � ����� ��������� �������
VOID CBoolGraphView::OnUnion()
{
	if(IsLegalOperation()==TRUE)
		StartOperation(OP_UNION);
}




//����� � ���� �������� ��������
//����������� �������������� �������� �������� � ����� ��������� �������
VOID CBoolGraphView::OnDifference()
{
	if(IsLegalOperation()==TRUE)
		StartOperation(OP_DIFFERENCE);
}



//����� � ���� �������� �����������
//����������� �������������� �������� ����������� � ����� ��������� �������
VOID CBoolGraphView::OnCross()
{
	if(IsLegalOperation()==TRUE)
		StartOperation(OP_INTERSECT);
}



//------------------- ��������� ������� ������������ ���� --------------------

//������� ������ ������������ ����
void CBoolGraphView::OnContextMenu(CWnd *pWnd, CPoint ptPoint)
{
	CMenu ShapeContextMenu;
	ShapeContextMenu.LoadMenuW(IDR_CONTEXTMENU);
	ShapeContextMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, ptPoint.x, ptPoint.y, this);
}



//��������� �������� �������� � ����������� ����
void CBoolGraphView::OnContextDelete()
{
	DeleteLayerFromList();
}




//��������� ������ ����� ������ � ����������� ����
void CBoolGraphView::OnContextOptions()
{
	OnBnClickedLayerOptions();
}


//������� �������� ����: ����������� ����������/�� ���������� �����
void CBoolGraphView::OnShowTransparent()
{
	if(bIfPaint)
		bIfPaint = FALSE;
	else
		bIfPaint = TRUE;
	CMenu *pMenu = AfxGetMainWnd()->GetMenu();
	UINT nState = pMenu->CheckMenuItem(ID_SHOW_TRANSPARENT, MF_BYCOMMAND);
	if(nState == MF_CHECKED)
		pMenu->CheckMenuItem(ID_SHOW_TRANSPARENT, MF_BYCOMMAND | MF_UNCHECKED);
	else
		pMenu->CheckMenuItem(ID_SHOW_TRANSPARENT, MF_BYCOMMAND | MF_CHECKED);
	Invalidate();
}




//������� �������� ����: ����������� ���� � ��������
void CBoolGraphView::OnViewGrid()
{
	if(bIfGrid)
		bIfGrid = FALSE;
	else
		bIfGrid = TRUE;
	CMenu *pMenu = AfxGetMainWnd()->GetMenu();
	UINT nState = pMenu->CheckMenuItem(ID_VIEW_GRID, MF_BYCOMMAND);
	if(nState == MF_CHECKED)
		pMenu->CheckMenuItem(ID_VIEW_GRID, MF_BYCOMMAND | MF_UNCHECKED);
	else
		pMenu->CheckMenuItem(ID_VIEW_GRID, MF_BYCOMMAND | MF_CHECKED);
	Invalidate();
}




//------------------------------ ������� �������� ����� -------------------------

//������� ������� ������(-��)-���������� ��������
//����: ������ ����� �� �������� ����� �������� ������ - ����������
//�����: ��������� ������, ����������� � ������ �����, ��������� �� �����
VOID CBoolGraphView::CreateShapeResult(CArray <SEdge>* pModel) //exception
{
	ASSERT(pModel);
	CArray <CPoint> ptPoints;
	ptPoints.SetSize(ZERO_SIZE, SINGLE_STEP);
	CBoolGraphDoc *pDoc=GetDocument();
	while(pModel->IsEmpty()==FALSE)
	{
		DetermineShape(pModel, &ptPoints); //������� ��������� ������
		if(ptPoints.IsEmpty() == FALSE)   //� ��������� �
		{
			CShapes *pShape = NULL;
			pDoc->SetPaintAreaOpen();

			TRY
			{
				pShape=new CPolygon;
			}
			CATCH(CMemoryException, pEx)
			{
				pEx->ReportError();
				AfxAbort();
			}
			END_CATCH
			pDoc->SetLoadedLayers(pDoc->GetLoadedLayers()+1);
			pShape->SetPen(THEME_PEN_3, THEME_WIDTH_3, THEME_STYLE_3);
			for(INT nJ=0; nJ<ptPoints.GetSize(); nJ++)
				pShape->AddPoint(ptPoints.GetAt(nJ));
			pDoc->m_LayersList.AddTail(pShape);  //��������� ������ � ����� ������ ����������
			AddLayerAtList();
			pDoc->SetPaintAreaClose();
			ptPoints.RemoveAll();  
			m_nCurrentLayer=NULL_LAYER;
			pDoc->SetModifiedFlag();
			Invalidate();
		}
		else
			pModel->RemoveAll();
	}
	ptPoints.RemoveAll();
}





//����� ������������ ����� ������ ������������� ����� � �������� �������
//������� ������: ������� � ������ �����, � ������� ������������ �����
//�������� ������: ����� �����, ������������ � �������, ���� -1 - ���� ������ ���
//���������: �� ����� ����, ����� ���� ������ - ����� � ������� ���������� ��������� �����,
//           �� ���� ������ ����������� � ������� ��������� �����
INT CBoolGraphView::SetIncedentEdges(const CPoint &ptPoint, const CArray <SEdge>* pSearchArray)
{
	ASSERT(ptPoint.x >= 0);
	ASSERT(ptPoint.y >= 0);
	ASSERT(pSearchArray);
	INT nNumber = -1;
	for(INT nI=0; nI<pSearchArray->GetSize(); nI++)
		if((ptPoint == pSearchArray->GetAt(nI).ptVertex1) || (ptPoint==pSearchArray->GetAt(nI).ptVertex2))
			nNumber=nI;
	return nNumber;
}





//��������� ����� ��������� ������, ���������� ����������� ��������, ����� ������� �����
//������� ������: ��������� �� ������� ����� � ������ ������� ������
//�������� ������: ����������� ������ ������ ������
VOID CBoolGraphView::DetermineShape(CArray <SEdge>* pModel, CArray <CPoint>* pPoints)
{
	ASSERT(pModel);
	ASSERT(pPoints);
	CPoint ptStartPoint = pModel->GetAt(0).ptVertex2;//�������� ������ �����(�����) � ���������� ��� �������
	CPoint ptEndPoint = pModel->GetAt(0).ptVertex1;
	pPoints->Add(ptEndPoint);   //���������� ��������� ����� - ������ � ������ ������ ������
	SEdge TempEdge = pModel->GetAt(0);
	pModel->RemoveAt(0,1);
	INT nNumberOfIncedent = -1;
	while(ptStartPoint != pPoints->GetAt(0))  //��������� ���� ������ �� ���������
	{
		pPoints->Add(ptStartPoint);
		nNumberOfIncedent=SetIncedentEdges(ptStartPoint, pModel);  //���� ����� ������������ �����
		if(nNumberOfIncedent>=0)
		{
			TempEdge = pModel->GetAt(nNumberOfIncedent);
			//� ������� ����� ����������� �������� �� �������������, ������� 
			//���������� ���������� ������� ������� - ������� ��� ��� � ������ ������:
			if(TempEdge.ptVertex1 != ptStartPoint && TempEdge.ptVertex2 == ptStartPoint) 
			{
				ptStartPoint = TempEdge.ptVertex1;  //���������� ������� ������ ���������� �����
				ptEndPoint = TempEdge.ptVertex2;
			}
			else if(TempEdge.ptVertex2 != ptStartPoint && TempEdge.ptVertex1 == ptStartPoint)
			{
				ptStartPoint=TempEdge.ptVertex2;
				ptEndPoint = TempEdge.ptVertex1;
			}
			pModel->RemoveAt(nNumberOfIncedent);
			nNumberOfIncedent=-1;
		}
		else 
		{
			//��-�� ����������� ��� �������� ����������� ����� ����� ���� ������������
			//����� ���������� ��� ������� ����������, ������� �� ������ ���� � �������� �������
			CArray <CPoint> TempPoints;
			TempPoints.SetSize(ZERO_SIZE, SINGLE_STEP);
			for(INT nI = (INT)pPoints->GetSize()-2; nI > 0; nI--)
				TempPoints.Add(pPoints->GetAt(nI));
			pPoints->Append(TempPoints);
			break;
		}
	}
}





//�������� ������� ���� �������
//���������� �������� ��� ��������� ������ ������ ������� 
VOID CBoolGraphView::CreatePolygon(const UINT &nOperation)         //exception
{
	CBoolGraphDoc *pDoc=GetDocument();
	if(IsLegalShape()==TRUE)
	{
		CShapes *pShape=NULL;
		TRY
		{
			pShape=new CPolygon;
		}
		CATCH(CMemoryException, pEx)
		{
			pEx->ReportError();
			AfxAbort();
		}
		END_CATCH
		pDoc->SetLoadedLayers(pDoc->GetLoadedLayers()+1);
		pShape->SetPen(THEME_PEN_1, THEME_WIDTH_1, THEME_STYLE_1);
		pDoc->m_LayersList.AddTail(pShape);
		(nOperation==OP_POLYGON)?m_nCurrentOperation=OP_POLYGON:m_nCurrentOperation=OP_POLYLINE;
		pDoc->SetPaintAreaOpen(); //��������� � ������� ��������� ������� ������
		pDoc->SetModifiedFlag();
		AddLayerAtList();
	}
	else
		m_nCurrentOperation=OP_NO_OPER;

}



//������� ��������� �������� ���������� �������� ��� ��������,
//����� ���� ����� ��������� �� ��� ������
//������� ������: ����� �������� (�����������, ����������� ��� ��������)
VOID CBoolGraphView::StartOperation(const UINT &nOperation)
{
	ASSERT(nOperation);
	CShapes* pFirstShape;
	CShapes* pSecondShape;     
	CBoolGraphDoc *pDoc=GetDocument();
	pFirstShape=pDoc->GetLayerByNumber(m_nShapesDeque.GetAt(FIRST_ELEMENT));
	pSecondShape=pDoc->GetLayerByNumber(m_nShapesDeque.GetAt(SECOND_ELEMENT));
	COverlay OverlayShape; //������� ������ ������ ������� ��� ���������� ��������
	CArray <SEdge> ModelEdges;
	ModelEdges.SetSize(ZERO_SIZE, SINGLE_STEP);
	switch(nOperation)
	{
		case OP_UNION:
			OverlayShape.MakeOperation(pFirstShape, pSecondShape, OP_UNION);
			break;
		case OP_INTERSECT:
			OverlayShape.MakeOperation(pFirstShape, pSecondShape, OP_INTERSECT);
			break;
		case OP_DIFFERENCE:
			OverlayShape.MakeOperation(pFirstShape, pSecondShape, OP_DIFFERENCE);
			break;
	}
	OverlayShape.GetResult(&ModelEdges);
	CreateShapeResult(&ModelEdges);
}
