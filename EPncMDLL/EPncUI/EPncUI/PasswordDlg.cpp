// PasswordDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "PasswordDlg.h"

// CPasswordDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPasswordDlg, CDialog)

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswordDlg::IDD, pParent)
	, strPassword_(_T(""))
{

}

CPasswordDlg::~CPasswordDlg()
{
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, strPassword_);
	DDX_Control(pDX, IDC_COMBO_SEL_USER, cboUsrMode_);
}


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPasswordDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_1, &CPasswordDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, &CPasswordDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, &CPasswordDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, &CPasswordDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, &CPasswordDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, &CPasswordDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, &CPasswordDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, &CPasswordDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, &CPasswordDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON_0, &CPasswordDlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON_CLR, &CPasswordDlg::OnBnClickedButtonClr)
	ON_BN_CLICKED(IDC_BUTTON_ENT, &CPasswordDlg::OnBnClickedButtonEnt)
	ON_CBN_SELCHANGE(IDC_COMBO_SEL_USER, &CPasswordDlg::OnCbnSelchangeComboSelUser)
	ON_WM_SHOWWINDOW()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CPasswordDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CPasswordDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CPasswordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	hFntCbo_.CreateFont(
		28, 0, 
		0, 0, FW_BOLD, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") );

	hFntEdit_.CreateFont(
		20, 0, 
		0, 0, FW_BOLD, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") );
	
	hFntButton_.CreateFont(
		24, 0, 
		0, 0, FW_BOLD, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") );

	cboUsrMode_.SetFont( &hFntCbo_, TRUE );
	((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->SetFont( &hFntEdit_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_0))->SetFont( &hFntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_1))->SetFont( &hFntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_2))->SetFont( &hFntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_3))->SetFont( &hFntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_4))->SetFont( &hFntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_5))->SetFont( &hFntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_6))->SetFont( &hFntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_7))->SetFont( &hFntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_8))->SetFont( &hFntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_9))->SetFont( &hFntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_CLR))->SetFont( &hFntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_ENT))->SetFont( &hFntButton_, TRUE );

	strPassword_.Format( _T("") );

// 	for( int i = 0; i<(int)pa::USER_MODE_NUM; i++ ) {
// 		cboUsrMode_.AddString( pa::STR_USER_MODE[i] );
// 	}
	cboUsrMode_.AddString(pa::STR_USER_MODE[0]);
	cboUsrMode_.AddString(pa::STR_USER_MODE[1]);
	cboUsrMode_.SetCurSel( 0 );

	UpdateData( FALSE );

	CRect rcWnd;
	GetWindowRect( &rcWnd );
	int xpos = 1024/2 - rcWnd.Width()/2;
	int ypos = 768/2 - rcWnd.Height()/2;
	SetWindowPos( &wndNoTopMost, xpos, ypos, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW );

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPasswordDlg::OnDestroy()
{
	hFntCbo_.DeleteObject();
	hFntEdit_.DeleteObject();
	hFntButton_.DeleteObject();

	CDialog::OnDestroy();
}

void CPasswordDlg::addChar( CString strChar )
{
	UpdateData( TRUE );
	strPassword_ += strChar;
	UpdateData( FALSE );
}

void CPasswordDlg::writeLog( LPCTSTR log_msg )
{
	//////////////////////////////////////////////////////////////////////////
	// log 
	WriteLog( CLog::TYPE_OPER, 1, log_msg );
	//////////////////////////////////////////////////////////////////////////
}

void CPasswordDlg::OnBnClickedButton1() { addChar( CString("1") ); }
void CPasswordDlg::OnBnClickedButton2() { addChar( CString("2") ); }
void CPasswordDlg::OnBnClickedButton3() { addChar( CString("3") ); }
void CPasswordDlg::OnBnClickedButton4() { addChar( CString("4") ); }
void CPasswordDlg::OnBnClickedButton5() { addChar( CString("5") ); }
void CPasswordDlg::OnBnClickedButton6() { addChar( CString("6") ); }
void CPasswordDlg::OnBnClickedButton7() { addChar( CString("7") ); }
void CPasswordDlg::OnBnClickedButton8() { addChar( CString("8") ); }
void CPasswordDlg::OnBnClickedButton9() { addChar( CString("9") ); }
void CPasswordDlg::OnBnClickedButton0() { addChar( CString("0") ); }

void CPasswordDlg::OnBnClickedButtonClr()
{
	strPassword_.Format( _T("") );
	UpdateData( FALSE );
}

// void CPasswordDlg::OnBnClickedButtonEnt()
// {
// // 	TCHAR	*PASSWORD = _T("2684");
// // 
// // 	UpdateData( TRUE );
// // 
// // 	if( strPassword_ == CString( PASSWORD ) ) {
// // 	if( strPassword_ == STR_PASSWORD[hLevel_] ) {
// // 		CDialog::OnOK();
// // 	}
// // 	else {
// // 		CDialog::OnCancel();
// // 	}
// 
// 	UpdateData( TRUE );
// 
// 	CString strTemp;
// 	int		nCurSel = ((CComboBox*)GetDlgItem(IDC_COMBO_SEL_USER))->GetCurSel(); 
// 	strTemp.Format( _T("%s"), pa::STR_USER_MODE_PW[nCurSel] );
// 
// 
// 	if( strTemp == strPassword_ ) 
// 	{
// 		switch( nCurSel )
// 		{
// 		case 0:	pa::SET_CURRENT_USERMODE( pa::USER_MODE_USR ); break;	//	pa::USER_MODE = pa::USER_MODE_USR; break;
// 		case 1: pa::SET_CURRENT_USERMODE( pa::USER_MODE_MGR ); break;	//	pa::USER_MODE = pa::USER_MODE_MGR; break;
// 		case 2: pa::SET_CURRENT_USERMODE( pa::USER_MODE_RND ); break;	//	pa::USER_MODE = pa::USER_MODE_RND; break;
// 		default:pa::SET_CURRENT_USERMODE( pa::USER_MODE_USR ); break;	//	pa::USER_MODE = pa::USER_MODE_USR; break;
// 		}
// 		CDialog::OnOK();
// 	}
// 	else 
// 	{
// 	//	pa::USER_MODE = pa::USER_MODE_USR;
// 		pa::SET_CURRENT_USERMODE( pa::USER_MODE_USR ); 
// 		CDialog::OnCancel();
// 	}
// }

void CPasswordDlg::OnBnClickedButtonEnt()
{
	UpdateData( TRUE );
	CString strLogTemp;

	int nCurSel = ((CComboBox*)GetDlgItem(IDC_COMBO_SEL_USER))->GetCurSel();
	
	strLogTemp.Format( _T("Login attempt as '%s', password '%s'"), pa::STR_USER_MODE[nCurSel], strPassword_ );
	writeLog( strLogTemp );
	
	if( pa::STR_USER_MODE_PW[pa::USER_MODE_USR] == strPassword_ && nCurSel == pa::USER_MODE_USR) 
	{
		pa::SET_CURRENT_USERMODE( pa::USER_MODE_USR );	//	pa::USER_MODE = pa::USER_MODE_USR;
		CDialog::OnOK();
	}

	else if ( pa::STR_USER_MODE_PW[pa::USER_MODE_MGR] == strPassword_ && nCurSel == pa::USER_MODE_MGR)
	{
		pa::SET_CURRENT_USERMODE( pa::USER_MODE_MGR );	//	pa::USER_MODE = pa::USER_MODE_MGR;
		CDialog::OnOK();
	}

	else if ( strPassword_ == pa::GET_RND_PASSWORD() && nCurSel == pa::USER_MODE_MGR )
	{
		pa::SET_CURRENT_USERMODE( pa::USER_MODE_RND );	//	pa::USER_MODE = pa::USER_MODE_RND;
		CDialog::OnOK();
	}

	else 
	{
		pa::SET_CURRENT_USERMODE( pa::USER_MODE_USR ); 
		CDialog::OnCancel();
	}
}

void CPasswordDlg::OnCbnSelchangeComboSelUser()
{
	strPassword_.Format( _T("") );
	UpdateData( FALSE );
}


void CPasswordDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

}

void CPasswordDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);

}
