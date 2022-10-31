// SetupToolSetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupToolSetDlg.h"

//////////////////////////////////////////////////////////////////////////
// CSetupToolSetDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSetupToolSetDlg, CDialog)

CSetupToolSetDlg::CSetupToolSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupToolSetDlg::IDD, pParent)
{
	nToolNo_			= 0;
	nToolNoSelection_ 	= 3; //reset
	isRight				= FALSE;
}

CSetupToolSetDlg::~CSetupToolSetDlg()
{
}

void CSetupToolSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_TOOL_14, nToolNoSelection_);
}

void CSetupToolSetDlg::SetToolNo( int toolNo )
{
	if( toolNo < 0 ) {
		return ;
	}
	/*
	// Automatically change left/right based on tool if not set before
	if(toolNo>3){
		SetRight();
	} else {
		SetLeft();
	}
	*/
	nToolNo_ = toolNo;
	UpdateSelection();
}

void CSetupToolSetDlg::SetLeft(){
	isRight = FALSE;
	UpdateToolNo();
}

void CSetupToolSetDlg::SetRight(){
	isRight = TRUE;
	UpdateToolNo();
}

void CSetupToolSetDlg::UpdateSelection(){
	nToolNoSelection_ = nToolNo_ - isRight*3 - 1;
	if(nToolNoSelection_ < 0){
		nToolNoSelection_ = 3;	//reset
	}
}

void CSetupToolSetDlg::UpdateToolNo(){
	nToolNo_ = nToolNoSelection_ + isRight*3 + 1;
	if(nToolNo_ > 3 + isRight*3){
		nToolNo_ = 0;	//reset
	}
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupToolSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSetupToolSetDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CSetupToolSetDlg::OnBnClickedButtonCancel)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupToolSetDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupToolSetDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSetupToolSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect recbutton;
	GetClientRect( &CUIrectSO );
	//
	fntUpDownBtn_.CreateFont( 
		16, 8, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );

	fntOkCancelBtn_.CreateFont( 
		18, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("MS Sans Serif") );

	//
	((CButton*)GetDlgItem(IDC_BUTTON_APPLY))->SetFont( &fntOkCancelBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_CANCEL))->SetFont( &fntOkCancelBtn_, TRUE );
	
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_SELECTION))->SetFont( &fntOkCancelBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_RADIO_TOOL_14))->SetFont( &fntUpDownBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_RADIO_TOOL_25))->SetFont( &fntUpDownBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_RADIO_TOOL_36))->SetFont( &fntUpDownBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_RADIO_TOOL_0))->SetFont( &fntUpDownBtn_, TRUE );
	
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_TOOL_SELECTION), this, &recbutton, &CUIrectSO);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_RADIO_TOOL_14), this, &recbutton, &CUIrectSO);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_RADIO_TOOL_25), this, &recbutton, &CUIrectSO);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_RADIO_TOOL_36), this, &recbutton, &CUIrectSO);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_RADIO_TOOL_0), this, &recbutton, &CUIrectSO);

	/*
	((CButton*)GetDlgItem(IDC_RADIO_TOOL_14))->EnableWindow( FALSE );
	((CButton*)GetDlgItem(IDC_RADIO_TOOL_25))->EnableWindow( FALSE );
	((CButton*)GetDlgItem(IDC_RADIO_TOOL_36))->EnableWindow( FALSE );
	((CButton*)GetDlgItem(IDC_RADIO_TOOL_0))->EnableWindow( FALSE );
	*/

	switch (nToolNo_){
		case 0:
			((CButton*)GetDlgItem(IDC_RADIO_TOOL_0))->EnableWindow( TRUE );
			break;
		case 1:
		case 4:
			((CButton*)GetDlgItem(IDC_RADIO_TOOL_14))->EnableWindow( TRUE );
			break;
		case 2:
		case 5:
			((CButton*)GetDlgItem(IDC_RADIO_TOOL_14))->EnableWindow( TRUE );
			break;
		case 3:
		case 6:
			((CButton*)GetDlgItem(IDC_RADIO_TOOL_14))->EnableWindow( TRUE );
			break;
		default:
			((CButton*)GetDlgItem(IDC_RADIO_TOOL_0))->EnableWindow( TRUE );
			break;	
	}

	CRect rcWnd;
	POINT p;
	GetWindowRect( &rcWnd );
	int xpos = 0;
	int ypos = 0;
	if (GetCursorPos(&p))
	{
		xpos = p.x - 30 - isRight * (rcWnd.Width() - 60);
		ypos = p.y - 30;
		//p.x and p.y are now relative to hwnd's client area
	}
	if(isRight){
		((CStatic*)GetDlgItem(IDC_STATIC_TOOL_SELECTION))->SetWindowText( _T("Select RIGHT tool number:") );
		((CButton*)GetDlgItem(IDC_RADIO_TOOL_14))->SetWindowText( _T(" #4") );
		((CButton*)GetDlgItem(IDC_RADIO_TOOL_25))->SetWindowText( _T(" #5") );
		((CButton*)GetDlgItem(IDC_RADIO_TOOL_36))->SetWindowText( _T(" #6") );
	} else {
		((CStatic*)GetDlgItem(IDC_STATIC_TOOL_SELECTION))->SetWindowText( _T("Select LEFT tool number:") );
		((CButton*)GetDlgItem(IDC_RADIO_TOOL_14))->SetWindowText( _T(" #1") );
		((CButton*)GetDlgItem(IDC_RADIO_TOOL_25))->SetWindowText( _T(" #2") );
		((CButton*)GetDlgItem(IDC_RADIO_TOOL_36))->SetWindowText( _T(" #3") );
	}
	
	SetWindowPos( &wndTopMost, xpos, ypos, 0, 0, SWP_NOSIZE );
	//////////////////////////////////////////////////////////////////////////
	//
	//CenterWindow();
	//////////////////////////////////////////////////////////////////////////

	//

	return TRUE;  // return TRUE unless you set the focus to a control
}

DWORD CSetupToolSetDlg::GetToolNo()
{
	UpdateToolNo();
	return nToolNo_;
}

void CSetupToolSetDlg::OnDestroy()
{
	UpdateToolNo();
	fntUpDownBtn_.DeleteObject();
	fntOkCancelBtn_.DeleteObject();
}

void CSetupToolSetDlg::OnBnClickedButtonApply()
{
	CDialog::OnOK();
}

void CSetupToolSetDlg::OnBnClickedButtonCancel()
{
	CDialog::OnCancel();
}
