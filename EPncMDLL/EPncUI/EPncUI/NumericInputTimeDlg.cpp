#include "stdafx.h"
#include "EPncUI.h"
#include "NumericInputTimeDlg.h"


// CNumericInputTimeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNumericInputTimeDlg, CDialog)

CNumericInputTimeDlg::CNumericInputTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNumericInputTimeDlg::IDD, pParent)
{
	strNum_.Format( _T("") );
	strPrevNum_.Format( _T("") );

}

CNumericInputTimeDlg::~CNumericInputTimeDlg()
{
}

void CNumericInputTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
/*	DDX_Control(pDX, IDC_BUTTON_M_UP, btnMinuteUp_);
	DDX_Control(pDX, IDC_BUTTON_M_DN, btnMinuteDown_);
	DDX_Control(pDX, IDC_BUTTON_S_UP, btnSecondUp_);
	DDX_Control(pDX, IDC_BUTTON_S_DN, btnSecondDown_);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, btnCancel_);
	DDX_Control(pDX, IDC_BUTTON_ENTER, btnEnter_);*/
}

void CNumericInputTimeDlg::updateTime()
{
	CTimeSpan tms( currTime );
	
	CString minutes;
	CString seconds;

	minutes.Format( _T("%d"), tms.GetTotalMinutes() );
	seconds.Format( _T("%d"), tms.GetSeconds() );

	((CStatic*)GetDlgItem(IDC_STATIC_M))->SetWindowText( minutes );
	((CStatic*)GetDlgItem(IDC_STATIC_S))->SetWindowText( seconds );
}

void CNumericInputTimeDlg::SetTimePerMilling( int newTime )
{
	currTime = newTime;
	updateTime();
}

int CNumericInputTimeDlg::GetTimePerMilling()
{
	return currTime;
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CNumericInputTimeDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CNumericInputTimeDlg::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_ENTER, &CNumericInputTimeDlg::OnBnClickedButtonEnter)
	ON_BN_CLICKED(IDC_BUTTON_M_UP, &CNumericInputTimeDlg::OnBnClickedButtonIncMin)
	ON_BN_CLICKED(IDC_BUTTON_M_DN, &CNumericInputTimeDlg::OnBnClickedButtonDecMin)
	ON_BN_CLICKED(IDC_BUTTON_S_UP, &CNumericInputTimeDlg::OnBnClickedButtonIncSec)
	ON_BN_CLICKED(IDC_BUTTON_S_DN, &CNumericInputTimeDlg::OnBnClickedButtonDecSec)
	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CNumericInputTimeDlg 메시지 처리기입니다.

BOOL CNumericInputTimeDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CNumericInputTimeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	currTime = 0;
	//bEnterFollowingEntrySameData_ = FALSE;

	// Font 
	fntOldNum_.CreateFont( 
		26, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );

	fntNum_.CreateFont( 
		32, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );

	fntBtn_.CreateFont( 
		24, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );
		
		
	fntUpDownBtn_.CreateFont( 
		16, 8, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );

	fntTime_.CreateFont( 
		24, 0, 
		0, 0, FW_NORMAL, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Noto Sans") ); //_T("MS Sans Serif") );

	((CButton*)GetDlgItem(IDC_BUTTON_M_UP))->SetFont( &fntUpDownBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_M_DN))->SetFont( &fntUpDownBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_S_UP))->SetFont( &fntUpDownBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_S_DN))->SetFont( &fntUpDownBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_CANCEL))->SetFont( &fntUpDownBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_ENTER))->SetFont( &fntUpDownBtn_, TRUE );
	
	((CStatic*)GetDlgItem(IDC_STATIC_M))->SetFont( &fntTime_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_S))->SetFont( &fntTime_, TRUE );

	//////////////////////////////////////////////////////////////////////////
	// Center Window 
	CRect rcWnd;
	GetWindowRect( &rcWnd );
	int xpos = 1024/2 - rcWnd.Width()/2;
	int ypos = 768/2 - rcWnd.Height()/2;
	SetWindowPos( &wndTopMost, xpos, ypos, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW );

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CNumericInputTimeDlg::OnDestroy()
{
	fntOldNum_.DeleteObject();
	fntNum_.DeleteObject();
	fntBtn_.DeleteObject();
	fntUpDownBtn_.DeleteObject();
	fntTime_.DeleteObject();
	CDialog::OnDestroy();
}

void CNumericInputTimeDlg::OnBnClickedButtonCancel()
{
	//bEnterFollowingEntrySameData_ = FALSE;

	CDialog::OnCancel();
}

void CNumericInputTimeDlg::OnBnClickedButtonEnter()
{
	//bEnterFollowingEntrySameData_ = ((CButton*)GetDlgItem(IDC_CHECK_SET_SAME_DATA))->GetCheck();

	CDialog::OnOK();
}

void CNumericInputTimeDlg::OnBnClickedButtonIncMin()
{
	int	dMinute = 60;
	currTime += dMinute;
	updateTime();
}

void CNumericInputTimeDlg::OnBnClickedButtonDecMin()
{
	int	dMinute = 60;
	if( currTime >= dMinute ) {
		currTime -= dMinute;
	}
	updateTime();
}

void CNumericInputTimeDlg::OnBnClickedButtonIncSec()
{
	int	dSecond = 1;
	currTime += dSecond;
	updateTime();
}

void CNumericInputTimeDlg::OnBnClickedButtonDecSec()
{
	int	dSecond = 1;
	if( currTime >= dSecond ) {
		currTime -= dSecond;
	}
	updateTime();
}