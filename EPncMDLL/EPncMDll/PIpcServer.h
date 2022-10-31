#pragma once

//////////////////////////////////////////////////////////////////////////
// IpcQueue는 단방향 Queue 이기 때문에,
//	- IpcServer에서는 Listen만 할 수 있고,
//	- IpcClient에서는 Send만 할 수 있다 
//////////////////////////////////////////////////////////////////////////

namespace pa 
{
//////////////////////////////////////////////////////////////////////////

class CPIpcServer : public CGeneralThread
{
private:
	hcipc::CIpcQueue	*pIpcQueue_;
	void subCommandProc( SIpcCommCommand& cmd );

	void uploadCoordinateOffset();
	void uploadTeachingPoint();
	void uploadOption();
	void uploadFlowSensorData();

	void downloadCoordinateOffset();
	void downloadTeachingPoint();
	void downloadOption();
	void downloadFlowSensorData();

	void writeLog_IPC( LPCTSTR logMsg );

public:
	BOOL Initialize( DWORD dwCycleTime, CString& strErrMsg );
	void Destroy();

	// Virtual Method
public:
	virtual void Execute();


public:
	CPIpcServer(void);
	virtual ~CPIpcServer(void);
};

//////////////////////////////////////////////////////////////////////////
}