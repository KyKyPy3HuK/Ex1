
// ChildView.h: интерфейс класса CChildView
#pragma once
// Окно CChildView
#include "rotateDialolg.h"

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
	RotateDialog rotateDialog;
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
	bool isImageDrawed = false;
	bool isFileOpen = false;
	bool isImageDrawing = false;
	bool isImageBltDrawing = false;
	COLORREF color = RGB(0, 255, 0);

// Реализация
public:
	int CChildView::alignWidthInBytes(int imageWidth);
	int CChildView::alignImageSizeInBytes(int imageWidth, int imageHeight);
	uint8_t* palletToNormalBitmap(BITMAPINFO& biInfo, RGBQUAD* pallet, uint8_t* bitmap);
	uint8_t* CChildView::tiffToNormalBitmap(TIFF* tiff);

	void drawPicture(CDC& dc, BYTE* bitMap, int width, int heigth, int x, int y);
	virtual ~CChildView();

	// Созданные функции схемы сообщений
protected:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos,
		CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos,
		CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnAppOpen();
	
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnAppRotate();

	afx_msg void OnBnClickedOk();
};

