// RestartDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "RestartDialog.h"
#include "NumericInputDlg.h"

// CRestartDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRestartDialog, CDialog)

CRestartDialog::CRestartDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRestartDialog::IDD, pParent)
{
	nStartLine_ = 0;
}

CRestartDialog::~CRestartDialog()
{
}

void CRestartDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CRestartDialog::SetStartLine( int nStartLine )
{
	nStartLine_ = nStartLine;
}

int CRestartDialog::GetStartLine()
{
	return nStartLine_;
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CRestartDialog, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CRestartDialog::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_LINE_NO, &CRestartDialog::OnBnClickedButtonLineNo)
	ON_BN_CLICKED(IDC_BUTTON_FROM_SELECTED_LINE, &CRestartDialog::OnBnClickedButtonFromSelectedLine)
	ON_BN_CLICKED(IDC_BUTTON_FROM_BEGINING, &CRestartDialog::OnBnClickedButtonFromBegining)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CRestartDialog 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CRestartDialog::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CRestartDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	fntMessage_.CreateFont(
		20, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );

	fntLineNo_.CreateFont(
		22, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );

	fntButton1_.CreateFont(
		22, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );

	fntButton2_.CreateFont(
		20, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );

	((CStatic*)GetDlgItem(IDC_STATIC_MSG1))->SetFont( &fntMessage_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_MSG2))->SetFont( &fntMessage_, TRUE );
	((CEdit*)GetDlgItem(IDC_EDIT_LINE_NO))->SetFont( &fntLineNo_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_LINE_NO))->SetFont( &fntLineNo_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_FROM_SELECTED_LINE))->SetFont( &fntButton1_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_FROM_BEGINING))->SetFont( &fntButton2_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_CANCEL))->SetFont( &fntButton2_, TRUE );

	CString strTemp;
	strTemp.Format( _T("%d"), nStartLine_ );
	((CButton*)GetDlgItem(IDC_BUTTON_LINE_NO))->SetWindowText( strTemp );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CRestartDialog::OnDestroy()
{
	fntMessage_.DeleteObject();
	fntLineNo_.DeleteObject();
	fntButton1_.DeleteObject();
	fntButton2_.DeleteObject();

	CDialog::OnDestroy();
}

// 라인 번호를 입력 받는다
void CRestartDialog::OnBnClickedButtonLineNo()
{
	CNumericInputDlg dlg;
	CString strTemp;

	((CButton*)GetDlgItem(IDC_BUTTON_LINE_NO))->GetWindowText( strTemp );

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( (int)_ttoi( (LPCTSTR)strTemp ) );
	dlg.SetProperty( CNumericInputDlg::PROPERTY_INPUT_SAMEDATA );

	if( dlg.DoModal() == IDOK )
	{
		int nVal = (int)_ttoi( (LPCTSTR)dlg.GetNumber() );
		CString strTemp;
		strTemp.Format( _T("%d"), nVal );
		((CButton*)GetDlgItem(IDC_BUTTON_LINE_NO))->SetWindowText( strTemp );
		nStartLine_ = (DWORD)nVal;
	}
}

void CRestartDialog::OnBnClickedButtonFromSelectedLine()
{
	CDialog::OnOK();
}

void CRestartDialog::OnBnClickedButtonFromBegining()
{
	nStartLine_ = 0;

	CDialog::OnOK();
}

void CRestartDialog::OnBnClickedButtonCancel()
{
	nStartLine_ = 0;
	
	CDialog::OnCancel();
}


