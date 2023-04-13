
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
	m_pictureFile = new CFile();
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDB_BTN_DRAW, OnBtnDrawClick)
	ON_BN_CLICKED(IDB_BTN_DRAWBLT, OnBtnDrawBltClick)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_APP_OPEN, &OnAppOpen)
END_MESSAGE_MAP()

uint8_t* CChildView::palletToNormalBitmap(BITMAPINFO& biInfo, RGBQUAD* pallet, uint8_t* bitmap)
{
	uint8_t		bpp					= biInfo.bmiHeader.biBitCount;
	uint32_t	width				= biInfo.bmiHeader.biWidth;
	uint32_t	height				= biInfo.bmiHeader.biHeight;
	uint32_t	sizeInBytes			= biInfo.bmiHeader.biSizeImage;
	uint32_t	widthInBits			= bpp * width;
	uint32_t	widthInBytes;		if (widthInBits % 8 == 0) {widthInBytes = widthInBits / 8;} else {widthInBytes = widthInBits / 8 + 1;}
	uint32_t	alignBytes			= (4 - (widthInBytes % 4)) % 4;
	uint32_t	fullWidthInBytes	= widthInBytes + alignBytes;


	uint8_t		n_bpp				= 24;
	uint32_t	n_widthInBytes		= width * 3;
	uint32_t	n_alignBytes		= (4 - (n_widthInBytes % 4)) % 4;
	uint32_t	n_fullWidthInBytes	= n_widthInBytes + n_alignBytes;
	uint32_t	n_sizeInBytes		= n_fullWidthInBytes * height;

	uint8_t*	n_bitmap = new uint8_t[n_sizeInBytes];

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			bitmap[i];
		}
	}




	return n_bitmap;
}

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
					::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), 
					LoadIcon(NULL,IDI_APPLICATION));
	
	return TRUE;
}


void CChildView::OnPaint() 
{
	CPaintDC dc(this); // контекст устройства для рисования
	
	std::wstring str = std::to_wstring(bmInfo.biSizeImage);
	LPCTSTR lpsBiWidth = str.c_str();
	std::wstring str1 = std::to_wstring(bmInfo.biHeight);
	LPCTSTR lpsBiHeight = str1.c_str();


	if (isImageDrawing && isFileOpen)
	{
		drawPicture(dc, m_bitmap, bmInfo.biWidth, bmInfo.biHeight, 0, 50);
	}



	if(isImageBltDrawing && isFileOpen){ // bitmap != NULL 
		CDC memDC;
		if (!memDC.CreateCompatibleDC(&dc))
		{
			return;
		}

		HGDIOBJ pOldBitmap = memDC.SelectObject(m_HBitmap);
		dc.StretchBlt(400, 50, m_bitmapInfo.bmiHeader.biWidth, m_bitmapInfo.bmiHeader.biHeight
			, &memDC, 0, 0, m_bitmapInfo.bmiHeader.biWidth, m_bitmapInfo.bmiHeader.biHeight,SRCCOPY);
		memDC.SelectObject(pOldBitmap);
	}

	// TODO: Добавьте код обработки сообщений
	// Не вызывайте CWnd::OnPaint() для сообщений рисования
}

void  CChildView::OnBtnDrawClick() {
	if (isFileOpen)
	{
		isImageDrawing = !isImageDrawing;
	}
	else
	{
		AfxMessageBox(_T("Файл не выбран!"));
	}
	Invalidate();
}

void  CChildView::OnBtnDrawBltClick() {
	
	if (isFileOpen)
	{
		isImageBltDrawing = !isImageBltDrawing;
	}
	else
	{
		AfxMessageBox(_T("Файл не выбран!"));
	}
	Invalidate();
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	//if (isImageBltDrawing)//c_bitmap.GetSafeHandle() != NULL && 
	//{
	//	return true;
	//}
	return CWnd::OnEraseBkgnd(pDC);
}


void CChildView::OnAppOpen() {
	CFileDialog opnFileDlg(TRUE,NULL,NULL,NULL, _T("bmp Files (*.bmp)|*.bmp|"),NULL, 0, 1);

	if (opnFileDlg.DoModal() == IDOK)
	{
		if (isFileOpen)
		{
			m_pictureFile->Close();
		}
		isFileOpen = true;

		AfxMessageBox(_T("Выбранный файл: ") + opnFileDlg.GetPathName());

		m_pictureFile->Open(opnFileDlg.GetPathName(), CFile::modeRead | CFile::shareDenyRead);
		m_pictureFile->Read(&bmHeader, sizeof(BITMAPFILEHEADER));
		m_pictureFile->Read(&bmInfo, sizeof(BITMAPINFOHEADER));

		uint8_t* inputBitmap = new uint8_t[bmInfo.biSizeImage];

		if (bmInfo.biBitCount <= 8)
		{
			uint8_t palletColorsCount;
			if (bmInfo.biBitCount = 8)
			{
				palletColorsCount = 256;
				m_pallet = new RGBQUAD[256];
			}
			else if (bmInfo.biBitCount = 4)
			{
				palletColorsCount = 16;
				m_pallet = new RGBQUAD[16];
			}
			else if (bmInfo.biBitCount = 1)
			{
				palletColorsCount = 2;
				m_pallet = new RGBQUAD[2];
			}

			m_pictureFile->Read(&m_pallet, sizeof(RGBQUAD) * palletColorsCount);

		}

		m_pictureFile->Read(inputBitmap, sizeof(uint8_t) * bmInfo.biSizeImage); //read File

		CDC dc;
		dc.CreateCompatibleDC(this->GetDC());

		m_bitmapInfo.bmiHeader = bmInfo;
		m_DIBSectionBitmap = new uint8_t[bmInfo.biSizeImage];
		m_HBitmap = CreateDIBSection(dc, &m_bitmapInfo, DIB_RGB_COLORS,
			(void**)&m_DIBSectionBitmap, NULL, 0);

		memcpy(m_DIBSectionBitmap, inputBitmap, bmInfo.biSizeImage);
		m_bitmap = inputBitmap;
	}
	else {
		AfxMessageBox(_T("Файл не выбран!"));
	}
	
}