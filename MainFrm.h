// MainFrm.h : interface of the CMainFrame class
#pragma once

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	CDialogBar* GetLayersDlg();
	BOOL _InitLayersDlgBar();

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CDialogBar	m_wndLayers;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};


