#pragma once
// версия файлов с которыми работает программа
const UINT FILE_VERSION = 1000;

const UINT MAX_WORKING_LAYERS = 2;        //максимальное число слоев для одновременной обработки матем. алгоритмами
const UINT MAX_LOADABLE_LAYERS = 20;     //максимальное число загружаемых в программу слоев

//Номера фигур, необходимые для упорядочивания операций
const UINT FIRST_SHAPE = 1;
const UINT SECOND_SHAPE = 2;
const UINT FIRST_ELEMENT = 0;
const UINT SECOND_ELEMENT = 1;


//-------------------Имена и знаки в List Control---------------------------------------------
//символ, который рисуется рядом с выделенной двойным кликом в списке фигурой
const CString POINT_SYMBOL = _T("+");
//полностью пустое имя
const CString EMPTY_NAME = _T("");
//имя главного слоя при пустом проекте
const CString VIEW_NULL_SHAPES = _T("Нет фигур");
//имя главного слоя при непустом проекте
const CString VIEW_ALL_SHAPES = _T("Все фигуры");
//Имя фигуры по умолчанию
const CString DEFAULT_NAME = _T("Новая фигура");


// макросы для получения указателя на элемент интерфеса List Control
#define GET_FRAME_WND	((CMainFrame*) theApp.m_pMainWnd)
#define GET_WND_LAYERS_ITEM(x) (GET_FRAME_WND->GetLayersDlg()->GetDlgItem(x))
#define GET_LST_LAYERS	((CListCtrl*) GET_WND_LAYERS_ITEM(IDC_LIST_LAYERS))
//в файле *.cpp, например, для получения указателя на List Control, будет выглядеть так: 
//CListCtrl* plstLayers =(CListCtrl*)((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetLayersDlg()->GetDlgItem(IDC_LIST_LAYERS);


//константы определяют выбранную операцию рисования фигуры:
const UINT OP_NO_OPER = 0;           //отсутствие операции
const UINT OP_POLYLINE = 1;          //рисование многоугольника
const UINT OP_POLYGON = 2;          //рисование произвольной фигуры
const UINT OP_ELLIPSE = 3;            //рисование эллипса


//тип операции, которую необходимо произвести с многоугольниками
const UINT OP_UNION = 11;
const UINT OP_INTERSECT = 12;
const UINT OP_DIFFERENCE = 13;


//константы определяют номера слоев, которые в списке слоев не фигурируют, но
//являются важными в процессе работы программы:
const INT FIRST_LAYER = 0; 
const INT NULL_LAYER = -1;


//ЦВЕТОВЫЕ ТЕМЫ ФИГУР
//Тема 1 Обычная
const COLORREF THEME_PEN_1 = RGB(100,0,0);
const UINT THEME_WIDTH_1 = 1;
const UINT THEME_STYLE_1 = PS_SOLID;
//Тема 2 Выделен
const COLORREF THEME_PEN_2 = RGB(0,0,100);
const UINT THEME_WIDTH_2 = 2;
const UINT THEME_STYLE_2 = PS_SOLID;
//Тема 3 Результат
const COLORREF THEME_PEN_3 = RGB(255,0,0);
const UINT THEME_WIDTH_3 = 2;
const UINT THEME_STYLE_3 = PS_SOLID;
//Цвет клетки в поле рисования
const COLORREF GRID_COLOR = RGB(158,224,216);


//Дополнительные константы
const UINT ZERO_SIZE = 0;
const UINT SINGLE_STEP = 1;
const UINT ONE_ELEMENT = 1;
const UINT SOLID_BRUSH = -1;
const UINT STEP_BACK = -1;
const DOUBLE MPI = 25.4;
const DOUBLE EPSILON = 5.0;