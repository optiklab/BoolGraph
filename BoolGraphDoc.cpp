// BoolGraphDoc.cpp : implementation of the CBoolGraphDoc class
#include "stdafx.h"
#include "BoolGraph.h"
#include "BoolGraphDoc.h"
#include "Shapes.h"
#include "Polygon.h"
#include "GlobalData.h"
#include "MainFrm.h"
#include "atlsimpstr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBoolGraphDoc
IMPLEMENT_DYNCREATE(CBoolGraphDoc, CDocument)

BEGIN_MESSAGE_MAP(CBoolGraphDoc, CDocument)
END_MESSAGE_MAP()



CBoolGraphDoc::CBoolGraphDoc():	m_nVersion(FILE_VERSION),//������ �������
								m_PaintAreaIsClear(FALSE)//������� ��������� ������
{
	ClearLayerList();  //������� ������ ����� ����� ������� ������
}





CBoolGraphDoc::~CBoolGraphDoc()
{
	ClearLayerList();   //������� ������ ����� �� ��������� ������
}



//������� ������������� ��������� ��������� ��� �������� ������ ���������
BOOL CBoolGraphDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	CBoolGraphDoc::ClearLayerList();
	UpdateAllViews(NULL); 
	return TRUE;
}



//����� �������� ������ ������� �� ����� ��� ������ � ����
//������� ������: ����� �� ����������-�����
void CBoolGraphDoc::Serialize(CArchive& ar)    //exception
{
	if (ar.IsStoring())
	{    //������ � ����
		TRY
		{
			ar<<m_nVersion;
			ar<<m_nLoadLayers;
		}
		CATCH(CFileException, pEx)
		{
			pEx->ReportError();
		}
		END_CATCH
	}
	else
	{   //������ �� �����
		TRY
		{
			ClearLayerList();
			ar>>m_nVersion;
			ar>>m_nLoadLayers;
			m_PaintAreaIsClear=FALSE;
		}
		CATCH(CFileException, pEx)
		{
			pEx->ReportError();
		}
		END_CATCH
	}

	TRY
	{
		m_LayersList.Serialize(ar);  //������� ����������/�������� ������ �����
	}
	CATCH_ALL(pEx)
	{
		pEx->ReportError();
		AfxAbort();
	}
	END_CATCH_ALL
	if(LoadLayersAtListControl()==FALSE)  //��������� ��� ������ � List Control
		AfxMessageBox(MSG_FILE_CORRUPT);
}




//����������� ��������������� �������
#ifdef _DEBUG
void CBoolGraphDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBoolGraphDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG




//������� ��������� ��� ������ � List Control
//�������� ������� �������� ������ ���������� �� ������
BOOL CBoolGraphDoc::LoadLayersAtListControl()    
{
	CListCtrl* plstLayers = GET_LST_LAYERS;    //�������� ��������� �� ������� List Control
	plstLayers->DeleteAllItems();  //������� List Control
	if(m_nLoadLayers<=0)
		return FALSE;
	else
	{
		if(m_nLoadLayers>MAX_LOADABLE_LAYERS)
			AfxMessageBox(MSG_TOO_MANY_LAYERS);
		CShapes *pShape = NULL;
		for(UINT nI=0; nI<m_nLoadLayers;nI++)   //��������� ���� � List Control
		{
			pShape=CBoolGraphDoc::GetLayerByNumber(nI); 
			INT nError=plstLayers->InsertItem(0, pShape->GetShapeName());
			if(nError==-1)
				AfxMessageBox(MSG_ADD_LAYER_ERROR);
		}
		UpdateAllViews(NULL);
	}
	return TRUE;
}



//������� ������� ������ �� �������� �����, ������� ���� ������� �����
VOID CBoolGraphDoc::ClearLayerList()
{
	POSITION pos = NULL;
	while(m_LayersList.IsEmpty()==0)  //������� ������ ����������
		delete m_LayersList.RemoveHead();
	m_pSelectedLayer=NULL;    //������ �� ��������
	m_nLoadLayers=0;      //�� ��������� �� ����� ������
	UpdateAllViews(NULL);
}






//---------------------- ����� � ��������� ������ � List Control -------------------

//������� ������������� ��������� ������ ��� ��������
//������� ������: ��������� �� ��������� ������
VOID CBoolGraphDoc::SetSelLayer(CShapes *pSelected)
{
	ASSERT(pSelected);
	if(pSelected != NULL)
		m_pSelectedLayer=pSelected;
	else
		m_pSelectedLayer=NULL;
}


//���������� ��������� ������ �� � ������ � ������ ����������
//������� ������: ����� ������ ����������� ������
//�������� ������: ��������� �� ������
CShapes* CBoolGraphDoc::GetLayerByNumber(const INT &nLayerNumber)const
{
	ASSERT(nLayerNumber >= NULL_LAYER);
	CShapes* pShape;
	POSITION pos=NULL;
	//���������, ����� ����� ������ ��� � ��������� ����� ����������� �����
	if(nLayerNumber>NULL_LAYER && nLayerNumber<(INT)m_nLoadLayers)
	{
		pos=m_LayersList.FindIndex(nLayerNumber);
		if(pos!=NULL)
			pShape = m_LayersList.GetAt(pos);
	}
	else
		pShape=NULL;
	return pShape;
}


//������� ���������� ��������� �� ��������� ����
//�������� �������: ��������� �� ������ ���� ������
CShapes* CBoolGraphDoc::GetSelLayer()const
{
	return m_pSelectedLayer;
}


//������� ��������� ������ �� ������ ����������
//������� ������: ����� ��������� ������
//���������� TRUE ��� �������� ��������, ���� FALSE
BOOL CBoolGraphDoc::DeleteSelectedLayer(const INT &nLayerNumber)
{
	ASSERT(nLayerNumber > NULL_LAYER);
	if(m_pSelectedLayer==NULL) 
		return FALSE;
	CShapes *pShape=NULL;
	POSITION pos=NULL;
	pos=m_LayersList.FindIndex(nLayerNumber); //������ ������� ������� � ������ 
	if(pos!=NULL) // ������� ������ � ���� �������
	{
		m_LayersList.RemoveAt(pos);
		delete m_pSelectedLayer;
		m_pSelectedLayer=NULL;
		return TRUE;
	}
	return FALSE;
}


//������� �������� �������, ������� ���������� �� �������� ������ �������
//������� ������: ����� �� ����������-������, � ������� ����� �������� ������
VOID CBoolGraphDoc::GetRegion(CRgn &Region)const
{
	m_pSelectedLayer->GetRegion(Region);
}





 //----------------------- ����� ����������� ����� ---------------------------------------------
//������� ���������� ����� ����������� ����� 
//�������� ������: ����� ����������� �����
UINT CBoolGraphDoc::GetLoadedLayers()const
{
	return m_nLoadLayers;
}


//������� ������������� ����� ����������� �����
//������� ������: �����, ������� ���������� ����������
VOID CBoolGraphDoc::SetLoadedLayers(const UINT &nLayersNumber)
{
	m_nLoadLayers = nLayersNumber;
}




//---------------------������ ������� � ������������ ������� ��������� ------------------

//������������� ���� ��������� ������� ���������
VOID CBoolGraphDoc::SetPaintAreaClose()
{
	m_PaintAreaIsClear=FALSE;
}


//������������� ���� ��������� ������� ���������
VOID CBoolGraphDoc::SetPaintAreaOpen()
{
	m_PaintAreaIsClear=TRUE;
}


//���������� ��������� ������� ��������� (������/��������)
//�������� ������: �� ��� ��� - �������� ��� ��� �������
BOOL CBoolGraphDoc::GetPaintArea()const
{
	return m_PaintAreaIsClear;
}

