
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

struct bitSet {
	uint8_t bit0 : 1;
	uint8_t bit1 : 1;
	uint8_t bit2 : 1;
	uint8_t bit3 : 1;
	uint8_t bit4 : 1;
	uint8_t bit5 : 1;
	uint8_t bit6 : 1;
	uint8_t bit7 : 1;
	bitSet(uint8_t byte) {
		this->bit0 = (byte & 0b00000001) >> 0;
		this->bit1 = (byte & 0b00000010) >> 1;
		this->bit2 = (byte & 0b00000100) >> 2;
		this->bit3 = (byte & 0b00001000) >> 3;
		this->bit4 = (byte & 0b00010000) >> 4;
		this->bit5 = (byte & 0b00100000) >> 5;
		this->bit6 = (byte & 0b01000000) >> 6;
		this->bit7 = (byte & 0b10000000) >> 7;
	}

	uint8_t getBit(uint8_t index) {
		switch (index)
		{
		case 0: {
			return this->bit0;
			break;
		}
		case 1: {
			return this->bit1;
			break;
		}
		case 2: {
			return this->bit2;
			break;
		}
		case 3: {
			return this->bit3;
			break;
		}
		case 4: {
			return this->bit4;
			break;
		}
		case 5: {
			return this->bit5;
			break;
		}
		case 6: {
			return this->bit6;
			break;
		}
		case 7: {
			return this->bit7;
			break;
		}

		default:
			return -1;
			break;
		}
	}
};

extern CEx1App theApp;
