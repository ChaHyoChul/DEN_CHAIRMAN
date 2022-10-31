#pragma once

namespace pa
{
class CGLCDCommException
{

};


class CGLCDCommEx : public hcsock::CClientSocket, public CGeneralThread
{
private:
	CRITICAL_SECTION hCS_;
	
	char szIPAddress_[64];
	int nPortNo_;

	char szCommandBuffer_[256];
	char szRecvBuffer_[256];
	int  nRecvBufferIndex_;

	char szThreadCommand_[64];

public:
	virtual void OnRecv(const char* p, int len);

public:
	BOOL Initialize(CString strFilepath, CString& strErrMsg);
	BOOL Initialize(CString strIpAddress, int portNo);
	void Destroy();

	void StartComm();
	void StartCommPlain();
	void Upgrade();
	void SendCommand(CString cmd);
	void SendCommand(char* cmd);
	BOOL SendRaw(char* cmd);

	virtual void Execute();

public:
	CGLCDCommEx();
	CGLCDCommEx(int buffer_size);
	~CGLCDCommEx(void);
	
};
}
