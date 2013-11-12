#pragma once
// ������ ������ � �������� �������� ���������
const UINT FILE_VERSION = 1000;

const UINT MAX_WORKING_LAYERS = 2;        //������������ ����� ����� ��� ������������� ��������� �����. �����������
const UINT MAX_LOADABLE_LAYERS = 20;     //������������ ����� ����������� � ��������� �����

//������ �����, ����������� ��� �������������� ��������
const UINT FIRST_SHAPE = 1;
const UINT SECOND_SHAPE = 2;
const UINT FIRST_ELEMENT = 0;
const UINT SECOND_ELEMENT = 1;


//-------------------����� � ����� � List Control---------------------------------------------
//������, ������� �������� ����� � ���������� ������� ������ � ������ �������
const CString POINT_SYMBOL = _T("+");
//��������� ������ ���
const CString EMPTY_NAME = _T("");
//��� �������� ���� ��� ������ �������
const CString VIEW_NULL_SHAPES = _T("��� �����");
//��� �������� ���� ��� �������� �������
const CString VIEW_ALL_SHAPES = _T("��� ������");
//��� ������ �� ���������
const CString DEFAULT_NAME = _T("����� ������");


// ������� ��� ��������� ��������� �� ������� ��������� List Control
#define GET_FRAME_WND	((CMainFrame*) theApp.m_pMainWnd)
#define GET_WND_LAYERS_ITEM(x) (GET_FRAME_WND->GetLayersDlg()->GetDlgItem(x))
#define GET_LST_LAYERS	((CListCtrl*) GET_WND_LAYERS_ITEM(IDC_LIST_LAYERS))
//� ����� *.cpp, ��������, ��� ��������� ��������� �� List Control, ����� ��������� ���: 
//CListCtrl* plstLayers =(CListCtrl*)((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetLayersDlg()->GetDlgItem(IDC_LIST_LAYERS);


//��������� ���������� ��������� �������� ��������� ������:
const UINT OP_NO_OPER = 0;           //���������� ��������
const UINT OP_POLYLINE = 1;          //��������� ��������������
const UINT OP_POLYGON = 2;          //��������� ������������ ������
const UINT OP_ELLIPSE = 3;            //��������� �������


//��� ��������, ������� ���������� ���������� � ����������������
const UINT OP_UNION = 11;
const UINT OP_INTERSECT = 12;
const UINT OP_DIFFERENCE = 13;


//��������� ���������� ������ �����, ������� � ������ ����� �� ����������, ��
//�������� ������� � �������� ������ ���������:
const INT FIRST_LAYER = 0; 
const INT NULL_LAYER = -1;


//�������� ���� �����
//���� 1 �������
const COLORREF THEME_PEN_1 = RGB(100,0,0);
const UINT THEME_WIDTH_1 = 1;
const UINT THEME_STYLE_1 = PS_SOLID;
//���� 2 �������
const COLORREF THEME_PEN_2 = RGB(0,0,100);
const UINT THEME_WIDTH_2 = 2;
const UINT THEME_STYLE_2 = PS_SOLID;
//���� 3 ���������
const COLORREF THEME_PEN_3 = RGB(255,0,0);
const UINT THEME_WIDTH_3 = 2;
const UINT THEME_STYLE_3 = PS_SOLID;
//���� ������ � ���� ���������
const COLORREF GRID_COLOR = RGB(158,224,216);


//�������������� ���������
const UINT ZERO_SIZE = 0;
const UINT SINGLE_STEP = 1;
const UINT ONE_ELEMENT = 1;
const UINT SOLID_BRUSH = -1;
const UINT STEP_BACK = -1;
const DOUBLE MPI = 25.4;
const DOUBLE EPSILON = 5.0;