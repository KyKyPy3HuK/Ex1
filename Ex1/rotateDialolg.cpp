// rotateDialolg.cpp: файл реализации
//

#include "pch.h"
#include "Ex1.h"
#include "afxdialogex.h"
#include "rotateDialolg.h"
#include "ChildView.h"
#include <iostream>
// Диалоговое окно rotateDialolg

IMPLEMENT_DYNAMIC(RotateDialog, CDialogEx)

RotateDialog::RotateDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ROTATE, pParent)
	, rotateValue(0)
{

}

RotateDialog::~RotateDialog()
{
}

void RotateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, rotateValue);
	DDV_MinMaxInt(pDX, rotateValue, 0, 359);
}


BEGIN_MESSAGE_MAP(RotateDialog, CDialogEx)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT1, &RotateDialog::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &RotateDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// Обработчики сообщений rotateDialolg

void RotateDialog::OnEnChangeEdit1()
{

}

void RotateDialog::OnPaint()
{
	CPaintDC dc(this);
}

//void rotateDialog::OnBnClickedDlgIdok()
//{
//	// TODO: добавьте свой код обработчика уведомлений
//	rotateValue += 1;
//}


void RotateDialog::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	std::cout << "ыыы" << std::endl;
	CDialogEx::OnOK();
}
