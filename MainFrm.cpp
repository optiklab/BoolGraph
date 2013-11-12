// MainFrm.cpp : implementation of the CMainFrame class
#include "stdafx.h"
#include "BoolGraph.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(!_InitLayersDlgBar()) return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Не удалось создать панель инструментов\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Не удалось создать панель статуса\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME ;
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

CDialogBar* CMainFrame::GetLayersDlg()
{
	return &m_wndLayers;
}

BOOL CMainFrame::_InitLayersDlgBar() 
{
	if (!m_wndLayers.Create(this, IDD_LAYERS,CBRS_ALIGN_RIGHT, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Не удалось создать диалоговое окно\n");
		return FALSE;
	}

	CListCtrl *plstLayers =(CListCtrl*) m_wndLayers.GetDlgItem(IDC_LIST_LAYERS);

	CRect ListClRect;
	plstLayers->GetClientRect(&ListClRect);
	INT ColumnWidth = INT(ListClRect.Width() * 0.6);
	plstLayers->InsertColumn(0, _T("Слои"), LVCFMT_LEFT, ColumnWidth);
	plstLayers->InsertColumn(1, _T("Выбран"), LVCFMT_LEFT, ListClRect.Width()-ColumnWidth);
	return TRUE;
}

