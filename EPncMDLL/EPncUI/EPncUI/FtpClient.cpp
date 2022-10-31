#include "StdAfx.h"
#include "FtpClient.h"


pa::CFtpClient::CFtpClient(void)
{
	m_hIntOpen = NULL;
	m_hIntFTP = NULL;
}

pa::CFtpClient::~CFtpClient(void)
{
}

// DWORD pa::CFtpClient::Connect( const char* strFtpUrl, const char* strFtpUser, const char* strFtpPass )
DWORD pa::CFtpClient::Connect( LPCTSTR strFtpUrl, LPCTSTR strFtpUser, LPCTSTR strFtpPass )
{
	DWORD	dwErrCode = 0;

	m_hIntOpen = InternetOpen( _T("Rnd_Ftp_Client"), INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,NULL );

	if(m_hIntOpen == NULL)
	{
		dwErrCode = GetLastError();
		return dwErrCode;
	}

	m_hIntFTP = InternetConnect(m_hIntOpen,
								strFtpUrl,
								INTERNET_DEFAULT_FTP_PORT,
								strFtpUser,
								strFtpPass,
								INTERNET_SERVICE_FTP,
								INTERNET_FLAG_PASSIVE |
								INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
								INTERNET_FLAG_SECURE,
								NULL);

	if(m_hIntFTP == NULL)
	{
		dwErrCode = GetLastError();

		InternetCloseHandle(m_hIntOpen);
		
		m_hIntOpen = NULL;
	}

	return dwErrCode;
}

void pa::CFtpClient::Disconnect()
{
	InternetCloseHandle( m_hIntFTP );
	m_hIntFTP = NULL;

	InternetCloseHandle( m_hIntOpen );
	m_hIntOpen = NULL;
}

BOOL pa::CFtpClient::SetFolder(LPCTSTR pFTPFolder)
{
	BOOL bRet = FALSE;

	if( m_hIntFTP != NULL ) {
		bRet = FtpSetCurrentDirectory(m_hIntFTP, pFTPFolder);
	}

	return bRet;
}

void pa::CFtpClient::GetFolder(LPCTSTR pFTPFolder, DWORD dwSize)
{
	if( m_hIntFTP != NULL ) {
//		FtpGetCurrentDirectory(m_hIntFTP, pFTPFolder, &dwSize);
		FtpGetCurrentDirectory(m_hIntFTP, (LPWSTR)pFTPFolder, &dwSize);
	}
}

BOOL pa::CFtpClient::CreateFolder(LPCTSTR pFTPFolder)
{
	BOOL bRet = FALSE;

	if( m_hIntFTP != NULL ) {
		bRet = FtpCreateDirectory(m_hIntFTP, pFTPFolder);
	}

	return bRet;
}

BOOL pa::CFtpClient::DeleteFolder(LPCTSTR pFTPFolder)
{
	BOOL bRet = FALSE;

	if( m_hIntFTP ) {
		bRet = FtpRemoveDirectory(m_hIntFTP, pFTPFolder);
	}

	return bRet;
}

HINTERNET pa::CFtpClient::FindFirst( LPCTSTR pFilename, WIN32_FIND_DATA* pWFD )
{
	HINTERNET hIntFind = NULL;

	if( m_hIntFTP ) 
	{
		hIntFind = FtpFindFirstFile(m_hIntFTP, pFilename, pWFD, INTERNET_FLAG_NO_CACHE_WRITE,NULL);
	}

	return hIntFind;
}

// 데이터가 없으면 FALSE 리턴 
BOOL pa::CFtpClient::FindNext( HINTERNET* h, WIN32_FIND_DATA* pWFD )
{
	if( h && m_hIntFTP )
	{
		if( InternetFindNextFile( *h ,pWFD ) == FALSE )
		{
			InternetCloseHandle( *h );
			return FALSE;
		}
		else 
		{
			return TRUE;	
		}
	}

	return FALSE;
}

BOOL pa::CFtpClient::SendFile(LPCTSTR strLocal, LPCTSTR strFTP)
{
	BOOL bRet = FALSE;

	if( m_hIntFTP ) 
	{
		bRet = FtpPutFile(
					m_hIntFTP,
					strLocal,
					strFTP,
					INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_DONT_CACHE,
					NULL);
	}

	return bRet;
}

BOOL pa::CFtpClient::ReceiveFile(LPCTSTR pFTP, LPCTSTR pLocal)
{
	BOOL bRet = FALSE;

	if( m_hIntFTP )
	{
		bRet = FtpGetFile(
					m_hIntFTP,
					pFTP,
					pLocal,
					FALSE,
					FILE_ATTRIBUTE_NORMAL,
					FTP_TRANSFER_TYPE_BINARY,
					NULL);
	}
	
	return bRet;
}

BOOL pa::CFtpClient::DeleteFile(LPCTSTR pFTPFile)
{
	BOOL bRet = FALSE;

	if( m_hIntFTP ) {
		bRet = FtpDeleteFile(m_hIntFTP, pFTPFile);
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////

CString pa::CFtpClient::GetErrorMessage( DWORD dwErrCode )
{ 
	CString strRet;

	switch( dwErrCode ) 
	{
	case 12001:
		strRet.Format( _T("Out of handles") );
		break;

	case 12002:
		strRet.Format( _T("Timeout") );
		break;

	case 12004:
		strRet.Format( _T("Internal Error") );
		break;

	case 12005:
		strRet.Format( _T("Invalid URL") );
		break;

	case 12006:
		strRet.Format( _T("Unrecognized Scheme : Please check the URL name in 'xxx'.") );
		break;

	case 12007:
		strRet.Format( _T("Service Name Not Resolved") );
		break;

	case 12008:
		strRet.Format( _T("Protocol Not Found") );
		break;

	case 12013:
		strRet.Format( _T("Incorrect User Name") );
		break;

	case 12014:
		strRet.Format( _T("Incorrect Password") );
		break;

	case 12015:
		strRet.Format( _T("Login Failure") );
		break;

	case 12016:
		strRet.Format( _T("Invalid Operation") );
		break;

	case 12017:
		strRet.Format( _T("Operation Canceled") );
		break;

	case 12020:
		strRet.Format( _T("Not Proxy Request") );
		break;

	case 12023:
		strRet.Format( _T("No Direct Access") );
		break;

	case 12026:
		strRet.Format( _T("Request Pending") );
		break;

	case 12027:
		strRet.Format( _T("Incorrect Format") );
		break;

	case 12028:
		strRet.Format( _T("Item not found") );
		break;

	case 12029:
		strRet.Format( _T("Cannot connect") );
		break;

	case 12030:
		strRet.Format( _T("Connection Aborted") );
		break;

	case 12031:
		strRet.Format( _T("Connection Reset") );
		break;

	case 12033:
		strRet.Format( _T("Invalid Proxy Request") );
		break;

	case 12034:
		strRet.Format( _T("Need UI") );
		break;

	case 12035:
		strRet.Format( _T("Sec Cert Date Invalid") );
		break;

	case 12038:
		strRet.Format( _T("Sec Cert CN Invalid") );
		break;

	case 12044:
		strRet.Format( _T("Client Auth Cert Needed") );
		break;

	case 12045:
		strRet.Format( _T("Invalid CA Cert") );
		break;

	case 12046:
		strRet.Format( _T("Client Auth Not Setup") );
		break;

	case 12150:
		strRet.Format( _T("HTTP Header Not Found") );
		break;

	case 12152:
		strRet.Format( _T("Invalid HTTP Server Response") );
		break;

	case 12153:
		strRet.Format( _T("Invalid HTTP Header") );
		break;

	case 120154:
		strRet.Format( _T("Invalid Query Request") );
		break;

	case 120156:
		strRet.Format( _T("Redirect Failed") );
		break;

	case 120159:
		strRet.Format( _T("TCP/IP not installed") );
		break;

	default:
		strRet.Format( _T("UnKnown") );
		break;
	}

	return strRet;
}

