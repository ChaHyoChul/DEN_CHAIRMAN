// ErrorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncMDLL.h"
#include "ErrorDlg.h"

//////////////////////////////////////////////////////////////////////////
// 에러 다이얼로그 변수를 사용하지 않는다. 필요할 때 지역변수로 선언하고 사용한다
// CErrorDlg* P_ERRORDLG = NULL;
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CErrorDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CErrorDlg, CDialog)

CErrorDlg::CErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CErrorDlg::IDD, pParent)
{
	bButtonStatus_ = TRUE;
}

CErrorDlg::~CErrorDlg()
{
}

void CErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, stcTitle_);
	DDX_Control(pDX, IDC_STATIC_MSG, stcMessage_);
	DDX_Control(pDX, IDC_BUTTON_RUN, btnRun_);
	DDX_Control(pDX, IDC_BUTTON_STOP, btnStop_);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, btnClose_);
}


BEGIN_MESSAGE_MAP(CErrorDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CErrorDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CErrorDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CErrorDlg::OnBnClickedButtonClose)
	ON_WM_KILLFOCUS()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CErrorDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CErrorDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CErrorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	btnStop_.ShowWindow(SW_HIDE);

	//////////////////////////////////////////////////////////////////////////

	fntTitle_.CreateFont(
		28, 0, 
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
		24, 0, 
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

	btnRun_.EnableWindow( bButtonStatus_ );
	btnStop_.EnableWindow( bButtonStatus_ );

	//////////////////////////////////////////////////////////////////////////

	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW);
	CenterWindow();

	GetWindowRect(&rcWnd_);

	SetTimer(1, 200, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CErrorDlg::OnDestroy()
{
	KillTimer(1);
	fntTitle_.DeleteObject();
	fntMsg_.DeleteObject();
	fntButton_.DeleteObject();

	CDialog::OnDestroy();
}

HBRUSH CErrorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CErrorDlg::OnBnClickedButtonRun()
{
	CDialog::OnOK();
}

void CErrorDlg::OnBnClickedButtonStop()
{
	CDialog::OnCancel();
}

void CErrorDlg::OnBnClickedButtonClose()
{
	CDialog::OnCancel();
}

void CErrorDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);
}

void CErrorDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void CErrorDlg::OnTimer(UINT_PTR nIDEvent)
{
// 	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW);
	SetWindowPos(&wndTopMost, rcWnd_.left, rcWnd_.top, rcWnd_.Width(), rcWnd_.Height(), SWP_NOSIZE|SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW);
	
	CDialog::OnTimer(nIDEvent);
}
