// SetupSystemChangeIPDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupSystemChangeIPDlg.h"

//////////////////////////////////////////////////////////////////////////
// CSetupSystemChangeIPDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSetupSystemChangeIPDlg, CDialog)

CSetupSystemChangeIPDlg::CSetupSystemChangeIPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupSystemChangeIPDlg::IDD, pParent)
{

}

CSetupSystemChangeIPDlg::~CSetupSystemChangeIPDlg()
{
}

void CSetupSystemChangeIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupSystemChangeIPDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetupSystemChangeIPDlg::OnBnClickedButtonClose)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_MC_1, &CSetupSystemChangeIPDlg::OnBnClickedButtonMc1)
	ON_BN_CLICKED(IDC_BUTTON_MC_2, &CSetupSystemChangeIPDlg::OnBnClickedButtonMc2)
	ON_BN_CLICKED(IDC_BUTTON_MC_3, &CSetupSystemChangeIPDlg::OnBnClickedButtonMc3)
	ON_BN_CLICKED(IDC_BUTTON_MC_4, &CSetupSystemChangeIPDlg::OnBnClickedButtonMc4)
	ON_BN_CLICKED(IDC_BUTTON_IO_1, &CSetupSystemChangeIPDlg::OnBnClickedButtonIo1)
	ON_BN_CLICKED(IDC_BUTTON_IO_2, &CSetupSystemChangeIPDlg::OnBnClickedButtonIo2)
	ON_BN_CLICKED(IDC_BUTTON_IO_3, &CSetupSystemChangeIPDlg::OnBnClickedButtonIo3)
	ON_BN_CLICKED(IDC_BUTTON_IO_4, &CSetupSystemChangeIPDlg::OnBnClickedButtonIo4)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CSetupSystemChangeIPDlg::OnBnClickedButtonDownload)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_IP_PA, &CSetupSystemChangeIPDlg::OnBnClickedButtonChangeIpPa)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_IP_IO, &CSetupSystemChangeIPDlg::OnBnClickedButtonChangeIpIo)
// 	ON_BN_CLICKED(IDC_BUTTON_MAXXLINK_LISTEN_PORT_NO, &CSetupSystemChangeIPDlg::OnBnClickedButtonMaxxlinkListenPortNo)
// 	ON_BN_CLICKED(IDC_BUTTON_MAXXLINK_FILERECEVIER_PORT_NO, &CSetupSystemChangeIPDlg::OnBnClickedButtonMaxxlinkFilerecevierPortNo)
// 	ON_BN_CLICKED(IDC_BUTTON_REMOTEPNC_LISTEN_PORT_NO, &CSetupSystemChangeIPDlg::OnBnClickedButtonRemotepncListenPortNo)
// 	ON_BN_CLICKED(IDC_BUTTON_REMOTEPNC_FILERECEIVER_PORT_NO, &CSetupSystemChangeIPDlg::OnBnClickedButtonRemotepncFilereceiverPortNo)
// 	ON_BN_CLICKED(IDC_BUTTON_CHANGE_PORT_NO, &CSetupSystemChangeIPDlg::OnBnClickedButtonChangePortNo)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupSystemChangeIPDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupSystemChangeIPDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSetupSystemChangeIPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	fntStatic_.CreateFont( 
		15, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); 

	fntButton_.CreateFont( 
		17, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); 

	fntButton2_.CreateFont( 
		17, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); 

	SetFont( &fntStatic_, TRUE );

	((CStatic*)GetDlgItem(IDC_STATIC_MC_IP))->SetFont( &fntStatic_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_IO_IP))->SetFont( &fntStatic_, TRUE );

	((CButton*)GetDlgItem(IDC_BUTTON_MC_1))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_MC_2))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_MC_3))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_MC_4))->SetFont( &fntButton_, TRUE );

	((CButton*)GetDlgItem(IDC_BUTTON_IO_1))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_IO_2))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_IO_3))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_IO_4))->SetFont( &fntButton_, TRUE );

	((CButton*)GetDlgItem(IDC_BUTTON_DOWNLOAD))->SetFont( &fntButton2_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->SetFont( &fntButton2_, TRUE );

// 	((CButton*)GetDlgItem(IDC_BUTTON_MAXXLINK_LISTEN_PORT_NO))->SetFont( &fntButton_, TRUE );
// 	((CButton*)GetDlgItem(IDC_BUTTON_MAXXLINK_FILERECEVIER_PORT_NO))->SetFont( &fntButton_, TRUE );
// 	((CButton*)GetDlgItem(IDC_BUTTON_REMOTEPNC_LISTEN_PORT_NO))->SetFont( &fntButton_, TRUE );
// 	((CButton*)GetDlgItem(IDC_BUTTON_REMOTEPNC_FILERECEIVER_PORT_NO))->SetFont( &fntButton_, TRUE );

// 	((CButton*)GetDlgItem(IDC_BUTTON_CHANGE_PORT_NO))->SetFont( &fntButton_, TRUE );
// 	((CButton*)GetDlgItem(IDC_BUTTON_CHANGE_PC_IP))->SetFont( &fntButton_, TRUE );

	init_control();

	//////////////////////////////////////////////////////////////////////////
	// Center Window 
	CenterWindow();
	//////////////////////////////////////////////////////////////////////////

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupSystemChangeIPDlg::OnDestroy()
{
	fntStatic_.DeleteObject();
	fntButton_.DeleteObject();
	fntButton2_.DeleteObject();

	CDialog::OnDestroy();
}

void CSetupSystemChangeIPDlg::OnBnClickedButtonClose()
{
	CDialog::OnOK();
}

void CSetupSystemChangeIPDlg::init_control()
{
	CString strPA_IP;
	CString strIO_IP;
	CString strPA[4];
	CString strIO[4];
	int	i;

	strPA_IP.Format( _T("%s"), pa::PPAStatus->GetThreadState()->szPA_IP_ARRD );
	for( i = 0; i<3; i++ ) {
		strPA[i] = strPA_IP.Left( strPA_IP.Find( _T(".") ) );
		strPA_IP = strPA_IP.Right( strPA_IP.GetLength() - strPA_IP.Find( _T(".") ) - 1 );
	}
	strPA[i] = strPA_IP;

	strIO_IP.Format( _T("%s"), pa::PPAStatus->GetThreadState()->szCANTOPS_IP_ADDR );
	for( i = 0; i<3; i++ ) {
		strIO[i] = strIO_IP.Left( strIO_IP.Find( _T(".") ) );
		strIO_IP = strIO_IP.Right( strIO_IP.GetLength() - strIO_IP.Find( _T(".") ) - 1 );
	}
	strIO[i] = strIO_IP;

	strPA_IP.Format( _T("%s"), pa::PPAStatus->GetThreadState()->szPA_IP_ARRD );
	strIO_IP.Format( _T("%s"), pa::PPAStatus->GetThreadState()->szCANTOPS_IP_ADDR );
	((CStatic*)GetDlgItem(IDC_STATIC_MC_IP))->SetWindowText( strPA_IP );
	((CStatic*)GetDlgItem(IDC_STATIC_IO_IP))->SetWindowText( strIO_IP );

	((CButton*)GetDlgItem(IDC_BUTTON_MC_1))->SetWindowText( strPA[0] );
	((CButton*)GetDlgItem(IDC_BUTTON_MC_2))->SetWindowText( strPA[1] );
	((CButton*)GetDlgItem(IDC_BUTTON_MC_3))->SetWindowText( strPA[2] );
	((CButton*)GetDlgItem(IDC_BUTTON_MC_4))->SetWindowText( strPA[3] );

	((CButton*)GetDlgItem(IDC_BUTTON_IO_1))->SetWindowText( strIO[0] );
	((CButton*)GetDlgItem(IDC_BUTTON_IO_2))->SetWindowText( strIO[1] );
	((CButton*)GetDlgItem(IDC_BUTTON_IO_3))->SetWindowText( strIO[2] );
	((CButton*)GetDlgItem(IDC_BUTTON_IO_4))->SetWindowText( strIO[3] );

	//////////////////////////////////////////////////////////////////////////
	// port number 
	//////////////////////////////////////////////////////////////////////////
	// maxxlink_config.ini 파일에서 값을 읽어서 컨트롤을 초기화 한다 
	CCEIniFile hIniFile;
	int		nMaxxLink_ListenPortNo;
	int		nMaxxLink_FileReceiver_PortNo;
	int		nRemotePNC_ListenPortNo;
	int		nRemotePNC_FileReceiver_PortNo;

	hIniFile.Open( INI_MAXXLINK_CONFIG_PATH );
	hIniFile.GetValue( _T("EPNC_MAXX_LISTEN"), _T("PNo"), (int*)&nMaxxLink_ListenPortNo );
	hIniFile.GetValue( _T("EPNC_FILE_RECEIVER"), _T("PNo"), (int*)&nMaxxLink_FileReceiver_PortNo );
	hIniFile.GetValue( _T("EPNC_REMOTE_PNC"), _T("ListenPNo"), (int*)&nRemotePNC_ListenPortNo );
	hIniFile.GetValue( _T("EPNC_REMOTE_PNC"), _T("FileReceiverPNo"), (int*)&nRemotePNC_FileReceiver_PortNo );
	hIniFile.Close();

	CString strTemp;
// 	strTemp.Format( _T("%d"), nMaxxLink_ListenPortNo );
// 	((CButton*)GetDlgItem(IDC_BUTTON_MAXXLINK_LISTEN_PORT_NO))->SetWindowText( strTemp );
// 	strTemp.Format( _T("%d"), nMaxxLink_FileReceiver_PortNo );
// 	((CButton*)GetDlgItem(IDC_BUTTON_MAXXLINK_FILERECEVIER_PORT_NO))->SetWindowText( strTemp );
// 	strTemp.Format( _T("%d"), nRemotePNC_ListenPortNo );
// 	((CButton*)GetDlgItem(IDC_BUTTON_REMOTEPNC_LISTEN_PORT_NO))->SetWindowText( strTemp );
// 	strTemp.Format( _T("%d"), nRemotePNC_FileReceiver_PortNo );
// 	((CButton*)GetDlgItem(IDC_BUTTON_REMOTEPNC_FILERECEIVER_PORT_NO))->SetWindowText( strTemp );
}

void CSetupSystemChangeIPDlg::OnBnClickedButtonMc1() { get_num( IDC_BUTTON_MC_1, TRUE ); }
void CSetupSystemChangeIPDlg::OnBnClickedButtonMc2() { get_num( IDC_BUTTON_MC_2, TRUE ); }
void CSetupSystemChangeIPDlg::OnBnClickedButtonMc3() { get_num( IDC_BUTTON_MC_3, TRUE ); }
void CSetupSystemChangeIPDlg::OnBnClickedButtonMc4() { get_num( IDC_BUTTON_MC_4, TRUE ); }

void CSetupSystemChangeIPDlg::OnBnClickedButtonIo1() { get_num( IDC_BUTTON_IO_1, TRUE ); }
void CSetupSystemChangeIPDlg::OnBnClickedButtonIo2() { get_num( IDC_BUTTON_IO_2, TRUE ); }
void CSetupSystemChangeIPDlg::OnBnClickedButtonIo3() { get_num( IDC_BUTTON_IO_3, TRUE ); }
void CSetupSystemChangeIPDlg::OnBnClickedButtonIo4() { get_num( IDC_BUTTON_IO_4, TRUE ); }

// void CSetupSystemChangeIPDlg::OnBnClickedButtonMaxxlinkListenPortNo() {}		// { get_num( IDC_BUTTON_MAXXLINK_LISTEN_PORT_NO, FALSE ); }
// void CSetupSystemChangeIPDlg::OnBnClickedButtonMaxxlinkFilerecevierPortNo() {}	// { get_num( IDC_BUTTON_MAXXLINK_FILERECEVIER_PORT_NO, FALSE ); }
// void CSetupSystemChangeIPDlg::OnBnClickedButtonRemotepncListenPortNo() {}		// { get_num( IDC_BUTTON_REMOTEPNC_LISTEN_PORT_NO, FALSE ); }
// void CSetupSystemChangeIPDlg::OnBnClickedButtonRemotepncFilereceiverPortNo() {}	// { get_num( IDC_BUTTON_REMOTEPNC_FILERECEIVER_PORT_NO, FALSE ); }

#include "NumericInputDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"
void CSetupSystemChangeIPDlg::get_num( UINT nButtonID, BOOL isIP )
{
	CNumericInputDlg dlg;
	CString strTemp;
	
	((CButton*)GetDlgItem(nButtonID))->GetWindowText( strTemp );

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( (int)_ttoi((LPCTSTR)strTemp) );
	
	if( dlg.DoModal() == IDOK )
	{
		int nVal = (int)_ttoi( (LPCTSTR)dlg.GetNumber() );

		if( (nVal >= 0 && nVal <= 255) || (isIP == FALSE) ) 
		{
			strTemp.Format( _T("%d"), nVal );
			((CButton*)GetDlgItem(nButtonID))->SetWindowText( strTemp );
		}
		else 
		{
			// 데이터 범위 에러 
			strTemp.Format( _T("enter data in the range of 0 to 255") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strTemp );
			CMsgDlgThread::GetInstance()->Wait();
		}
	}
}

void CSetupSystemChangeIPDlg::OnBnClickedButtonDownload()
{
// 	CString strMsg;
// 	CMsgDlg::EN_RET ret;
// 
// 	strMsg.Format( _T("Do you want to change the IP address of the controller ?") );
// 	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
// 	ret = CMsgDlgThread::GetInstance()->Wait();
// 
// 	if( ret == CMsgDlg::RET_OK )
// 	{
// 		// 새로 입력된 ip 주소를 저장한다 
// 		CString strPA[4], strIO[4];
// 		((CButton*)GetDlgItem(IDC_BUTTON_MC_1))->GetWindowText( strPA[0] );
// 		((CButton*)GetDlgItem(IDC_BUTTON_MC_2))->GetWindowText( strPA[1] );
// 		((CButton*)GetDlgItem(IDC_BUTTON_MC_3))->GetWindowText( strPA[2] );
// 		((CButton*)GetDlgItem(IDC_BUTTON_MC_4))->GetWindowText( strPA[3] );
// 		((CButton*)GetDlgItem(IDC_BUTTON_IO_1))->GetWindowText( strIO[0] );
// 		((CButton*)GetDlgItem(IDC_BUTTON_IO_2))->GetWindowText( strIO[1] );
// 		((CButton*)GetDlgItem(IDC_BUTTON_IO_3))->GetWindowText( strIO[2] );
// 		((CButton*)GetDlgItem(IDC_BUTTON_IO_4))->GetWindowText( strIO[3] );
// 		// 
// 		CString strNEW_PA = strPA[0] + CString(_T(".")) + strPA[1] + CString(_T(".")) + strPA[2] + CString(_T(".")) + strPA[3];
// 		CString strNEW_IO = strIO[0] + CString(_T(".")) + strIO[1] + CString(_T(".")) + strIO[2] + CString(_T(".")) + strIO[3];
// 		// 변환 
// 		hcutil::UNICODE_TO_ASCII( (wchar_t*)(LPCTSTR)strNEW_PA, pa::PPAStatus->GetThreadState()->szNEW_PA_IP_ADDR, 32);
// 		hcutil::UNICODE_TO_ASCII( (wchar_t*)(LPCTSTR)strNEW_IO, pa::PPAStatus->GetThreadState()->szNEW_CANTOPS_IP_ADDR, 32);
// 		PPNC_IPC_CLIENT->ChangeIPAddress(2);	// 2는 아무것도 안함 
// 		// 종료 
// 		CDialog::OnOK();
// 	}
}

void CSetupSystemChangeIPDlg::OnBnClickedButtonChangeIpPa()
{
	CString strMsg;
	CMsgDlg::EN_RET ret;

	strMsg.Format( _T("Do you want to change the IP address of the pa controller ?") );
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
	ret = CMsgDlgThread::GetInstance()->Wait();

	if( ret == CMsgDlg::RET_OK )
	{
		strMsg.Format( _T("wait... changing ip...") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// 새로 입력된 ip 주소를 저장한다 
		CString strPA[4];
		((CButton*)GetDlgItem(IDC_BUTTON_MC_1))->GetWindowText( strPA[0] );
		((CButton*)GetDlgItem(IDC_BUTTON_MC_2))->GetWindowText( strPA[1] );
		((CButton*)GetDlgItem(IDC_BUTTON_MC_3))->GetWindowText( strPA[2] );
		((CButton*)GetDlgItem(IDC_BUTTON_MC_4))->GetWindowText( strPA[3] );
		// 
		CString strNEW_PA = strPA[0] + CString(_T(".")) + strPA[1] + CString(_T(".")) + strPA[2] + CString(_T(".")) + strPA[3];
		// 변환 
		hcutil::UNICODE_TO_ASCII( (wchar_t*)(LPCTSTR)strNEW_PA, pa::PPAStatus->GetThreadState()->szNEW_PA_IP_ADDR, 32);

		pa::PPAStatus->SetIpcCommandComplete( FALSE );
		PPNC_IPC_CLIENT->ChangeIPAddress(0);
		Sleep(1000);
		DWORD dwTime = GetTickCount();
		while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) {
			if( ( GetTickCount() - dwTime ) > 60*1000 ) {
				// 에러 
				strMsg.Format( _T("Timeout error") );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_ERROR, strMsg );
				CMsgDlgThread::GetInstance()->Wait();
				return ;
			}
			Sleep( 100 );
		}

		strMsg.Format( _T("ip change complete !") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_INFO, strMsg );
		CMsgDlgThread::GetInstance()->Wait();
		// AfxMessageBox(_T("ip change complete !"), MB_OK|MB_ICONINFORMATION);
		// 종료 
		// CDialog::OnOK();
	}
}

void CSetupSystemChangeIPDlg::OnBnClickedButtonChangeIpIo()
{
	CString strMsg;
	CMsgDlg::EN_RET ret;

	strMsg.Format( _T("Do you want to change the IP address of the i/o board ?") );
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
	ret = CMsgDlgThread::GetInstance()->Wait();

	if( ret == CMsgDlg::RET_OK )
	{
		strMsg.Format( _T("wait... changing ip...") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// 새로 입력된 ip 주소를 저장한다 
		CString strIO[4];
		((CButton*)GetDlgItem(IDC_BUTTON_IO_1))->GetWindowText( strIO[0] );
		((CButton*)GetDlgItem(IDC_BUTTON_IO_2))->GetWindowText( strIO[1] );
		((CButton*)GetDlgItem(IDC_BUTTON_IO_3))->GetWindowText( strIO[2] );
		((CButton*)GetDlgItem(IDC_BUTTON_IO_4))->GetWindowText( strIO[3] );
		// 
		CString strNEW_IO = strIO[0] + CString(_T(".")) + strIO[1] + CString(_T(".")) + strIO[2] + CString(_T(".")) + strIO[3];
		// 변환 
		hcutil::UNICODE_TO_ASCII( (wchar_t*)(LPCTSTR)strNEW_IO, pa::PPAStatus->GetThreadState()->szNEW_CANTOPS_IP_ADDR, 32);

		pa::PPAStatus->SetIpcCommandComplete( FALSE );
		PPNC_IPC_CLIENT->ChangeIPAddress(1);
		Sleep(1000);
		DWORD dwTime = GetTickCount();
		while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) {
			if( ( GetTickCount() - dwTime ) > 60*1000 ) {
				// 에러 
				strMsg.Format( _T("Timeout error") );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
				CMsgDlgThread::GetInstance()->Wait();
				return ;
			}
			Sleep( 100 );
		}

		strMsg.Format( _T("ip change complete !") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_INFO, strMsg );
		CMsgDlgThread::GetInstance()->Wait();
		// 종료 
		// CDialog::OnOK();
	}
}

// maxxlini_config.ini 파일에 저장 
// void CSetupSystemChangeIPDlg::OnBnClickedButtonChangePortNo()
// {
// 	//////////////////////////////////////////////////////////////////////////
// 	// port number 
// 	//////////////////////////////////////////////////////////////////////////
// 	// maxxlink_config.ini 파일에서 값을 읽어서 컨트롤을 초기화 한다 
// 	CCEIniFile hIniFile;
// 	int		nMaxxLink_ListenPortNo;
// 	int		nMaxxLink_FileReceiver_PortNo;
// 	int		nRemotePNC_ListenPortNo;
// 	int		nRemotePNC_FileReceiver_PortNo;
// 	CString strTemp;
// 
// 	((CButton*)GetDlgItem(IDC_BUTTON_MAXXLINK_LISTEN_PORT_NO))->GetWindowText( strTemp );
// 	nMaxxLink_ListenPortNo = _ttoi( (LPCTSTR)strTemp );
// 	((CButton*)GetDlgItem(IDC_BUTTON_MAXXLINK_FILERECEVIER_PORT_NO))->GetWindowText( strTemp );
// 	nMaxxLink_FileReceiver_PortNo = _ttoi( (LPCTSTR)strTemp );
// 	((CButton*)GetDlgItem(IDC_BUTTON_REMOTEPNC_LISTEN_PORT_NO))->GetWindowText( strTemp );
// 	nRemotePNC_ListenPortNo = _ttoi( (LPCTSTR)strTemp );
// 	((CButton*)GetDlgItem(IDC_BUTTON_REMOTEPNC_FILERECEIVER_PORT_NO))->GetWindowText( strTemp );
// 	nRemotePNC_FileReceiver_PortNo = _ttoi( (LPCTSTR)strTemp );
// 
// 	hIniFile.Open( INI_MAXXLINK_CONFIG_PATH );
// 	hIniFile.SetValue( _T("EPNC_MAXX_LISTEN"), _T("PNo"), (int)nMaxxLink_ListenPortNo );
// 	hIniFile.SetValue( _T("EPNC_FILE_RECEIVER"), _T("PNo"), (int)nMaxxLink_FileReceiver_PortNo );
// 	hIniFile.SetValue( _T("EPNC_REMOTE_PNC"), _T("ListenPNo"), (int)nRemotePNC_ListenPortNo );
// 	hIniFile.SetValue( _T("EPNC_REMOTE_PNC"), _T("FileReceiverPNo"), (int)nRemotePNC_FileReceiver_PortNo );
// 	hIniFile.Close();
// }

