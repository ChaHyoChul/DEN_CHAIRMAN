#include "StdAfx.h"
#include "RemoteServer.h"

//////////////////////////////////////////////////////////////////////////

CRemoteServer* P_REMOTE_SERVER = NULL;
int CRemoteClient::PACKET_SIZE = 65000;

//////////////////////////////////////////////////////////////////////////

CRemoteServer::CRemoteServer(void)
{
	pClient_ = new CRemoteClient(CRemoteClient::PACKET_SIZE*2);
}

CRemoteServer::~CRemoteServer(void)
{
	if( pClient_ )
	{
		delete pClient_;
		pClient_ = NULL;
	}
}

void CRemoteServer::OnAccept()
{
	if( pClient_ == NULL )
	{
		return ;
	}

	P_LOG->WriteLog(CLog::TYPE_OPER, 0, _T("Maxxlink -> CRemoteServer::OnAccept()"));

	if( pClient_->GetConnectState() != hcsock::ISocket::NOT_CONNECT )
	{
		pClient_->Close();
		P_LOG->WriteLog(CLog::TYPE_OPER, 0, _T("Maxxlink -> pClient->Close()"));
	}

	if( Accept( *pClient_ ) == FALSE )
	{
		// 접속 에러 
		pClient_->Close();
		P_LOG->WriteLog(CLog::TYPE_OPER, 0, _T("Maxxlink -> Accept Fail"));
	}
	TRACE( _T("RemoteServer::OnAccept() -- OK !!!\n") );
	P_LOG->WriteLog(CLog::TYPE_OPER, 0, _T("Maxxlink -> Accept OK"));

	//////////////////////////////////////////////////////////////////////////
	// client socket 의 옵션을 변경한다 
	int nOptVal = 65535*2;			// send 버퍼의 크기 
	int nOptLen = sizeof(int);
	setsockopt( pClient_->GetSocketHandle(), SOL_SOCKET, SO_SNDBUF, (char *)&nOptVal, nOptLen );
	nOptVal = 0;
	nOptLen = 0;
	getsockopt( pClient_->GetSocketHandle(), SOL_SOCKET, SO_SNDBUF, (char *)&nOptVal, &nOptLen );
	//////////////////////////////////////////////////////////////////////////

	// 이전 이미지를 초기화 한다 
	P_SCRN_CAPTURE->Reset();
}

BOOL CRemoteServer::IsConnectedClient()
{
	BOOL ret = FALSE;

	if( pClient_ )
	{
		switch( pClient_->GetConnectState() )
		{
		case ISocket::CONNECTED:
			ret = TRUE;
			break;
		}
	}

	return ret;
}

void CRemoteServer::DisconnectClient()
{
	if( pClient_ != NULL ) 
	{
		pClient_->Close();
		delete pClient_;
		pClient_ = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CRemoteClient::CRemoteClient( int buffer_size ) : hcsock::CClientSocket( buffer_size )
{
	InitializeCriticalSection( &cs_ );
	pRecvBuffer_ = new BYTE [buffer_size * 2];	// 데이터를 이동해야 하기 때문에, 기본 크기의 2배로 잡는다 
	memset( (void*)pRecvBuffer_, 0, sizeof(BYTE)*(buffer_size*2) );
	nRecvBufferIndex_ = 0;

	nImagePacketNo_ = 0;	// 초기값 0
	nImageDataIndex_= 0;	// 초기값 0
}

CRemoteClient::~CRemoteClient()
{
	DeleteCriticalSection( &cs_ );
	if( pRecvBuffer_ != NULL )
	{
		delete [] pRecvBuffer_;
		pRecvBuffer_ = NULL;
	}
}

// Client에서 보내는 명령 코드
//  - format | Code(4byte) | Len(4byte) | data(n-byte) |
//	- Code 
//		> 0 : None 
//		> 1 : 화면 전송 요청
//		> 2 : 
void CRemoteClient::OnRecv( const char* p, int len )
{
	static TCHAR	FILENAME[256];
	static TCHAR	PNC_ID[64];
	static int		CODE;
	static int		LEN;

	if( p == NULL || len == 0 ) 
	{
		return ;
	}

	// recv 버퍼에 복사 
	memcpy((void*)(pRecvBuffer_+nRecvBufferIndex_), p, len);
	nRecvBufferIndex_ += len;

	while( nRecvBufferIndex_ >= 8 )								// CODE + LEN => 8byte
	{
		memcpy((void*)&CODE, pRecvBuffer_+0, 4 );		// CODE
		memcpy((void*)&LEN, pRecvBuffer_+4, 4 );			// LEN

		// 데이터를 다 받았는지 확인. 다 못 받났으면 다시 받는다
		if( nRecvBufferIndex_ < LEN ) 
		{
			break;
		}

		// 받은 명령을 처리 한다 
		switch( CODE )
		{
		case 1:
			// 화면 전송. 화면 전송은 64k 단위로 전송하고, 크기가 넘을 경우 나눠서 전송 한다  
			// EPNC 프로그램에서만 사용
//			send_screen();
			break;

		case 2:
			// 마우스 클릭.
			{
				int isdown, x, y;
				memcpy((void*)&isdown, (void*)(pRecvBuffer_+8), 4);
				memcpy((void*)&x, (void*)(pRecvBuffer_+12), 4);
				memcpy((void*)&y, (void*)(pRecvBuffer_+16), 4);
				mouse_click( isdown, x, y );
			}
			break;

		case 3:
			// 파일 등록 
			{
				memset((void*)FILENAME, 0, sizeof(TCHAR)*256);
				memcpy((void*)FILENAME, pRecvBuffer_+8, 256-8);
				CString strFilePath, strFileName, strTemp;
				strFileName.Format( _T("%s"), FILENAME );
				TRACE( strFileName ); TRACE( _T("\n") );
				strFilePath.Format( _T("%s"), FILENAME );
				pa::GET_NCFILE_FULL_PATH( strFilePath );

				if( hcutil::IsExistFile( strFilePath ) )
				{
					CTime	tm = CTime::GetCurrentTime();
					CString strDateTime = tm.Format( _T("%y%m%d_%H%M%S") );
					CString	strID;
					DWORD	dwFileSize = 0;

					strID.Format( _T("%s_001"), strDateTime );

					hcutil::GetFileSize( strFilePath, &dwFileSize, strTemp );

					pa::PNCFileMgr->AddWorkNCFileInfo( (TCHAR*)(LPCTSTR)strID, (TCHAR*)(LPCTSTR)strFileName, dwFileSize );

					pa::PPAStatus->GetThreadState()->bUpdateNcFileList_ = TRUE;					// NC-File 리스트를 화면 업데이트 한다
				}
				else 
				{

				}
			}
			break;

		case 4:
			// PNC-ID 등록  
			{
				memset((void*)PNC_ID, 0, sizeof(TCHAR)*64);
				memcpy((void*)PNC_ID, pRecvBuffer_+8, sizeof(TCHAR)*64);		// PNC-ID는 64+8=72 byte를 받는다 
				pa::PPAStatus->GetThreadState()->bIsReceivedPNCID_ = TRUE;
				memcpy( (void*)pa::PPAStatus->GetThreadState()->szPNCID_, (const void*)PNC_ID, sizeof(TCHAR)*64 );
			}
			break;

		case 5:
			// 선택된 로그파일 이름 전송 
			send_selected_logfiles();
			break;

		case 6:
			// EPNC의 각 프로그램 버전 전송 
			send_sw_version();
			break;

		case 7:
			// PC의 해상도 전송
			send_sw_resolution();
			break;

		case 8:
			send_screen();
			break;
		}

		// Recv 버퍼와 인덱스 변수를 정리 한다 
		nRecvBufferIndex_ -= LEN;
		memcpy((void*)(pRecvBuffer_), (void*)(pRecvBuffer_+LEN), nRecvBufferIndex_);
	}
}

//////////////////////////////////////////////////////////////////////////
// 화면을 capture 해서 전송한다 
//	- 만약 화면 데이터의 크기가 64k를 넘을 경우, 64k 까지 먼전 전달하고, 나머지는 다음에 전송 한다
//	- |패킷번호|데이터길이|데이터|
void CRemoteClient::send_screen()
{
	EnterCriticalSection( &cs_ );

	int nSendData = 0;

	// 이미지 패킷 번호가 0이면, 화면을 capture 해서. 압축 한다 
	if( nImagePacketNo_ == 0 )
	{
		P_SCRN_CAPTURE->Capture();
		nImageDataIndex_ = 0;
	}
		
	// 패킷 번호를 갱신 한다 
	// - 전체 보낼 데이터를 65000으로 나눈 몫을 사용 
	int nTemp = P_SCRN_CAPTURE->GetSendImageLen() - nImageDataIndex_;
	nImagePacketNo_ = (int)( nTemp / PACKET_SIZE );	// 65000byte씩 잘라서 보낸다 
	nSendData		= (nImagePacketNo_ != 0) ? (PACKET_SIZE) : (P_SCRN_CAPTURE->GetSendImageLen() - nImageDataIndex_);

	send_image_packet( nImagePacketNo_, nSendData, P_SCRN_CAPTURE->GetSendImageBuffer()+nImageDataIndex_ );

	nImageDataIndex_ += nSendData;

	LeaveCriticalSection( &cs_ );
}

void CRemoteClient::send_image_packet( int packet_no, int len, char* p )
{
	int code = 8;
	int nTempLen = len + (sizeof(int) * 3);

	Send( (const char*)&code, sizeof(int) );
	Send( (const char*)&packet_no, sizeof(int) );
	Send( (const char*)&nTempLen, sizeof(int) );
	Send( (const char*)p, len );
}

void CRemoteClient::mouse_click( int isdown, int x, int y)
{
	EnterCriticalSection( &cs_ );

// #ifdef _DEBUG 
// 	CString str;
// 	str.Format( _T("Mouse Click : %d, %d, %d\n"), isdown, x, y);
// 	TRACE( str );
// #endif 

	INPUT Lclick;
	ZeroMemory( &Lclick, sizeof(INPUT) );

	Lclick.type			= INPUT_MOUSE;
	if( isdown == 0 ) {
		Lclick.mi.dwFlags	= MOUSEEVENTF_LEFTDOWN;
	} else {
		Lclick.mi.dwFlags	= MOUSEEVENTF_LEFTUP;
	}

	SetCursorPos( x, y );

	::SendInput( 1, &Lclick, sizeof(INPUT) );

	LeaveCriticalSection( &cs_ );
}

// 선택된 로그파일 이름을 보낸다 
// 로그파일;로그파일;로그파일; ... 
// EPncUI 프로그램에서 선택된 로그파일을 파일에 저장해 놓는다 ?
void CRemoteClient::send_selected_logfiles()
{
	EnterCriticalSection( &cs_ );
// 	int		code = 5;
// 	int		len = 0;
// 	int		num = 0;
	DWORD	dwTime = GetTickCount();
	BOOL	bNotSelected = FALSE;

	//////////////////////////////////////////////////////////////////////////
	// selected log 파일 정보 저장 
	pa::PPAStatus->GetThreadState()->bSaveSelectedLogFiles_ = TRUE;
	Sleep(300);
	while( pa::PPAStatus->GetThreadState()->bSaveSelectedLogFiles_ != FALSE ) {
		Sleep(100);
		if( GetTickCount() - dwTime > 1500 ) {
			pa::PPAStatus->GetThreadState()->bSaveSelectedLogFiles_ = FALSE;
		//	return ;
			bNotSelected = TRUE;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// selected log 파일 정보 전송 
	// | CODE 번호:5 | 패킷번호:0 | 패킷데이터길이:+12 | 데이터 | 
	// 데이터 = | 파일개수 | 
// 	len = pa::PPAStatus->GetThreadState()->nLenSelectLogFiles_ + 12;
// 	num = pa::PPAStatus->GetThreadState()->nNumSelectLogFiles_;
// 	char* pS = (char*)(pa::PPAStatus->GetThreadState()->szSelectedLogFiles_);
// 	Send( (const char*)&code, sizeof(int) );	// 4 +
// 	Send( (const char*)&len,  sizeof(int) );	// 4 +
// 	Send( (const char*)&num,  sizeof(int) );	// 4 = 12 
// 	Send( (const char*)pS, pa::PPAStatus->GetThreadState()->nLenSelectLogFiles_ );

	if( bNotSelected == FALSE )
	{
		int code = 5;
		int packet_no = 0;
		int packet_size = 12 + pa::PPAStatus->GetThreadState()->nLenSelectLogFiles_;
		int num_filse = pa::PPAStatus->GetThreadState()->nNumSelectLogFiles_;
		packet_size += 4;	// for num_files 
 		char* pS = (char*)(pa::PPAStatus->GetThreadState()->szSelectedLogFiles_);

		Send( (const char*)&code, sizeof(int) );
		Send( (const char*)&packet_no, sizeof(int) );
		Send( (const char*)&packet_size, sizeof(int) );
		
		Send( (const char*)&num_filse, sizeof(int) );
		Send( (const char*)pS, pa::PPAStatus->GetThreadState()->nLenSelectLogFiles_ );
	}
	else 
	{
		int code = 5;
		int packet_no = 0;
		int packet_size = 12; // + pa::PPAStatus->GetThreadState()->nLenSelectLogFiles_;
		int num_filse = 0;
		packet_size += 4;	// for num_files 
		char* pS = (char*)(pa::PPAStatus->GetThreadState()->szSelectedLogFiles_);

		Send( (const char*)&code, sizeof(int) );
		Send( (const char*)&packet_no, sizeof(int) );
		Send( (const char*)&packet_size, sizeof(int) );

		Send( (const char*)&num_filse, sizeof(int) );
	//	Send( (const char*)pS, pa::PPAStatus->GetThreadState()->nLenSelectLogFiles_ );
	}
	LeaveCriticalSection( &cs_ );
}

/*
void maxxlink::CMLClient::doGetSystemInfo()
{
	maxxlink::SSystemInfo hSysteminfo;

	memset( (void*)&hSysteminfo, 0, sizeof(maxxlink::SSystemInfo) );

	hSysteminfo.nModelKind	= 2;				// 0:DM_IO1, 1:DM_IO2, 2:DS
	hSysteminfo.nModelID	= pa::MODEL_INFO.GetModelID(); //theApp.nModelNo;
	hSysteminfo.nNumAxis	= pa::MODEL_INFO.GetNumAxis(); //pa::PConfig->pConfig_->nNumAxis;
// 	memcpy( (void*)&hSysteminfo.szMachineName, (const void*)pa::PConfig->pConfig_->szMachineName, sizeof(TCHAR)*64 );
// 	memcpy( (void*)&hSysteminfo.szModelName, (const void*)pa::PConfig->pConfig_->szModelName, sizeof(TCHAR)*64 );
	memcpy( (void*)&hSysteminfo.szMachineName, (const char*)(LPCTSTR)pa::MODEL_INFO.GetMachineName(), sizeof(TCHAR)*64 );
	memcpy( (void*)&hSysteminfo.szMotionProgVersion, pa::PPAStatus->GetThreadState()->szMotionProgVersion, sizeof(TCHAR)*128 );
	memcpy( (void*)&hSysteminfo.szUIProgVersion, pa::PPAStatus->GetThreadState()->szUIProgVersion, sizeof(TCHAR)*128 );
	memcpy( (void*)&hSysteminfo.szFileReceiverVersion, pa::PPAStatus->GetThreadState()->szFileReceiverVersion, sizeof(TCHAR)*128 );
	memcpy( (void*)&hSysteminfo.szFileReceiver2Version, pa::PPAStatus->GetThreadState()->szFileReceiverVersion2, sizeof(TCHAR)*128 );
// 	_stprintf_s( (TCHAR*)&hSysteminfo.szFileReceiverVersion, 128, _T("%s / %s"), 
// 		pa::PPAStatus->GetThreadState()->szFileReceiverVersion, 
// 		pa::PPAStatus->GetThreadState()->szFileReceiverVersion2 );
	memcpy( (void*)&hSysteminfo.szControllerVersion, pa::PPAStatus->GetThreadState()->szPAControllerVersion, sizeof(TCHAR)*128 );

	respond( maxxlink::SC_GET_SYSTEM_INFO, maxxlink::SE_NONE, (char*)&hSysteminfo, sizeof(maxxlink::SSystemInfo) );
}
*/

void CRemoteClient::send_sw_version()
{
	EnterCriticalSection( &cs_ );

	TCHAR szEPncM[128];
	TCHAR szEPncUI[128];
	TCHAR szFR2[128];

	memcpy((void*)szEPncM, (const char*)pa::PPAStatus->GetThreadState()->szMotionProgVersion, sizeof(TCHAR)*128);
	memcpy((void*)szEPncUI, (const char*)pa::PPAStatus->GetThreadState()->szUIProgVersion, sizeof(TCHAR)*128);
	memcpy((void*)szFR2, (const char*)pa::PPAStatus->GetThreadState()->szFileReceiverVersion2, sizeof(TCHAR)*128);

	int code = 6;
	int packet_no = 0;
	int packet_size = 12 + sizeof(TCHAR)*128*3;

	Send( (const char*)&code, sizeof(int) );
	Send( (const char*)&packet_no, sizeof(int) );
	Send( (const char*)&packet_size, sizeof(int) );
	Send( (const char*)szEPncM, sizeof(TCHAR)*128 );
	Send( (const char*)szEPncUI, sizeof(TCHAR)*128 );
	Send( (const char*)szFR2, sizeof(TCHAR)*128 );

	CString strDbg;
	strDbg.Format(_T("Maxxlink -> Send EPncM Version : %s"), szEPncM);
	P_LOG->WriteLog(CLog::TYPE_OPER, 0, strDbg);
	strDbg.Format(_T("Maxxlink -> Send EPncUI Version : %s"), szEPncUI);
	P_LOG->WriteLog(CLog::TYPE_OPER, 0, strDbg);
	strDbg.Format(_T("Maxxlink -> Send FR2 Version : %s"), szFR2);
	P_LOG->WriteLog(CLog::TYPE_OPER, 0, strDbg);

	LeaveCriticalSection( &cs_ );
}

void CRemoteClient::send_sw_resolution()
{
	EnterCriticalSection( &cs_ );
	CSize		szScreen_;
	szScreen_.cx = GetSystemMetrics( SM_CXSCREEN );
	szScreen_.cy = GetSystemMetrics( SM_CYSCREEN );

	int nScreen_x = szScreen_.cx;
	int nScreen_y = szScreen_.cy;

	int code = 7;
	int packet_no = 0;
	int packet_size = 12;
	packet_size += 8;	// for num_files 


	Send( (const char*)&code, sizeof(int) );
	Send( (const char*)&packet_no, sizeof(int) );
	Send( (const char*)&packet_size, sizeof(int) );
	Send( (const char*)&nScreen_x, sizeof(int) );
	Send( (const char*)&nScreen_y, sizeof(int) );

	LeaveCriticalSection( &cs_ );
}
