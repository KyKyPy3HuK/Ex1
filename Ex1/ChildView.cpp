﻿
// ChildView.cpp: реализация класса CChildView
//

#include "pch.h"
#include "framework.h"
#include "Ex1.h"
#include "ChildView.h"
#include <string>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define COLORS_COUNT 3
#define RED 2
#define GREEN 1
#define BLUE 0

// CChildView

CChildView::CChildView()
{
	pictureFile = new CFile();
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDB_BTN_DRAW, OnBtnDrawClick)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CChildView::fText(CPaintDC& dc, LPCTSTR text, int x, int y)
{
	dc.TextOutW(x, y, text);
}


void CChildView::drawPicture(CDC& dc, BYTE* bitmap, int width, int heigth, int x, int y) {
	for (int i = 0; i < heigth * width * COLORS_COUNT; i+=3)
	{
		int row = (i/3) % width;
		int column =  heigth - (i/3) / width;
		dc.SetPixelV(row + x, column + y, RGB(bitmap[i+2], bitmap[i+1], bitmap[i]));
	}
}
// Обработчики сообщений CChildView

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), LoadIcon(NULL,IDI_APPLICATION));

	pictureFile->Open(_T("YuiUSSR.bmp"), CFile::modeRead | CFile::shareDenyRead);
	pictureFile->Read(&bmHeader, sizeof(BITMAPFILEHEADER));
	pictureFile->Read(&bmInfo, sizeof(BITMAPINFOHEADER));

	BYTE* bitmap = new BYTE[bmInfo.biWidth * bmInfo.biHeight * COLORS_COUNT];
	BYTE readByte;

	for (int i = 0; i < bmInfo.biWidth * bmInfo.biHeight * COLORS_COUNT; i++)
	{
		pictureFile->Read(&readByte, sizeof(BYTE)); //read BLUE
		bitmap[i] = readByte;
	}
	mbitmap = bitmap;

	//c_bitmap.CreateBitmap(bmInfo.biWidth, bmInfo.biHeight, bmInfo.biPlanes, bmInfo.biBitCount, bitmap);
	return TRUE;
}


void CChildView::OnPaint() 
{
	CPaintDC dc(this); // контекст устройства для рисования
	
	std::wstring str = std::to_wstring(bmInfo.biWidth);
	LPCTSTR lpsBiWidth = str.c_str();
	std::wstring str1 = std::to_wstring(bmInfo.biHeight);
	LPCTSTR lpsBiHeight = str1.c_str();

	if(isImageDrawing){ // bitmap != NULL && 
		fText(dc, _T("sas"), 10, 10);
		drawPicture(dc, mbitmap, bmInfo.biWidth, bmInfo.biHeight, 50, 50);
	}
	else
	{
		fText(dc, lpsBiWidth, 10, 10);
		fText(dc, lpsBiHeight, 10, 30);

	}
	
	
	// TODO: Добавьте код обработки сообщений
	
	
	
	// Не вызывайте CWnd::OnPaint() для сообщений рисования
}

void  CChildView::OnBtnDrawClick() {
	isImageDrawing = !isImageDrawing;
	Invalidate();
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (isImageDrawing)//c_bitmap.GetSafeHandle() != NULL && 
	{
		return true;
	}
	return CWnd::OnEraseBkgnd(pDC);
}
BOOL CChildView::SetBitmap(UINT nIDResourse){
	return c_bitmap.LoadBitmapW(nIDResourse);
}