
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

	CFile* m_pictureFile;

	BITMAPFILEHEADER bmHeader;
	BITMAPINFOHEADER bmInfo;

	uint8_t* m_bitmap;
	uint8_t* m_DIBSectionBitmap;

	RGBQUAD* m_pallet;
	HBITMAP m_HBitmap;
	BITMAPINFO m_bitmapInfo;

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
	afx_msg void OnAppOpen();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

