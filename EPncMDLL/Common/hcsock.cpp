#include "StdAfx.h"
#include "hcsock.h"


namespace hcsock
{

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

MFCCTRLEXDLL_API CSocketHelper SOCKET_HELPER;								// SocketHelper 클래스 객체를 전역으로 만들어 사용한다 	

//////////////////////////////////////////////////////////////////////////

CSocketHelper::SKeepAliveInfo CSocketHelper::KEEPALIVE_INFO;	// KEEPALIVE_INFO 변수를 선언 한다 

void CSocketHelper::SET_KEEPALIVE_INFO( DWORD dwRecvInterval, DWORD dwSendInterval, char cSignal )
{
	KEEPALIVE_INFO.dwRecvInterval = dwRecvInterval;
	KEEPALIVE_INFO.dwSendInterval = dwSendInterval;
	KEEPALIVE_INFO.cSignal = cSignal;
}

void CSocketHelper::SET_USING_KEEPALIVE( BOOL bUsingSendKeepAlive,  BOOL bUsingRecvKeepAlive )
{
	KEEPALIVE_INFO.bUsingSendKeepAlive = bUsingSendKeepAlive;
	KEEPALIVE_INFO.bUsingRecvKeepAlive = bUsingRecvKeepAlive;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CSocketHelper::CSocketHelper()
{
	hThread_	= NULL;
	tfuncFlag_	= TRUE;

	InitializeCriticalSection( &hCS_ );
	mapSocket_.RemoveAll();

	// Thread 시작 
	unsigned long	uStackSize = 0;
	DWORD			usThreadAddr;
	hThread_ = ::CreateThread( NULL, uStackSize, (LPTHREAD_START_ROUTINE)ThreadProc, (LPVOID)this, CREATE_SUSPENDED, &usThreadAddr );
	if( hThread_ != NULL ) {
		ResumeThread( hThread_ );
		DWORD dw = GetLastError();
		//////////////////////////////////////////////////////////////////////////
		CString strDbg;
		strDbg.Format( _T("CSocketHelper::CSocketHelper() : ResumeThread is %d\n"), dw );
		TRACE(strDbg);
		//////////////////////////////////////////////////////////////////////////
	}
}

CSocketHelper::~CSocketHelper()
{
	// Thread 종료 및 확인 
	tfuncFlag_ = FALSE;
	WaitForSingleObject( hThread_, 5000 );

	// Thread 종료 후 나어지 리소스를 정리 한다 
	mapSocket_.RemoveAll();
	DeleteCriticalSection( &hCS_ );
}

void CSocketHelper::Init()
{

}

void CSocketHelper::AddToMap( ISocket* pSocket )
{
	ASSERT( pSocket );
	EnterCriticalSection( &hCS_ );
	mapSocket_[(WORD)(pSocket->GetSocketHandle())] = pSocket;
	LeaveCriticalSection( &hCS_ );
}

void CSocketHelper::RemoveFromMap( ISocket* pSocket )
{
	ASSERT( pSocket );
	EnterCriticalSection( &hCS_ );
	mapSocket_.RemoveKey((WORD)(pSocket->GetSocketHandle()));
	LeaveCriticalSection( &hCS_ );
}

DWORD CSocketHelper::tfunc_()
{
	fd_set		fdRead;
	fd_set		fdWrite;
	fd_set		fdExcep;
	timeval		tv;

	POSITION	pos;
	WORD		wTemp;
	SOCKET		keySocket;
	ISocket*	valSocket;

	TRACE( _T("START CSocketHelper::tfunc_()\n") ); 

	while( tfuncFlag_ )
	{
		//////////////////////////////////////////////////////////////////////////
		CString strDbg;
		strDbg.Format( _T("CSocketHelper::tfunc_() : mapSocket_ count is %d\n"), mapSocket_.GetCount() );
		TRACE(strDbg);
		//////////////////////////////////////////////////////////////////////////

		if( mapSocket_.GetCount() ) 
		{
			FD_ZERO( &fdRead );
			FD_ZERO( &fdWrite );
			FD_ZERO( &fdExcep );

			// map에 등록된 socket을 fd_set에 등록 한다 
			EnterCriticalSection( &hCS_ );
			for( pos = mapSocket_.GetStartPosition(); pos != NULL; )
			{
				mapSocket_.GetNextAssoc( pos, (WORD&)wTemp, (void*&)valSocket );
				keySocket = (SOCKET)wTemp;
				FD_SET( keySocket, &fdRead );
				// 보낼 데이터가 있거나,
				// Socket의 접속 상태가 PendingConnect이면 추가 한다
				if( valSocket->haveSendData() || valSocket->GetConnectState()==ISocket::PENDING_CONNECT ) {
					FD_SET( keySocket, &fdWrite );
				}
				if( valSocket->GetConnectState()==ISocket::PENDING_CONNECT ) {
					FD_SET( keySocket, &fdExcep );
				}
			}	
			LeaveCriticalSection( &hCS_ );

			// select 함수 호출  
			tv.tv_sec = 0;
			tv.tv_usec= 0;
			int ret = select( 0, &fdRead, &fdWrite, &fdExcep, &tv );
			DWORD dw = GetLastError();

			if( ret == SOCKET_ERROR )
			{
				DWORD dwErrorCode = GetLastError();
				CString strErrorCode;
				strErrorCode.Format( _T("tfunc_::select() error (%d)\n"), dwErrorCode );
				TRACE( strErrorCode );
				continue;
			}
		
			if( ret == 0 ) 
			{
				/*
				// KEEP ALIVE 기능을 구현 한다. 
				// 연결된 client socket에 대해서 일정 시간 데이터 전송이 없으면 접속을 끊는다  
				if( (CSocketHelper::KEEPALIVE_INFO.bUsingRecvKeepAlive == TRUE) && 
					(CSocketHelper::KEEPALIVE_INFO.cSignal != NULL) )
				{
					EnterCriticalSection( &hCS_ );
					DWORD dwCurrTick = GetTickCount();
					for( pos = mapSocket_.GetStartPosition(); pos != NULL; ) 
					{
						DWORD dwTemp;
						mapSocket_.GetNextAssoc( pos, (WORD&)wTemp, (void*&)valSocket );
						if( valSocket->GetConnectState() == ISocket::CONNECTED )
						{
							dwTemp = dwCurrTick - valSocket->getLastRecvTick();
							if( dwTemp > CSocketHelper::KEEPALIVE_INFO.dwRecvInterval ) {
								// N초 이상 데이터 수신이 없으면, 
								// 접속을 끊는다 
								valSocket->disconnectTimeout();
							}
							dwTemp = dwCurrTick - valSocket->getLastSendTick();
							if( dwTemp > CSocketHelper::KEEPALIVE_INFO.dwSendInterval ) {
								// N초 이상 데이터 송신이 없으면,
								// KEEP-ALIVE 문자를 송신 한다
								char strKeepAliveSignal[2] = { 0, 0 };
								strKeepAliveSignal[0] = CSocketHelper::KEEPALIVE_INFO.cSignal;
								((CClientSocket*)valSocket)->Send( (const char*)strKeepAliveSignal );
							}
						}
					}
					LeaveCriticalSection( &hCS_ );
				}
				*/
				EnterCriticalSection( &hCS_ );
				if(CSocketHelper::KEEPALIVE_INFO.cSignal != NULL )
				{
					DWORD	dwCurrTick = GetTickCount();
					char	strKeepAlive[2] = {0, 0};
					strKeepAlive[0] = CSocketHelper::KEEPALIVE_INFO.cSignal;

					for( pos = mapSocket_.GetStartPosition(); pos != NULL; ) 
					{
						DWORD dwTemp;
						mapSocket_.GetNextAssoc( pos, (WORD&)wTemp, (void*&)valSocket );
						
						if( valSocket->GetConnectState() == ISocket::CONNECTED ) 
						{
							if( CSocketHelper::KEEPALIVE_INFO.bUsingSendKeepAlive == TRUE ) 
							{
								dwTemp = dwCurrTick - valSocket->getLastSendTick();
								if( dwTemp > CSocketHelper::KEEPALIVE_INFO.dwSendInterval ) {
									// N초 이상 데이터 송신이 없으면 KEEP-ALIVE 문자를 전송 한다 
									((CClientSocket*)valSocket)->Send( (const char*)strKeepAlive );
								}
							}

							if( CSocketHelper::KEEPALIVE_INFO.bUsingRecvKeepAlive == TRUE ) 
							{
								dwTemp = dwCurrTick - valSocket->getLastRecvTick();
								if( dwTemp > CSocketHelper::KEEPALIVE_INFO.dwRecvInterval ) {
									// N초 이상 데이터 수신이 없으면 연결을 끊는다 
									valSocket->disconnectTimeout();
								}
							}
						}
					}
				}
				LeaveCriticalSection( &hCS_ );
			}
			else 
			{
				EnterCriticalSection( &hCS_ );
				for( pos = mapSocket_.GetStartPosition(); pos != NULL; ) 
				{
					mapSocket_.GetNextAssoc( pos, (WORD&)wTemp, (void*&)valSocket );
					keySocket = (SOCKET)wTemp;
					if( FD_ISSET( keySocket, &fdRead ) )	// 수신할 데이터가 있는가?, 연결이 끊어 졌는가?, accept가 성공할 것 인가?
					{
						switch( valSocket->getSocketType() )
						{
						case ISocket::LISTEN_SOCKET:
							valSocket->OnAccept();			// 접속 요청한 Client가 있음 
							break;
						case ISocket::CLIENT_SOCKET:
							valSocket->setLastRecvTick();	// 마지막 데이터 수신 시간을 저장 한다 
							valSocket->fdReadImpl();			// 수신할 데이터가 있음 or 연결이 끊어 졌음 (recv의 리턴 값이 0 이하)
							break;
						}
					}
					if( FD_ISSET( keySocket, &fdWrite ) )	// 데이터가 송신 되었는가?(x), 연결이 성공 되었는가?
					{
						switch( valSocket->getSocketType() )
						{
						case ISocket::LISTEN_SOCKET:
							TRACE( _T("[fd_write::IHSocket::LISTEN_SOCKET] \n") );
							break;
						case ISocket::CLIENT_SOCKET:
							valSocket->fdWriteImpl();			// 데이터를 송신할 수 있음
							break;
						}
					}
					if( FD_ISSET( keySocket, &fdExcep ) ) // 연결이 실폐하였는가 ?
					{
						switch( valSocket->getSocketType() ) 
						{
						case ISocket::LISTEN_SOCKET:
							TRACE( _T("[fd_except::ISocket::LISTEN_SOCKET] \n") );
							break;
						case ISocket::CLIENT_SOCKET:
							valSocket->fdExceptImpl();
							break;
						}
					}
				}
				LeaveCriticalSection( &hCS_ );
			}

			Sleep( 10 );
		} 
		else 
		{
			Sleep( 10 );
		}
	}

	TRACE( _T("STOP CSocketHelper::tfunc_()\n") );

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CListenSocket::CListenSocket()
{
	hSocketType_	= ISocket::LISTEN_SOCKET;
	socket_			= INVALID_SOCKET;
	setConnectState( ISocket::NOT_CONNECT );
}

CListenSocket::~CListenSocket()
{
	Close();
}

int CListenSocket::Listen( int nPortNo )
{
	int errcode = 0;

// 	if( IsConnected() == TRUE ) {
// 		return 0;
// 	}
	if( GetConnectState() == ISocket::NOT_CONNECT ) {

	} else {
		return 0;
	}

	try 
	{
	setConnectState( ISocket::PENDING_CONNECT );

		// 1. Socket 생성 
		socket_ = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		if( socket_ == INVALID_SOCKET ) {
			errcode = GetLastError();
			throw errcode;
		}

		// 2. Binding 
		SOCKADDR_IN ServerAddr;
		ServerAddr.sin_family		= AF_INET;
		ServerAddr.sin_port			= htons(nPortNo);
		ServerAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
		errcode = bind( socket_, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr) );
		if( errcode != 0 ) {
			throw errcode;
		}

		// 3. 
		errcode = listen( socket_, 5 );
		if( errcode != 0 ) {
			throw errcode;
		}

		// 4. Socket을 Non-Blocking 으로 만든다
		unsigned long ul = 1;
		if( ioctlsocket( socket_, FIONBIO, (unsigned long*)&ul ) == SOCKET_ERROR ) {
			errcode = GetLastError();
			throw errcode;
		}

		// 5.
		accept( socket_, NULL, NULL );
		errcode = GetLastError();
		switch( errcode )
		{
		case WSAEWOULDBLOCK:
			errcode = 0;
			break;
		default:
			throw errcode;
		}

		// 6. Map에 추가 
		SOCKET_HELPER.AddToMap( this );

		// 7. connected 상태로 전환 
		setConnectState( ISocket::CONNECTED );
	}
	catch( int& ) 
	{
		if( socket_ != INVALID_SOCKET ) {
			closesocket( socket_ );
			socket_ = INVALID_SOCKET;
		}
		setConnectState( ISocket::NOT_CONNECT );
	}

	return errcode;
}

void CListenSocket::Close()
{
	SOCKET_HELPER.RemoveFromMap( this );
	closesocket( socket_ ); 
	socket_ = INVALID_SOCKET;
	setConnectState( ISocket::NOT_CONNECT );
}

// client의 접속을 받고, Helper에 등록 한다
BOOL CListenSocket::Accept( CClientSocket& hClientSocket )
{
	SOCKET tempSocket = accept( socket_, NULL, NULL );
	
	if( tempSocket == INVALID_SOCKET ) {
		return FALSE;
	}

	hClientSocket.Attach( tempSocket );

	return TRUE;
}

SOCKET CListenSocket::GetSocketHandle()
{
	return socket_;
}

ISocket::EN_SOCKET_TYPE CListenSocket::getSocketType()
{
	return hSocketType_;
}

BOOL CListenSocket::haveSendData()
{
	return hSocketType_;
}

void CListenSocket::setLastSendTick()
{

}

DWORD CListenSocket::getLastSendTick()
{
	return GetTickCount();
}

void CListenSocket::setLastRecvTick()
{
	
}

DWORD CListenSocket::getLastRecvTick()
{
	return GetTickCount();
}

void CListenSocket::disconnectTimeout()
{

}

void CListenSocket::setConnectState( EN_CONNECT_STATE connectState )
{
	hConnectState_ = connectState;
}

ISocket::EN_CONNECT_STATE CListenSocket::GetConnectState()
{
//	return ISocket::CONNECTED;
	return hConnectState_;
}

void CListenSocket::OnAccept()
{

}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CClientSocket::CClientSocket( int buffer_size )
{
	InitializeCriticalSection( &hCS_ );
	hSocketType_	= ISocket::CLIENT_SOCKET;
	socket_			= INVALID_SOCKET;
	bHaveSendData_	= FALSE;
	setConnectState( ISocket::NOT_CONNECT );
	nBufferSize_	= buffer_size;

	pSendDataInfo_ = new SCommDataInfo( buffer_size );
	pRecvDataInfo_ = new SCommDataInfo( buffer_size );
}

CClientSocket::~CClientSocket()
{
	Close();
	DeleteCriticalSection( &hCS_ );

	if( pSendDataInfo_ ) {
		delete pSendDataInfo_;
		pSendDataInfo_ = NULL;
	}
	if( pRecvDataInfo_ ) {
		delete pRecvDataInfo_;
		pRecvDataInfo_ = NULL;
	}
}

int CClientSocket::Attach( SOCKET socket )
{
	int		errcode = 0;

	socket_ = socket;

	// 1. Socket을 Non-Blocking 으로 만든다
	unsigned long ul = 1;
	if( ioctlsocket( socket_, FIONBIO, (unsigned long *)&ul ) == SOCKET_ERROR ) {
		socket_ = INVALID_SOCKET;
		errcode = GetLastError(); 
	} else {
		setConnectState( ISocket::CONNECTED );	// 접속 상태로 만든다 
		SOCKET_HELPER.AddToMap( this );
	}

	return errcode;
}

int CClientSocket::Connect( const char* pHostAddr, int nPortNo )
{
	int errcode = 0;

	try 
	{
		// 접속 상태 변수를 False로 만든다 
		setConnectState( ISocket::NOT_CONNECT );

		// 1. Socket 생성 
		socket_ = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		if( socket_ == INVALID_SOCKET ) {
			errcode = GetLastError();
			throw errcode;
		}

		// 2. Socket을 Non-Blocking 으로 만든다
		unsigned long ul = 1;
		if( ioctlsocket( socket_, FIONBIO, (unsigned long *)&ul ) == SOCKET_ERROR ) {
			errcode = GetLastError();
			throw errcode;
		}

		// 3. 연결 
		SOCKADDR_IN ServerAddr;
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = htons(nPortNo);
		ServerAddr.sin_addr.s_addr = inet_addr( pHostAddr );

		if( connect( socket_, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr) ) != 0 ) {
			errcode = GetLastError();
			switch( errcode )
			{
			case WSAEWOULDBLOCK:
				setConnectState( ISocket::PENDING_CONNECT );
				errcode = 0;
				break;
			default:
				throw errcode;
			}
		} else {
			setConnectState( ISocket::CONNECTED );	// 접속 상태로 만든다 
		}

		// 4. 
		SOCKET_HELPER.AddToMap( this );
	}
	catch( int& ) 
	{
		if( socket_ != INVALID_SOCKET ) {
			closesocket( socket_ );
			socket_ = INVALID_SOCKET;
			setConnectState( ISocket::NOT_CONNECT );
		}
	}

	return errcode;
}

void CClientSocket::Close()
{
	//////////////////////////////////////////////////////////////////////////
	// 클라이언트 접속이 끊기면, lock flag를 리셋 한다 
#ifdef _EPNC_SW_BYCHUL_
// 	pa::PPAStatus->GetThreadState()->bRemoteLock_ = FALSE;
#endif
	//////////////////////////////////////////////////////////////////////////
	SOCKET_HELPER.RemoveFromMap(this);
	closesocket( socket_ );
	socket_			= INVALID_SOCKET;
	setConnectState( ISocket::NOT_CONNECT );
}

// 에러가 없을 경우 전송된 byte 개수 리턴. 에러가 있을 경우 SOCKET_ERROR 리턴  
int CClientSocket::Send( const char* pMessage )
{
	int errcode = 0;
	int sentLen = 0;

	if( pMessage == NULL ) {
		return 0;
	} 

// 	if( hSendDataInfo_.nLen != 0 ) {
// 		return SOCKET_ERROR;
// 	}
	if( pSendDataInfo_ && pSendDataInfo_->nLen != 0 ) {
		return SOCKET_ERROR;
	}

	EnterCriticalSection( &hCS_ );

	bHaveSendData_ = FALSE;
// 	hSendDataInfo_.Set( pMessage, strlen( pMessage ) );
	pSendDataInfo_->Set( pMessage, strlen( pMessage ) );

//	sentLen = send( socket_, hSendDataInfo_.pBuffer, hSendDataInfo_.nLen, 0 );
	sentLen = send( socket_, pSendDataInfo_->pBuffer, pSendDataInfo_->nLen, 0 );

	if( sentLen == SOCKET_ERROR ) {
		errcode = GetLastError();
		switch( errcode )
		{
		case WSAEWOULDBLOCK:
			errcode = 0;
			bHaveSendData_ = TRUE;
			break;
		default:
			errcode = SOCKET_ERROR;
// 			hSendDataInfo_.Reset();		// 이래야 나중에 다시 전송할 수 있다 
			pSendDataInfo_->Reset();
			break;
		}
	}
	else {
// 		if( sentLen < hSendDataInfo_.nLen ) {
// 			hSendDataInfo_.Set( (char *)(hSendDataInfo_.pBuffer+sentLen), hSendDataInfo_.nLen-sentLen );
// 			bHaveSendData_ = TRUE;
// 		} else {
// 			hSendDataInfo_.Reset();
// 		}
		if( sentLen < pSendDataInfo_->nLen ) {
			pSendDataInfo_->Set( (char*)(pSendDataInfo_->pBuffer + sentLen), pSendDataInfo_->nLen - sentLen );
		}
		errcode = sentLen;
	}

	setLastSendTick();	// 마지막 데이터 전송 시간을 저장 

	LeaveCriticalSection( &hCS_ );

	return errcode;
}

int  CClientSocket::Send( const char* p, int len )
{
	int errcode = 0;
	int sentLen = 0;

	if( p == NULL || len == 0 ) {
		return 0;
	} 

// 	if( hSendDataInfo_.nLen != 0 ) {
// 		return SOCKET_ERROR;
// 	}
	if( pSendDataInfo_->nLen != 0 ) {
		return SOCKET_ERROR;
	}

	EnterCriticalSection( &hCS_ );

	bHaveSendData_ = FALSE;
// 	hSendDataInfo_.Set( p, len );
	pSendDataInfo_->Set( p, len );

// 	sentLen = send( socket_, hSendDataInfo_.pBuffer, hSendDataInfo_.nLen, 0 );
	sentLen = send( socket_, pSendDataInfo_->pBuffer, pSendDataInfo_->nLen, 0 );

	if( sentLen == SOCKET_ERROR ) {
		errcode = GetLastError();
		switch( errcode )
		{
		case WSAEWOULDBLOCK:
			errcode = 0;
			bHaveSendData_ = TRUE;
			break;
		default:
			errcode = SOCKET_ERROR;
// 			hSendDataInfo_.Reset();		// 이래야 나중에 다시 전송할 수 있다 
			pSendDataInfo_->Reset();
			break;
		}
	}
	else {
// 		if( sentLen < hSendDataInfo_.nLen ) {
// 			hSendDataInfo_.Set( (char *)(hSendDataInfo_.pBuffer+sentLen), hSendDataInfo_.nLen-sentLen );
// 			bHaveSendData_ = TRUE;
// 		} else {
// 			hSendDataInfo_.Reset();
// 		}
		if( sentLen < pSendDataInfo_->nLen ) {
			pSendDataInfo_->Set( (char*)(pSendDataInfo_->pBuffer + sentLen), pSendDataInfo_->nLen - sentLen ); 
		} else {
			pSendDataInfo_->Reset();
		}
		errcode = sentLen;
	}

	setLastSendTick();	// 마지막 데이터 전송 시간을 저장 

	LeaveCriticalSection( &hCS_ );

	return errcode;
}

SOCKET CClientSocket::GetSocketHandle()
{
	return socket_;	
}

ISocket::EN_SOCKET_TYPE CClientSocket::getSocketType()
{
	return hSocketType_;
}

BOOL CClientSocket::haveSendData()
{
	return bHaveSendData_;
}

void CClientSocket::setLastSendTick()
{
	dwLastSendTick_ = GetTickCount();
}

DWORD CClientSocket::getLastSendTick()
{
	return dwLastSendTick_;
}

void CClientSocket::setLastRecvTick()
{
	dwLastRecvTick_ = GetTickCount();
}

DWORD CClientSocket::getLastRecvTick()
{
	return dwLastRecvTick_;
}

void CClientSocket::disconnectTimeout()
{
	Close();
}

void CClientSocket::setConnectState( ISocket::EN_CONNECT_STATE connectState ) 
{
 	if( connectState == ISocket::CONNECTED ) {
		setLastRecvTick();
		setLastSendTick();
	} 

	hConnectState_ = connectState;
}

ISocket::EN_CONNECT_STATE CClientSocket::GetConnectState()
{
	return hConnectState_;
}

void CClientSocket::fdReadImpl()
{
	int		nLen;

// 	hRecvDataInfo_.Reset();
	pRecvDataInfo_->Reset();

// 	nLen = recv(socket_, hRecvDataInfo_.pBuffer, hRecvDataInfo_.BUFFER_SIZE-1, 0 );
// 	hRecvDataInfo_.nLen = nLen;
	nLen = recv( socket_, pRecvDataInfo_->pBuffer, pRecvDataInfo_->BUFFER_SIZE-1, 0 );

	if( nLen <= 0 ) {
		DWORD dwErrCode = WSAGetLastError();
		// 비 정상적 접속 해제 
		if( dwErrCode != 0 ) 
		{
			CString strDbg;
			strDbg.Format( _T("dwErrCode:%d\n"), dwErrCode );
			TRACE( strDbg );
			OnAdnormal_Disconnect();
		}
		// 접속 종료 
		Close();
	} 
	else {
		// Recv KeepAlive 기능을 사용할 경우, 버퍼에 KeepAlive 문자열이 있는지 확인하고, 
		// OnRecv에는 보내지 않는다 
		if( (CSocketHelper::KEEPALIVE_INFO.cSignal != NULL) && 
			(CSocketHelper::KEEPALIVE_INFO.bUsingRecvKeepAlive == TRUE) ) 
		{
			char strKeepAlive[2] = { 0, 0 };
			strKeepAlive[0] = CSocketHelper::KEEPALIVE_INFO.cSignal;
			while( TRUE ) 
			{
// 				char* ptemp = strstr( (char *)hRecvDataInfo_.pBuffer, (const char *)strKeepAlive );
// 				nLen = strlen( hRecvDataInfo_.pBuffer );
				char *ptemp = strstr( (char*)(pRecvDataInfo_->pBuffer), (const char*)strKeepAlive );
				nLen = strlen( pRecvDataInfo_->pBuffer );
				if( ptemp == NULL ) {
					break;
				} else {
				//	TRACE( _T("Reecive KEEP-ALIVE\n") );
					int nLenKeepAlive = 1;
					memcpy( (void*)(ptemp), (const void*)(ptemp+nLenKeepAlive), strlen(ptemp)-nLenKeepAlive );
// 					hRecvDataInfo_.pBuffer[nLen - nLenKeepAlive] = NULL;
					pRecvDataInfo_->pBuffer[nLen - nLenKeepAlive] = NULL;
				}
			}
		}
		if( nLen > 0 ) 
		{
// 			OnRecv( (const char *)hRecvDataInfo_.pBuffer, nLen );
			OnRecv( (const char *)(pRecvDataInfo_->pBuffer), nLen );
		} 
	}
}

void CClientSocket::fdWriteImpl()
{
	int errcode = 0;
	int sentLen = 0;
	
	switch( hConnectState_ )
	{
	case ISocket::PENDING_CONNECT:
		setConnectState( ISocket::CONNECTED );	// 접속 상태로 만든다 
		break;

	case ISocket::CONNECTED:
		bHaveSendData_ = FALSE;

// 		sentLen = send( socket_, hSendDataInfo_.pBuffer, hSendDataInfo_.nLen, 0 );
		sentLen = send( socket_, pSendDataInfo_->pBuffer, pSendDataInfo_->nLen, 0 );

		if( sentLen == SOCKET_ERROR ) {
			errcode = GetLastError();
			switch( errcode )
			{
			case WSAEWOULDBLOCK:
				errcode = 0;
				bHaveSendData_ = TRUE;
				break;
			default:
				errcode = SOCKET_ERROR;
// 				hSendDataInfo_.Reset();		// 이래야 나중에 다시 전송할 수 있다 
				pSendDataInfo_->Reset();
				break;
			}
		}
		else {
// 			if( sentLen < hSendDataInfo_.nLen ) {
// 				hSendDataInfo_.Set( (char *)(hSendDataInfo_.pBuffer+sentLen), hSendDataInfo_.nLen-sentLen );
// 				bHaveSendData_ = TRUE;
// 			} else {
// 				hSendDataInfo_.Reset();
// 			}
			if( sentLen < pSendDataInfo_->nLen ) {
				pSendDataInfo_->Set( (char*)(pSendDataInfo_->pBuffer + sentLen), pSendDataInfo_->nLen - sentLen );
				bHaveSendData_ = TRUE;
			} else {
				pSendDataInfo_->Reset();
			}
		}

	default:
		break;
	}
}

void CClientSocket::fdExceptImpl()
{
	Close();	// 접속을 종료 한다
}

void CClientSocket::OnRecv( const char* p, int len )
{
}

void CClientSocket::OnAdnormal_Disconnect()
{

}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

// 에러 코드에 대응하는 에러 메시지를 리턴 한다
void GET_ERROR_MESSAGE( DWORD dwErrorCode, CString& strErrorMessage )
{
	TCHAR szMsg[ 1024 ] = {0,};

	memset( (void*)szMsg, 0, sizeof(TCHAR)*1024 );

	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErrorCode, 0, szMsg, 1024, NULL );

	strErrorMessage.Format( _T("%s"), szMsg );
}

// Ascii 문자열을 Unicode 문자열로 변경 (mbstowcs)
void ASCII_TO_UNICODE( char* pAscii, wchar_t* pUnicode, int nBufferLen )
{
// 	mbstowcs( pUnicode, pAscii, strlen(pAscii) );

	size_t numberOfConverted = 0;

	memset( (void*)pUnicode, 0, sizeof(TCHAR)*nBufferLen );

	mbstowcs_s( &numberOfConverted, pUnicode, nBufferLen, pAscii, _TRUNCATE );
}

// Unicode 문자열을 Ascii 문자열로 변경 (wcstombs)
void UNICODE_TO_ASCII( wchar_t* pUnicode, char* pAscii, int nBufferLen )
{
// 	wcstombs( pAscii, pUnicode, wcslen(pUnicode) );

	size_t numberOfConverts = 0;

	memset( (void*)pAscii, 0, sizeof(char)*nBufferLen );

	wcstombs_s( &numberOfConverts, pAscii, nBufferLen, pUnicode, _TRUNCATE );
}

//////////////////////////////////////////////////////////////////////////

BOOL STARTUP_SOCKET_COMM( CString& strErrMsg )
{
	WSADATA	wsaData;
	int		n = WSAStartup(MAKEWORD(2,0), &wsaData);

	strErrMsg.Format( _T("") );

	switch( n ) 
	{
	case WSASYSNOTREADY :		strErrMsg.Format( _T("WSASYSNOTREADY") ); break;
	case WSAVERNOTSUPPORTED :	strErrMsg.Format( _T("WSAVERNOTSUPPORTED") ); break;
	case WSAEPROCLIM :			strErrMsg.Format( _T("WSAEPROCLIM") ); break;
	case WSAEFAULT :			strErrMsg.Format( _T("WSAEFAULT") ); break;
	}

	return (BOOL)( n==0 );
}

void CLEANUP_SOCKET_COMM()
{
	WSACleanup();
}

int GET_IP_ADDRESS( CString strIpAddress[], int nSize )
{
	int	 count = 0;
	char sztemp[64];

	for( int i = 0; i<nSize; i++ ) {
		strIpAddress[i].Format( _T("") );
	}

	memset((void*)sztemp, 0, sizeof(char)*64);

	gethostname( sztemp, 63 );

	hostent *pHost = gethostbyname( sztemp );

	while( *pHost->h_addr_list!=NULL ) {
		long int *ptemp = (long int *)*pHost->h_addr_list;
		SOCKADDR_IN addr;
		addr.sin_addr.s_addr = *ptemp;
		strIpAddress[count++].Format( _T("%d.%d.%d.%d"), 
			addr.sin_addr.S_un.S_un_b.s_b1,		//	.sin_addr.S_un_b.s_b1,
			addr.sin_addr.S_un.S_un_b.s_b2,		//sin_addr.s_b2,
			addr.sin_addr.S_un.S_un_b.s_b3,		//sin_addr.s_b3,
			addr.sin_addr.S_un.S_un_b.s_b4 );	//sin_addr.s_b4 );
		
		pHost->h_addr_list++;
	} 

	return count;
}

//////////////////////////////////////////////////////////////////////////
}

