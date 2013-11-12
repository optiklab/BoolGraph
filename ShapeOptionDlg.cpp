// ShapeOptionDlg.cpp : implementation file
#include "stdafx.h"
#include "BoolGraph.h"
#include "ShapeOptionDlg.h"
#include "GlobalData.h"

// CShapeOptionDlg dialog

IMPLEMENT_DYNAMIC(CShapeOptionDlg, CDialog)


//Инициализация полей класса
CShapeOptionDlg::CShapeOptionDlg(CWnd* pParent):CDialog(CShapeOptionDlg::IDD, pParent), 
												m_ShapeName(DEFAULT_NAME),
												m_PenColor(RGB(0,0,0)),
												m_BrushColor(RGB(0,0,0))
{
	m_Brush.CreateStockObject(WHITE_BRUSH);	  //Создать белую кисть
}



CShapeOptionDlg::~CShapeOptionDlg()
{}




//Функция обмена данными между элементами интерфейса и классом
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




//Возвращает имя фигуры
//Выходные данные: строка с именем фигуры
CString CShapeOptionDlg::GetShapeName()const
{
	return m_ShapeName;
}




//Возвращает цвет заливки
//Выходные данные: длинное целое характеризующее цвет кисти(заливки)
COLORREF CShapeOptionDlg::GetBrushColor()const
{
	return m_BrushColor;
}




//Возвращает цвет пера
//Выходные данные: длинное целое характеризующее цвет пера
COLORREF CShapeOptionDlg::GetPenColor()const
{
	return m_PenColor;
}




//Возвращает стиль заливки
//Выходные данные: целое число - номер типа заливки
INT CShapeOptionDlg::GetBrushStyle()const
{
	return m_BrushStyle;
}




//Устанавливает имя фигуры
//Входные данные: адрес на строку с именем фигуры
VOID CShapeOptionDlg::SetShapeName(const CString &sShapeName)
{
	ASSERT(sShapeName);
	m_ShapeName = sShapeName;
}



//Устанавливает цвет заливки
//Входные данные: длинное целое содержащее цвет кисти(заливки)
VOID CShapeOptionDlg::SetBrushColor(const COLORREF rgbBrushColor)
{	
	m_BrushColor = rgbBrushColor;
}



//Устанавливает цвет пера
//Входные данные: длинное целое содержащее цвет пера
VOID CShapeOptionDlg::SetPenColor(const COLORREF rgbPenColor)
{
	m_PenColor = rgbPenColor;
}


//Устанавливает тип заливки
//Входные данные: адрес на целое число - номер стиля заливки
VOID CShapeOptionDlg::SetBrushStyle(const INT &iBrushStyle)   
{
	m_BrushStyle = iBrushStyle;
}



//Инициализация диалогового окна
//Выходные данные: TRUE или FALSE - успешно или нет закончилась операция
BOOL CShapeOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//Добавляем названия стилей в элемент ListBox
	m_BrushStyleCtrl.AddString(_T("сплошная"));
	m_BrushStyleCtrl.AddString(_T("НЛ-ВП диагональ"));
	m_BrushStyleCtrl.AddString(_T("ВЛ-НП диагональ"));
	m_BrushStyleCtrl.AddString(_T("клетка"));
	m_BrushStyleCtrl.AddString(_T("диагональная клетка"));
	m_BrushStyleCtrl.AddString(_T("горизонтальная"));
	m_BrushStyleCtrl.AddString(_T("вертикальная"));
	m_BrushStyleCtrl.SetCurSel(0);
	return TRUE;
}



//Обработка клика на пиктограмме выбора цвета пера
//Появляется стандартный диалог выбора цвета Windows,
//после нажатия ОК значения сохраняются
VOID CShapeOptionDlg::OnStnClickedColorPen()
{
	CColorDialog dlg(m_PenColor); 
	if(dlg.DoModal()==IDOK)
	{
		m_PenColor=dlg.GetColor(); //Установка цвета
		CBrush swatch;     
		swatch.CreateSolidBrush(m_PenColor);  //Создание кисти
		CClientDC dc(this);
		//Закрашиваем идикатор цвета пера...
		CWnd* pPenColor=GetDlgItem(IDC_COLOR_PEN); //Получаем указатель на элемент управления
		pPenColor->GetWindowRect(&m_PenColorSwatch);
		ScreenToClient(&m_PenColorSwatch); 	//Изменяем координаты относительно клиентской области
		dc.FillRect(&m_PenColorSwatch, &swatch);    //Закрашиваем индикатор выбранным цветом
	}
}



//Обработка клика на пиктограмме выбора цвета заливки
//В стандартном диалоге выбора цвета пользователю предлагается выбрать цвет заливки кистью
VOID CShapeOptionDlg::OnStnClickedColorBrush()
{
	CColorDialog dlg(m_BrushColor); 

	if(dlg.DoModal()==IDOK)
	{
		m_BrushColor=dlg.GetColor(); //Установка цвета
		CBrush swatch;
		swatch.CreateSolidBrush(m_BrushColor);
		CClientDC dc(this);
		//Закрашиваем идикатор цвета кисти...
		CWnd* pBrushColor=GetDlgItem(IDC_COLOR_BRUSH);
		pBrushColor->GetWindowRect(&m_BrushColorSwatch);
		ScreenToClient(&m_BrushColorSwatch);
		dc.FillRect(&m_BrushColorSwatch, &swatch);
	}
	OnLbnSelchangeBrushStyleList();
}


//Выбор стиля заливки в ListBox
//Кликнув в списке ListBox на определенном значении, выбирается соответствующий стиль
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
	PaintBrushPreview(); //Зарисовка области просмотра стиля заливки
}




//Функция закрашивает область просмотра стиля заливки
VOID CShapeOptionDlg::PaintBrushPreview()
{
	CClientDC dc(this);
	CWnd* pBrushStyle=GetDlgItem(IDC_BRUSH_STYLE);
	pBrushStyle->GetWindowRect(&m_BrushStyleSwatch);
	ScreenToClient(&m_BrushStyleSwatch);
	m_BrushStyleSwatch.DeflateRect(2,2,1,1); 
	dc.FillRect(&m_BrushStyleSwatch,&m_Brush);
}
