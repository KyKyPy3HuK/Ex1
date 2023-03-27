
// ChildView.cpp: реализация класса CChildView

#include "pch.h"
#include "framework.h"
#include "Ex1.h"
#include "ChildView.h"
#include <string>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BYTES_PER_PIXEL 3
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
	ON_BN_CLICKED(IDB_BTN_DRAWBLT, OnBtnDrawBltClick)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CChildView::fText(CPaintDC& dc, LPCTSTR text, int x, int y)
{
	dc.TextOutW(x, y, text);
}


void CChildView::drawPicture(CDC& dc, BYTE* bitmap, int width, int heigth, int x, int y) {
	UINT widthInBytes = width * BYTES_PER_PIXEL;
	UINT offset = (4 - (widthInBytes % 4)) % 4; 
	UINT fullWidth = widthInBytes + offset;
	for (int i = 0; i < heigth; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			UINT column = j;
			UINT row = heigth - i;
			UINT k = (fullWidth * i + j * 3);
			dc.SetPixel(column + x, row + y, RGB(bitmap[k + 2], bitmap[k + 1], bitmap[k]));
		}
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
	

	pictureFile->Open(_T("mouse.bmp"), CFile::modeRead | CFile::shareDenyRead);
	pictureFile->Read(&bmHeader, sizeof(BITMAPFILEHEADER));
	pictureFile->Read(&bmInfo, sizeof(BITMAPINFOHEADER));
	
	LPBYTE bitmap = new BYTE [bmInfo.biSizeImage];
	LPBYTE readByte = new BYTE [bmInfo.biSizeImage];
	
	pictureFile->Read(readByte, sizeof(BYTE) * bmInfo.biSizeImage); //read BLUE
	bitmap = readByte;
	mbitmap = readByte;
	
	c_bitmap.LoadBitmapW(IDB_BITMAP_MOUSE);
	
	return TRUE;
}


void CChildView::OnPaint() 
{
	CPaintDC dc(this); // контекст устройства для рисования
	
	std::wstring str = std::to_wstring(bmInfo.biSizeImage);
	LPCTSTR lpsBiWidth = str.c_str();
	std::wstring str1 = std::to_wstring(bmInfo.biHeight);
	LPCTSTR lpsBiHeight = str1.c_str();


	if (isImageDrawing)
	{
		drawPicture(dc, mbitmap, bmInfo.biWidth, bmInfo.biHeight, 0, 50);
	}


	if(isImageBltDrawing){ // bitmap != NULL 
		CDC memDC;
		if (!memDC.CreateCompatibleDC(&dc))
		{
			return;
		}
		BITMAP bm;
		c_bitmap.GetBitmap(&bm);
		//drawPicture(memDC, mbitmap, bmInfo.biWidth, bmInfo.biHeight, 0, 0);
		CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&c_bitmap);
		dc.StretchBlt(400, 50, bmInfo.biWidth, bmInfo.biHeight, &memDC, 0, 0, bmInfo.biWidth, bmInfo.biHeight,SRCCOPY);
		memDC.SelectObject(pOldBitmap);
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

void  CChildView::OnBtnDrawBltClick() {
	isImageBltDrawing = !isImageBltDrawing;
	Invalidate();
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (isImageBltDrawing )//c_bitmap.GetSafeHandle() != NULL && 
	{
		return true;
	}
	return CWnd::OnEraseBkgnd(pDC);
}
BOOL CChildView::SetBitmap(UINT nIDResourse){
	return c_bitmap.LoadBitmapW(nIDResourse);
}