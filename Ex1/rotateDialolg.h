#pragma once
#include "afxdialogex.h"


// Диалоговое окно rotateDialolg

class RotateDialog : public CDialogEx
{
	DECLARE_DYNAMIC(RotateDialog)
	int rotateValue;
	int isBilinearInterpolation;
public:
	RotateDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~RotateDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ROTATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	inline int GetRotateValue() { return rotateValue; };
	inline int GetIsBillinearInterpolation() { return isBilinearInterpolation; };
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBilincheck();
};
