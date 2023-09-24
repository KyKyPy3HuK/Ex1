#pragma once
#include "afxdialogex.h"


// Диалоговое окно rotateDialolg

class RotateDialog : public CDialogEx
{
	DECLARE_DYNAMIC(RotateDialog)

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
	int rotateValue;


	afx_msg void OnEnChangeEdit1();
	afx_msg void OnPaint();
	
	afx_msg void OnBnClickedOk();
};
