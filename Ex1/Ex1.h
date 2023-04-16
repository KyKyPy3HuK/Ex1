
// Ex1.h: основной файл заголовка для приложения Ex1
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CEx1App:
// Сведения о реализации этого класса: Ex1.cpp
//

class CEx1App : public CWinApp
{
public:
	CEx1App() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEx1App theApp;
