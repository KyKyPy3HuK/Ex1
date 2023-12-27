#pragma once
#include "afxdialogex.h"
#include <iostream>

// Диалоговое окно ConvDialg

class ConvDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ConvDialog)

public:
	inline void printVals() {
		for (int i = 0; i < 9; i++)
		{
			std::cout << convVals[i] << std::endl;
		}
	}

	inline double* getConvVals() {
		return convVals;
	}
	ConvDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~ConvDialog();
	int convMatrixSize = 9;
	double convVals[9] = { 0., 0., 0.,
						   0., 0., 0.,
						   0., 0., 0.};
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONV_DLG };
#endif

protected:
	

	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
};
