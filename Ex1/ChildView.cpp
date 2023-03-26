
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
	for (int i = 0; i < heigth * width * COLORS_COUNT; i += 3)
	{
		int row = (i / 3) % width;
		int column = heigth - (i / 3) / width;
		dc.SetPixel(row + x, column + y, RGB(bitmap[i], bitmap[i + 1], bitmap[i+2]));
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
	
	LPBYTE bitmap = new BYTE [bmInfo.biSizeImage];
	BYTE readByte;
	
	for (int i = 0; i < bmInfo.biSizeImage; i++)
	{
		pictureFile->Read(&readByte, sizeof(BYTE)); //read BLUE
		bitmap[i] = readByte;
	}

	c_bitmap.CreateBitmap(1024,628, bmInfo.biPlanes, bmInfo.biBitCount, bitmap);
	c_bitmap.LoadBitmapW(IDB_BITMAP2);
	mbitmap = bitmap;
	return TRUE;
}


void CChildView::OnPaint() 
{
	CPaintDC dc(this); // контекст устройства для рисования
	
	std::wstring str = std::to_wstring(bmInfo.biWidth);
	LPCTSTR lpsBiWidth = str.c_str();
	std::wstring str1 = std::to_wstring(bmInfo.biHeight);
	LPCTSTR lpsBiHeight = str1.c_str();

	if(isImageDrawing && c_bitmap.GetSafeHandle() != NULL){ // bitmap != NULL 
		
		CDC memDC;
		if (!memDC.CreateCompatibleDC(&dc))
		{
			return;
		}
		BITMAP bm;

		c_bitmap.GetBitmap(&bm);

		printf("%d", bm.bmBits);
		CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&c_bitmap);
		dc.StretchBlt(50, 50, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, bm.bmWidth, bm.bmHeight,SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		//drawPicture(memDC, mbitmap, bmInfo.biWidth, bmInfo.biHeight, 50, 50);
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
	if (isImageDrawing && c_bitmap.GetSafeHandle() != NULL)//c_bitmap.GetSafeHandle() != NULL && 
	{
		return true;
	}
	return CWnd::OnEraseBkgnd(pDC);
}
BOOL CChildView::SetBitmap(UINT nIDResourse){
	return c_bitmap.LoadBitmapW(nIDResourse);
}