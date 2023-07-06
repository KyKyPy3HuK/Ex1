
// ChildView.cpp: реализация класса CChildView
#include <tiffio.h>
#include "pch.h"
#include "framework.h"
#include "Ex1.h"
#include "ChildView.h"
#include <string>

#define getBit(val,x) ((val >> x) & 0x1)
#define setBit(val,x) (val |= (1 << x))
#define clrBit(val,x) (val &= ~(1 << x))

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
	m_scrBarVPos = 0;
	m_pictureFile = new CFile();
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_APP_OPEN, &OnAppOpen)
END_MESSAGE_MAP()

uint8_t* CChildView::palletToNormalBitmap(BITMAPINFO& biInfo, RGBQUAD* pallet, uint8_t* bitmap)
{
	uint8_t		clrUsed				= biInfo.bmiHeader.biClrUsed;
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
	memset(n_bitmap, 0, n_sizeInBytes);
	
	if (bpp == 1)
	{
		for (int i = 0; i < height; i++)
		{
			int cnt = 0;
			for (int j = 0; j < widthInBytes; j++)
			{
				uint32_t k = (fullWidthInBytes * i + j);
				uint8_t tempByte = bitmap[k];
				for ( ; cnt < width && cnt < 8 * (j + 1); ++cnt)
				{
					uint8_t bitNum = getBit(tempByte,7 - (cnt % 8));
					uint32_t n_k = ((cnt * 3) + (i * n_fullWidthInBytes));
					n_bitmap[n_k]	  = pallet[bitNum].rgbGreen;
					n_bitmap[n_k + 1] = pallet[bitNum].rgbRed;
					n_bitmap[n_k + 2] = pallet[bitNum].rgbBlue;
				}
			}
		}
	}
	else if (bpp == 8)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				uint32_t k = (fullWidthInBytes * i + j);
				uint8_t tempByte = bitmap[k];
				uint32_t n_k = (((j) * 3) + (i * n_fullWidthInBytes));
				n_bitmap[n_k	] = pallet[tempByte].rgbBlue;
				n_bitmap[n_k + 1] = pallet[tempByte].rgbGreen;
				n_bitmap[n_k + 2] = pallet[tempByte].rgbRed;
			}
		}
	}
	biInfo.bmiHeader.biSizeImage = n_sizeInBytes;
	biInfo.bmiHeader.biBitCount = n_bpp;
	biInfo.bmiHeader.biClrUsed = 0;

	return n_bitmap;
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
	CRect winRect = new CRect;
	GetWindowRect(winRect);

	int32_t VRange = m_bitmapInfo.bmiHeader.biHeight - winRect.Height() + 4;
	int32_t HRange = m_bitmapInfo.bmiHeader.biWidth - winRect.Width() + 4;

	const uint8_t SCROLL_OFFSET = 18;

	if (VRange > 0)
	{
		SetScrollRange(SB_VERT, 0, VRange);
		if (HRange + SCROLL_OFFSET > 0)
		{
			SetScrollRange(SB_VERT, 0, VRange + SCROLL_OFFSET);
			SetScrollRange(SB_HORZ, 0, HRange + SCROLL_OFFSET);
		}
		else
		{
			SetScrollRange(SB_HORZ, 0, 0);
			m_scrBarHPos = 0;
		}
	}
	else if (HRange > 0)
	{
		SetScrollRange(SB_HORZ, 0, HRange);
		if (VRange + SCROLL_OFFSET > 0)
		{
			SetScrollRange(SB_HORZ, 0, HRange + SCROLL_OFFSET);
			SetScrollRange(SB_VERT, 0, VRange + SCROLL_OFFSET);
		}
		else
		{
			SetScrollRange(SB_VERT, 0, 0);
			m_scrBarVPos = 0;
		}
	}
	else
	{
		SetScrollRange(SB_VERT, 0, 0);
		m_scrBarVPos = 0;
		SetScrollRange(SB_HORZ, 0, 0);
		m_scrBarHPos = 0;
	}

	if(isFileOpen){ // bitmap != NULL 
		CDC memDC;
		if (!memDC.CreateCompatibleDC(&dc))
		{
			return;
		}

		HGDIOBJ pOldBitmap = memDC.SelectObject(m_HBitmap);
		dc.StretchBlt(0, 0, m_bitmapInfo.bmiHeader.biWidth, m_bitmapInfo.bmiHeader.biHeight
			, &memDC, 0 + m_scrBarHPos, 0 + m_scrBarVPos, m_bitmapInfo.bmiHeader.biWidth, m_bitmapInfo.bmiHeader.biHeight,SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		isImageDrawed = true;
	}

	// TODO: Добавьте код обработки сообщений
	// Не вызывайте CWnd::OnPaint() для сообщений рисования
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	//TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (isImageDrawed)//c_bitmap.GetSafeHandle() != NULL && 
	{
		return false;
	}
	return CWnd::OnEraseBkgnd(pDC);
}

void CChildView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (isFileOpen)
	{
	// TODO: Add your message handler code here and/or call default
	
	m_scrBarVPos = nPos; // Сохранение текущей позиции ползунка
	switch (nSBCode) // Что было нажато на линейке
	{
		// Зацепили и потащили ползунок
	case SB_THUMBPOSITION:
		// Выставить ползунок в текущую позицию
		SetScrollPos(SB_VERT, nPos);
		break;
		// Закончили работу с линейкой
	case SB_ENDSCROLL:
		// Получить текущую позицию ползунка
		m_scrBarVPos = GetScrollPos(SB_VERT);
		break;
		// Нажали на стрелку вверх (наверху линейки)
	case SB_LINEUP:
		nPos = GetScrollPos(SB_VERT);
		nPos--;
		if (nPos < 0)
			nPos = 0;
		SetScrollPos(SB_VERT, nPos);
		m_scrBarVPos = nPos;
		break;
			// Нажали на стрелку вниз (внизу линейки)
	case SB_LINEDOWN:
		nPos = GetScrollPos(SB_VERT);
		nPos++;
		if (nPos > 100)
			nPos = 100;
		SetScrollPos(SB_VERT, nPos);
		m_scrBarVPos = nPos;
		break;
	}
	// Перерисовать окно
	Invalidate();
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}

void CChildView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (isFileOpen)
	{
		// TODO: Add your message handler code here and/or call default

		m_scrBarHPos = nPos; // Сохранение текущей позиции ползунка
		switch (nSBCode) // Что было нажато на линейке
		{
			// Зацепили и потащили ползунок
		case SB_THUMBPOSITION:
			// Выставить ползунок в текущую позицию
			SetScrollPos(SB_HORZ, nPos);
			break;
			// Закончили работу с линейкой
		case SB_ENDSCROLL:
			// Получить текущую позицию ползунка
			m_scrBarHPos = GetScrollPos(SB_HORZ);
			break;
			// Нажали на стрелку вверх (наверху линейки)
		case SB_LINEUP:
			nPos = GetScrollPos(SB_HORZ);
			nPos--;
			if (nPos < 0)
				nPos = 0;
			SetScrollPos(SB_HORZ, nPos);
			m_scrBarHPos = nPos;
			break;
			// Нажали на стрелку вниз (внизу линейки)
		case SB_LINEDOWN:
			nPos = GetScrollPos(SB_HORZ);
			nPos++;
			if (nPos > 100)
				nPos = 100;
			SetScrollPos(SB_HORZ, nPos);
			m_scrBarHPos = nPos;
			break;
		}
		// Перерисовать окно
		Invalidate();
		CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}

void CChildView::OnAppOpen() {
	CFileDialog opnFileDlg(TRUE,NULL,NULL,NULL, _T("bmp Files (*.bmp)|*.bmp|"),NULL, 0, 1);

	if (opnFileDlg.DoModal() == IDOK)
	{
		if (isFileOpen)
		{
			m_pictureFile->Close();
		}
		isImageDrawed = false;
		isFileOpen = true;

		m_pictureFile->Open(opnFileDlg.GetPathName(), CFile::modeRead | CFile::shareDenyRead);
		m_pictureFile->Read(&bmHeader, sizeof(BITMAPFILEHEADER));
		m_pictureFile->Read(&bmInfo, sizeof(BITMAPINFOHEADER));

		uint8_t* inputBitmap = new uint8_t[bmInfo.biSizeImage];
		bool isPalletBmp = false;
		if (bmInfo.biBitCount <= 8)
		{
			m_pallet = new RGBQUAD[bmInfo.biClrUsed];
			m_pictureFile->Read(m_pallet, sizeof(RGBQUAD) * bmInfo.biClrUsed);
			isPalletBmp = true;  
		}
		
		m_pictureFile->Read(inputBitmap, sizeof(uint8_t) * bmInfo.biSizeImage); //read File

		CDC dc;
		dc.CreateCompatibleDC(this->GetDC());

		m_bitmapInfo.bmiHeader = bmInfo;

		if (isPalletBmp)
		{
			inputBitmap = palletToNormalBitmap(m_bitmapInfo, m_pallet, inputBitmap);
		}
		
		m_DIBSectionBitmap = new uint8_t[m_bitmapInfo.bmiHeader.biSizeImage];
		m_HBitmap = CreateDIBSection(dc, &m_bitmapInfo, DIB_RGB_COLORS,
			(void**)&m_DIBSectionBitmap, NULL, 0);

		memcpy(m_DIBSectionBitmap, inputBitmap, m_bitmapInfo.bmiHeader.biSizeImage);
		m_bitmap = inputBitmap;

		Invalidate();
	}
	else {
		AfxMessageBox(_T("Файл не выбран!"));
	}
	
}