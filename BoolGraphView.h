#pragma once

//Класс CBoolGraphView - Вид
//отвечает за отображение данных и сочетает в себе функции класса-контроллера, согласуя 
//состояние приложения с возможностями элементов управления
class CBoolGraphView : public CView
{
protected:
	__declspec(nothrow) CBoolGraphView();
	DECLARE_DYNCREATE(CBoolGraphView)

//Переопределения
public:
	__declspec(nothrow) virtual ~CBoolGraphView();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//функция перерисовки окна 
	virtual void OnDraw(CDC* pDC);

#ifdef _DEBUG
	virtual void AssertValid() const;
	__declspec(nothrow) virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	//метод возвращает указатель на документ
	CBoolGraphDoc* GetDocument() const;

	//функция рисования указывающей линии-метки
	__declspec(nothrow) VOID DrawMoveLine(const CPoint &, const CPoint &);

	//функция рисования сетки
	__declspec(nothrow) VOID DrawGrid(CDC* pDC);

	//----------- функции для работы с элементом интерфеса List Control -------------
	//функция добавления новой фигуры в List Control
	VOID AddLayerAtList();      //exception

	//функция удаления фигуры из List Control
	__declspec(nothrow) VOID DeleteLayerFromList();

	//функция очистки List Control
	__declspec(nothrow) BOOL ClearListControl();
	
	//функция очистки очереди выбранных фигур
	__declspec(nothrow) VOID ClearShapeDeque();

	//функция установки метки  POINT_SYMBOL(+) возле указанной иконки в List Control
	__declspec(nothrow) BOOL SetShapePoint(const INT &);

	//-------------------------------- функции проверки -----------------------------
	//функция возвращает ответ, корректен ли номер выбранной в List Control фигуры
	__declspec(nothrow) BOOL IsShapeNumberCorrect(const INT &)const;

	//функция проверяет, возможно ли следующее добавление фигуры в проект
	__declspec(nothrow) BOOL IsLegalShape();

	//функция проверяет, существуют ли фигуры, под выбранными номерами
	__declspec(nothrow) BOOL IsLegalOperation();

	//------------------------------ функции создания фигур -------------------------
	//функция создает фигуры-результаты операции
	VOID CreateShapeResult(CArray <SEdge>*);      //exception

	//метод осуществляет поиск номера инциндентного ребра к заданной вершине в массиве ребер
	__declspec(nothrow) INT SetIncedentEdges(const CPoint &, const CArray <SEdge>*);

	//выполняет поиск очередной фигуры, являющейся результатом операции, среди массива ребер
	__declspec(nothrow) VOID DetermineShape(CArray <SEdge>*, CArray <CPoint>*);

	//создание объекта типа Полигон
	VOID CreatePolygon(const UINT &);            //exception
	
	//функция запускает механизм выполнения операций над фигурами
	__declspec(nothrow) VOID StartOperation(const UINT &);

private:
	//------------------------ обработка нажатия кнопок мыши -----------------------
	afx_msg VOID OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg VOID OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg VOID OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg VOID OnMouseMove(UINT nFlags, CPoint point);

	//---------------------- обработка нажатий кнопок на форме ---------------------
	//Нажатие на кнопку "Добавить" - добавляет новый чистый слой для рисования фигуры
	afx_msg VOID OnBnClickedAddLayer();

	//Нажатие на кнопку "Удалить" - удаляет выбранную фигуру
	afx_msg VOID OnBnClickedDeleteLayer();

	//Нажатие на кнопку "Опции фигуры"
	afx_msg VOID OnBnClickedLayerOptions();

	//-------------- обработка нажатий мышкой в списке List Control ----------------
	//обработка одинарного клика - выделения фигуры
	afx_msg VOID OnNMClickListLayers(NMHDR* pNMHDR, LRESULT* pResult);

	//обработка двойного клика - выбор фигуры для операции
	afx_msg VOID OnNMDblclkListLayers(NMHDR* pNMHDR, LRESULT* pResult);

	//---------------------- обработка функций меню --------------------------------
	//создание нового проекта
	afx_msg VOID OnProjCreate();  

	//рисование произвольной фигуры
	afx_msg VOID OnPolygon();

	//рисование многоугольника
	afx_msg VOID OnPolyline();

	//операция объединения
	afx_msg VOID OnUnion();  

	//операция разность
	afx_msg VOID OnDifference();  

	 //операция пересечение
	afx_msg VOID OnCross();      

	//------------------- обработка функций контекстного меню --------------------
	//вызов контекстного меню
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint ptPoint);
	//обработка операции удаления в контекстном меню
	afx_msg void OnContextDelete();
	//обработка вызова опций фигуры в контекстном меню
	afx_msg void OnContextOptions();
	
	//Функция главного меню: отображения прозрачных/не прозрачных фигур
	afx_msg void OnShowTransparent();
	
	//Функция главного меню: отображение поля в клеточку
	afx_msg void OnViewGrid();

private:
	UINT m_nCurrentOperation;   //Номер текущей операции рисования, сами номера перечислены в GlobalData.h
	INT m_nCurrentLayer;	    //Номер выбранной фигуры в списке List Control
	CPoint m_ptFirstPoint;      //Начальная точка фигуры при рисовании
	CPoint m_ptCurMovePoint;    //Текущая точка, в которой находится пользователь
	CArray <INT> m_nShapesDeque;  //очередь номеров из списка фигур, которые выделены для произведения операций
	UINT m_nShapesChoosed;      //текущее количество фигур, которые уже выбраны для выполнения операции
	BOOL bIfPaint;              //метка необходимости заполнения фигур при рисовании
	BOOL bIfGrid;               //метка необходимости рисования клетки на поле рисования
};

#ifndef _DEBUG  // debug version in BoolGraphView.cpp
inline CBoolGraphDoc* CBoolGraphView::GetDocument() const
   { return reinterpret_cast<CBoolGraphDoc*>(m_pDocument); }
#endif

