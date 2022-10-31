// MsgDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "MsgDlg.h"

CMsgDlg	*PMSG_DLG = NULL; 

// CMsgDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMsgDlg, CDialog)

CMsgDlg::CMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgDlg::IDD, pParent)
{
	Create( IDD_DIALOG_MSG );
}

CMsgDlg::~CMsgDlg()
{
}

void CMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_YES, btnYES_);
	DDX_Control(pDX, IDC_BUTTON_OK, btnOK_);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, btnCLOSE_);
	DDX_Control(pDX, IDC_BUTTON_NO, btnNO_);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, btnCANCEL_);
	DDX_Control(pDX, IDC_STATIC_MSG, stcMsg_);
}

BEGIN_MESSAGE_MAP(CMsgDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CMsgDlg::OnBnClickedButtonOk)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_YES, &CMsgDlg::OnBnClickedButtonYes)
	ON_BN_CLICKED(IDC_BUTTON_NO, &CMsgDlg::OnBnClickedButtonNo)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CMsgDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CMsgDlg::OnBnClickedButtonCancel)
	ON_MESSAGE(WM_NOTIFY_WINDOW_MOVE,&CMsgDlg::OnWindowMove )
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CMsgDlg 메시지 처리기입니다.

BOOL CMsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	brhBkgnd_.CreateSolidBrush( RGB(204, 204, 204) );
	fntMsg_.CreateFont(
//		20, 8,
		17, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	stcMsg_.SetFont( &fntMsg_, TRUE );

	fntButton_.CreateFont(
//		22, 10,
		18, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	btnYES_.SetFont( &fntButton_, FALSE );
	btnOK_.SetFont( &fntButton_, FALSE );
	btnCLOSE_.SetFont( &fntButton_, FALSE );
	btnNO_.SetFont( &fntButton_, FALSE );
	btnCANCEL_.SetFont( &fntButton_, FALSE );

	SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

	CenterWindow( NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMsgDlg::OnDestroy()
{
	fntMsg_.DeleteObject();
	fntButton_.DeleteObject();
	brhBkgnd_.DeleteObject();
}

void CMsgDlg::OnBnClickedButtonYes()
{
	hRet_ = RET_YES;
	ShowWindow( SW_HIDE );
}

void CMsgDlg::OnBnClickedButtonOk()
{
	hRet_ = RET_OK;
	ShowWindow( SW_HIDE );
}

void CMsgDlg::OnBnClickedButtonNo()
{
	hRet_ = RET_NO;
	ShowWindow( SW_HIDE );
}

void CMsgDlg::OnBnClickedButtonClose()
{
	hRet_ = RET_CLOSE;
	ShowWindow( SW_HIDE );
}

void CMsgDlg::OnBnClickedButtonCancel()
{
	hRet_ = RET_CANCEL;
	ShowWindow( SW_HIDE );
}

void CMsgDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// 
}

void CMsgDlg::SetType( CMsgDlg::EN_TYPE hType )
{
	hType_ = hType;

	switch( hType_ )
	{
	case CMsgDlg::TYPE_CLOSE:
		btnYES_.ShowWindow( SW_HIDE );
		btnOK_.ShowWindow( SW_HIDE );;
		btnCLOSE_.ShowWindow( SW_SHOW );;
		btnNO_.ShowWindow( SW_HIDE );;
		btnCANCEL_.ShowWindow( SW_HIDE );;
		break;

	case CMsgDlg::TYPE_OKCANCEL:
		btnYES_.ShowWindow( SW_HIDE );
		btnOK_.ShowWindow( SW_SHOW );;
		btnCLOSE_.ShowWindow( SW_HIDE );;
		btnNO_.ShowWindow( SW_HIDE );;
		btnCANCEL_.ShowWindow( SW_SHOW );;
		break;

	case CMsgDlg::TYPE_YESNOCANCEL:
		btnYES_.ShowWindow( SW_SHOW );
		btnOK_.ShowWindow( SW_HIDE );;
		btnCLOSE_.ShowWindow( SW_HIDE );;
		btnNO_.ShowWindow( SW_SHOW );;
		btnCANCEL_.ShowWindow( SW_SHOW );;
		break;

	case CMsgDlg::TYPE_WAIT:
		btnYES_.ShowWindow( SW_HIDE );
		btnOK_.ShowWindow( SW_HIDE );;
		btnCLOSE_.ShowWindow( SW_HIDE );;
		btnNO_.ShowWindow( SW_HIDE );;
		btnCANCEL_.ShowWindow( SW_HIDE );;
		break;
	}
}

void CMsgDlg::SetMsg( CString& strMsg )
{
	stcMsg_.SetWindowText( strMsg );
}

void CMsgDlg::SetLeftIcon( EN_ICON icon )
{
	((CStatic*)GetDlgItem(IDC_STATIC_INFO))->ShowWindow( SW_HIDE );
	((CStatic*)GetDlgItem(IDC_STATIC_QUEST))->ShowWindow( SW_HIDE );
	((CStatic*)GetDlgItem(IDC_STATIC_WARM))->ShowWindow( SW_HIDE );
	((CStatic*)GetDlgItem(IDC_STATIC_ERROR))->ShowWindow( SW_HIDE );

	switch( icon )
	{
	case ICON_INFO:
		((CStatic*)GetDlgItem(IDC_STATIC_INFO))->ShowWindow( SW_SHOW );
		break;
	case ICON_QUEST:
		((CStatic*)GetDlgItem(IDC_STATIC_QUEST))->ShowWindow( SW_SHOW );
		break;
	case ICON_WARM:
		((CStatic*)GetDlgItem(IDC_STATIC_WARM))->ShowWindow( SW_SHOW );
		break;
	case ICON_ERROR:
		((CStatic*)GetDlgItem(IDC_STATIC_ERROR))->ShowWindow( SW_SHOW );
		break;
	default:
		return ;
	}
}

BOOL CMsgDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CMsgDlg::OnWindowMove(WPARAM wparam, LPARAM lparam)
{
	LPPOINT ChangedPos=(LPPOINT)lparam;
	CenterWindow();

	return 0;
}

HBRUSH CMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)brhBkgnd_;
}

void CMsgDlg::Show()
{
	SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
	CenterWindow();
}

void CMsgDlg::Hide()
{
	SetWindowPos( &wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW );
}

