// MaxxLinkServiceDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "MaxxLinkServiceDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"


// CMaxxLinkServiceDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMaxxLinkServiceDlg, CDialog)

CMaxxLinkServiceDlg::CMaxxLinkServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaxxLinkServiceDlg::IDD, pParent)
{

}

CMaxxLinkServiceDlg::~CMaxxLinkServiceDlg()
{
}

void CMaxxLinkServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMaxxLinkServiceDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CMaxxLinkServiceDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMaxxLinkServiceDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_LINKER_IP1, &CMaxxLinkServiceDlg::OnBnClickedButtonLinkerIp1)
	ON_BN_CLICKED(IDC_BUTTON_LINKER_IP2, &CMaxxLinkServiceDlg::OnBnClickedButtonLinkerIp2)
	ON_BN_CLICKED(IDC_BUTTON_LINKER_IP3, &CMaxxLinkServiceDlg::OnBnClickedButtonLinkerIp3)
	ON_BN_CLICKED(IDC_BUTTON_LINKER_IP4, &CMaxxLinkServiceDlg::OnBnClickedButtonLinkerIp4)
	ON_BN_CLICKED(IDC_BUTTON_LINKER_PORTNO, &CMaxxLinkServiceDlg::OnBnClickedButtonLinkerPortno)
	ON_BN_CLICKED(IDC_BUTTON_SERVICE_IP1, &CMaxxLinkServiceDlg::OnBnClickedButtonServiceIp1)
	ON_BN_CLICKED(IDC_BUTTON_SERVICE_IP2, &CMaxxLinkServiceDlg::OnBnClickedButtonServiceIp2)
	ON_BN_CLICKED(IDC_BUTTON_SERVICE_IP3, &CMaxxLinkServiceDlg::OnBnClickedButtonServiceIp3)
	ON_BN_CLICKED(IDC_BUTTON_SERVICE_IP4, &CMaxxLinkServiceDlg::OnBnClickedButtonServiceIp4)
	ON_BN_CLICKED(IDC_BUTTON_SERVICE_PORTNO, &CMaxxLinkServiceDlg::OnBnClickedButtonServicePortno)
	ON_BN_CLICKED(IDC_CHECK_USE_LINKER, &CMaxxLinkServiceDlg::OnBnClickedCheckUseLinker)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CMaxxLinkServiceDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CMaxxLinkServiceDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CMaxxLinkServiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//
	load_maxxlink_config();

	// 
	CRect rcWnd;
	GetClientRect( &rcWnd );
	int xpos = 1024/2 - rcWnd.Width()/2;
	int ypos = 768/2 - rcWnd.Height()/2;
	SetWindowPos( &wndTopMost, xpos, ypos, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW );

	CenterWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMaxxLinkServiceDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

// ..config/maxxlink_config.ini 파일에서 설정 데이터를 읽어서 초기화 한다 
void CMaxxLinkServiceDlg::load_maxxlink_config()
{
	CCEIniFile	hIniFile;
	int			nTemp;

	TCHAR		szTemp[64];
	DWORD		dwTemp;
	CString		strTemp;

	if( hcutil::IsExistFile( INI_MAXXLINK_CONFIG_PATH ) == TRUE )
	{
		hIniFile.Open( INI_MAXXLINK_CONFIG_PATH );

		nTemp = 0;
		hIniFile.GetValue( _T("MAXX_LINK"), _T("UsingLinker"), &nTemp );
		bUsingLinker_ = ( nTemp == 0 ) ? FALSE  : TRUE;

		hIniFile.GetValue( _T("Linker"), _T("Ip0"), &strLinkerIp_[0] );
		hIniFile.GetValue( _T("Linker"), _T("Ip1"), &strLinkerIp_[1] );
		hIniFile.GetValue( _T("Linker"), _T("Ip2"), &strLinkerIp_[2] );
		hIniFile.GetValue( _T("Linker"), _T("Ip3"), &strLinkerIp_[3] );
		hIniFile.GetValue( _T("Linker"), _T("PNo"), &strLinkerPortNo_ );

		hIniFile.GetValue( _T("Service"), _T("Ip0"), &strServiceIp_[0] );
		hIniFile.GetValue( _T("Service"), _T("Ip1"), &strServiceIp_[1] );
		hIniFile.GetValue( _T("Service"), _T("Ip2"), &strServiceIp_[2] );
		hIniFile.GetValue( _T("Service"), _T("Ip3"), &strServiceIp_[3] );
		hIniFile.GetValue( _T("Service"), _T("PNo"), &strServicePortNo_ );

		hIniFile.Close();
	}
	else 
	{
		bUsingLinker_ = FALSE;
		strLinkerIp_[0].Format( _T("0") );
		strLinkerIp_[1].Format( _T("0") );
		strLinkerIp_[2].Format( _T("0") );
		strLinkerIp_[3].Format( _T("0") );
		strLinkerPortNo_.Format( _T("0") );
		strServiceIp_[0].Format( _T("0") );
		strServiceIp_[1].Format( _T("0") );
		strServiceIp_[2].Format( _T("0") );
		strServiceIp_[3].Format( _T("0") );
		strServicePortNo_.Format( _T("0") );
	}

	((CButton*)GetDlgItem(IDC_CHECK_USE_LINKER))->SetCheck( bUsingLinker_ );

	((CButton*)GetDlgItem(IDC_BUTTON_LINKER_IP1))->SetWindowText( strLinkerIp_[0] );
	((CButton*)GetDlgItem(IDC_BUTTON_LINKER_IP2))->SetWindowText( strLinkerIp_[1] );
	((CButton*)GetDlgItem(IDC_BUTTON_LINKER_IP3))->SetWindowText( strLinkerIp_[2] );
	((CButton*)GetDlgItem(IDC_BUTTON_LINKER_IP4))->SetWindowText( strLinkerIp_[3] );
	((CButton*)GetDlgItem(IDC_BUTTON_LINKER_PORTNO))->SetWindowText( strLinkerPortNo_ );

	((CButton*)GetDlgItem(IDC_BUTTON_SERVICE_IP1))->SetWindowText( strServiceIp_[0] );
	((CButton*)GetDlgItem(IDC_BUTTON_SERVICE_IP2))->SetWindowText( strServiceIp_[1] );
	((CButton*)GetDlgItem(IDC_BUTTON_SERVICE_IP3))->SetWindowText( strServiceIp_[2] );
	((CButton*)GetDlgItem(IDC_BUTTON_SERVICE_IP4))->SetWindowText( strServiceIp_[3] );
	((CButton*)GetDlgItem(IDC_BUTTON_SERVICE_PORTNO))->SetWindowText( strServicePortNo_ );
}

void CMaxxLinkServiceDlg::save_maxxlink_config()
{
	CCEIniFile	hIniFile;

	hIniFile.Open( INI_MAXXLINK_CONFIG_PATH );

	int nTemp = bUsingLinker_ ? 1 : 0;
	hIniFile.SetValue( _T("MAXX_LINK"), _T("UsingLinker"), nTemp );

	hIniFile.SetValue( _T("Linker"), _T("Ip0"), strLinkerIp_[0] );
	hIniFile.SetValue( _T("Linker"), _T("Ip1"), strLinkerIp_[1] );
	hIniFile.SetValue( _T("Linker"), _T("Ip2"), strLinkerIp_[2] );
	hIniFile.SetValue( _T("Linker"), _T("Ip3"), strLinkerIp_[3] );
	hIniFile.SetValue( _T("Linker"), _T("PNo"), strLinkerPortNo_ );

	hIniFile.SetValue( _T("Service"), _T("Ip0"), strServiceIp_[0] );
	hIniFile.SetValue( _T("Service"), _T("Ip1"), strServiceIp_[1] );
	hIniFile.SetValue( _T("Service"), _T("Ip2"), strServiceIp_[2] );
	hIniFile.SetValue( _T("Service"), _T("Ip3"), strServiceIp_[3] );
	hIniFile.SetValue( _T("Service"), _T("PNo"), strServicePortNo_ );

	hIniFile.Close();
}

void CMaxxLinkServiceDlg::OnBnClickedButtonClose()
{
	save_maxxlink_config();
	CDialog::OnOK();
}

void CMaxxLinkServiceDlg::OnBnClickedButtonConnect()
{
	char szTemp[64];

	//////////////////////////////////////////////////////////////////////////
	// 메시지 박스를 띄운다 
	CString strMsg;
	strMsg.Format( _T("Please wait. trying to connect...") );
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

	//////////////////////////////////////////////////////////////////////////
	// 현재 설정을 저장 
	save_maxxlink_config();

// 	_stprintf_s( pa::PPAStatus->GetThreadState()->szMaxxLink_LinkerIPAddr, 
// 		64, _T("%s.%s.%s.%s"),
// 		strLinkerIp_[0], strLinkerIp_[1], strLinkerIp_[2], strLinkerIp_[3] );
// 	hcutil::CSTRING_TO_ASCII( strLinkerPortNo_, szTemp, 64 );
// 
// 	_stprintf_s( pa::PPAStatus->GetThreadState()->szMaxxLink_ServiceIPAddr, 
// 		64, _T("%s.%s.%s.%s"),
// 		strServiceIp_[0], strServiceIp_[1], strServiceIp_[2], strServiceIp_[3] );
// 	hcutil::CSTRING_TO_ASCII( strServicePortNo_, szTemp, 64 );

	Sleep( 1000 );

	//////////////////////////////////////////////////////////////////////////
	// 연결 확인 
	DWORD dwTime = GetTickCount();
	while( pa::PPAStatus->GetThreadState()->bIsClientConnected_ == FALSE )
	{
		if( GetTickCount()-dwTime > 20*1000 )
		{
			// 접속 에러 
			break;
		}
	}

	if( pa::PPAStatus->GetThreadState()->bIsClientConnected_ == FALSE )
	{
		strMsg.Format( _T("service is not connected !!!\n(check ip-address and port number)") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_WARM, strMsg );
		CMsgDlgThread::GetInstance()->Wait();
	}
	else 
	{
		CMsgDlgThread::GetInstance()->Hide();
		CDialog::OnOK();
	}
	//////////////////////////////////////////////////////////////////////////
}

#include "NumericInputDlg.h"

int  CMaxxLinkServiceDlg::input_data( UINT nButtonID, CString& strData )
{
	CNumericInputDlg dlg;

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( (int)_ttoi(strData) );
	dlg.SetProperty( 0 );

	if( dlg.DoModal() == IDOK )
	{
		int nVal = (int)_ttoi( (LPCTSTR)dlg.GetNumber() );
		CString strVal;
		strVal.Format( _T("%d"), nVal );
		((CButton*)GetDlgItem(nButtonID))->SetWindowText( strVal );
		strData = strVal;
	}

	return 0;
}

void CMaxxLinkServiceDlg::OnBnClickedButtonLinkerIp1()
{
	int nVal = input_data( IDC_BUTTON_LINKER_IP1, strLinkerIp_[0] );
}

void CMaxxLinkServiceDlg::OnBnClickedButtonLinkerIp2()
{
	int nVal = input_data( IDC_BUTTON_LINKER_IP2, strLinkerIp_[1] );
}

void CMaxxLinkServiceDlg::OnBnClickedButtonLinkerIp3()
{
	int nVal = input_data( IDC_BUTTON_LINKER_IP3, strLinkerIp_[2] );
}

void CMaxxLinkServiceDlg::OnBnClickedButtonLinkerIp4()
{
	int nVal = input_data( IDC_BUTTON_LINKER_IP4, strLinkerIp_[3] );
}

void CMaxxLinkServiceDlg::OnBnClickedButtonLinkerPortno()
{
	int nVal = input_data( IDC_BUTTON_LINKER_PORTNO, strLinkerPortNo_ );
}

void CMaxxLinkServiceDlg::OnBnClickedButtonServiceIp1()
{
	int nVal = input_data( IDC_BUTTON_SERVICE_IP1, strServiceIp_[0] );
}

void CMaxxLinkServiceDlg::OnBnClickedButtonServiceIp2()
{
	int nVal = input_data( IDC_BUTTON_SERVICE_IP2, strServiceIp_[1] );
}

void CMaxxLinkServiceDlg::OnBnClickedButtonServiceIp3()
{
	int nVal = input_data( IDC_BUTTON_SERVICE_IP3, strServiceIp_[2] );
}

void CMaxxLinkServiceDlg::OnBnClickedButtonServiceIp4()
{
	int nVal = input_data( IDC_BUTTON_SERVICE_IP4, strServiceIp_[3] );
}

void CMaxxLinkServiceDlg::OnBnClickedButtonServicePortno()
{
	int nVal = input_data( IDC_BUTTON_SERVICE_PORTNO, strServicePortNo_ );
}

void CMaxxLinkServiceDlg::OnBnClickedCheckUseLinker()
{
	int nChecked = ((CButton*)GetDlgItem(IDC_CHECK_USE_LINKER))->GetCheck();
	bUsingLinker_ = ( nChecked == 0 ) ? FALSE : TRUE;
}
