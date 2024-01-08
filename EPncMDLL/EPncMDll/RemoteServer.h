#pragma once

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

class CRemoteClient : public hcsock::CClientSocket
{
private:
	CRITICAL_SECTION cs_;

	BYTE*	pRecvBuffer_;
	int		nRecvBufferIndex_;

	int		nImagePacketNo_;		// 이미지 패킷 번호. 
									// 이지지를 캡쳐해서 보내애 할 때, 0이면 새로 챕쳐.
									// 0이 아니면, 남은 이미지를 보내고 새로 갱신 
	int		nImageDataIndex_;		// 이미지 데이터에서 다음 전송할 인덱스 값을 저장한다 

private:
	virtual void OnRecv( const char* p, int len );

	void send_screen();	// 화면을 capture 하고, send 한다 
	void send_image_packet( int packet_no, int len, char* p );
	void mouse_click( int isdown, int x, int y);
	void send_selected_logfiles();
	void send_sw_version();
	void send_sw_resolution();

public:
	CRemoteClient( int buffer_size );
	virtual ~CRemoteClient();

	static int PACKET_SIZE;
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

class CRemoteServer : public hcsock::CListenSocket
{
	CRemoteClient* pClient_;

public:
	virtual void OnAccept();

	BOOL IsConnectedClient();

	void DisconnectClient();

	CRemoteClient* GetClient()
	{
		return pClient_;
	}

public:
	CRemoteServer(void);
	virtual ~CRemoteServer(void);
};

extern CRemoteServer* P_REMOTE_SERVER;
