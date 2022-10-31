// SelectJogStepDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SelectJogStepDlg.h"


// CSelectJogStepDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSelectJogStepDlg, CDialog)

CSelectJogStepDlg::CSelectJogStepDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectJogStepDlg::IDD, pParent)
	, nSel_(0)
{

}

CSelectJogStepDlg::~CSelectJogStepDlg()
{
}

void CSelectJogStepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_001, nSel_);
}


BEGIN_MESSAGE_MAP(CSelectJogStepDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_01, &CSelectJogStepDlg::OnBnClickedRadio01)
	ON_BN_CLICKED(IDC_RADIO_02, &CSelectJogStepDlg::OnBnClickedRadio02)
	ON_BN_CLICKED(IDC_RADIO_03, &CSelectJogStepDlg::OnBnClickedRadio03)
	ON_BN_CLICKED(IDC_RADIO_10, &CSelectJogStepDlg::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO_20, &CSelectJogStepDlg::OnBnClickedRadio20)
	ON_BN_CLICKED(IDC_RADIO_001, &CSelectJogStepDlg::OnBnClickedRadio001)
	ON_BN_CLICKED(IDC_RADIO_002, &CSelectJogStepDlg::OnBnClickedRadio002)
	ON_BN_CLICKED(IDC_RADIO_005, &CSelectJogStepDlg::OnBnClickedRadio005)
END_MESSAGE_MAP()

// CSelectJogStepDlg 메시지 처리기입니다.

BOOL CSelectJogStepDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSelectJogStepDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos( &wndTop, ptPos_.x, ptPos_.y, 0, 0, SWP_NOSIZE );

	UpdateData( TRUE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSelectJogStepDlg::OnBnClickedRadio01()
{
	UpdateData( TRUE );
	CDialog::OnOK();
}

void CSelectJogStepDlg::OnBnClickedRadio02()
{
	UpdateData( TRUE );
	CDialog::OnOK();
}

void CSelectJogStepDlg::OnBnClickedRadio03()
{
	UpdateData( TRUE );
	CDialog::OnOK();
}

void CSelectJogStepDlg::OnBnClickedRadio10()
{
	UpdateData( TRUE );
	CDialog::OnOK();
}

void CSelectJogStepDlg::OnBnClickedRadio20()
{
	UpdateData( TRUE );
	CDialog::OnOK();
}

void CSelectJogStepDlg::OnBnClickedRadio001()
{
	UpdateData( TRUE );
	CDialog::OnOK();
}

void CSelectJogStepDlg::OnBnClickedRadio002()
{
	UpdateData( TRUE );
	CDialog::OnOK();
}

void CSelectJogStepDlg::OnBnClickedRadio005()
{
	UpdateData( TRUE );
	CDialog::OnOK();
}
