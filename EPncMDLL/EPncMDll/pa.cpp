#include "stdafx.h"
#include "pa.h"
#include "EPncMDLL.h"
#include "ErrorDlg.h"

//////////////////////////////////////////////////////////////////////////
// ���� ���� 
//////////////////////////////////////////////////////////////////////////
namespace pa 
{
CString STR_ERROR_TYPE_FILEPATH	= CString( INI_PA_ERROR_TYPE_PATH );
CString STR_ERROR_CODE_FILEPATH	= CString( INI_PA_ERROR_CODE_PATH );
CString STR_ERROR_MESSAGE_FILEPATH	= CString( INI_PA_ERROR_MESSAGE_PATH );

CPAAsyncComm*		PPAAsyncComm[2]	= { NULL, NULL };
CPAStatus*			PPAStatus		= NULL;
CGLCDCommEx*		pGLCD			= NULL;
CPAMotion*			PAMotion		= NULL;
CPThread*			PThread			= NULL;
CPNCFile*			PNCFile			= NULL;
CPNCFileMgr*		PNCFileMgr		= NULL;
CPIpcServer*		PIpcServer		= NULL;
CPConfig*			PConfig			= NULL;
CPContinueRunInfo*	PContinueRunInfo= NULL;
CPTool*				PTool			= NULL;
CPMaintenance*		PMaintenance	= NULL;

CToolPocketAutoTeachingBase* PToolPocketAutoTeaching = NULL;

BOOL				IS_SUCCESS_FIRST_CONNECT = FALSE;

DWORD				todaySpindleRunTime = 0;
SYSTEMTIME			currentReadWriteTime;
int					nMaxxLinkRemoteServer_PortNo = 0;
int					nRemotePNC_PortNo = 0;
HANDLE				hJobRestApi = NULL;
}

//////////////////////////////////////////////////////////////////////////
// ���� �Լ�  
//////////////////////////////////////////////////////////////////////////

/** 
 * Socket �ʱ�ȭ �� ���� 
 */
BOOL pa::STARTUP_SOCKET_COMM( CString& strErrMsg )
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

void pa::CLEANUP_SOCKER_COMM()
{
	WSACleanup();
}

/** 
 * Socket ���� �ڵ带 �޽����� ��ȯ 
 */
void pa::GET_SOCKET_ERROR_MESSAGE( DWORD dwErrorCode, CString& strErrorCode )
{
	TCHAR szMsg[1024] = {0,};

	memset( (void*)szMsg, 0, sizeof(char)*1024 );

	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErrorCode, 0, szMsg, 1024, NULL );

	strErrorCode.Format( _T("[%d] %s"), dwErrorCode, szMsg );
}

/** 
 * PA ����� ���� �޽����� ���� �Ѵ� 
 */
void pa::GET_PA_ERROR_MESSAGE( int nErrorType, int nErrorCode, CString& strErrorType, int& nErrorTypeIsAlarm, CString& strErrorCode, CString& strErrorMessage )
{
	CCEIniFile hIniFile;
	CString strKeyName, strValueName, strValueName2, strSide;
	int nErrorCodeClean;

	strErrorType.Format( _T("") );
	strErrorCode.Format( _T("") );
	strErrorMessage.Format( _T("") );
	strSide.Format( _T("") );

	switch( nErrorType )
	{
	case pa::ERR_RND_CMD:
	case pa::ERR_PA_STREAM:
		strKeyName.Format( _T("RND_CMD") );
		break;
	case pa::ERR_PNC:
		strKeyName.Format( _T("PNC") );
		break;
	default:
		strErrorType.Format( _T("*") );
		strErrorCode.Format( _T("*") );
		strErrorMessage.Format( _T("*") ); 
		return ;
	}

	// Take care of CHAIRMAN error codes for LEFT/RIGHT spindle (+10000 - LEFT, +20000 - RIGHT)
	if(nErrorCode > 10000){
		if(nErrorCode > 20000){
			nErrorCodeClean = nErrorCode - 20000;
			strSide.Format( _T(" (right)") );
		} else {
			nErrorCodeClean = nErrorCode - 10000;
			strSide.Format( _T(" (left)") );
		}
	} else {
		nErrorCodeClean = nErrorCode;
	}
	
	strValueName.Format( _T("E%d"), nErrorCodeClean );
	strValueName2.Format( _T("A%d"), nErrorCodeClean );

	hIniFile.Open( STR_ERROR_TYPE_FILEPATH );	// PAErrorType.ini

	hIniFile.GetValue( strKeyName, strValueName, (CString*)&strErrorType );
	hIniFile.GetValue( strKeyName, strValueName2, (int*)&nErrorTypeIsAlarm ); 

	hIniFile.Close();

	hIniFile.Open( STR_ERROR_CODE_FILEPATH );	// PAErrorCode.ini

	hIniFile.GetValue( strKeyName, strValueName, (CString*)&strErrorCode );
	strErrorCode += strSide;	// add side if necessary

	hIniFile.Close();

	hIniFile.Open( STR_ERROR_MESSAGE_FILEPATH );	// PAErrorMessage.ini

	hIniFile.GetValue( strKeyName, strValueName, (CString*)&strErrorMessage );

	hIniFile.Close();
}

/** 
 * port1 = 10100, port2 = 10000
 */
BOOL pa::PA_INITIALIZE( char* pIpAddr, int nPortNo1, int nPortNo2, int nPortNoForStream, CString& strErrMsg )
{
	CString	strConfigFilePath;
	CString strReplaceNCCodeConfigFilePath;
	CString strCheckNcCodeConfigFilePath;
	BOOL	bRet;
	char	sztemp[128];

#ifdef _DEBUG
	Sleep( 1*1000 );	// ����� ���ñ��� 1�ʰ� ��� 
#else
	Sleep( 1*1000 );	// ����� ���ñ��� 30�ʰ� ��� 
#endif 

	strConfigFilePath = CString( INI_PA_CONFIG_PATH );
	strReplaceNCCodeConfigFilePath = CString( INI_REPLACE_NC_CODE_CONFIG_PATH );
	strCheckNcCodeConfigFilePath = CString( INI_NC_FILE_CHECKER_CONFIG_PATH );

	// 0. �ʿ��� ���丮�� ���� ���, �̸� ����� 
	CREATE_NECESSARY_DIRECTORIES();

	// 1. PAComm �ʱ�ȭ �� ���� 
	PPAAsyncComm[0] = new CPAAsyncComm(2048);
	if( PPAAsyncComm[0] == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pa::PAAsyncComm[0] object") );
	}
	PPAAsyncComm[1] = new CPAAsyncComm( 2048 );
	if( PPAAsyncComm[1] == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pa::PAAsyncComm[1] object") );
	}

	DWORD	dwErrCode = 0;
	int		retry_count = 0;
	
#ifdef _USE_PA_

	DWORD dwTime = GetTickCount();
	while(PPAAsyncComm[0]->GetConnectState() != hcsock::ISocket::CONNECTED || PPAAsyncComm[1]->GetConnectState() != hcsock::ISocket::CONNECTED){
		hcsock::ISocket::EN_CONNECT_STATE conn_status0 = PPAAsyncComm[0]->GetConnectState();
		hcsock::ISocket::EN_CONNECT_STATE conn_status1 = PPAAsyncComm[1]->GetConnectState();
		
		// Timeout Checking
		if( (GetTickCount() - dwTime) > 120*1000 ){
			ShowConnectFailDialog( 1 );
			return FALSE;
		}

		// ���� �ɶ����� ���
		if(conn_status0 == hcsock::ISocket::CONNECTED && conn_status1 == hcsock::ISocket::CONNECTED){
			break;
		} else {
			if(conn_status0 == hcsock::ISocket::NOT_CONNECT){
				PPAAsyncComm[0]->Connect( theApp.szIpAddr, theApp.nPortNo1 );
			}
			if(conn_status1 == hcsock::ISocket::NOT_CONNECT){
				PPAAsyncComm[1]->Connect( theApp.szIpAddr, theApp.nPortNo2 );
			}
			Sleep( 500 );
		}
	}
#endif

	// 2. PMotion �ʱ�ȭ 
	PAMotion = new CPAMotion();
	if( PAMotion == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pmac::PMotion object") );
		return FALSE;
	}

	// 3. PConfig �ʱ�ȭ
	PConfig = new CPConfig();
	if( PConfig == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pmac::PConfig object") );
		return FALSE;
	}
	bRet = PConfig->Initialize( strConfigFilePath, strErrMsg );
	if( bRet == FALSE ) {
		return FALSE;
	}
	
	bRet = PConfig->Load( strErrMsg );
	if( bRet == FALSE ) {
		return FALSE;
	}

	// 4. log ��ü �ʱ�ȭ 
	P_LOG = new CLog( 
		PConfig->pConfig_->nEnableOperationLog, PConfig->pConfig_->nEnableIpcCommLog,
		PConfig->pConfig_->nEnableThreadModeLog, PConfig->pConfig_->nEnableOpPenalLog, 
		PConfig->pConfig_->nEnableExtLog, PConfig->pConfig_->nEnableErrLog );
	if( P_LOG ) {
		P_LOG->Initialize( 30 );
		P_LOG->WriteLog( CLog::TYPE_OPER, 0, _T("<<< Start. EPncM.Dll >>>") );
		P_LOG->WriteLog( CLog::TYPE_OPER, 1, theApp.P_VERSION );
	}

	P_LOG->EnableOperationLog( PConfig->pConfig_->nEnableOperationLog );
	P_LOG->EnableIpcCommLog( PConfig->pConfig_->nEnableIpcCommLog );
	P_LOG->EnableThreadModeLog( PConfig->pConfig_->nEnableThreadModeLog );
	P_LOG->EnableOpPanelLog( PConfig->pConfig_->nEnableOpPenalLog );
	P_LOG->EnableExtLog( PConfig->pConfig_->nEnableExtLog );
	P_LOG->EnableErrLog( PConfig->pConfig_->nEnableErrLog );

	//	> ���� ���ɵ� ���ð� ������ ���� �Ѵ� 
	DELETE_TODAY_SPINDLE_RUN_TIME(100);

	// 5. ReplaceNCCode �ʱ�ȭ 
	bRet = CGCodeHelper::LoadReplaceCommand( strReplaceNCCodeConfigFilePath, strErrMsg );
	if( bRet == FALSE ) {
		return FALSE;
	}

	// 6. NcFileChecker �ʱ�ȭ 
	bRet = CGCodeHelper::LoadCheckCommand( strCheckNcCodeConfigFilePath, strErrMsg );
	if( bRet = FALSE ) {
		return FALSE;
	}

	// 7. PTool �ʱ�ȭ 
	PTool = new CPTool();
	if( PTool == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pmac::PTool object") );
		return FALSE;
	}
	bRet = PTool->Initialize( strErrMsg );
	if( bRet == FALSE ) {
		return FALSE;
	}
	
	// 7.1 PMaintenance init
	
	PMaintenance = new CPMaintenance();
	if( PMaintenance == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pmac::PMaintenance object") );
		return FALSE;
	}
	bRet = PMaintenance->Initialize( strErrMsg );
	if( bRet == FALSE ) {
		return FALSE;
	}

	// 8. PPAState 
	PPAStatus = new CPAStatus();
	if( PPAStatus == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pmac::PAState object") );
		return FALSE;
	} 
	bRet = PPAStatus->Initialize( strErrMsg );
	if( bRet == FALSE ) {
		return FALSE;
	}

#ifdef _USE_PA_
	PPAAsyncComm[0]->SetRefPAStatus( PPAStatus->GetPAStatus() );
	PPAAsyncComm[0]->SetRefThreadState( PPAStatus->GetThreadState() );
	PPAAsyncComm[1]->SetRefPAStatus( PPAStatus->GetPAStatus() );
	PPAAsyncComm[1]->SetRefThreadState( PPAStatus->GetThreadState() );
#endif 

	// �����͸� ����⿡ ������ ���, ����⿡�� �о� �´� 
	try {
#ifdef _USE_PA_
		// SMCT ���� ���� (2017.05.01)
		//	- 2017.05.19 �������� �������� �ʴ´� 
		DWORD	dwErr = 0;
		DWORD	dwSysErr = 0;
		int		nRespErrCode = 0;
		int		nRespSysErrCode = 0;

		pa::PPAStatus->GetThreadState()->nIsConnectedIOBoard = 1;		// IO Board Connection flag 1�� �ʱ�ȭ

		try {
			PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_SMCT );
			PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_SMCT, 10000 );
			dwErr = CPAAsyncComm::DW_RESPONSE_SMCT_CMD;
			nRespErrCode = CPAAsyncComm::N_ERRORCODE_SMCT_CMD;
		}
		catch ( pa::CPException& e ) {
			dwErr = 0;
		}
		try {
			PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RISM);
			PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RISM, 10000 );
			dwSysErr = CPAAsyncComm::DW_RESPONSE_RISMCT_CMD;
			nRespSysErrCode = CPAAsyncComm::N_ERRORCODE_RISMCT_CMD;	
		}
		catch ( pa::CPException& e ) {
			dwSysErr = 0;
		}
		BOOL	coord_offset = TRUE;
		BOOL	teaching_point = TRUE;

		// coordinate offset �����͸� �а�...
		for( int i = 0; i<pa::COORD_NUM; i++ ) 
		{
			pa::PAMotion->RCFG( (pa::EN_COORDINATE)i );
			Sleep(10);
		}
		Sleep(100);
        
		// z-origin offset �����͸� �а�...
		pa::PAMotion->RTOO(1);
		Sleep(10);
		pa::PAMotion->RTOO(2);
		Sleep(10);
		pa::PAMotion->RTDATA("spindleoffset");
		Sleep(10);
		pa::PAMotion->RTHS();
		Sleep(10);
		pa::PAMotion->RTLS();
		Sleep(10);
		pa::PAMotion->RTMG();
		Sleep(10);
		pa::PAMotion->RTPPO();
		Sleep(10);
        
		// teaching point �����͸� �а�...
		for( int i = 0; i<pa::TEACHING_POINT_NUM; i++ ) 
		{
			pa::PAMotion->RTCP( (pa::EN_TEACHING_POINT)i );
			Sleep(10);
		}
		Sleep(100);
        
		// PAStatus::Initialize() �Լ����� �о��� 
		// - coordinate offset data range�� �о���
		// - tool pocket range �Ķ��Ÿ�� �о��� (poweron_test_config.ini)
        
		// coordinate offset ������ ���� ���� Ȯ�� 
		coord_offset = CHECK_COORD_OFFSET_RANGE();
        
		// teaching point ������ ���� ���� Ȯ�� 
		teaching_point = CHECK_TEACH_POINT_RANGE();
        
		// ���信 ������ �ִ��� Ȯ�� 
		// ������ �ִٸ�, ���� ������ ǥ���� �ְ� ��� ���� ���θ� Ȯ�� �Ѵ� 
		if( ShowPowerOnTestFailDialog( dwErr, dwSysErr, nRespErrCode, nRespSysErrCode, coord_offset, teaching_point ) == IDCANCEL )
		{
			// Turn off power �޽��� ��� 
			return FALSE;
		}
#endif 
	}
	catch ( pa::CPException& e ) {

	}

	PPAStatus->GetPAStatus()->bLCDPassiveMode = FALSE;
	pGLCD = new CGLCDCommEx(1024);
	pGLCD->Initialize(INI_GLCD_PATH, strErrMsg);

	//
	PToolPocketAutoTeaching = NULL;
	switch (pa::MODEL_INFO.GetToolPocketType())
	{
	case 0:
	default:
		PToolPocketAutoTeaching = (CToolPocketAutoTeachingP1*)(new pa::CToolPocketAutoTeachingP1());
		PToolPocketAutoTeaching->Initialize(INI_AT_TOOL_POCKET_PARAM_PATH, strErrMsg);
		break; 
	case 1:
		PToolPocketAutoTeaching = (CToolPocketAutoTeachingP2*)(new pa::CToolPocketAutoTeachingP2());
		PToolPocketAutoTeaching->Initialize(INI_AT_TOOL_POCKET_PARAM_PATH, strErrMsg);
		break; 
	}
	
	// 7. PThread �ʱ�ȭ 
	PThread = new CPThread();
	if( PThread == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pmac::PThread object") );
		return FALSE;
	}
	bRet = PThread->Initialize( 100, strErrMsg );
	if( bRet == FALSE ) {
		return FALSE;
	}

	try
	{
#ifdef _USE_PA_
		PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_STOP );
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_STOP );
		PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_INIT );
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_INIT );
		PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_RST );
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_RST, 60000 );
		PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_VER );
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_VER );
        
		// 2017.01.13
		// ������� IP�ּҸ� �д´� 
		PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RADR );
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RADR );
		PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RRIOADR );
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RRIOADR );
        
		// 2017.03.24 
		// Select M28/M29 ������ �д´� 
		PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_GWVF );
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_GWVF );
		// TOOL ���� ���� 
		int		tool_no = 0;
		double	tool_length = 0;
		int		tool_lenght_update_flag = 0;
		pa::CPThread::READ_TOOL_INF0( 0, &tool_no, &tool_length, &tool_lenght_update_flag );
		PAMotion->RND_STIN( tool_no, tool_length, tool_lenght_update_flag );
		pa::CPThread::READ_TOOL_INF0( 1, &tool_no, &tool_length, &tool_lenght_update_flag );
		PAMotion->RND_STIN( tool_no, tool_length, tool_lenght_update_flag );

		// 2018.02.27 UsingAirLimitSensor, nAirLimitInterval ���� �����͸� ������ �����Ѵ� 
		int nUsingAirLimitSensor = PConfig->pConfig_->bUsingAirLimitSensor == FALSE ? 0 : 1;
		int nAirLimitInterval = PConfig->pConfig_->nAirLimitInterval;
		int nSensorType = (int)pa::MODEL_INFO.GetAirPressureSensorType();
		PAMotion->RND_SALF( TRUE, nUsingAirLimitSensor, nAirLimitInterval, nSensorType );

		// 2018.05.16 ���� �÷ο� ���� �ĸ���Ÿ�� ������ ���� �Ѵ� 
		int nUsingWaterFlowSensor = PConfig->pConfig_->bUsingFlowSensor == FALSE ? 0 : 1;
		int startTimeout = PConfig->pConfig_->nFlowSensorStartTimeout;
		int sensingTimeout = PConfig->pConfig_->nFlowSensorTimeout;
		int nUsingWaterLevelSensor = PConfig->pConfig_->bUsingWaterLevelSensor == FALSE ? 0 : 1;
		PAMotion->RND_SFSF( TRUE, nUsingWaterFlowSensor, startTimeout, sensingTimeout, nUsingWaterLevelSensor);
        
		// 2020.03.19 Purge air hold time �Ķ��Ÿ�� ������ �����Ѵ� 
		BOOL bUsingSpindleAirPurge = PConfig->pConfig_->bUsingSpindleAirPurge;
		int nPurgeAirHoldTime = (bUsingSpindleAirPurge == TRUE) ? PConfig->pConfig_->nPurgeAirHoldTime : 0;
		PAMotion->RND_WPAR( TRUE, nPurgeAirHoldTime );

		// 2020.05.01 JoSpeed ���� 
		PAMotion->WJSS(100);

#endif
	}
	catch( CPException& e )
	{
	}

	// 2020.03.19. ����� �ð� �ҷ��ͼ� PC�ð����� ����
	DWORD	dwErr = 0;
	int		nRespErrCode = 0;
	try {
#ifdef _USE_PA_
		PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RDT );		// Command ����
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RDT, 10000 );
		dwErr = CPAAsyncComm::DW_RESPONSE_RDT_CMD;
		nRespErrCode = CPAAsyncComm::N_ERRORCODE_RDT_CMD;
#endif 
	}
	catch ( pa::CPException& e ) {
		dwErr = 0;
	}
    
	// 3. PNcFile �ʱ�ȭ 
	PNCFile = new CPNCFile();
	if( PNCFile == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pmac::PNcFile object") );
		return FALSE;
	}

	// 8. PNCFileMgr
	PNCFileMgr = new CPNCFileMgr();
	if( PNCFileMgr == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pmac::PNCFileMgr object") );
		return FALSE;
	}
	bRet = PNCFileMgr->Initialize( strErrMsg );
	if( bRet == FALSE ) {
		return FALSE;
	}

	// 9. PIpcServer �ʱ�ȭ 
	PIpcServer = new CPIpcServer;
	if( PIpcServer == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pmac::PIpcServer objct") );
		return FALSE;
	}
	bRet = PIpcServer->Initialize( 50, strErrMsg );
	if( bRet == FALSE ) {
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// Maxxlink ��� �߰� 
	P_REMOTE_SERVER = new CRemoteServer();
	if (P_REMOTE_SERVER == NULL) {
		strErrMsg.Format( _T("memory alloc error for P_REMOTE_SERVER object") );
		return FALSE;
	}
// 	if (P_REMOTE_SERVER->Listen(21000) != 0) {
	int portno = theApp.nMAXXLINK_SERVER_PORTNO;
	if (P_REMOTE_SERVER->Listen(portno) != 0) {
		strErrMsg.Format( _T("maxxlink remote server. listen error") );
		return FALSE;
	}

	P_SCRN_CAPTURE = new CScreenCapture();
	if (P_SCRN_CAPTURE == NULL) {
		strErrMsg.Format( _T("memory alloc error for P_SCRN_CAPTURE") );
		return FALSE;
	}
	P_SCRN_CAPTURE->Initialize();

	//////////////////////////////////////////////////////////////////////////

	PContinueRunInfo = new CPContinueRunInfo();
	if( PContinueRunInfo == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pmac::PContinueRunInfo object") );
		return FALSE;
	}
	if( PContinueRunInfo->Initialize( strErrMsg ) == FALSE ) {
		return FALSE;
	}

	// 10. Start IPC Server/Main/Graphic LCD Thread
	if( PConfig->pConfig_->bUsingLCD )
	{
		PPAStatus->GetPAStatus()->bLCDAlive = FALSE;
		pGLCD->StartComm();
		Sleep(100);
		
		DWORD dwRunningTime = pa::PPAStatus->GetThreadState()->dwRunningTime;
		CTimeSpan	tms( dwRunningTime );
		int			milling_time[3] = { tms.GetHours(), tms.GetMinutes(), tms.GetSeconds() };
		CString strSendmessage;
		strSendmessage.Format(_T("pgMain.txtTimer.txt=\"%02d:%02d\""), milling_time[1], milling_time[2]);
		pGLCD->SendCommand(strSendmessage);
		strSendmessage.Format(_T("pgMainRunning.txtTimer.txt=\"%02d:%02d\""), milling_time[1], milling_time[2]);
		pGLCD->SendCommand(strSendmessage);
		strSendmessage.Format(_T("pgMain.txtStatus.txt=\"Connected\""));
		pGLCD->SendCommand(strSendmessage);
				
		
		pGLCD->SendCommand(_T("pgMain.sctxtFilename.txt=\"\""));
		pGLCD->SendCommand(_T("pgMainRunning.sctxtFilename.txt=\"\""));
		pGLCD->SendCommand(_T("pgMain.btnColletR.picc=0"));
		pGLCD->SendCommand(_T("pgMain.btnColletR.picc2=0"));
		pGLCD->SendCommand(_T("pgMain.btnColletL.picc=0"));
		pGLCD->SendCommand(_T("pgMain.btnColletL.picc2=0"));

		CString strMsg; 
		strMsg.Format(_T("pgMain.txtModelInfo1.txt=\"%s\""), _T(""));
		pGLCD->SendCommand(strMsg);
		strMsg.Format(_T("pgMain.txtModelInfo2.txt=\"%s\""), _T(""));
		pGLCD->SendCommand(strMsg);
		strMsg.Format(_T("pgRunningMain.txtModelInfo1.txt=\"%s\""), _T(""));
		pGLCD->SendCommand(strMsg);
		strMsg.Format(_T("pgRunningMain.txtModelInfo2.txt=\"%s\""), _T(""));
		pGLCD->SendCommand(strMsg);

		PPAStatus->GetPAStatus()->bLCDRefresh=TRUE;
	}
	
	// 11. Start IPC Server
	PIpcServer->Start();

	// 12. Start main thread 
	PThread->Start();

	// �Ʒ� ����� ���� -> ������ ��� deselect �ϰ�, �ֱ� ����� ���ϵ� ���� �ʴ´� 
	pa::PThread->CloseNCFile();
	pa::PNCFileMgr->DeselectAllNCFiles( FALSE );

	PPAStatus->GetThreadState()->bCompleteResetOrigin_  = TRUE;		// ʱ  true,   false

	RUN_REST_API_SERVER();

	return TRUE;
	}

void pa::PA_DESTROY()
{
	if (P_SCRN_CAPTURE) { 
		delete P_SCRN_CAPTURE;
		P_SCRN_CAPTURE = NULL; 
	}

	if (P_REMOTE_SERVER) { 
		delete P_REMOTE_SERVER; 
		P_REMOTE_SERVER = NULL; 
	}

	if( PThread ) {
		PThread->Stop();
	}
	if( PIpcServer ) {
		PIpcServer->Stop();
	}
	if( PContinueRunInfo ) {
		delete PContinueRunInfo;
		PContinueRunInfo = NULL;
	}

	Sleep(500);

	if( PIpcServer ) {
		delete PIpcServer;
		PIpcServer = NULL;
	}
	
	if( PThread ) {
		delete PThread;
		PThread = NULL;
	}

	if( PNCFileMgr  ) {
		delete PNCFileMgr;
		PNCFileMgr = NULL;
	}

	if( PNCFile ) {
		delete PNCFile;
		PNCFile = NULL;
	}

	if( PAMotion ) {
		delete PAMotion;
		PAMotion = NULL;
	}

	if( PPAStatus ) {
		delete PPAStatus;
		PPAStatus = NULL;
	}

	if( PPAAsyncComm[0] ) {
		delete PPAAsyncComm[0];
		PPAAsyncComm[0] = NULL;
	}
	if( PPAAsyncComm[1] ) {
		delete PPAAsyncComm[1];
		PPAAsyncComm[1] = NULL;
	}

	if (PToolPocketAutoTeaching) {
		delete PToolPocketAutoTeaching;
		PToolPocketAutoTeaching = NULL;
	}

	if( PConfig ) {
		delete PConfig;
		PConfig = NULL;
	}

	if( P_LOG )
	{
		delete P_LOG;
		P_LOG = NULL;
	}

	if( PTool )
	{
		delete PTool;
		PTool = NULL;
	}
	
	if( PMaintenance )
	{
		delete PMaintenance;
		PMaintenance = NULL;
	}
	
	if( pGLCD )
	{
		((CGLCDCommEx*)pGLCD)->Destroy();
		delete ((CGLCDCommEx*)pGLCD);
	}

	if (hJobRestApi != NULL) {
		CloseHandle(hJobRestApi);
		hJobRestApi = NULL;
	}
}

void pa::RUN_REST_API_SERVER()
{
	// SmartFactory.ini 파일에서 RestAPI 사용여부를 확인 한다 
	int isUsing = 0;
	CString exeFilePath(_T(""));
	CCEIniFile iniFile;
	iniFile.Open(INI_SMART_FACTORY_PATH);

	iniFile.GetValue(_T("RestAPI"), _T("Using"), (int*)&isUsing);
	iniFile.GetValue(_T("RestAPI"), _T("Exe"), (CString*)&exeFilePath);

	//
	if (isUsing == 0) { return; }
	if (exeFilePath.IsEmpty()) { return; }
	// 
	if (hJobRestApi != NULL) return; // Already running

	hJobRestApi = CreateJobObject(NULL, NULL);
	if (hJobRestApi == NULL) return;

	JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };
	jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
	if (!SetInformationJobObject(hJobRestApi, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli))) {
		CloseHandle(hJobRestApi);
		hJobRestApi = NULL;
		return;
	}

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi = { 0 };
	//TCHAR szCmdLine[] = _T("c:/pnc/restapi/smrsvr.exe");
	//TCHAR szCmdLine[] = _T("../RestAPI/CMSvr.exe");
	TCHAR szCmdLine[512];
	_stprintf_s(szCmdLine, 512, _T("\"\%s\""), (LPCTSTR)exeFilePath);

	if (CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, CREATE_BREAKAWAY_FROM_JOB | CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		AssignProcessToJobObject(hJobRestApi, pi.hProcess);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else {
		CloseHandle(hJobRestApi);
		hJobRestApi = NULL;
	}
}

void pa::INIT_TOTAL_LEFT_SPINDLE_RUN_TIME()
{
	TCHAR	*PATH = _T("..\\..\\Data\\EPnc\\rnd");
	FILE *pf = _tfopen( PATH, _T("rb") );

	if( pf == NULL ) {
		pf = _tfopen( _T("..\\..\\Data\\EPnc\\rnd"), _T("wb") );
		PPAStatus->GetThreadState()->dwTOTAL_LEFT_SPINDLE_RUN_TIME = 0;
		SAVE_TOTAL_LEFT_SPINDLE_RUN_TIME();
	}

	fread( (void*)&(PPAStatus->GetThreadState()->dwTOTAL_LEFT_SPINDLE_RUN_TIME), 1, sizeof(DWORD), pf );
	
	fclose( pf ); pf = NULL;

	// does not work without the part above for some reason
	pa::PAMotion->RTDATA("spindle1runtime");
}

void pa::INIT_TOTAL_RIGHT_SPINDLE_RUN_TIME()
{
	TCHAR	*PATH = _T("..\\..\\Data\\EPnc\\rnd2");
	FILE *pf = _tfopen( PATH, _T("rb") );

	if( pf == NULL ) {
		pf = _tfopen( _T("..\\..\\Data\\EPnc\\rnd2"), _T("wb") );
		PPAStatus->GetThreadState()->dwTOTAL_RIGHT_SPINDLE_RUN_TIME = 0;
		SAVE_TOTAL_RIGHT_SPINDLE_RUN_TIME();
	}

	fread( (void*)&(PPAStatus->GetThreadState()->dwTOTAL_RIGHT_SPINDLE_RUN_TIME), 1, sizeof(DWORD), pf );

	fclose( pf ); pf = NULL;
	
	// does not work without the part above for some reason
	pa::PAMotion->RTDATA("spindle2runtime");
}

void pa::SAVE_TOTAL_LEFT_SPINDLE_RUN_TIME()
{
	TCHAR	*PATH = _T("..\\..\\Data\\EPnc\\rnd");
	FILE *pf = _tfopen( PATH, _T("wb") );

	if( pf == NULL )
		return ;

	fwrite( (void*)&(PPAStatus->GetThreadState()->dwTOTAL_LEFT_SPINDLE_RUN_TIME), 1, sizeof(DWORD), pf );

	fclose( pf ); pf = NULL;
	
	pa::PAMotion->WTDATA("spindle1runtime",(double)PPAStatus->GetThreadState()->dwTOTAL_LEFT_SPINDLE_RUN_TIME);
}

void pa::SAVE_TOTAL_RIGHT_SPINDLE_RUN_TIME()
{
	TCHAR	*PATH = _T("..\\..\\Data\\EPnc\\rnd2");
	FILE *pf = _tfopen( PATH, _T("wb") );

	if( pf == NULL )
		return ;

	fwrite( (void*)&(PPAStatus->GetThreadState()->dwTOTAL_RIGHT_SPINDLE_RUN_TIME), 1, sizeof(DWORD), pf );

	fclose( pf ); pf = NULL;
	
	pa::PAMotion->WTDATA("spindle2runtime",(double)PPAStatus->GetThreadState()->dwTOTAL_RIGHT_SPINDLE_RUN_TIME);
}

void pa::READ_TODAY_SPINDLE_RUN_TIME()
{
	SYSTEMTIME	stm;
	CString		filepath;

	GetLocalTime( &stm );
	filepath.Format( _T("%s\\%04d_%02d_%02d_spindle_runtime.log"),
		LOG_SPINDLE_PATH,
		stm.wYear, stm.wMonth, stm.wDay );

	FILE* pf = _tfopen( filepath, _T("rt") );

	if( pf == NULL ) 
	{
		pf = _tfopen( filepath, _T("wt") );
		todaySpindleRunTime = 0;
		pa::WRITE_TODAY_SPINDLE_RUN_TIME( stm );
	}
	else 
	{
		_ftscanf( pf, _T("%u"), &todaySpindleRunTime );

		currentReadWriteTime = stm;

		fclose( pf );
		pf = NULL;
	}
}

void pa::WRITE_TODAY_SPINDLE_RUN_TIME(SYSTEMTIME stm)
{
	CString		filePath;
	
	filePath.Format( _T("%s\\%04d_%02d_%02d_spindle_runtime.log"), LOG_SPINDLE_PATH, stm.wYear, stm.wMonth, stm.wDay );
	FILE *pf = _tfopen( filePath, _T("wt") );

	if( pf == NULL ) {
		return ;
	}

	_ftprintf( pf, _T("%u"), todaySpindleRunTime );

	fclose( pf ); 
	
	pf = NULL;
}

// keeping day ���� ������ ���� �Ѵ� 
void pa::DELETE_TODAY_SPINDLE_RUN_TIME(int nKeepingDays)
{
	pa::DELETE_OLD_FILE( nKeepingDays, _T("log"), LOG_SPINDLE_PATH );
}

void pa::CREATE_NECESSARY_DIRECTORIES()
{
	CreateDirectory( NCFILE_PATH, NULL );
	CreateDirectory( LOG_PATH, NULL );
	CreateDirectory( LOG_SPINDLE_PATH, NULL );
}

//////////////////////////////////////////////////////////////////////////
//

void pa::ShowConnectFailDialog( int nCount )
{
	CString strTitle;
	CString strErrMsg;
	CString strTemp;
	strTitle.Format( _T(" Connection Failure ") );

	strErrMsg.Format( _T("") );
	strTemp.Format( _T(" Failed to connect to the motion controller. (connection=%d) \r\n"), nCount ); strErrMsg += strTemp;
	strTemp.Format( _T("\r\n") ); strErrMsg += strTemp;
	strTemp.Format( _T(" Check the following: \r\n")); strErrMsg += strTemp;
	strTemp.Format( _T(" > Connection between the embedded PC and the controller,\r\n") ); strErrMsg += strTemp;
	strTemp.Format( _T(" > IP addresses of the embedded PC and the motion controller,\r\n") ); strErrMsg += strTemp;
	strTemp.Format( _T(" > correct initialization of the motion controller (blinking LEDs).\r\n") ); strErrMsg += strTemp;

	CErrorDlg dlg;
	dlg.SetTitle( strTitle );
	dlg.SetMessage( strErrMsg );
	dlg.SetButtonStatus( FALSE );
	dlg.DoModal();
}

int pa::ShowPowerOnTestFailDialog( DWORD dwErrCode, DWORD dwSysErrCode, int nRespErrCode, int nRespSysErrCode, BOOL coord_offset, BOOL teach_point )
{
	static CString ERR_MSG[] = {
		_T(" > Touch sensor1 error \r\n"),
		_T(" > Touch sensor2 error \r\n" ),
		_T(" > X Axis limit sensor error \r\n"),
		_T(" > Y Axis limit sensor error \r\n"),

		_T(" > Z Axis limit sensor error \r\n"),
		_T(" > I/O board communication error \r\n"),
		_T(" > ERR 002 \r\n"),
		_T(" > Colling pan error \r\n"),
		
		_T(" > Spindle driver Communication2 error \r\n"),
		_T(" > Spindle driver communication error \r\n"),
		_T(" > Controller error status \r\n"),
		_T(" > Main air limit error \r\n"),

		_T(" > Teaching point and system parameter out of range \r\n"),
		_T(" > License checking fail \r\n")
	};
	static CString ERR_MSG2[] = {
		_T(" > ERR 001\r\n"),
		_T(" > Acc/Dec data error\r\n"),
		_T(" > max speed error\r\n"),
		_T(" > axis direction error\r\n"),
		_T(" > origin speed error\r\n"),
		_T(" > ERR 002\r\n"),
		_T(" > origin direction error\r\n"),

		_T(" > ERR 004\r\n"),
		_T(" > ERR 005\r\n"),
		_T(" > ERR 006\r\n"),
		_T(" > ERR 007\r\n"),
		_T(" > ERR 008\r\n"),
		_T(" > sol timeout\r\n")
	};

	CString strTitle;
	CString strErrMsg;
	CString strTemp;
	DWORD	dwMask = 0x00000001;
	int		nShowDlg = 0;

	strTitle.Format( _T(" Power On Test Result ") );
	strErrMsg.Format( _T("") );

	P_LOG->WriteLog( CLog::TYPE_OPER, 0, _T("PowerOnTest Log") );

	if( dwErrCode != 0 )
	{
		if( dwErrCode == 99999 ) 
		{
			// ���� ���信 ���� E-nnn. nRespErrCode�� ���� �ڵ�
			strTitle.Format( _T("SMCT ERROR [%d]\n"), nRespErrCode );
			strErrMsg += strTitle;
			nShowDlg = 1;
		}
		else 
		{
			dwMask = 0x00000001;
			for( int i = 0; i < 14; i++ )
			{
				if( (dwErrCode & dwMask) == dwMask )
				{
					strErrMsg += ERR_MSG[i];
					if (i == 5)	// I/O Board Connection fail
					{
						pa::PPAStatus->GetThreadState()->nIsConnectedIOBoard = 0;
					}
					P_LOG->WriteLog( CLog::TYPE_OPER, 0, ERR_MSG[i] );
					nShowDlg = 1;
				}

				dwMask <<= 1;
			}
		}
	}

	if( dwSysErrCode != 0 )
	{
		if( dwSysErrCode == 99999 )
		{
			// ���� ������ E-nnn. nRespErrCode�� ���� �ڵ�
			strTemp.Format( _T("RISM ERROR [%d]\n"), nRespSysErrCode );
			P_LOG->WriteLog( CLog::TYPE_OPER, 0, strTemp );
			strErrMsg += strTemp;
			nShowDlg = 1;
		}
		else 
		{
			strTemp.Format( _T("\r\n") ); strErrMsg += strTemp;
			strTemp.Format( _T("System param error : %d\n\r"), dwSysErrCode ); strErrMsg += strTemp;
			P_LOG->WriteLog( CLog::TYPE_OPER, 0, strTemp );
			strErrMsg += ERR_MSG2[dwSysErrCode-1];
			P_LOG->WriteLog( CLog::TYPE_OPER, 0, ERR_MSG2[dwSysErrCode-1] );
			nShowDlg = 1;
		}
	}

	if( coord_offset == FALSE || teach_point == FALSE )
	{
		strTemp.Format( _T("system parameter error :\r\n") ); strErrMsg += strTemp;
		if( coord_offset == FALSE )
		{
			strTemp.Format( _T(" > coordinate offset range error \r\n") );
			P_LOG->WriteLog( CLog::TYPE_OPER, 0, strTemp );
			strErrMsg += strTemp;
			nShowDlg = 1;
		}

		if( teach_point == FALSE )
		{
			strTemp.Format( _T(" > teaching point range error \r\n") );
			P_LOG->WriteLog( CLog::TYPE_OPER, 0, strTemp );
			strErrMsg += strTemp;
			nShowDlg = 1;
		}
	}

	int nRet = IDOK;
	if( nShowDlg != 0 )
	{
		CErrorDlg dlg;
		dlg.SetTitle( strTitle );
		dlg.SetMessage( strErrMsg );
		dlg.SetButtonStatus( TRUE );
		nRet = dlg.DoModal();
	}

	return nRet;
}

void pa::ShowPowerOffDialog()
{
	CString strTitle;
	CString strErrMsg;

	strTitle.Format( _T(" Turn off power ") );
	strErrMsg.Format( _T(" > turn off power ") );

	CErrorDlg dlg;
	dlg.SetTitle( strTitle );
	dlg.SetMessage( strErrMsg );
	dlg.SetButtonStatus( FALSE );
	dlg.DoModal();
}

// Coordinate Offset ���� ������ �� �Ѵ�
BOOL pa::CHECK_COORD_OFFSET_RANGE()
{
	double	fx_min = pa::PPAStatus->GetCoordinateOffsetDataRange()->fMin[pa::AXIS_X];
	double	fx_max = pa::PPAStatus->GetCoordinateOffsetDataRange()->fMax[pa::AXIS_X];
	double	fy_min = pa::PPAStatus->GetCoordinateOffsetDataRange()->fMin[pa::AXIS_Y];
	double	fy_max = pa::PPAStatus->GetCoordinateOffsetDataRange()->fMax[pa::AXIS_Y];
	double	fz_min = pa::PPAStatus->GetCoordinateOffsetDataRange()->fMin[pa::AXIS_Z];
	double	fz_max = pa::PPAStatus->GetCoordinateOffsetDataRange()->fMax[pa::AXIS_Z];
	double	z_org_offset = 0.0; 

	// X, Y�� ��. 
	// �񱳴� G54���� 
	for( int i = (int)COORD_G54; i<(int)COORD_NUM; i++ )
	{
		double fx = pa::PConfig->pConfig_->fCoordOffset[i][pa::AXIS_X];
		double fy = pa::PConfig->pConfig_->fCoordOffset[i][pa::AXIS_Y];

		if( fx < fx_min || fx > fx_max ) {
			return FALSE;
		}
		if( fy < fy_min || fy > fy_max ) {
			return FALSE;
		}
	}

	return TRUE;
}

BOOL pa:: CHECK_TEACH_POINT_RANGE()
{
	int		nCheckPoint[] = 
	{
		TEACHING_POINT_LEFT_TOOL1, TEACHING_POINT_LEFT_TOOL2, TEACHING_POINT_LEFT_TOOL3, 
		TEACHING_POINT_RIGHT_TOOL4, TEACHING_POINT_RIGHT_TOOL5, TEACHING_POINT_RIGHT_TOOL6, TEACHING_POINT_READYPOS,
		TEACHING_POINT_SENSING_UP1, TEACHING_POINT_SENSING_DN1, TEACHING_POINT_SENSING_UP2, TEACHING_POINT_SENSING_DN2
	};
	double	fx_min	= pa::PPAStatus->GetPowerOnTestConfig()->toolpocket_x_min;
	double	fx_max	= pa::PPAStatus->GetPowerOnTestConfig()->toolpocket_x_max;
	double	fy_min	= pa::PPAStatus->GetPowerOnTestConfig()->toolpocket_y_min;
	double	fy_max	= pa::PPAStatus->GetPowerOnTestConfig()->toolpocket_y_max;

	if( pa::PPAStatus->GetPowerOnTestConfig()->IsLoaded == 0 )
	{
		return FALSE;
	}

	// Tool Pocket ��ġ�� ���� �ȿ� ����ִ��� Ȯ���Ѵ� 
	for( int i = 0; i<TEACHING_POINT_NUM; i++ )
	{
		double fx = pa::PConfig->pConfig_->fTeachingPoint[nCheckPoint[i]][pa::AXIS_X];
		double fy = pa::PConfig->pConfig_->fTeachingPoint[nCheckPoint[i]][pa::AXIS_Y];

		if( fx < fx_min || fx > fx_max ) {
			return FALSE;
		}
		if( fy < fy_min || fy > fy_max ) {
			return FALSE;
		}
	}

	return TRUE;
}
