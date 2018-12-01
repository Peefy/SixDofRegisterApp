// DialogRegister.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ECATSample.h"
#include "DialogRegister.h"
#include "afxdialogex.h"

#include "register\register.h"

void DialogRegister::AppInit()
{
	
}


bool DialogRegister::IsRegister = false;
// DialogRegister �Ի���

IMPLEMENT_DYNAMIC(DialogRegister, CDialogEx)

DialogRegister::DialogRegister(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogRegister::IDD, pParent)
{
	
}

DialogRegister::~DialogRegister()
{
}

BOOL DialogRegister::OnInitDialog()
{
	CDialog::OnInitDialog();
	AppInit();
	return TRUE;
}
void DialogRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(DialogRegister, CDialogEx)
	ON_BN_CLICKED(IDOK, &DialogRegister::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DialogRegister::OnBnClickedCancel)
END_MESSAGE_MAP()


// DialogRegister ��Ϣ�������


void DialogRegister::OnBnClickedOk()
{
	CString str = "";
	GetDlgItemText(IDC_EDIT_MC, str);
	std::string STDStr(CW2A(str.GetString()));
	std::string md5 = GetMD5String(GetMD5String(STDStr));
	SetDlgItemText(IDC_EDIT_RC, CA2T(md5.c_str()));
	
}

void DialogRegister::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
