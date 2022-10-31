// SetupSystemRunTimeInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupSystemRunTimeInfoDlg.h"


// CSetupSystemRunTimeInfoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupSystemRunTimeInfoDlg, CDialog)

CSetupSystemRunTimeInfoDlg::CSetupSystemRunTimeInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupSystemRunTimeInfoDlg::IDD, pParent)
{

}

CSetupSystemRunTimeInfoDlg::~CSetupSystemRunTimeInfoDlg()
{
}

void CSetupSystemRunTimeInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetupSystemRunTimeInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetupSystemRunTimeInfoDlg::OnBnClickedButton1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSetupSystemRunTimeInfoDlg 메시지 처리기입니다.

void CSetupSystemRunTimeInfoDlg::OnBnClickedButton1()
{
	WriteLog( CLog::TYPE_OPER, 9, _T("close button click") );

	CDialog::OnOK();
}

BOOL CSetupSystemRunTimeInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect   rcLB;

	hcutil::GetControlPos( IDC_STATIC_RUN_TIME_LIST_AREA, this, &rcLB, TRUE );

	runTimeListBox = new CRunTimeListBox();
	runTimeListBox->SetID( IDC_LISTBOX_RUN_TIME );
	runTimeListBox->Create( WS_CHILD|WS_VISIBLE, rcLB, this, IDC_LISTBOX_RUN_TIME );

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupSystemRunTimeInfoDlg::OnDestroy()
{
	destroy_RunTimeListBox();

	CDialog::OnDestroy();
}

void CSetupSystemRunTimeInfoDlg::destroy_RunTimeListBox()
{
	if( runTimeListBox ) {
	runTimeListBox->DestroyWindow();
		delete runTimeListBox;
		runTimeListBox = NULL;
	}
}

