// SetupTerminalDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupTerminalDlg.h"
#include "MsgDlgThread.h"

//////////////////////////////////////////////////////////////////////////
// CSetupTerminalDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSetupTerminalDlg, CDialogListPage)

CSetupTerminalDlg::CSetupTerminalDlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupTerminalDlg::IDD, pParent)
{
	pResourcePath_ = RESOURCE_2_PATH;
}

CSetupTerminalDlg::~CSetupTerminalDlg()
{
}

void CSetupTerminalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SEND, editSend_);
	DDX_Control(pDX, IDC_CHECK_SEND_MODE, chkSendMode_);
	DDX_Control(pDX, IDC_CHECK_SHIFT, chkShift_);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, btnBack_);
}

void CSetupTerminalDlg::StartPageWork()
{
	chkSendMode_.SetWindowText( _T("MDA") );
	chkSendMode_.SetCheck( FALSE );
	chkShift_.SetCheck( FALSE );
	SetTimer( 1, 1000, NULL );
}

void CSetupTerminalDlg::StopPageWork()
{
	KillTimer( 1 );
}

void CSetupTerminalDlg::UpdatePage()
{

}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupTerminalDlg, CDialogListPage)
	ON_BN_CLICKED(IDC_BUTTON_TILT, &CSetupTerminalDlg::OnBnClickedButtonTilt)
	ON_BN_CLICKED(IDC_BUTTON_1, &CSetupTerminalDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, &CSetupTerminalDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, &CSetupTerminalDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, &CSetupTerminalDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, &CSetupTerminalDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, &CSetupTerminalDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, &CSetupTerminalDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, &CSetupTerminalDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, &CSetupTerminalDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON_0, &CSetupTerminalDlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON_MINUS, &CSetupTerminalDlg::OnBnClickedButtonMinus)
	ON_BN_CLICKED(IDC_BUTTON_PLUS, &CSetupTerminalDlg::OnBnClickedButtonPlus)
	ON_BN_CLICKED(IDC_BUTTON_Q, &CSetupTerminalDlg::OnBnClickedButtonQ)
	ON_BN_CLICKED(IDC_BUTTON_W, &CSetupTerminalDlg::OnBnClickedButtonW)
	ON_BN_CLICKED(IDC_BUTTON_E, &CSetupTerminalDlg::OnBnClickedButtonE)
	ON_BN_CLICKED(IDC_BUTTON_R, &CSetupTerminalDlg::OnBnClickedButtonR)
	ON_BN_CLICKED(IDC_BUTTON_T, &CSetupTerminalDlg::OnBnClickedButtonT)
	ON_BN_CLICKED(IDC_BUTTON_Y, &CSetupTerminalDlg::OnBnClickedButtonY)
	ON_BN_CLICKED(IDC_BUTTON_U, &CSetupTerminalDlg::OnBnClickedButtonU)
	ON_BN_CLICKED(IDC_BUTTON_I, &CSetupTerminalDlg::OnBnClickedButtonI)
	ON_BN_CLICKED(IDC_BUTTON_O, &CSetupTerminalDlg::OnBnClickedButtonO)
	ON_BN_CLICKED(IDC_BUTTON_P, &CSetupTerminalDlg::OnBnClickedButtonP)
	ON_BN_CLICKED(IDC_BUTTON_BRACKET1, &CSetupTerminalDlg::OnBnClickedButtonBracket1)
	ON_BN_CLICKED(IDC_BUTTON_BRACKET2, &CSetupTerminalDlg::OnBnClickedButtonBracket2)
	ON_BN_CLICKED(IDC_BUTTON_A, &CSetupTerminalDlg::OnBnClickedButtonA)
	ON_BN_CLICKED(IDC_BUTTON_S, &CSetupTerminalDlg::OnBnClickedButtonS)
	ON_BN_CLICKED(IDC_BUTTON_D, &CSetupTerminalDlg::OnBnClickedButtonD)
	ON_BN_CLICKED(IDC_BUTTON_F, &CSetupTerminalDlg::OnBnClickedButtonF)
	ON_BN_CLICKED(IDC_BUTTON_G, &CSetupTerminalDlg::OnBnClickedButtonG)
	ON_BN_CLICKED(IDC_BUTTON_H, &CSetupTerminalDlg::OnBnClickedButtonH)
	ON_BN_CLICKED(IDC_BUTTON_J, &CSetupTerminalDlg::OnBnClickedButtonJ)
	ON_BN_CLICKED(IDC_BUTTON_K, &CSetupTerminalDlg::OnBnClickedButtonK)
	ON_BN_CLICKED(IDC_BUTTON_L, &CSetupTerminalDlg::OnBnClickedButtonL)
	ON_BN_CLICKED(IDC_BUTTON_COLON, &CSetupTerminalDlg::OnBnClickedButtonColon)
	ON_BN_CLICKED(IDC_BUTTON_QUOTE, &CSetupTerminalDlg::OnBnClickedButtonQuote)
	ON_BN_CLICKED(IDC_BUTTON_H_B, &CSetupTerminalDlg::OnBnClickedButtonHB)
	ON_BN_CLICKED(IDC_CHECK_SHIFT, &CSetupTerminalDlg::OnBnClickedCheckShift)
	ON_BN_CLICKED(IDC_BUTTON_Z, &CSetupTerminalDlg::OnBnClickedButtonZ)
	ON_BN_CLICKED(IDC_BUTTON_X, &CSetupTerminalDlg::OnBnClickedButtonX)
	ON_BN_CLICKED(IDC_BUTTON_C, &CSetupTerminalDlg::OnBnClickedButtonC)
	ON_BN_CLICKED(IDC_BUTTON_V, &CSetupTerminalDlg::OnBnClickedButtonV)
	ON_BN_CLICKED(IDC_BUTTON_B, &CSetupTerminalDlg::OnBnClickedButtonB)
	ON_BN_CLICKED(IDC_BUTTON_N, &CSetupTerminalDlg::OnBnClickedButtonN)
	ON_BN_CLICKED(IDC_BUTTON_M, &CSetupTerminalDlg::OnBnClickedButtonM)
	ON_BN_CLICKED(IDC_BUTTON_COMMA, &CSetupTerminalDlg::OnBnClickedButtonComma)
	ON_BN_CLICKED(IDC_BUTTON_POINT, &CSetupTerminalDlg::OnBnClickedButtonPoint)
	ON_BN_CLICKED(IDC_BUTTON_QUEST, &CSetupTerminalDlg::OnBnClickedButtonQuest)
	ON_BN_CLICKED(IDC_CHECK_SEND_MODE, &CSetupTerminalDlg::OnBnClickedCheckSendMode)
	ON_BN_CLICKED(IDC_BUTTON_SPACE, &CSetupTerminalDlg::OnBnClickedButtonSpace)
	ON_BN_CLICKED(IDC_BUTTON_BACK, &CSetupTerminalDlg::OnBnClickedButtonBack)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSetupTerminalDlg::OnBnClickedButtonSend)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetupTerminalDlg::OnBnClickedButtonClose)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupTerminalDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupTerminalDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

BOOL CSetupTerminalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	hfntInEdit_.CreateFont(		
		20, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	hfntOutListBox_.CreateFont(		
		17, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	hfntButton_.CreateFont(		
		15, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	hfntMenuButton_.CreateFont(
		17, 0, 
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	editSend_.SetFont( &hfntInEdit_, TRUE );
	editSend_.SetWindowText( _T("") );
	chkShift_.SetCheck( FALSE );
	OnBnClickedCheckShift();
	chkSendMode_.SetCheck( FALSE );
	OnBnClickedCheckSendMode();

	btnBack_.SetFont( &hfntMenuButton_, FALSE );

	// 
	UINT nButtonIDs [] = {
		IDC_BUTTON_TILT, IDC_BUTTON_1, IDC_BUTTON_2, IDC_BUTTON_3, IDC_BUTTON_4, IDC_BUTTON_5, IDC_BUTTON_6, IDC_BUTTON_7,
		IDC_BUTTON_8, IDC_BUTTON_9, IDC_BUTTON_0, IDC_BUTTON_MINUS, IDC_BUTTON_PLUS, IDC_BUTTON_Q, IDC_BUTTON_W, IDC_BUTTON_E, 
		IDC_BUTTON_R, IDC_BUTTON_T, IDC_BUTTON_Y, IDC_BUTTON_U, IDC_BUTTON_I, IDC_BUTTON_O, IDC_BUTTON_P, IDC_BUTTON_BRACKET1, IDC_BUTTON_BRACKET2, 
		IDC_BUTTON_A, IDC_BUTTON_S, IDC_BUTTON_D, IDC_BUTTON_F, IDC_BUTTON_G, IDC_BUTTON_H, IDC_BUTTON_J, IDC_BUTTON_K, IDC_BUTTON_L,
		IDC_BUTTON_COLON, IDC_BUTTON_QUOTE, IDC_BUTTON_H_B, IDC_CHECK_SHIFT, IDC_BUTTON_Z, IDC_BUTTON_X, IDC_BUTTON_C, IDC_BUTTON_V,
		IDC_BUTTON_B, IDC_BUTTON_N, IDC_BUTTON_M, IDC_BUTTON_COMMA, IDC_BUTTON_POINT, IDC_BUTTON_QUEST, IDC_CHECK_SEND_MODE, IDC_BUTTON_SPACE, IDC_BUTTON_BACK,
		IDC_BUTTON_SEND };

	int len = sizeof(nButtonIDs) / sizeof(UINT);

	CRect recbutton;
	for( int i = 0; i<len; i++ ) {
		((CButton*)GetDlgItem(nButtonIDs[i]))->SetFont( &hfntButton_, TRUE );
		hcutil::reposbutton( (CButton*)GetDlgItem(nButtonIDs[i]), this, &recbutton, &CUIrectST);
	}

	//
	brhSend_.CreateSolidBrush( RGB(64, 240 , 64) );
	brhTermanal_.CreateSolidBrush( RGB(240, 64, 64) );
	brhMDA_.CreateSolidBrush( RGB(64, 240, 64) );

	brhBackButton_.CreateSolidBrush( pa::CLR_BUTTON_BACK );

	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC), this, &recbutton, &CUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_CLOSE), this, &recbutton, &CUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_SEND), this, &recbutton, &CUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON1), this, &recbutton, &CUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON5), this, &recbutton, &CUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON7), this, &recbutton, &CUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON14), this, &recbutton, &CUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON15), this, &recbutton, &CUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON25), this, &recbutton, &CUIrectST);

	hcutil::reposedit( (CEdit*)GetDlgItem(IDC_EDIT_SEND), this, &recbutton, &CUIrectST);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupTerminalDlg::OnDestroy()
{
	hfntInEdit_.DeleteObject();
	hfntOutListBox_.DeleteObject();
	hfntButton_.DeleteObject();
	hfntMenuButton_.DeleteObject();

	brhSend_.DeleteObject();
	brhMDA_.DeleteObject();
	brhTermanal_.DeleteObject();
	brhBackButton_.DeleteObject();

	if( pCanvasCE_ ) {
		delete pCanvasCE_;
		pCanvasCE_ = NULL;
	}

	CDialogListPage::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////

void CSetupTerminalDlg::OnBnClickedButtonTilt()  { addChar( chkShift_.GetCheck() ? CString(_T("~")) : CString(_T("`")) ); }
void CSetupTerminalDlg::OnBnClickedButton1() { addChar( chkShift_.GetCheck() ? CString(_T("!")) : CString(_T("1")) ); }
void CSetupTerminalDlg::OnBnClickedButton2() { addChar( chkShift_.GetCheck() ? CString(_T("@")) : CString(_T("2")) ); }
void CSetupTerminalDlg::OnBnClickedButton3() { addChar( chkShift_.GetCheck() ? CString(_T("#")) : CString(_T("3")) ); }
void CSetupTerminalDlg::OnBnClickedButton4() { addChar( chkShift_.GetCheck() ? CString(_T("$")) : CString(_T("4")) );}
void CSetupTerminalDlg::OnBnClickedButton5() { addChar( chkShift_.GetCheck() ? CString(_T("%")) : CString(_T("5")) ); }
void CSetupTerminalDlg::OnBnClickedButton6() { addChar( chkShift_.GetCheck() ? CString(_T("^")) : CString(_T("6")) ); }
void CSetupTerminalDlg::OnBnClickedButton7() { addChar( chkShift_.GetCheck() ? CString(_T("&")) : CString(_T("7")) ); }
void CSetupTerminalDlg::OnBnClickedButton8() { addChar( chkShift_.GetCheck() ? CString(_T("*")) : CString(_T("8")) ); }
void CSetupTerminalDlg::OnBnClickedButton9() { addChar( chkShift_.GetCheck() ? CString(_T("(")) : CString(_T("9")) ); }
void CSetupTerminalDlg::OnBnClickedButton0() { addChar( chkShift_.GetCheck() ? CString(_T(")")) : CString(_T("0")) ); }
void CSetupTerminalDlg::OnBnClickedButtonMinus() { addChar( chkShift_.GetCheck() ? CString(_T("_")) : CString(_T("-")) ); }
void CSetupTerminalDlg::OnBnClickedButtonPlus() { addChar( chkShift_.GetCheck() ? CString(_T("+")) : CString(_T("=")) ); }
void CSetupTerminalDlg::OnBnClickedButtonQ() { addChar( CString(_T("Q")) ); }
void CSetupTerminalDlg::OnBnClickedButtonW() { addChar( CString(_T("W")) ); }
void CSetupTerminalDlg::OnBnClickedButtonE() { addChar( CString(_T("E")) ); }
void CSetupTerminalDlg::OnBnClickedButtonR() { addChar( CString(_T("R")) ); }
void CSetupTerminalDlg::OnBnClickedButtonT() { addChar( CString(_T("T")) ); }
void CSetupTerminalDlg::OnBnClickedButtonY() { addChar( CString(_T("Y")) ); }
void CSetupTerminalDlg::OnBnClickedButtonU() { addChar( CString(_T("U")) ); }
void CSetupTerminalDlg::OnBnClickedButtonI() { addChar( CString(_T("I")) ); }
void CSetupTerminalDlg::OnBnClickedButtonO() { addChar( CString(_T("O")) ); }
void CSetupTerminalDlg::OnBnClickedButtonP() { addChar( CString(_T("P")) ); }
void CSetupTerminalDlg::OnBnClickedButtonBracket1() { addChar( chkShift_.GetCheck() ? CString(_T("{")) : CString(_T("[")) ); }
void CSetupTerminalDlg::OnBnClickedButtonBracket2() { addChar( chkShift_.GetCheck() ? CString(_T("}")) : CString(_T("]")) ); }
void CSetupTerminalDlg::OnBnClickedButtonA() { addChar( CString(_T("A")) ); }
void CSetupTerminalDlg::OnBnClickedButtonS() { addChar( CString(_T("S")) ); }
void CSetupTerminalDlg::OnBnClickedButtonD() { addChar( CString(_T("D")) ); }
void CSetupTerminalDlg::OnBnClickedButtonF() { addChar( CString(_T("F")) ); }
void CSetupTerminalDlg::OnBnClickedButtonG() { addChar( CString(_T("G")) ); }
void CSetupTerminalDlg::OnBnClickedButtonH() { addChar( CString(_T("H")) ); }
void CSetupTerminalDlg::OnBnClickedButtonJ() { addChar( CString(_T("J")) ); }
void CSetupTerminalDlg::OnBnClickedButtonK() { addChar( CString(_T("K")) ); }
void CSetupTerminalDlg::OnBnClickedButtonL() { addChar( CString(_T("L")) ); }
void CSetupTerminalDlg::OnBnClickedButtonColon() { addChar( chkShift_.GetCheck() ? CString(_T(":")) : CString(_T(";")) ); }
void CSetupTerminalDlg::OnBnClickedButtonQuote() { addChar( chkShift_.GetCheck() ? CString(_T("\"")) : CString(_T("'")) ); }
void CSetupTerminalDlg::OnBnClickedButtonHB() { addChar( chkShift_.GetCheck() ? CString(_T("|")) : CString(_T("\\")) ); }
void CSetupTerminalDlg::OnBnClickedButtonZ() { addChar( CString(_T("Z")) ); }
void CSetupTerminalDlg::OnBnClickedButtonX() { addChar( CString(_T("X")) ); }
void CSetupTerminalDlg::OnBnClickedButtonC() { addChar( CString(_T("C")) ); }
void CSetupTerminalDlg::OnBnClickedButtonV() { addChar( CString(_T("V")) ); }
void CSetupTerminalDlg::OnBnClickedButtonB() { addChar( CString(_T("B")) ); }
void CSetupTerminalDlg::OnBnClickedButtonN() { addChar( CString(_T("N")) ); }
void CSetupTerminalDlg::OnBnClickedButtonM() { addChar( CString(_T("M")) ); }
void CSetupTerminalDlg::OnBnClickedButtonComma() { addChar( chkShift_.GetCheck() ? CString(_T("<")) : CString(_T(",")) ); }
void CSetupTerminalDlg::OnBnClickedButtonPoint() { addChar( chkShift_.GetCheck() ? CString(_T(">")) : CString(_T(".")) ); }
void CSetupTerminalDlg::OnBnClickedButtonQuest() { addChar( chkShift_.GetCheck() ? CString(_T("?")) : CString(_T("/")) ); }
void CSetupTerminalDlg::OnBnClickedButtonSpace() { addChar( CString(_T(" ")) ); }

// 전송 모드 변경 
void CSetupTerminalDlg::OnBnClickedCheckSendMode()
{
	if( chkSendMode_.GetCheck() == TRUE ) 
	{
		chkSendMode_.SetWindowText( _T("TERMINAL") );
	}
	else 
	{
		chkSendMode_.SetWindowText( _T("MDA") );		
	}
	editSend_.SetWindowText(_T(""));
	((CButton*)GetDlgItem(IDC_BUTTON_SEND))->SetWindowText(_T("Send"));

}

void CSetupTerminalDlg::OnBnClickedButtonBack()
{
	if( chkShift_.GetCheck() == TRUE )
	{
		editSend_.SetWindowText( _T("") );
	}
	else 
	{
		CString strTemp;
		editSend_.GetWindowText( strTemp );
		strTemp = strTemp.Left( strTemp.GetLength()-1 );
		editSend_.SetWindowText( strTemp );
	}

}

void CSetupTerminalDlg::addChar( CString str )
{
	CString strTemp;

	editSend_.GetWindowText( strTemp );
	strTemp += str;
	editSend_.SetWindowText( strTemp );

}

// Shift 버튼 누름 상태. 버튼의 Text를 변경한다 
void CSetupTerminalDlg::OnBnClickedCheckShift()
{
	UINT nID[] = {
		IDC_BUTTON_TILT, IDC_BUTTON_1, IDC_BUTTON_2, IDC_BUTTON_3, IDC_BUTTON_4,
		IDC_BUTTON_5, IDC_BUTTON_6, IDC_BUTTON_7, IDC_BUTTON_8, IDC_BUTTON_9, 
		IDC_BUTTON_0, IDC_BUTTON_MINUS, IDC_BUTTON_PLUS, IDC_BUTTON_BRACKET1, IDC_BUTTON_BRACKET2, 
		IDC_BUTTON_BACK, IDC_BUTTON_COLON, IDC_BUTTON_QUOTE, IDC_BUTTON_H_B, IDC_BUTTON_COMMA, 
		IDC_BUTTON_POINT, IDC_BUTTON_QUEST
	};
	
	TCHAR *chs[22][2] = { 
		{ _T("`"), _T("~") },
		{ _T("1"), _T("!") },
		{ _T("2"), _T("@") },
		{ _T("3"), _T("#") },
		{ _T("4"), _T("$") },

		{ _T("5"), _T("%") },
		{ _T("6"), _T("^") },
		{ _T("7"), _T("&&") },
		{ _T("8"), _T("*") },
		{ _T("9"), _T("(") },
		
		{ _T("0"), _T(")") },
		{ _T("-"), _T("_") },
		{ _T("="), _T("+") }, // { _T("+"), _T("=") },
		{ _T("["), _T("{") },
		{ _T("]"), _T("}") },

		{ _T("BACK"), _T("CLR") },	
		{ _T(";"), _T(":") },
		{ _T("'"), _T("\"") },
		{ _T("\\") , _T("|") },		/*{ _T("|"), _T("\\") },*/
		{ _T(","), _T("<") },

		{ _T("."), _T(">") },
		{ _T("/"), _T("?") }
	};

	int		nNum = sizeof(nID) / sizeof(UINT);
	int		j = chkShift_.GetCheck() == FALSE ? 0 : 1;

	for( int i = 0; i<nNum; i++ )
	{
		((CButton *)GetDlgItem( nID[i] ))->SetWindowText( CString( chs[i][j]) );
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void CSetupTerminalDlg::OnBnClickedButtonSend()
{
	const DWORD TIMEOUT = 120*1000;		// 60초 
	BOOL bSendMode = chkSendMode_.GetCheck();
	CString	strCommand;
	CString strResp;
	char	szCommand[128];
	DWORD	dwTime;
	CString	strLog;

	memset((void*)szCommand, 0, sizeof(char)*128);

	editSend_.GetWindowText( strCommand );

	//////////////////////////////////////////////////////////////////////////
	// 앞 공백 제거 후 입력된 데이터가 없으면, 전송하지 않는다 
	CString strTemp = strCommand;
	strTemp.Replace( _T(" "), _T("") );
	if( strTemp.IsEmpty() ) {
		return ;
	}
	//////////////////////////////////////////////////////////////////////////

	hcutil::UNICODE_TO_ASCII( (wchar_t*)(LPCTSTR)strCommand, szCommand, 127 );

	dwTime = GetTickCount();

	if( !bSendMode ) 
	{
		//////////////////////////////////////////////////////////////////////////
		// log 
		CString strCmd = hcutil::ASCII_TO_CSTRING( szCommand );
		strLog.Format( _T("send mda command : %s"), strCmd );
		writeLog( strLog );
		// MDA Command 
		pa::PPAStatus->SetIpcCommandComplete( FALSE );
		PPNC_IPC_CLIENT->SendMDACommand( szCommand );
	}
	else 
	{
		//////////////////////////////////////////////////////////////////////////
		// log 
		CString strCmd = hcutil::ASCII_TO_CSTRING( szCommand );
		strLog.Format( _T("send term. command : %s"), strCmd );
		writeLog( strLog );
		// 2015.10.15 명령코드 확인 
		if( strcmp(szCommand, "SAV") == 0 || strcmp(szCommand, "SAVE") == 0 ) {
			memset((void*)szCommand, 0, sizeof(char)*128);
			sprintf_s( szCommand, 128, "sav" );
		}
	
		// Terminal Command 
		pa::PPAStatus->SetIpcCommandComplete( FALSE );
		PPNC_IPC_CLIENT->SendCommand( szCommand, 0 );
	}

	Sleep( 500 );
	
	while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE )
	{
		if( (GetTickCount() - dwTime ) > TIMEOUT ) {
			// Timeout 에러 
			CString strMsg;
			strMsg.Format( _T("Timeout error") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
			CMsgDlgThread::GetInstance()->Wait();
			return ;
		}

		Sleep( 100 );
	}

	if( bSendMode ) {
		// Terminal Command 를 listbox에 출력 한다 
		char *p = pa::PPAStatus->GetThreadState()->szResponseTerminalCommand_;
		int ln = strlen(p);
// 		if( p[0]==6 ) { // 6 is '-'		'-' ) {
// 			p[ln-1] = 0;
// 		} else {
// 			p[ln-2] = 0;
// 		}
// 		ln = strlen(p);

		if( ln > 0 ) {
			strResp = hcutil::ASCII_TO_CSTRING( p );
			strCommand += CString(_T("=")) + strResp;

		}
		strCommand = CString( _T("[T] ") ) + strCommand;
		//////////////////////////////////////////////////////////////////////////
		// log 
		strLog.Format( _T("recv term. : %s"), strCommand );
		writeLog( strLog );

	} else {
		strCommand = CString( _T("[M] ") ) + strCommand;
	}

	strCommand.Format( _T("") );
	editSend_.SetWindowText( strCommand );
}

//////////////////////////////////////////////////////////////////////////

HBRUSH CSetupTerminalDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogListPage::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode( TRANSPARENT );

	if( nCtlColor == 4 ) {
		hbr = hbr;
	}
	else {
		UINT nCtrlID = pWnd->GetDlgCtrlID();
		switch( nCtrlID )
		{
		case IDC_CHECK_SEND_MODE:
			if( chkSendMode_.GetCheck() == FALSE ) {
				// MDA
				hbr = (HBRUSH)brhMDA_;
			}
			else {
				// Treminal
				hbr = (HBRUSH)brhTermanal_;
			}
			break;

		case IDC_EDIT_SEND:
		case IDC_BUTTON_SEND:
			if( chkSendMode_.GetCheck() == FALSE ) {
				// MDA 
				hbr = (HBRUSH)brhMDA_;
			}
			else {
				// Treminal
				hbr = (HBRUSH)brhTermanal_;
			}
			break;

		case IDC_BUTTON_CLOSE:
			hbr = (HBRUSH)brhBackButton_;
			break;
		}
	}

	return hbr;
}

//////////////////////////////////////////////////////////////////////////

void CSetupTerminalDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 ) 
	{
		KillTimer( 1 );

		updateButtonState();

		if( IsWindowVisible() ) {
			SetTimer( 1, 200, NULL );
		}
	}
	
	CDialogListPage::OnTimer(nIDEvent);
}

void CSetupTerminalDlg::updateButtonState()
{
	static int PREV_STATE = -1;
	int	curr_state = 0;

	if( pa::PPAStatus->GetThreadState()->hConnectStatus == pa::CONNECT_STATUS_CONNECTED &&
		pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ &&
		pa::PPAStatus->GetPAStatus()->nRunStatus == pa::PA_RUN_STATUS_IDLE &&
		(pa::PPAStatus->GetThreadState()->hRunMode == pa::RUNMODE_STOP || pa::PPAStatus->GetThreadState()->hRunMode == pa::RUNMODE_ERROR) 
		)
	{
		curr_state = 1;
	}

	if( PREV_STATE != curr_state ) 
	{
		((CButton*)GetDlgItem(IDC_BUTTON_SEND))->EnableWindow( curr_state );
	}
	
	//////////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////

void CSetupTerminalDlg::OnBnClickedButtonClose()
{
	//////////////////////////////////////////////////////////////////////////
	// log
	writeLog( _T("back button click") );
	//////////////////////////////////////////////////////////////////////////
	ASSERT( pParentWnd_ );
	pParentWnd_->PostMessage( WM_SETUP, (WPARAM)SETUP_BACK, (LPARAM)0 );
}

//void CSetupTerminalDlg::OnBnClickedButtonEmoReset()
//{
//	if( pa::PPAStatus->GetRunMode() == pa::RUNMODE_ERROR ) {
//		//////////////////////////////////////////////////////////////////////////
//		// log
//		writeLog( _T("reset button click") );
//		//////////////////////////////////////////////////////////////////////////
//		// Reset
//		PPNC_IPC_CLIENT->ErrorReset();
//	} else {
//		//////////////////////////////////////////////////////////////////////////
//		// log
//		writeLog( _T("emo button click") );
//		//////////////////////////////////////////////////////////////////////////
//		// EMO
//		PPNC_IPC_CLIENT->Emergency();
//	}
//}

void CSetupTerminalDlg::writeLog( LPCTSTR log_msg )
{
	//////////////////////////////////////////////////////////////////////////
	// log 
	WriteLog( CLog::TYPE_OPER, 6, log_msg );
	//////////////////////////////////////////////////////////////////////////
}


void CSetupTerminalDlg::PreInitDialog()
{
	CDC*	pDC = GetDC();
	CRect	rcWnd;

	GetClientRect(&CUIrectST);
	MoveWindow(0,0,1025,621);
	GetClientRect( &rcWnd );

	pCanvasCE_ = new hcutil::CCanvasCE();
	ASSERT(pCanvasCE_ );
	pCanvasCE_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(1, 1, 0) );
	pCanvasCE_->GetCanvasCELayerMgr()->Add( FALSE, RGB(0, 0, 0) );

	pCanvasCE_->GetCanvasCELayerMgr()->Get( 0 )->FillSolidRect( rcWnd, RGB(190, 190, 190) );

	ReleaseDC( pDC );
	pDC = NULL;

	CDialogListPage::PreInitDialog();
}

void CSetupTerminalDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc.m_hDC, dc.m_ps.rcPaint );
	}
}
