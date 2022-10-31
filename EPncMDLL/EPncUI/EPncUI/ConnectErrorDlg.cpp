// ConnectErrorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "ConnectErrorDlg.h"


// CConnectErrorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConnectErrorDlg, CDialog)

CConnectErrorDlg::CConnectErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectErrorDlg::IDD, pParent)
{
	bButtonStatus_ = TRUE;
}

CConnectErrorDlg::~CConnectErrorDlg()
{
}

void CConnectErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, stcTitle_);
	DDX_Control(pDX, IDC_STATIC_MSG, stcMessage_);
	DDX_Control(pDX, IDC_BUTTON_RUN, btnRun_);
	DDX_Control(pDX, IDC_BUTTON_STOP, btnStop_);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, btnClose_);
}


BEGIN_MESSAGE_MAP(CConnectErrorDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CConnectErrorDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CConnectErrorDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CConnectErrorDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CConnectErrorDlg 메시지 처리기입니다.

BOOL CConnectErrorDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CConnectErrorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	btnStop_.ShowWindow(SW_HIDE);

	//////////////////////////////////////////////////////////////////////////

	fntTitle_.CreateFont(
		26, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); 

	fntMsg_.CreateFont(
		18, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); 

	fntButton_.CreateFont(
		20, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );

	stcTitle_.SetFont( &fntTitle_, TRUE );
	stcMessage_.SetFont( &fntMsg_, TRUE );
	btnRun_.SetFont( &fntButton_, TRUE );
	btnStop_.SetFont( &fntButton_, TRUE );
	btnClose_.SetFont( &fntButton_, TRUE );

	//////////////////////////////////////////////////////////////////////////

	stcTitle_.SetWindowText( strTitle_ );
	stcMessage_.SetWindowText( strMseeage_ );

	if (bButtonStatus_ == FALSE) {
		btnRun_.EnableWindow( bButtonStatus_ );
		btnStop_.EnableWindow( bButtonStatus_ );
		btnRun_.ShowWindow(SW_HIDE);
		btnStop_.ShowWindow(SW_HIDE);
	}

	//////////////////////////////////////////////////////////////////////////

	SetWindowPos(&wndTopMost, 0, 0, 0, 0,  SWP_NOSIZE|SWP_SHOWWINDOW);
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CConnectErrorDlg::OnDestroy()
{
	CDialog::OnDestroy();

	fntTitle_.DeleteObject();
	fntMsg_.DeleteObject();
	fntButton_.DeleteObject();
}

HBRUSH CConnectErrorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CConnectErrorDlg::OnBnClickedButtonRun()
{
	CDialog::OnOK();
}

void CConnectErrorDlg::OnBnClickedButtonStop()
{
	CDialog::OnCancel();
}

void CConnectErrorDlg::OnBnClickedButtonClose()
{
	CDialog::OnCancel();
}
