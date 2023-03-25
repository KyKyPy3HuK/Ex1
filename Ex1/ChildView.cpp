
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


void CChildView::drawPicture(CDC& dc, int* bitMap, int width, int heigth, int x, int y) {


	for (int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (bitMap[i+ (heigth*j)] == 1)
			{
				dc.SetPixel(i + x, j + y, RGB(0, 0, 0));
			}
			else {
				dc.SetPixel(i + x, j + y, RGB(255, 0, 0));
			}
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

	pictureFile->Open(_T("YuiUSSR.bmp"), CFile::modeRead | CFile::shareDenyRead);
	pictureFile->Read(&bmHeader, sizeof(BITMAPFILEHEADER));
	pictureFile->Read(&bmInfo, sizeof(BITMAPINFOHEADER));
	void* bmBits = new char[bmInfo.biSizeImage];
	pictureFile->Read(bmBits, sizeof(bmBits));
	c_bitmap.CreateBitmap(bmInfo.biWidth, bmInfo.biHeight, bmInfo.biPlanes, bmInfo.biBitCount, bmBits);
	return TRUE;
}


void CChildView::OnPaint() 
{
	CPaintDC dc(this); // контекст устройства для рисования
	
	std::wstring str = std::to_wstring(bmInfo.biWidth);
	LPCTSTR lpsBiWidth = str.c_str();
	std::wstring str1 = std::to_wstring(bmInfo.biHeight);
	LPCTSTR lpsBiHeight = str1.c_str();
	std::wstring str2 = std::to_wstring(bmInfo.biSize);
	LPCTSTR lpsBiSize = str2.c_str();
	std::wstring str3 = std::to_wstring(bmInfo.biBitCount);
	LPCTSTR lpsBiBitCount = str3.c_str();
	std::wstring str4 = std::to_wstring(bmInfo.biSizeImage);
	LPCTSTR lpsBiSizeImage = str4.c_str();
	if( bitmap != NULL && isImageDrawing){ 
		fText(dc, _T("sas"), 10, 10);
	}
	else
	{
		fText(dc, lpsBiWidth, 10, 10);
		fText(dc, lpsBiHeight, 10, 30);
		fText(dc, lpsBiSize, 10, 50);
		fText(dc, lpsBiBitCount, 10, 70);
		fText(dc, lpsBiSizeImage, 10, 90);
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
	if (c_bitmap.GetSafeHandle() != NULL && isImageDrawing)
	{
		return true;
	}
	return CWnd::OnEraseBkgnd(pDC);
}
BOOL CChildView::SetBitmap(UINT nIDResourse){
	return c_bitmap.LoadBitmapW(nIDResourse);
}