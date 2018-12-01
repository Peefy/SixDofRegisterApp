
#include "StdAfx.h"
#include "ECATSample.h"
#include "ECATSampleDlg.h"

#include <math.h>
#include <fstream>

#include "DialogRegister.h"

#include "ui/uiconfig.h"

#include "util/model.h"

#include "register\register.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

//#define PID_CONTROL_ENABLE 1
#define IS_HAS_SCENE 1

#define TIMER_MS 10
#define SCENE_THREAD_DELAY 8
#define SENSOR_THREAD_DELAY 100
#define SIXDOF_CONTROL_DELEY 91

void SixdofControl();
void SensorRead();

bool closeDataThread = true;
bool stopAndMiddle = true;

volatile HANDLE DataThread;
volatile HANDLE SensorThread;
volatile HANDLE SceneThread;

double sin_time_pulse = 0;
double sin_time_pulse_delta = 0.01;

bool isStart = false;
bool isAutoInit = true;
bool isTest = true;

double controltime = 0;

double runTime = 0;
double chartTime = 0;

DWORD WINAPI DataTransThread(LPVOID pParam)
{
	while (true)
	{
		SixdofControl();
	}
	return 0;
}

DWORD WINAPI SensorInfoThread(LPVOID pParam)
{
	while (true)
	{
		SensorRead();
	}
	return 0;
}

DWORD WINAPI SceneInfoThread(LPVOID pParam)
{
	while (true)
	{

	}
	return 0;
}

void CloseThread()
{
	try
	{
		if (DataThread != INVALID_HANDLE_VALUE)
		{
			CloseHandle(DataThread);
			DataThread = INVALID_HANDLE_VALUE;
		}
	}
	catch (CException* e)
	{
		
	}
	try
	{
		if (SensorThread != INVALID_HANDLE_VALUE)
		{
			CloseHandle(SensorThread);
			SensorThread = INVALID_HANDLE_VALUE;
		}
	}
	catch (CException* e)
	{

	}
	try
	{
		if (SceneThread != INVALID_HANDLE_VALUE)
		{
			CloseHandle(SceneThread);
			SceneThread = INVALID_HANDLE_VALUE;
		}
	}
	catch (CException* e)
	{

	}
}

void OpenThread()
{
	DataThread = (HANDLE)CreateThread(NULL, 0, DataTransThread, NULL, 0, NULL);
	SensorThread = (HANDLE)CreateThread(NULL, 0, SensorInfoThread, NULL, 0, NULL);
	SceneThread = (HANDLE)CreateThread(NULL, 0, SceneInfoThread, NULL, 0, NULL);
}

void SensorRead()
{

}

bool isCsp = false;
double t = 0;

void SixdofControl()
{

}

void MoveValPoint()
{
	

}

CECATSampleDlg::CECATSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CECATSampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CECATSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CECATSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CECATSampleDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_InitialCard, OnBTNInitialCard)
	ON_BN_CLICKED(IDC_BTN_FindSlave, OnBTNFindSlave)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHK_SVON, OnChkSvon)
	ON_BN_CLICKED(IDC_CHK_ABS, OnChkAbs)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_Rise, &CECATSampleDlg::OnBnClickedBtnRise)
	ON_BN_CLICKED(IDC_BTN_Middle, &CECATSampleDlg::OnBnClickedBtnMiddle)
	ON_BN_CLICKED(IDC_BTN_Start, &CECATSampleDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_StopMe, &CECATSampleDlg::OnBnClickedBtnStopme)
	ON_BN_CLICKED(IDC_BTN_Down, &CECATSampleDlg::OnBnClickedBtnDown)
	ON_BN_CLICKED(IDOK, &CECATSampleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CECATSampleDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_Resetme, &CECATSampleDlg::OnBnClickedBtnResetme)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, &CECATSampleDlg::OnBnClickedBtnDisconnect)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_SINGLE_UP, &CECATSampleDlg::OnBnClickedBtnSingleUp)
	ON_BN_CLICKED(IDC_BTN_SINGLE_DOWN, &CECATSampleDlg::OnBnClickedBtnSingleDown)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CECATSampleDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CECATSampleDlg::OnBnClickedButtonExit)
END_MESSAGE_MAP()

void CECATSampleDlg::ChartInit()
{
	

}

void CECATSampleDlg::AppInit()
{
	int statusTemp = 0;
	auto isRegister = TestAppIsRegister();
	if (isRegister == false)
	{
		DialogRegister * dlg = new DialogRegister(this);
		dlg->DoModal();
		dlg->Create(IDD_DIALOG_REGISTER, this);
		dlg->ShowWindow(SW_SHOW);
		delete dlg;
		if (DialogRegister::IsRegister == false)
		{
			OnBnClickedOk();
		}
	}

	SetDlgItemText(IDC_EDIT_X_VAL, _T("0"));
	SetDlgItemText(IDC_EDIT_Y_VAL, _T("0"));
	SetDlgItemText(IDC_EDIT_Z_VAL, _T("0"));
	SetDlgItemText(IDC_EDIT_ROLL_VAL, _T("0"));
	SetDlgItemText(IDC_EDIT_PITCH_VAL, _T("0"));
	SetDlgItemText(IDC_EDIT_YAW_VAL, _T("0"));

	SetDlgItemText(IDC_EDIT_X_HZ, _T("0"));
	SetDlgItemText(IDC_EDIT_Y_HZ, _T("0"));
	SetDlgItemText(IDC_EDIT_Z_HZ, _T("0"));
	SetDlgItemText(IDC_EDIT_ROLL_HZ, _T("0"));
	SetDlgItemText(IDC_EDIT_PITCH_HZ, _T("0"));
	SetDlgItemText(IDC_EDIT_YAW_HZ, _T("0"));

	CDialog::SetWindowTextW(_T(WINDOW_TITLE));
	GetDlgItem(IDC_BTN_Start)->SetWindowTextW(_T(IDC_BTN_START_SHOW_TEXT));
	GetDlgItem(IDC_BTN_SINGLE_UP)->SetWindowTextW(_T(IDC_BTN_SINGLE_UP_SHOW_TEXT));
	GetDlgItem(IDC_BTN_SINGLE_DOWN)->SetWindowTextW(_T(IDC_BTN_SINGLE_DOWN_SHOW_TEXT));

	GetDlgItem(IDC_STATIC_POSE)->SetWindowTextW(_T(IDC_STATIC_POSE_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_LENGTH)->SetWindowTextW(_T(IDC_STATIC_LENGTH_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_SENSOR)->SetWindowTextW(_T(IDC_STATIC_SENSOR_SHOW_TEXT));

	GetDlgItem(IDC_STATIC_X_VAL)->SetWindowTextW(_T(IDC_STATIC_X_VAL_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_Y_VAL)->SetWindowTextW(_T(IDC_STATIC_Y_VAL_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_Z_VAL)->SetWindowTextW(_T(IDC_STATIC_Z_VAL_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_ROLL_VAL)->SetWindowTextW(_T(IDC_STATIC_ROLL_VAL_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_PITCH_VAL)->SetWindowTextW(_T(IDC_STATIC_PITCH_VAL_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_YAW_VAL)->SetWindowTextW(_T(IDC_STATIC_YAW_VAL_SHOW_TEXT));

	GetDlgItem(IDC_STATIC_X_HZ)->SetWindowTextW(_T(IDC_STATIC_X_HZ_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_Y_HZ)->SetWindowTextW(_T(IDC_STATIC_Y_HZ_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_Z_HZ)->SetWindowTextW(_T(IDC_STATIC_Z_HZ_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_ROLL_HZ)->SetWindowTextW(_T(IDC_STATIC_ROLL_HZ_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_PITCH_HZ)->SetWindowTextW(_T(IDC_STATIC_PITCH_HZ_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_YAW_HZ)->SetWindowTextW(_T(IDC_STATIC_YAW_HZ_SHOW_TEXT));

	GetDlgItem(IDC_STATIC_TEST)->SetWindowTextW(_T(IDC_STATIC_TEST_SHOW_TEXT));
	GetDlgItem(IDC_BUTTON_TEST)->SetWindowTextW(_T(IDC_BUTTON_TEST_SHOW_TEXT));

	GetDlgItem(IDC_STATIC_APP_STATUS)->SetWindowTextW(_T(CORPORATION_NAME));

	ChartInit();

	((CComboBox*)GetDlgItem(IDC_CBO_SingleNo))->SetCurSel(0);
	OpenThread();

}

double CECATSampleDlg::GetCEditNumber(int cEditId)
{
	CString str;
	GetDlgItemText(cEditId, str);
	auto val = _tstof(str);
	return val;
}

void CECATSampleDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

BOOL CECATSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);	
	
	AppInit();
	SetTimer(0, TIMER_MS, NULL);
	return TRUE;  
}

int isShowSingleUpDown = 0;
BOOL CECATSampleDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_SHIFT)
		{
			isShowSingleUpDown = !isShowSingleUpDown;
			((CButton*)GetDlgItem(IDC_BTN_SINGLE_UP))->ShowWindow(isShowSingleUpDown);
			((CButton*)GetDlgItem(IDC_BTN_SINGLE_DOWN))->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_BTN_FindSlave)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_EDIT_SlaveNum)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_CBO_SingleNo)->ShowWindow(isShowSingleUpDown);

			GetDlgItem(IDC_CHK_SVON)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_BTN_Resetme)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_BTN_CONNECT)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_BTN_DISCONNECT)->ShowWindow(isShowSingleUpDown);

			GetDlgItem(IDC_BTN_InitialCard)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_STATIC)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_CBO_CardNo)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_STATIC_SHOW_INIT)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_EDIT_InitialStatus)->ShowWindow(isShowSingleUpDown);
		}
	}
	else if (pMsg->message == WM_KEYUP)
	{

	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CECATSampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CECATSampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CECATSampleDlg::OnBTNInitialCard() 
{
	
}

void CECATSampleDlg::OnBTNFindSlave() 
{

}

void CECATSampleDlg::EanbleButton(int isenable)
{	
	((CButton*) GetDlgItem(IDC_CHK_SVON))->EnableWindow(isenable);
	((CButton*) GetDlgItem(IDC_BTN_Rise))->EnableWindow(isenable);
	((CButton*) GetDlgItem(IDC_BTN_Middle))->EnableWindow(isenable);
	((CButton*) GetDlgItem(IDC_BTN_Start))->EnableWindow(isenable);
	((CButton*) GetDlgItem(IDC_BTN_StopMe))->EnableWindow(isenable);
	((CButton*) GetDlgItem(IDC_BTN_Down))->EnableWindow(isenable);
	((CButton*) GetDlgItem(IDC_BTN_Resetme))->EnableWindow(isenable);
	((CButton*) GetDlgItem(IDC_BUTTON_TEST))->EnableWindow(isenable);
}

void CECATSampleDlg::OnTimer(UINT nIDEvent) 
{

	CString statusStr;

	if (isAutoInit == true)
	{
		isAutoInit = false;
		
	}

	CDialog::OnTimer(nIDEvent);
}

void CECATSampleDlg::OnChkSvon() 
{
	
}

void CECATSampleDlg::OnOK() 
{

	CDialog::OnOK();
}

void CECATSampleDlg::OnChkAbs() 
{
	CButton *lpBtnPtr = (CButton *)GetDlgItem(IDC_CHK_ABS);
}

void CECATSampleDlg::OnBnClickedBtnRise()
{	
	
}

void CECATSampleDlg::OnBnClickedBtnMiddle()
{

}

void CECATSampleDlg::OnBnClickedBtnStart()
{

}

void CECATSampleDlg::OnBnClickedBtnStopme()
{
	
}

void CECATSampleDlg::OnBnClickedBtnDown()
{
	
}

void CECATSampleDlg::OnBnClickedOk()
{

	CDialog::OnOK();
}

void CECATSampleDlg::OnBnClickedBtnConnect()
{

}

void CECATSampleDlg::OnBnClickedBtnResetme()
{
	
}

void CECATSampleDlg::OnBnClickedBtnDisconnect()
{

}

void CECATSampleDlg::OnBnClickedBtnSingleUp()
{
	
}

void CECATSampleDlg::OnBnClickedBtnSingleDown()
{
	
}

void CECATSampleDlg::OnBnClickedButtonTest()
{
	
}

void CECATSampleDlg::OnBnClickedButtonExit()
{
	OnBnClickedOk();
}

