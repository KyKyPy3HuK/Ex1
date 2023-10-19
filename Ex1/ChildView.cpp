
// ChildView.cpp: реализация класса CChildView

#include "pch.h"
#include "framework.h"
#include "Ex1.h"
#include "ChildView.h"
#include <string>
#include <iostream>
#include <stdarg.h>
#include "rotateDialolg.h"
#include <math.h>
#define getBit(val,x) ((val >> x) & 0x1)
#define setBit(val,x) (val |= (1 << x))
#define clrBit(val,x) (val &= ~(1 << x))
#define M_PI       3.14159265358979323846

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
	ON_COMMAND(ID_APP_ROTATE, &OnAppRotate)
	ON_COMMAND(ID_APP_OPEN, &OnAppOpen)
END_MESSAGE_MAP()


int CChildView::getPixelIndex(BITMAPINFO biInfo, int row, int column) {
	uint32_t	width = biInfo.bmiHeader.biWidth;
	uint32_t	height = biInfo.bmiHeader.biHeight;
	uint32_t	sizeInBytes = biInfo.bmiHeader.biSizeImage;
	uint32_t	widthInBytes = sizeInBytes / height;
	uint32_t	alignBytes = (4 - (widthInBytes % 4)) % 4;
	uint32_t	fullWidthInBytes = widthInBytes + alignBytes;

	return 0;
};

int CChildView::alignWidthInBytes(int imageWidthInBytes) {
	uint32_t	alignBytes = (4 - (imageWidthInBytes % 4)) % 4;
	uint32_t	fullWidthInBytes = imageWidthInBytes + alignBytes;
	return fullWidthInBytes;
}

int CChildView::alignImageSizeInBytes(int imageWidth, int imageHeight) {

	uint32_t fullWidthInBytes = alignWidthInBytes(imageWidth * 3);
	return fullWidthInBytes * imageHeight;
}

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

uint8_t* CChildView::tiffToNormalBitmap(TIFF* tiff) {
	uint32_t imageLength = -1;
	TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &imageLength);
	uint32_t imageWidth = -1;
	TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &imageWidth);
	std::cout << imageLength << std::endl;
	uint32_t lineSize = TIFFScanlineSize(tiff);


	uint32_t fullWidthInBytes = alignWidthInBytes(lineSize);
	uint32_t fullSizeInBytes = alignImageSizeInBytes(imageWidth, imageLength);

	uint8_t* bitmap = new uint8_t[fullSizeInBytes]{ 0 };

	std::cout << lineSize << "   " << fullWidthInBytes << "   " << imageWidth << " FSIB " << fullSizeInBytes;

	uint8_t* buf = new uint8_t[lineSize]{ 0 };

	for (int i = 0; i < imageLength; i++)
	{
		TIFFReadScanline(tiff, buf,i);

		for (int j = 0; j < lineSize; j++)
		{
			bitmap[(fullSizeInBytes - fullWidthInBytes * (i + 1)) + j] = buf[j];
		}
	}

	return bitmap;
}



uint8_t* CChildView::rotateBitmap(BITMAPINFO& biInfo, uint8_t* bitmap, double angleDEG) {

	int32_t	width = biInfo.bmiHeader.biWidth;
	int32_t	height = biInfo.bmiHeader.biHeight;
	int32_t	sizeInBytes = biInfo.bmiHeader.biSizeImage;
	int32_t	widthInBytes = sizeInBytes / height;
	int32_t	alignBytes = (4 - (widthInBytes % 4)) % 4;
	int32_t	fullWidthInBytes = widthInBytes + alignBytes;

	double angleRAD = (M_PI /180) * angleDEG;

	int32_t	nX; 
	int32_t	nY;
	double		cosf = std::cos(angleRAD);
	double		sinf = std::sin(angleRAD);
	CDC dc;
	
	//начальные координаты углов
	int32_t	XLeftTop = 0;
	int32_t	YLeftTop = 0;

	int32_t	XRightTop = width;
	int32_t	YRightTop = 0;

	int32_t	XLeftBottom = 0;
	int32_t	YLeftBottom = height;

	int32_t	XRightBottom = width;
	int32_t	YRightBottom = height;

	int32_t	XCorners[4] = { XLeftTop, XRightTop, XLeftBottom, XRightBottom };
	int32_t	YCorners[4] = { YLeftTop, YRightTop, YLeftBottom, YRightBottom };

	int32_t	maxY = 0, 
				maxX = 0, 
				minY = 0, 
				minX = 0;

	int32_t	newXCorners[4]{0};
	int32_t	newYCorners[4]{0};

	//Поиск новых координат углов
	//X
	for (int i = 0; i < 4; ++i) {
		newXCorners[i] = XCorners[i] * cosf - YCorners[i] * sinf;
		newYCorners[i] = XCorners[i] * sinf + YCorners[i] * cosf;
	}

	//find min max X
	for (int i = 0; i < 4; ++i) {
		if (newXCorners[i] < minX)
		{
			minX = newXCorners[i];
		}
		else if (newXCorners[i] > maxX)
		{
			maxX = newXCorners[i];
		}
	}

	//find min max Y
	for (int i = 0; i < 4; ++i) {
		if (newYCorners[i] < minY)
		{
			minY = newYCorners[i];
		}
		else if (newYCorners[i] > maxY)
		{
			maxY = newYCorners[i];
		}
	}

	int32_t newWidth = std::abs(minX - maxX);
	int32_t newheight = std::abs(minY - maxY);

	uint32_t newBiSizeImage = alignImageSizeInBytes(newWidth, newheight);

	BITMAPINFO newBiInfo;
	newBiInfo.bmiHeader = {
		40,		//biSize,
		newWidth,	//biWidth,
		newheight,	//biHeight,
		1,		//biPlanes,
		24,		//biBitCount,
		0,		//biCompression,
		newBiSizeImage,	//biSizeImage,
		0,		//biXPelsPerMeter,
		0,		//biYPelsPerMeter,
		0,		//biClrUsed,
		0		//biClrImportant,
	};
	uint8_t* newBitmap = new uint8_t[newBiSizeImage]{255};

	int srcX = 0;
	int srcY = 0;
	for (int i = 0; i < newheight; ++i)
	{
		for (int j = 0; j < newWidth; ++j)
		{
			uint32_t column = j;
			uint32_t row = newheight - i;
			uint32_t k = (newWidth * i + j * 3);

			srcX = i * cosf - j * sinf;
			srcY = i * sinf + j * cosf;

			if (srcX < width && srcY < height)
			{
				// SetPixelFromNewBitmap[col,row]  = GetPixelFromOldBitmap[srcX,srcY];
			}

			dc.SetPixel(column, row, RGB(bitmap[k + 2], bitmap[k + 1], bitmap[k]));
		}
	}


	m_bitmapInfo = newBiInfo;

	dc.CreateCompatibleDC(this->GetDC());
	m_DIBSectionBitmap = new uint8_t[newBiSizeImage];
	m_HBitmap = CreateDIBSection(dc, &newBiInfo, DIB_RGB_COLORS,
		(void**)&m_DIBSectionBitmap, NULL, 0);
	memcpy(m_DIBSectionBitmap, newBitmap, newBiSizeImage);





	//for (int i = 0; i < sizeInBytes; i++)
	//{
	//	bitmap[i] = bitmap[i] + angle;
	//}
	
	

	return 0;
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

void myTIFFErrorHandler(const char* module, const char* fmt, va_list ap)
{
	CHAR s[10000];
	vsprintf_s(s, 9999, fmt, ap);
	//AddLog(s);
}

void CChildView::OnAppOpen() {

	CFileDialog opnFileDlg(TRUE,NULL,NULL,NULL, _T("BMP files (*.bmp)|*.bmp|TIFF files (*.tif)|*.tif|"),NULL, 0, 1);

	if (opnFileDlg.DoModal() == IDOK)
	{

		isImageDrawed = false;

		CDC dc;
		dc.CreateCompatibleDC(this->GetDC());

		CString ext = opnFileDlg.GetFileExt(); // Получение расширения файла
		if ( ext == "bmp") //Если файл имеет расширение bmp
		{
			if (isFileOpen)
			{
				
			}
			isFileOpen = true;
			m_pictureFile->Open(opnFileDlg.GetPathName(), CFile::modeRead | CFile::shareDenyNone);
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
			m_pictureFile->Close();
		}
		else if (ext == "tif" || ext == "tiff")
		{
			isFileOpen = true;
			CString filePath(opnFileDlg.GetPathName());
			TIFF* tiff = TIFFOpenW(filePath, "r");

			TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &bmInfo.biWidth);
			TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &bmInfo.biHeight);
			bmInfo.biSizeImage = alignImageSizeInBytes(bmInfo.biWidth, bmInfo.biHeight);
			
			bmInfo.biSize =			40;
			bmInfo.biClrUsed =		0 ;
			bmInfo.biBitCount =		24;
			bmInfo.biClrImportant = 0 ;
			bmInfo.biCompression =	0 ;
			bmInfo.biPlanes =		1 ;
			bmInfo.biXPelsPerMeter= 0 ;
			bmInfo.biYPelsPerMeter= 0 ;

			m_bitmapInfo.bmiHeader = bmInfo;

			uint8_t* inputBitmap = new uint8_t[bmInfo.biSizeImage];

			inputBitmap = tiffToNormalBitmap(tiff);

			m_DIBSectionBitmap = new uint8_t[m_bitmapInfo.bmiHeader.biSizeImage];
			m_HBitmap = CreateDIBSection(dc, &m_bitmapInfo, DIB_RGB_COLORS,
				(void**)&m_DIBSectionBitmap, NULL, 0);
			memcpy(m_DIBSectionBitmap, inputBitmap, m_bitmapInfo.bmiHeader.biSizeImage);
			m_bitmap = inputBitmap;

			TIFFClose(tiff);
		}
		Invalidate();
	}
	else {
		AfxMessageBox(_T("Файл не выбран!"));
	}
	
}

void CChildView::OnAppRotate() {
	if (isFileOpen)
	{
		switch (rotateDialog.DoModal())
		{
		case IDOK: {
			
			rotateBitmap(m_bitmapInfo,m_DIBSectionBitmap,rotateDialog.GetRotateValue());
			Invalidate();
			break;
		}
		case IDCANCEL: {
			std::cout << "Cancel" << std::endl;
			break;
		}
		default:
			break;
		}

		std::cout << rotateDialog.GetRotateValue() << std::endl;
	}
	else
	{
		AfxMessageBox(L"Изображение не открыто!");
	}
	
}
