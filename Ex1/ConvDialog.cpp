// ConvDialg.cpp: файл реализации
//

#include "pch.h"
#include "Ex1.h"
#include "afxdialogex.h"
#include "ConvDialog.h"


// Диалоговое окно ConvDialg

IMPLEMENT_DYNAMIC(ConvDialog, CDialogEx)

ConvDialog::ConvDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONV_DLG, pParent)
{

}

ConvDialog::~ConvDialog()
{
}

void ConvDialog::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDC_EDIT1, convVals[0]);
	DDX_Text(pDX, IDC_EDIT2, convVals[1]);
	DDX_Text(pDX, IDC_EDIT3, convVals[2]);
	DDX_Text(pDX, IDC_EDIT4, convVals[3]);
	DDX_Text(pDX, IDC_EDIT5, convVals[4]);
	DDX_Text(pDX, IDC_EDIT6, convVals[5]);
	DDX_Text(pDX, IDC_EDIT7, convVals[6]);
	DDX_Text(pDX, IDC_EDIT8, convVals[7]);
	DDX_Text(pDX, IDC_EDIT9, convVals[8]);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ConvDialog, CDialogEx)
END_MESSAGE_MAP()


// Обработчики сообщений ConvDialg
