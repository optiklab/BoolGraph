#pragma once
#include "Shapes.h"

//Класс CBoolGraphDoc - Модель
//Отвечает за открытие, загрузку и сохранение данных
//также содержит некоторые дополнительные функции для работы со списком фигур
class CBoolGraphDoc : public CDocument
{
protected:
	__declspec(nothrow) CBoolGraphDoc();
	DECLARE_DYNCREATE(CBoolGraphDoc)
	__declspec(nothrow) virtual ~CBoolGraphDoc();
	//Переопределения
public:
	//функция создания нового документа
	virtual BOOL OnNewDocument();
	//стандартный метод сохранения и загрузки данных
	virtual void Serialize(CArchive& ar);   //exception

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//загрузить все фигуры в List Control (при открытии документа)
	__declspec(nothrow) BOOL LoadLayersAtListControl();

	//очищаем список указателей на фигуры
	__declspec(nothrow) VOID ClearLayerList();

	//устанавливает выбранную фигуру в List Control, как активную
	__declspec(nothrow) VOID SetSelLayer(CShapes*);

	//получить указатель на фигуру по её номеру в списке фигур
	__declspec(nothrow) CShapes* GetLayerByNumber(const INT &)const;

	//получить указатель на выбранную фигуру
	__declspec(nothrow) CShapes* GetSelLayer()const;

	//удалить выбранную фигуру
	__declspec(nothrow) BOOL DeleteSelectedLayer(const INT &);

	//получение области, занятой выделенной на текующий момент фигурой
	__declspec(nothrow) VOID GetRegion(CRgn &)const;

	//позволяет получить число загруженных фигур
	__declspec(nothrow) UINT GetLoadedLayers()const;

	//позволяет установить число загруженных фигур
	__declspec(nothrow) VOID SetLoadedLayers(const UINT &);   

	//Устанавливает флаг занятости области рисования
	__declspec(nothrow) VOID SetPaintAreaClose();

	//Устанавливает флаг свободной области рисования
	__declspec(nothrow) VOID SetPaintAreaOpen();

	//Возвращает состояние области рисования
	__declspec(nothrow) BOOL GetPaintArea()const;
 
public:
	CTypedPtrList<CObList, CShapes*>  m_LayersList;    //список фигур, которыми будет оперировать пользователь

private:
	CShapes* m_pSelectedLayer; //указатель на выбранную пользователем фигуру
	UINT m_nVersion;           //номер версии документа
	UINT m_nLoadLayers;        //число загружаемых фигур
	BOOL m_PaintAreaIsClear;   //метка, разрешающая или запрещающая рисовать в Области рисования

protected:
	DECLARE_MESSAGE_MAP()
};


