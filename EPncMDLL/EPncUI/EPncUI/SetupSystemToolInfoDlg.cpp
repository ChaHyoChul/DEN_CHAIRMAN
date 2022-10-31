// SetupSystemToolInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupSystemToolInfoDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"

// CSetupSystemToolInfoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupSystemToolInfoDlg, CDialog)

CSetupSystemToolInfoDlg::CSetupSystemToolInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupSystemToolInfoDlg::IDD, pParent)
{

}

CSetupSystemToolInfoDlg::~CSetupSystemToolInfoDlg()
{
}

void CSetupSystemToolInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_LEFT_HOUR, btnLeftHour_);
	DDX_Control(pDX, IDC_BUTTON_LEFT_MIN, btnLeftMin_);
	DDX_Control(pDX, IDC_BUTTON_LEFT_SEC, btnLeftSec_);
	DDX_Control(pDX, IDC_BUTTON_RIGHT_HOUR, btnRightHour_);
	DDX_Control(pDX, IDC_BUTTON_RIGHT_MIN, btnRightMin_);
	DDX_Control(pDX, IDC_BUTTON_RIGHT_SEC, btnRightSec_);
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupSystemToolInfoDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetupSystemToolInfoDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_HOUR, &CSetupSystemToolInfoDlg::OnBnClickedButtonLeftHour)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_MIN, &CSetupSystemToolInfoDlg::OnBnClickedButtonLeftMin)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_SEC, &CSetupSystemToolInfoDlg::OnBnClickedButtonLeftSec)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_RESET, &CSetupSystemToolInfoDlg::OnBnClickedButtonLeftReset)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_SET, &CSetupSystemToolInfoDlg::OnBnClickedButtonLeftSet)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_HOUR, &CSetupSystemToolInfoDlg::OnBnClickedButtonRightHour)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_MIN, &CSetupSystemToolInfoDlg::OnBnClickedButtonRightMin)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_SEC, &CSetupSystemToolInfoDlg::OnBnClickedButtonRightSec)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_RESET, &CSetupSystemToolInfoDlg::OnBnClickedButtonRightReset)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_SET, &CSetupSystemToolInfoDlg::OnBnClickedButtonRightSet)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupSystemToolInfoDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupSystemToolInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSetupSystemToolInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////

	fntEdit_.CreateFont(
		20, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	btnLeftHour_.SetFont( &fntEdit_, FALSE );
	btnLeftMin_.SetFont( &fntEdit_, FALSE );
	btnLeftSec_.SetFont( &fntEdit_, FALSE );
	btnRightHour_.SetFont( &fntEdit_, FALSE );
	btnRightMin_.SetFont( &fntEdit_, FALSE );
	btnRightSec_.SetFont( &fntEdit_, FALSE );

	//////////////////////////////////////////////////////////////////////////

	fntButton_.CreateFont(
		17, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	((CStatic*)GetDlgItem(IDC_STATIC_LBL_LEFT_SPINDLE))->SetFont( &fntButton_, FALSE );
	((CStatic*)GetDlgItem(IDC_STATIC_LBL_LEFT_TOTAL_TIME))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_LEFT_SET))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_LEFT_RESET))->SetFont( &fntButton_, FALSE );

	((CStatic*)GetDlgItem(IDC_STATIC_LBL_RIGHT_SPINDLE))->SetFont( &fntButton_, FALSE );
	((CStatic*)GetDlgItem(IDC_STATIC_LBL_RIGHT_TOTAL_TIME))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_RIGHT_SET))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_RIGHT_RESET))->SetFont( &fntButton_, FALSE );

	((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->SetFont( &fntButton_, FALSE );

	//////////////////////////////////////////////////////////////////////////
	// Center Window 
	CenterWindow();
	//////////////////////////////////////////////////////////////////////////

	displayToolInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupSystemToolInfoDlg::OnDestroy()
{
	fntEdit_.DeleteObject();
	fntButton_.DeleteObject();

	CDialog::OnDestroy();
}

void CSetupSystemToolInfoDlg::OnBnClickedButtonClose()
{
	CDialog::OnOK();
}

void CSetupSystemToolInfoDlg::displayToolInfo()
{
	// 왼쪽 스핀들 사용 시간
	DWORD dwTemp = pa::PPAStatus->GetThreadState()->dwTOTAL_LEFT_SPINDLE_RUN_TIME;
	CTimeSpan	tms(dwTemp);
	CString		strHour, strMin, strSec;

	strHour.Format( _T("%d"), tms.GetTotalHours() );
	strMin.Format( _T("%d"), tms.GetMinutes() );
	strSec.Format( _T("%d"), tms.GetSeconds() );

	btnLeftHour_.SetWindowText( strHour );
	btnLeftMin_.SetWindowText( strMin );
	btnLeftSec_.SetWindowText( strSec );

	// 오른쪽 스핀들 사용 시간 표시
	DWORD dwTemp2 = pa::PPAStatus->GetThreadState()->dwTOTAL_RIGHT_SPINDLE_RUN_TIME;
	CTimeSpan	tms2(dwTemp2);
	CString		strHour2, strMin2, strSec2;

	strHour2.Format( _T("%d"), tms2.GetTotalHours() );
	strMin2.Format( _T("%d"), tms2.GetMinutes() );
	strSec2.Format( _T("%d"), tms2.GetSeconds() );

	btnRightHour_.SetWindowText( strHour2 );
	btnRightMin_.SetWindowText( strMin2 );
	btnRightSec_.SetWindowText( strSec2 );
}

#include "NumericInputDlg.h"

void CSetupSystemToolInfoDlg::OnBnClickedButtonLeftHour()
{
	CNumericInputDlg dlg;
	CString strTemp;
	btnLeftHour_.GetWindowText( strTemp );
	int prev_num = _ttoi(strTemp);

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( prev_num );
	dlg.SetProperty( 0 );

	if( dlg.DoModal() == IDOK ) {
		strTemp = dlg.GetNumber();
		if (strTemp.IsEmpty()) {
			strTemp.Format(_T("0"));
		}
		int nMin = _ttoi( strTemp );
		if( nMin >= 0 ) {
			btnLeftHour_.SetWindowText( strTemp );
		}
	}
}

void CSetupSystemToolInfoDlg::OnBnClickedButtonRightHour()
{
	CNumericInputDlg dlg;
	CString strTemp;
	btnRightHour_.GetWindowText( strTemp );
	int prev_num = _ttoi(strTemp);

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( prev_num );
	dlg.SetProperty( 0 );

	if( dlg.DoModal() == IDOK ) {
		strTemp = dlg.GetNumber();
		if (strTemp.IsEmpty()) {
			strTemp.Format(_T("0"));
		}
		int nMin = _ttoi( strTemp );
		if( nMin >= 0 ) {
			btnRightHour_.SetWindowText( strTemp );
		}
	}
}

void CSetupSystemToolInfoDlg::OnBnClickedButtonLeftMin()
{
	CNumericInputDlg dlg;
	CString strTemp;
	btnLeftMin_.GetWindowText( strTemp );
	int prev_num = _ttoi(strTemp);

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( prev_num );
	dlg.SetProperty( 0 );

	if( dlg.DoModal() == IDOK ) {
		strTemp = dlg.GetNumber();
		if (strTemp.IsEmpty()) {
			strTemp.Format(_T("0"));
		}
		int nMin = _ttoi( strTemp );
		if( nMin >= 0 && nMin <= 59 ) {
			btnLeftMin_.SetWindowText( strTemp );
		}
	}
}

void CSetupSystemToolInfoDlg::OnBnClickedButtonRightMin()
{
	CNumericInputDlg dlg;
	CString strTemp;
	btnRightMin_.GetWindowText( strTemp );
	int prev_num = _ttoi(strTemp);

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( prev_num );
	dlg.SetProperty( 0 );

	if( dlg.DoModal() == IDOK ) {
		strTemp = dlg.GetNumber();
		if (strTemp.IsEmpty()) {
			strTemp.Format(_T("0"));
		}
		int nMin = _ttoi( strTemp );
		if( nMin >= 0 && nMin <= 59 ) {
			btnRightMin_.SetWindowText( strTemp );
		}
	}
}

void CSetupSystemToolInfoDlg::OnBnClickedButtonLeftSec()
{
	CNumericInputDlg dlg;
	CString strTemp;
	btnLeftSec_.GetWindowText( strTemp );
	int prev_num = _ttoi(strTemp);

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( prev_num );
	dlg.SetProperty( 0 );

	if( dlg.DoModal() == IDOK ) {
		strTemp = dlg.GetNumber();
		if (strTemp.IsEmpty()) {
			strTemp.Format(_T("0"));
		}
		int nMin = _ttoi( strTemp );
		if( nMin >= 0 && nMin <= 59 ) {
			btnLeftSec_.SetWindowText( strTemp );
		}
	}
}

void CSetupSystemToolInfoDlg::OnBnClickedButtonRightSec()
{
	CNumericInputDlg dlg;
	CString strTemp;
	btnRightSec_.GetWindowText( strTemp );
	int prev_num = _ttoi(strTemp);

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( prev_num );
	dlg.SetProperty( 0 );

	if( dlg.DoModal() == IDOK ) {
		strTemp = dlg.GetNumber();
		if (strTemp.IsEmpty()) {
			strTemp.Format(_T("0"));
		}
		int nMin = _ttoi( strTemp );
		if( nMin >= 0 && nMin <= 59 ) {
			btnRightSec_.SetWindowText( strTemp );
		}
	}
}

// 시간을 초기화 한다 
void CSetupSystemToolInfoDlg::OnBnClickedButtonLeftReset()
{
	CString strMsg;

	strMsg.Format( _T("Do you want to reset total time ?") );

	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
	CMsgDlg::EN_RET hRet = CMsgDlgThread::GetInstance()->Wait();

	if( hRet == CMsgDlg::RET_OK ) 
	{
		pa::PPAStatus->GetThreadState()->dwTOTAL_LEFT_SPINDLE_RUN_TIME = 0;

		PPNC_IPC_CLIENT->SaveLeftToolTotalTime();
	}

	displayToolInfo();
}

void CSetupSystemToolInfoDlg::OnBnClickedButtonRightReset()
{
	CString strMsg;

	strMsg.Format( _T("Do you want to reset total time ?") );

	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
	CMsgDlg::EN_RET hRet = CMsgDlgThread::GetInstance()->Wait();

	if( hRet == CMsgDlg::RET_OK ) 
	{
		pa::PPAStatus->GetThreadState()->dwTOTAL_RIGHT_SPINDLE_RUN_TIME = 0;

		PPNC_IPC_CLIENT->SaveRightToolTotalTime();
	}

	displayToolInfo();
}

// 시간을 변경한다 
void CSetupSystemToolInfoDlg::OnBnClickedButtonLeftSet()
{
	CString strMsg;
	CString strHour, strMin, strSec;
	int		nHour, nMin, nSec;

	strMsg.Format( _T("Do you want to change total time ?") );

	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
	CMsgDlg::EN_RET hRet = CMsgDlgThread::GetInstance()->Wait();

	if( hRet == CMsgDlg::RET_OK ) 
	{
		btnLeftHour_.GetWindowText( strHour );
		btnLeftMin_.GetWindowText( strMin );
		btnLeftSec_.GetWindowText( strSec );

		nHour	= _ttoi( strHour );
		nMin	= _ttoi( strMin );
		nSec	= _ttoi( strSec );

		DWORD dwTotal = (nHour*60*60) + (nMin*60) + nSec;
		pa::PPAStatus->GetThreadState()->dwTOTAL_LEFT_SPINDLE_RUN_TIME = dwTotal;

		PPNC_IPC_CLIENT->SaveLeftToolTotalTime();
	}

	displayToolInfo();
}

void CSetupSystemToolInfoDlg::OnBnClickedButtonRightSet()
{
	CString strMsg;
	CString strHour, strMin, strSec;
	int		nHour, nMin, nSec;

	strMsg.Format( _T("Do you want to change total time ?") );

	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
	CMsgDlg::EN_RET hRet = CMsgDlgThread::GetInstance()->Wait();

	if( hRet == CMsgDlg::RET_OK ) 
	{
		btnRightHour_.GetWindowText( strHour );
		btnRightMin_.GetWindowText( strMin );
		btnRightSec_.GetWindowText( strSec );

		nHour	= _ttoi( strHour );
		nMin	= _ttoi( strMin );
		nSec	= _ttoi( strSec );

		DWORD dwTotal = (nHour*60*60) + (nMin*60) + nSec;
		pa::PPAStatus->GetThreadState()->dwTOTAL_RIGHT_SPINDLE_RUN_TIME = dwTotal;

		PPNC_IPC_CLIENT->SaveRightToolTotalTime();
	}

	displayToolInfo();
}