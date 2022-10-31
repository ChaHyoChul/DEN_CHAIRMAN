// ErrorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "ErrorDlg.h"

//////////////////////////////////////////////////////////////////////////
CErrorDlg* PERROR_DLG;
//////////////////////////////////////////////////////////////////////////
// CErrorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CErrorDlg, CDialog)

CErrorDlg::CErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CErrorDlg::IDD, pParent)
	, strErrorType_(_T(""))
	, strErrorCode_(_T(""))
	, strErrorMessage_(_T(""))
	, bMelodyOff_(FALSE)
{
}

CErrorDlg::~CErrorDlg()
{
}

void CErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TYPE, strErrorType_);
	DDX_Text(pDX, IDC_STATIC_CODE, strErrorCode_);
	DDX_Text(pDX, IDC_STATIC_MESSAGE, strErrorMessage_);
	DDX_Check(pDX, IDC_CHECK_MELODY_OFF, bMelodyOff_);
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CErrorDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_OK, &CErrorDlg::OnBnClickedButtonOk)
//	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CErrorDlg::OnBnClickedButtonClose)
	ON_MESSAGE(WM_NOTIFY_WINDOW_MOVE,&CErrorDlg::OnWindowMove )
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CErrorDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CErrorDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CErrorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	bIsShow_ = FALSE;

	brhBkgnd_.CreateSolidBrush( RGB(255, 50, 50) );
	textBkgnd_.CreateSolidBrush( RGB(255, 244, 219) );

	fntTitle_.CreateFont(
		16, 0, 
		0, 0, FW_BOLD, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	fntMessage_.CreateFont(
		16, 0, 
		0, 0, FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	fntButton_.CreateFont(
		14, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	((CStatic*)GetDlgItem(IDC_STATIC_T_TYPE))->SetFont( &fntTitle_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_T_CODE))->SetFont( &fntTitle_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_T_MESSAGE))->SetFont( &fntTitle_, TRUE );

	((CStatic*)GetDlgItem(IDC_STATIC_TYPE))->SetFont( &fntMessage_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_CODE))->SetFont( &fntMessage_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_MESSAGE))->SetFont( &fntMessage_, TRUE );

	((CStatic*)GetDlgItem(IDC_CHECK_MELODY_OFF))->SetFont( &fntMessage_, TRUE );

// 	((CButton*)GetDlgItem(IDC_BUTTON_OK))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->SetFont( &fntButton_, TRUE );

// 	((CButton*)GetDlgItem(IDC_CHECK_MELODY_OFF))->SetCheck( TRUE );

 	CenterWindow( NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CErrorDlg::OnDestroy()
{
	brhBkgnd_.DeleteObject();
	textBkgnd_.DeleteObject();
	fntTitle_.DeleteObject();
	fntMessage_.DeleteObject();
	fntButton_.DeleteObject();

	CDialog::OnDestroy();
}

void CErrorDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);	

	if( bShow )
	{
		bIsShow_	= TRUE;		// 에러가 났을 때, 에러 메시지 다이얼로그가 한번 출력 됨
		((CButton*)GetDlgItem(IDC_CHECK_MELODY_OFF))->SetCheck( TRUE );
	}
}

void CErrorDlg::OnBnClickedButtonOk()
{
	ShowWindow( SW_HIDE );
}

void CErrorDlg::OnBnClickedButtonClose()
{
	ShowWindow( SW_HIDE );

}

void CErrorDlg::RESET_SHOW_ERROR_DLG()
{
	bIsShow_ = FALSE;			// 에러를 리셋할 때, 에러 메시지 다이얼로그 박스의 옵션을 리셋.
								// 같은 에러에 다이얼로그를 띄우지 않고,
								// 리셋 후 난 에러에 대해 다이얼로그를 띄운다 

	ShowWindow( SW_HIDE );
}

void CErrorDlg::SHOW_ERROR_DLG()
{
	bIsShow_ = TRUE;

	strErrorType_.Format( _T("%s"), pa::PPAStatus->GetThreadState()->szErrorType );
	strErrorCode_.Format( _T("%s [%d]"), pa::PPAStatus->GetThreadState()->szErrorCode, pa::PPAStatus->GetThreadState()->nErrorCode );
	strErrorMessage_.Format( _T("%s"),	pa::PPAStatus->GetThreadState()->szErrorMessage );


	int n;
	n = strErrorType_.Replace( _T("\\n"), _T("\r\n") );			// \n에서 \이 하나의 문자로 인식됨. 그래서 |\|n| 으로 2바이트로 저장됨 
	n = strErrorCode_.Replace( _T("\\n"), _T("\r\n") );		// 그래서 replace에서 찾으려면 "\\n"으로 찾아야 한다. 
	n = strErrorMessage_.Replace( _T("\\n"), _T("\r\n") );

	UpdateData( FALSE );

	SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );

}

BOOL CErrorDlg::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CErrorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH	hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT	nCtrlID;

	pDC->SetBkMode( TRANSPARENT );

	if( nCtlColor == 4 ) {
		hbr = (HBRUSH)brhBkgnd_;
	}
	else {
		nCtrlID = pWnd->GetDlgCtrlID();
		if( nCtrlID == IDC_STATIC_T_TYPE ||
			nCtrlID == IDC_STATIC_T_CODE ||
			nCtrlID == IDC_STATIC_T_MESSAGE ) 
		{
			hbr = (HBRUSH)textBkgnd_;
		}
		else if( nCtrlID == IDC_STATIC_TYPE ||
				 nCtrlID == IDC_STATIC_CODE ||
				 nCtrlID == IDC_STATIC_MESSAGE ||
				 nCtrlID == IDC_STATIC_MESSAGE_BKG )
		{
			hbr = (HBRUSH)textBkgnd_;
		}
		else if( nCtrlID == IDC_CHECK_MELODY_OFF )
		{
			hbr = (HBRUSH)brhBkgnd_;
		}
		else if( nCtrlID == IDC_BUTTON_OPEN ) {
			hbr = (HBRUSH)brhBkgnd_;
		}
	}

	return hbr;
}

LRESULT CErrorDlg::OnWindowMove(WPARAM wparam, LPARAM lparam)
{
	LPPOINT ChangedPos=(LPPOINT)lparam;
	CenterWindow();

	return 0;
}
