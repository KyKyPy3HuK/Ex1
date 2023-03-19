﻿
// ChildView.cpp: реализация класса CChildView
//

#include "pch.h"
#include "framework.h"
#include "Ex1.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CChildView::fText(CPaintDC& dc, LPCTSTR text, int x, int y)
{
	dc.TextOutW(x, y, text);
}


void CChildView::drawPicture(CPaintDC& dc, int* bitMap, int width, int heigth, int x, int y) {


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

	return TRUE;
}


void CChildView::OnPaint() 
{
	CPaintDC dc(this); // контекст устройства для рисования
	
	// TODO: Добавьте код обработки сообщений
	fText(dc,_T("Hello Hell!"), 10,10);
	
	int* bMap = new int[256] {

		0, 1, 1, 1,  1, 1, 0, 0,  0, 0, 1, 0,  1, 1, 1, 0,
		1, 0, 1, 1,  1, 1, 0, 1,  1, 0, 1, 0,  0, 1, 0, 0,
		1, 1, 0, 1,  1, 1, 0, 1,  1, 0, 1, 0,  0, 1, 0, 0,
		1, 1, 1, 0,  1, 1, 0, 1,  1, 0, 1, 0,  1, 0, 1, 0,

		1, 1, 1, 1,  0, 1, 0, 1,  1, 0, 1, 0,  1, 0, 1, 0,
		1, 1, 1, 1,  0, 1, 0, 1,  1, 0, 1, 0,  1, 1, 1, 0,
		1, 1, 1, 1,  0, 1, 0, 1,  1, 0, 1, 0,  1, 1, 1, 0,
		1, 1, 1, 1,  0, 1, 0, 0,  0, 0, 1, 0,  1, 1, 1, 0,

		0, 0, 0, 0,  0, 1, 0, 1,  1, 0, 1, 0,  1, 1, 1, 0,
		1, 1, 1, 1,  0, 1, 0, 1,  1, 0, 1, 0,  1, 1, 1, 0,
		1, 1, 1, 1,  0, 1, 0, 1,  1, 0, 1, 0,  1, 1, 1, 0,
		1, 1, 1, 1,  0, 1, 0, 1,  1, 0, 1, 0,  1, 1, 1, 0,

		1, 1, 1, 0,  1, 1, 0, 1,  1, 0, 1, 0,  1, 1, 1, 0,
		1, 1, 0, 1,  1, 1, 0, 1,  1, 0, 1, 0,  1, 1, 1, 0,
		1, 0, 1, 1,  1, 1, 0, 1,  1, 0, 1, 0,  1, 1, 1, 0, 
		0, 1, 1, 1,  1, 1, 0, 0,  0, 0, 1, 0,  1, 1, 1, 0,
	};

	drawPicture(dc,bMap,16,16,50,50 );
	// Не вызывайте CWnd::OnPaint() для сообщений рисования
}

