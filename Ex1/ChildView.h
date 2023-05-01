
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
	CScrollBar	m_scrBarV,
				m_scrBarH;
	int m_scrBarVPos,
		m_scrBarHPos;
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

	BITMAPINFO m_biInfo;
	BITMAP m_bm;

	bool isFileOpen = false;
	bool isImageDrawing = false;
	bool isImageBltDrawing = false;
	COLORREF color = RGB(0, 255, 0);

// Реализация
public:

	uint8_t* palletToNormalBitmap(BITMAPINFO& biInfo, RGBQUAD* pallet, uint8_t* bitmap);

	void fText(CPaintDC& dc, LPCTSTR text, int x, int y);
	void drawPicture(CDC& dc, BYTE* bitMap, int width, int heigth, int x, int y);
	virtual ~CChildView();

	// Созданные функции схемы сообщений
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnScrollH();
	afx_msg void OnPaint();
	afx_msg void OnAppOpen();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

