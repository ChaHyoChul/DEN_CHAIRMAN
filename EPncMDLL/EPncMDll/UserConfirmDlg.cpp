// UserConfirmDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncMDLL.h"
#include "UserConfirmDlg.h"


//////////////////////////////////////////////////////////////////////////

HANDLE CUserConfirmDlg::hWAIT_EVENT					= NULL;
CUserConfirmDlg* CUserConfirmDlg::P_USER_CONFIRM_DLG= NULL;

void CUserConfirmDlg::INITIALIZE_DLG()
{
	CUserConfirmDlg::DELETE_DLG();

	CUserConfirmDlg::P_USER_CONFIRM_DLG = new CUserConfirmDlg();
	CUserConfirmDlg::P_USER_CONFIRM_DLG->Create( IDD_DIALOG_USER_CONFIRM );
	CUserConfirmDlg::P_USER_CONFIRM_DLG->ShowWindow( SW_HIDE );

	CUserConfirmDlg::hWAIT_EVENT = CreateEvent( NULL, TRUE, FALSE, NULL );
}

void CUserConfirmDlg::DELETE_DLG()
{
	if (CUserConfirmDlg::P_USER_CONFIRM_DLG != NULL) 
	{
		CUserConfirmDlg::P_USER_CONFIRM_DLG->DestroyWindow();
		delete CUserConfirmDlg::P_USER_CONFIRM_DLG;
		CUserConfirmDlg::P_USER_CONFIRM_DLG = NULL; 
	}
}

void CUserConfirmDlg::SHOW_DLG(BOOL bToolDirection)
{
	if (CUserConfirmDlg::P_USER_CONFIRM_DLG != NULL) 
	{
		ResetEvent( CUserConfirmDlg::hWAIT_EVENT );
		CUserConfirmDlg::P_USER_CONFIRM_DLG->display_toolerror_info(bToolDirection);
		CUserConfirmDlg::P_USER_CONFIRM_DLG->ShowWindow( SW_SHOW );
		pa::PPAStatus->GetThreadState()->bIsShowUserConfirmDlg = TRUE;		// Wait...() 함수로 진입하면, 상태를 갱신할 수 없다 
	}
	else 
	{
		SetEvent( CUserConfirmDlg::hWAIT_EVENT );
	}

	// 2016.10.07. Air Purge 사양 변경
	CUserConfirmDlg::P_USER_CONFIRM_DLG->SetTimer( 10, 1000, NULL );
}

void CUserConfirmDlg::HIDE_DLG()
{
	// 2016.10.07. Air Purge 사양 변경
	CUserConfirmDlg::P_USER_CONFIRM_DLG->KillTimer( 10 );

	if (CUserConfirmDlg::P_USER_CONFIRM_DLG != NULL) 
	{
		CUserConfirmDlg::P_USER_CONFIRM_DLG->ShowWindow( SW_HIDE );
	}
}

BOOL  CUserConfirmDlg::IS_SHOW_DLG()
{
	BOOL bRet = FALSE; 

	if (CUserConfirmDlg::P_USER_CONFIRM_DLG)
	{
		bRet = CUserConfirmDlg::P_USER_CONFIRM_DLG->IsWindowVisible();
	}

	return bRet;
}

int CUserConfirmDlg::WAIT_FOR_SELECT()
{
	WaitForSingleObject(CUserConfirmDlg::hWAIT_EVENT, INFINITE);

	int ret = 0;

	if (CUserConfirmDlg::P_USER_CONFIRM_DLG) 
	{
		ret = CUserConfirmDlg::P_USER_CONFIRM_DLG->nSelect_;
	} 
	else
	{
		ret = 0;	// Stop. 다이얼로그 리소스가 없으면 Stop 한다  
	}

	return ret; 
}

// 리모트 프로그램에서 응답을 보낸것을 적용
void CUserConfirmDlg::REMOTE_SET_RESPONSE( int nSelect )
{
	CUserConfirmDlg::P_USER_CONFIRM_DLG->nSelect_ = nSelect;

	SetEvent( CUserConfirmDlg::P_USER_CONFIRM_DLG->hWAIT_EVENT );
}

//////////////////////////////////////////////////////////////////////////
// CUserConfirmDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserConfirmDlg, CDialog)

CUserConfirmDlg::CUserConfirmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserConfirmDlg::IDD, pParent)
	, nSelect_(0)
{
	bToolDirection_ = TRUE;
}

CUserConfirmDlg::~CUserConfirmDlg()
{
}

void CUserConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, nSelect_);
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUserConfirmDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_OK, &CUserConfirmDlg::OnBnClickedButtonOk)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CUserConfirmDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CUserConfirmDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CUserConfirmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	brhBackground_.CreateSolidBrush( RGB(255, 64, 64) );
	brhOkBtn_.CreateSolidBrush( RGB(128, 128,250) );

	hFntMsg_.CreateFont(
		18, 9,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );

	((CStatic *)GetDlgItem(IDC_STATIC_TLT))->SetFont( &hFntMsg_, FALSE );
	((CButton *)GetDlgItem(IDC_RADIO1))->SetFont( &hFntMsg_, FALSE );
	((CButton *)GetDlgItem(IDC_RADIO2))->SetFont( &hFntMsg_, FALSE );
	((CButton *)GetDlgItem(IDC_RADIO3))->SetFont( &hFntMsg_, FALSE );
	((CButton *)GetDlgItem(IDC_RADIO4))->SetFont( &hFntMsg_, FALSE );

	hFntBtn_.CreateFont(
		24, 12,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );

	((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetFont( &hFntBtn_, FALSE );

	nSelect_ = 0;

	bToolDirection_ = TRUE;

	UpdateData( FALSE );

	SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CUserConfirmDlg::OnDestroy()
{
	CDialog::OnDestroy();

	brhBackground_.DeleteObject();
	brhOkBtn_.DeleteObject();

	hFntMsg_.DeleteObject();
	hFntBtn_.DeleteObject();
}

HBRUSH CUserConfirmDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == 4) 
	{
		hbr = (HBRUSH)brhBackground_;
	}
	else 
	{
		UINT nID = pWnd->GetDlgCtrlID();
		switch (nID)
		{
		case IDC_STATIC_G:
			hbr = (HBRUSH)brhBackground_;
			break;
		case IDC_BUTTON_OK: 
			hbr = (HBRUSH)brhOkBtn_;
			break;
		case IDC_STATIC_TLT:
		case IDC_RADIO1:
		case IDC_RADIO2:
		case IDC_RADIO3:
		case IDC_RADIO4:
		case IDC_STATIC_ERROR_INFO:
			pDC->SetBkMode( TRANSPARENT );
			hbr = (HBRUSH)brhBackground_;
			break;
		}
	}

	return hbr;
}

void CUserConfirmDlg::OnBnClickedButtonOk()
{
	UpdateData( TRUE );

	SetEvent( CUserConfirmDlg::P_USER_CONFIRM_DLG->hWAIT_EVENT );
}

void CUserConfirmDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 10:
		{
			KillTimer( 10 );

			pa::PThread->checkSpindleAirPurge();

			if( IsWindowVisible() )
			{
				SetTimer( 10, 500, NULL );
			}
		}
		break;

	}

	CDialog::OnTimer(nIDEvent);
}

void CUserConfirmDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		display_toolerror_info(bToolDirection_);
	}
}

// 현재 Tool 번호와 툴 에러 정보를 출력 한다 
void CUserConfirmDlg::display_toolerror_info(BOOL bToolDirection)
{
	static TCHAR *P_ERR_MSG[] = {
		_T("Empty"),
		_T("Tool broken"),
		_T("Tool long"),
		_T("Tool short"),
		_T("Unknown")
	};

	bToolDirection_ = bToolDirection;

	CString strMsg;
	int toolNo			= bToolDirection_ == FALSE ? pa::PPAStatus->GetPAStatus()->nCurrentToolNo : pa::PPAStatus->GetPAStatus()->nCurrentTool2No;
	int toolErrorCode	= pa::PTool->GetToolErrCode( toolNo );
	int err_msg_index = 0;

	switch( toolErrorCode )
	{
	case 10010:
	case 20010: err_msg_index = 0; break;
	case 10014:
	case 20014: err_msg_index = 1; break;
	case 10018:
	case 20018: err_msg_index = 2; break;
	case 10019:
	case 20019: err_msg_index = 3; break;
	default:	err_msg_index = 4; break;
	}

	strMsg.Format( _T("%d-th Tool. (error code %d : %s)"), toolNo, toolErrorCode, P_ERR_MSG[err_msg_index] );

	((CStatic *)GetDlgItem(IDC_STATIC_ERROR_INFO))->SetWindowText( strMsg );
}


