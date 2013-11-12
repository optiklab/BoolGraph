#pragma once
#include "afxwin.h"


// ласс CShapeOptionDlg 
//отвечает за элементы управлени€, наход€щиес€ в окне ќпций фигуры,
//сохран€ет все значени€, которые установлены пользователем
class CShapeOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(CShapeOptionDlg)
public:
	__declspec(nothrow) CShapeOptionDlg(CWnd* pParent = NULL);
	__declspec(nothrow) virtual ~CShapeOptionDlg();
	//им€ диалогового окна
	enum { IDD = IDD_SHAPE_OPTIONS };

protected:
	//функци€, реализующа€ обмен данными
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP() //объ€вл€ем карту сообщений
public:
	//возвращает им€ фигуры
	__declspec(nothrow) CString GetShapeName()const;

	//возвращает цвет заливки
	__declspec(nothrow) COLORREF GetBrushColor()const;

	//возвращает цвет пера
	__declspec(nothrow) COLORREF GetPenColor()const;

	//возвращает тип заливки
	__declspec(nothrow) INT GetBrushStyle()const;

	//устанавливает им€ фигуры
	__declspec(nothrow) VOID SetShapeName(const CString &);

	//устанавливает цвет заливки
	__declspec(nothrow) VOID SetBrushColor(const COLORREF);

	//устанавливает цвет пера
	__declspec(nothrow) VOID SetPenColor(const COLORREF);

	//устанавливает тип заливки
	__declspec(nothrow) VOID SetBrushStyle(const INT &);


private:
	//функци€ инициализации диалогового окна
	virtual BOOL OnInitDialog();

	//нажатие на кнопке ÷вет пера
	afx_msg VOID OnStnClickedColorPen();

	//нажатие на кнопке ÷вет заливки
	afx_msg VOID OnStnClickedColorBrush();

	//изменение стил€ заливки
	afx_msg VOID OnLbnSelchangeBrushStyleList();

	//зарисовать область просмотра стил€ заливки
	__declspec(nothrow) VOID PaintBrushPreview();

private:
	
	CString m_ShapeName;
	COLORREF m_PenColor;
	CRect m_PenColorSwatch;     //координаты индикатора цвета пера
	CBrush m_Brush;
	COLORREF m_BrushColor;
	CRect m_BrushColorSwatch;   //координаты индикатора цвета кисти
	CRect m_BrushStyleSwatch;   //координаты индикатора просмотра стил€ заливки
	INT m_BrushStyle;
	CListBox m_BrushStyleCtrl;  //элемент управлени€ ListBox
};
