
// ChildView.h: интерфейс класса CChildView
//


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

// Реализация
public:
	COLORREF color = RGB(0, 255, 0);

	void fText(CPaintDC& dc, LPCTSTR text, int x, int y);
	void drawPicture(CPaintDC& dc, int* bitMap, int width, int heigth, int x, int y);

	virtual ~CChildView();

	// Созданные функции схемы сообщений
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

