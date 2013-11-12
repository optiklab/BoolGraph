#pragma once
#include "Shapes.h"

//����� CBoolGraphDoc - ������
//�������� �� ��������, �������� � ���������� ������
//����� �������� ��������� �������������� ������� ��� ������ �� ������� �����
class CBoolGraphDoc : public CDocument
{
protected:
	__declspec(nothrow) CBoolGraphDoc();
	DECLARE_DYNCREATE(CBoolGraphDoc)
	__declspec(nothrow) virtual ~CBoolGraphDoc();
	//���������������
public:
	//������� �������� ������ ���������
	virtual BOOL OnNewDocument();
	//����������� ����� ���������� � �������� ������
	virtual void Serialize(CArchive& ar);   //exception

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//��������� ��� ������ � List Control (��� �������� ���������)
	__declspec(nothrow) BOOL LoadLayersAtListControl();

	//������� ������ ���������� �� ������
	__declspec(nothrow) VOID ClearLayerList();

	//������������� ��������� ������ � List Control, ��� ��������
	__declspec(nothrow) VOID SetSelLayer(CShapes*);

	//�������� ��������� �� ������ �� � ������ � ������ �����
	__declspec(nothrow) CShapes* GetLayerByNumber(const INT &)const;

	//�������� ��������� �� ��������� ������
	__declspec(nothrow) CShapes* GetSelLayer()const;

	//������� ��������� ������
	__declspec(nothrow) BOOL DeleteSelectedLayer(const INT &);

	//��������� �������, ������� ���������� �� �������� ������ �������
	__declspec(nothrow) VOID GetRegion(CRgn &)const;

	//��������� �������� ����� ����������� �����
	__declspec(nothrow) UINT GetLoadedLayers()const;

	//��������� ���������� ����� ����������� �����
	__declspec(nothrow) VOID SetLoadedLayers(const UINT &);   

	//������������� ���� ��������� ������� ���������
	__declspec(nothrow) VOID SetPaintAreaClose();

	//������������� ���� ��������� ������� ���������
	__declspec(nothrow) VOID SetPaintAreaOpen();

	//���������� ��������� ������� ���������
	__declspec(nothrow) BOOL GetPaintArea()const;
 
public:
	CTypedPtrList<CObList, CShapes*>  m_LayersList;    //������ �����, �������� ����� ����������� ������������

private:
	CShapes* m_pSelectedLayer; //��������� �� ��������� ������������� ������
	UINT m_nVersion;           //����� ������ ���������
	UINT m_nLoadLayers;        //����� ����������� �����
	BOOL m_PaintAreaIsClear;   //�����, ����������� ��� ����������� �������� � ������� ���������

protected:
	DECLARE_MESSAGE_MAP()
};


