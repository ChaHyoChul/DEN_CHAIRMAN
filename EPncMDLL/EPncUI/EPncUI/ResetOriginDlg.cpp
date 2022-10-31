// ResetOriginDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "ResetOriginDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"

// CResetOriginDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CResetOriginDlg, CDialog)

CResetOriginDlg::CResetOriginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResetOriginDlg::IDD, pParent)
{

}

CResetOriginDlg::~CResetOriginDlg()
{
}

void CResetOriginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SEL_AXIS, cboSelectAxis_);
	DDX_Control(pDX, IDC_BUTTON_ORIGIN, btnOrigin_);
	DDX_Control(pDX, IDC_BUTTON2, btnClose_);
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CResetOriginDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, &CResetOriginDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN, &CResetOriginDlg::OnBnClickedButtonOrigin)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CResetOriginDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CResetOriginDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CResetOriginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// font 초기화 
	fntLabel_.CreateFont( 
		18, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	fntCombo_.CreateFont( 
		20, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );
	
	fntButton_.CreateFont( 
		20, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	// ctrl 초기화 
	((CStatic*)GetDlgItem(IDC_STATIC_AREA))->SetFont( &fntLabel_, TRUE );
	cboSelectAxis_.SetFont( &fntCombo_, TRUE );
	btnOrigin_.SetFont( &fntButton_, TRUE );
	btnClose_.SetFont( &fntButton_, TRUE );

	cboSelectAxis_.SetCurSel(0);	
	cboSelectAxis_.EnableWindow( TRUE );
	btnClose_.EnableWindow( TRUE );
	btnOrigin_.SetWindowText( _T("Origin") );

	//////////////////////////////////////////////////////////////////////////
	// Center Window 
	CRect rcWnd;
	GetWindowRect( &rcWnd );
	int xpos = 1024/2 - rcWnd.Width()/2;
	int ypos = 768/2 - rcWnd.Height()/2;
	SetWindowPos( &wndTopMost, xpos, ypos, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW );
	//////////////////////////////////////////////////////////////////////////

	// 상태 업데이트 타이머 시작 
	SetTimer( 1, 1000, NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CResetOriginDlg::OnDestroy()
{
	KillTimer( 1 );

	fntLabel_.DeleteObject();
	fntCombo_.DeleteObject();
	fntButton_.DeleteObject();

	CDialog::OnDestroy();
}

void CResetOriginDlg::OnBnClickedButton2()
{
	CDialog::OnOK();
}

/** 
 * 현재 선택된 축의 원점 복귀를 실행한다 
 */
void CResetOriginDlg::OnBnClickedButtonOrigin()
{
	if( pa::PPAStatus->GetThreadState()->bCompleteResetOrigin_ == FALSE ) 
	{
		// 멈춤  
		PPNC_IPC_CLIENT->Stop();
	}
	else 
	{
		int sel = cboSelectAxis_.GetCurSel();

		//////////////////////////////////////////////////////////////////////////
		// softlimit 범위를 확인해서 -500 ~ 500 사이이면, softlimit이 걸린것으로 간주 한다 
		double	fSoftLimitP = pa::PPAStatus->GetThreadState()->fSoftLimit_[sel][0];
		double	fSoftLimitM = pa::PPAStatus->GetThreadState()->fSoftLimit_[sel][1];
		BOOL	is_set_softlimit = FALSE;
		CString strMsg;

		if( sel != 2 ) {
			// x, y, a, b축일 경우... 
			if( fSoftLimitM > -500.0 ) {
				is_set_softlimit = TRUE;
				strMsg.Format( _T("softlimit(-) data is set.\r\nplease disable softlimit and try again.") );
			}
		}
		else {
			// z축일 경우...
			if( fSoftLimitP < 500.0 ) {
				is_set_softlimit = TRUE;
				strMsg.Format( _T("softlimit(+) data is set.\r\nplease disable softlimit and try again.") );
			}
		}
		if( is_set_softlimit == TRUE ) {
			// SoftLimit이 설정되어 있는 것으로 간주하고, 알림 메시지를 띄운다 
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_WARM, strMsg );
			CMsgDlgThread::GetInstance()->Wait();
			return ;
		}

		//////////////////////////////////////////////////////////////////////////
		// PPAStatus->GetThreadState()->bCompleteResetOrigin_ = FALSE;	
		// 시작 
		PPNC_IPC_CLIENT->ResetOrigin(sel);
		Sleep(500);
	}
}

void CResetOriginDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		KillTimer( 1 );

		update_state();

		if( IsWindowVisible() == TRUE )
		{
			SetTimer( 1, 500, NULL );
		}
	}

	CDialog::OnTimer(nIDEvent);
}

// 현재 상태에 따라, 컨트롤의 상태를 변경 한다 
void CResetOriginDlg::update_state()
{
	static int PREV_MODE = -1;
	int curr_mode = (pa::PPAStatus->GetThreadState()->bCompleteResetOrigin_ == FALSE) ? 0 : 1;

	if( PREV_MODE != curr_mode )
	{
		PREV_MODE = curr_mode;

		if( curr_mode == 0 )
		{
			// 동작 중 
			cboSelectAxis_.EnableWindow( FALSE );
			btnClose_.EnableWindow( FALSE );
			btnOrigin_.SetWindowText( _T("Stop") );
		}
		else 
		{
			// 멈춤 
			cboSelectAxis_.EnableWindow( TRUE );
			btnClose_.EnableWindow( TRUE );
			btnOrigin_.SetWindowText( _T("Origin") );
		}
	}
}

