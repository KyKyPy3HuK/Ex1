
// ChildView.h: интерфейс класса CChildView
#pragma once
// Окно CChildView

class CChildView : public CWnd
{
// Создание
public:
	CChildView();

// Атрибуты
public:
	
// Операции
public:

// Переопределение
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Элементы
public:
	CButton btnDraw,
			btnDrawBlt,
			btnErase;
	
// Переменные
protected:

	CFile* pictureFile;
	CBitmap c_bitmap;
	BITMAPFILEHEADER bmHeader;
	BITMAPINFOHEADER bmInfo;
	BYTE* mbitmap;
	BITMAP bmp;
	bool isImageDrawing = false;
	bool isImageBltDrawing = false;
	COLORREF color = RGB(0, 255, 0);

// Реализация
public:

	void fText(CPaintDC& dc, LPCTSTR text, int x, int y);
	void drawPicture(CDC& dc, BYTE* bitMap, int width, int heigth, int x, int y);
	BOOL SetBitmap(UINT nIDResourse);
	virtual ~CChildView();

	// Созданные функции схемы сообщений
protected:
	afx_msg void OnBtnDrawClick();
	afx_msg void OnBtnDrawBltClick();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

