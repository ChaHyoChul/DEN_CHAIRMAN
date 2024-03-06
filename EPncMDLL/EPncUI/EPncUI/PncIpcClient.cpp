#include "StdAfx.h"
#include "PncIpcClient.h"

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CPncIpcClient	*PPNC_IPC_CLIENT = NULL;

//////////////////////////////////////////////////////////////////////////

CPncIpcClient::CPncIpcClient(void)
{
	pIpcClient_ = NULL;
}

CPncIpcClient::~CPncIpcClient(void)
{
	Destroy();
}

BOOL CPncIpcClient::Initialize( CString& strErrMsg )
{
	pIpcClient_ = new hcipc::CIpcQueue();

	if( pIpcClient_ == NULL ) {
		strErrMsg.Format( _T("memory alloc error for CPIpcClient::pIpcClient_ object") );
		return FALSE;
	}

	CString strObjectName = pa::GET_OBJECT_NAME_WITH_TAG(IPC_SERVER_NAME);
// 	if( pIpcClient_->Create( IPC_SERVER_NAME, IPC_SERVER_Q_SIZE, sizeof(pa::SIpcCommCommand) ) == NULL ) {
	if( pIpcClient_->Create( (TCHAR*)(LPCTSTR)strObjectName, IPC_SERVER_Q_SIZE, sizeof(pa::SIpcCommCommand) ) == NULL ) {
		strErrMsg.Format( _T("create error for ipc client object") );
		return FALSE;
	}
	
	return TRUE;
}

void CPncIpcClient::Destroy()
{
	if( pIpcClient_ ) {
		pIpcClient_->Destroy();
		delete pIpcClient_;
		pIpcClient_ = NULL;
	}
}

void CPncIpcClient::Send( pa::SIpcCommCommand* pCommand )
{
	ASSERT( pIpcClient_ );
	ASSERT( pCommand );

	if( pIpcClient_ == NULL || pCommand == NULL ) {
		return ;
	}

	pIpcClient_->Write( (void*)pCommand );
}

//////////////////////////////////////////////////////////////////////////

void CPncIpcClient::Emergency()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd = pa::IPC_COMMAND_EMG;
	
	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::ErrorReset()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd = pa::IPC_COMMAND_RESET;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Home()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd = pa::IPC_COMMAND_HOME;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Open( int nIndex )
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_OPEN;
	memset((void*)(cmd.param.mp.param), 0, sizeof(char)*63);
	cmd.param.mp.param[0]	= (char)nIndex;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Close()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd = pa::IPC_COMMAND_CLOSE;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Run( int nStartLine )
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd = pa::IPC_COMMAND_RUN;
	memset((void*)(cmd.param.mp.param), 0, sizeof(char)*63);
	memcpy((void*)(cmd.param.mp.param), (const void*)&nStartLine, sizeof(int));	

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Pause()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd = pa::IPC_COMMAND_PAUSE;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Stop()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd = pa::IPC_COMMAND_STOP;

	pIpcClient_->Write( &cmd );
}

//////////////////////////////////////////////////////////////////////////

void CPncIpcClient::SendCommand( char* p, DWORD dwTimeout )
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );
	ASSERT( p );

	cmd.m_cmd			= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd	= pa::IPC_SUBCMD_SEND_COMMAND;
	//cmd.param.sp.param
// 	memset((void*)(cmd.param.sp.param), 0, sizeof(char)*62);
// 	memcpy_s((void*)(cmd.param.sp.param), 61, (const void*)p, sizeof(char)*strlen(p));
	memset((void*)(cmd.param.sp.param), 0, sizeof(char)*62);
	memcpy((void*)(cmd.param.sp.param), (void*)&dwTimeout, sizeof(DWORD));
	memcpy_s((void*)(cmd.param.sp.param + sizeof(DWORD)), 61-sizeof(DWORD), (const void*)p, sizeof(char)*strlen(p));

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::SendMDACommand( char* p )
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );
	ASSERT( p );

	cmd.m_cmd			= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd	= pa::IPC_SUBCMD_SEND_MDACOMMAND;
	//cmd.param.sp.param
	memset((void*)(cmd.param.sp.param), 0, sizeof(char)*62);
	memcpy_s((void*)(cmd.param.sp.param), 61, (const void*)p, sizeof(char)*strlen(p));

	pIpcClient_->Write( &cmd );
}

//////////////////////////////////////////////////////////////////////////

void CPncIpcClient::Servo( pa::EN_AXIS hAxis, BOOL bOnOff )
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_SERVO;
	cmd.param.sp.param[0]	= hAxis;
	cmd.param.sp.param[1]	= bOnOff==TRUE ? 1:0;

	pIpcClient_->Write( &cmd );
}

//////////////////////////////////////////////////////////////////////////

void CPncIpcClient::StartJog( pa::EN_AXIS hAxis, BOOL bDir, BOOL bStep, double fDist )
{
	pa::SIpcCommCommand cmd;
	
	ASSERT( pIpcClient_ );
	
	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_JOGSTART;
	cmd.param.sp.param[0]	= hAxis;
	cmd.param.sp.param[1]	= bDir ==TRUE ? 1:0;	// true:+ false:-
	cmd.param.sp.param[2]	= bStep==TRUE ? 1:0;	// step:1 cont:0
	sprintf_s( (char*)cmd.param.sp.param+3, 32, "%1.3f", fDist );
// 	while( pIpcClient_->Write( &cmd ) == 0 ) {
// 		Sleep(1);
// 	}
	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::StopJog( pa::EN_AXIS hAxis, BOOL dir )
{
	pa::SIpcCommCommand cmd;
	
	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_JOGSTOP;
	cmd.param.sp.param[0]	= hAxis;
	cmd.param.sp.param[1]	= dir==TRUE ? 1 : 0;

// 	while( pIpcClient_->Write( &cmd ) == 0 ) {
// 		Sleep(1);
// 	}
	pIpcClient_->Write( &cmd );
}

//////////////////////////////////////////////////////////////////////////

void CPncIpcClient::ToolClamp( BOOL bDirection, BOOL bClamp )
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_TOOLCLAMP;
	cmd.param.sp.param[0]	= bDirection;
	cmd.param.sp.param[1]	= bClamp ? 1:0;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::BlockClamp( BOOL bClamp )
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_BLOCKCLAMP;
	cmd.param.sp.param[0]	= bClamp ? 1:0;

	pIpcClient_->Write( &cmd );
}

//////////////////////////////////////////////////////////////////////////

// void CPncIpcClient::Melody( pa::EN_MELODY hMelody )
// {
// 	pa::SIpcCommCommand cmd;
// 
// 	ASSERT( pIpcClient_ );
// 
// 	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
// 	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_MELODY;
// 	cmd.param.sp.param[0]	= hMelody;
// 
// 	pIpcClient_->Write( &cmd );
// }

//////////////////////////////////////////////////////////////////////////

void CPncIpcClient::ATCDoor( BOOL bOpen )
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_ATCDOOR;
	cmd.param.sp.param[0]	= bOpen ? 1:0;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::ResetToolUsingTime( int nToolNo, BOOL resetCurrentTime, DWORD dwMaximumTime )
{
	pa::SIpcCommCommand cmd;
	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_TOOL_SET_TIME;
	cmd.param.sp.param[0]	= (char)nToolNo;
	cmd.param.sp.param[1]	= resetCurrentTime ? 1:0;

	char* p = (char*)cmd.param.sp.param+2;
	sprintf_s( (char*)p, 32, "%d", dwMaximumTime );

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::ToolErrorClear( int nToolNo )
{
	pa::SIpcCommCommand cmd;
	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_TOOL_RESET;
	cmd.param.sp.param[0]	= (char)nToolNo;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::ToolSetNumber( int nToolNo, int nTool2No )
{
	pa::SIpcCommCommand cmd;
	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_TOOL_SET_NUMBER;
	cmd.param.sp.param[0]	= (char)nToolNo;
	cmd.param.sp.param[1]	= (char)nTool2No;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::SetSpindleRPM( int newRPM )
{
	pa::SIpcCommCommand cmd;
	ASSERT( pIpcClient_ );
	
	pa::PPAStatus->GetPAStatus()->nSpindleSpeedSetting = newRPM;	//helper variable to store new spindle RPM setting
	
	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_SET_SPINDLE_SPEED;

	pIpcClient_->Write( &cmd );
}

// 사용 안함 
void CPncIpcClient::SaveConfig()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_SAVE_CONFIG;

	pIpcClient_->Write( &cmd );
}

//////////////////////////////////////////////////////////////////////////

void CPncIpcClient::UploadCoordinateOffset()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_UPLOAD_PARAM;
	cmd.param.sp.param[0]	= 0;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::UploadTeachingPoint()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_UPLOAD_PARAM;
	cmd.param.sp.param[0]	= 1;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::UploadOption()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_UPLOAD_PARAM;
	cmd.param.sp.param[0]	= 2;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::UploadFlowSensorData()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_UPLOAD_PARAM;
	cmd.param.sp.param[0]	= 4;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::UploadSoftLimit()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_UPLOAD_SOFTLIMIT;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::DownloadCoordinateOffset()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_DOWNLOAD_PARAM;
	cmd.param.sp.param[0]	= 0;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::DownloadTeachingPoint()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_DOWNLOAD_PARAM;
	cmd.param.sp.param[0]	= 1;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::DownloadOption()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_DOWNLOAD_PARAM;
	cmd.param.sp.param[0]	= 2;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::DownloadFlowSensorData()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_DOWNLOAD_PARAM;
	cmd.param.sp.param[0]	= 4;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::DownloadSoftLimit()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_DOWNLOAD_SOFTLIMIT;

	pIpcClient_->Write( &cmd );
}

//////////////////////////////////////////////////////////////////////////
// 2017.03.24 
void CPncIpcClient::UploadOperationM28()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_UPLOAD_OPERATION_M28;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::DownloadOperationM28()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_DOWNLOAD_OPERATION_M28;

	pIpcClient_->Write( &cmd );
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

// void CPncIpcClient::SaveParam()
// {
// 	pa::SIpcCommCommand cmd;
// 
// 	ASSERT( pIpcClient_ );
// 
// 	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
// 	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_SAVE_PARAM;
// 
// 	pIpcClient_->Write( &cmd );
// }

//////////////////////////////////////////////////////////////////////////

void CPncIpcClient::ALR_OpenDoor()
{
//	SendMDACommand( "M771" );
	SendCommand( "M771", 0 );
}

void CPncIpcClient::ALR_CloseDoor()
{
// 	SendMDACommand( "M772" );
	SendCommand( "M772", 0 );
}

void CPncIpcClient::ALR_Gripper_Grip()
{
// 	SendMDACommand( "M777" );
	SendCommand( "M777", 0 );
}

void CPncIpcClient::ALR_Gripper_Ungrip()
{
//	SendMDACommand( "M778" );
	SendCommand( "M778", 0 );
}

void CPncIpcClient::ALR_Hand_Forward()
{
//	SendMDACommand( "M774" );
	SendCommand( "M774", 0 );
}

void CPncIpcClient::ALR_Hand_Backward()
{
//	SendMDACommand( "M773" );
	SendCommand( "M773", 0 );
}

void CPncIpcClient::ALR_Rotate_Cassette()
{
// 	SendMDACommand( "M775" );
	SendCommand( "M775", 0 );
}

void CPncIpcClient::ALR_Rotate_XYStage()
{
//	SendMDACommand( "M776" );
	SendCommand( "M776", 0 );
}

void CPncIpcClient::ALR_GetBlockFromCassette( int nBlockNo )
{
// 	char sztemp[32];
// 	memset((void*)sztemp, 0, sizeof(char)*32);
// 	sprintf_s( sztemp, 32, "P5101=%d", nBlockNo);
// 	TerminalCommand( sztemp );
// 	Sleep(10);
// 	MacroCommand( "M751" );
	char szCmd[64];
	sprintf_s( szCmd, 64, "M751 %d", nBlockNo );
	SendCommand( szCmd, 0 );
}

void CPncIpcClient::ALR_PutBlockToCassette( int nBlockNo )
{
// 	char sztemp[32];
// 	memset((void*)sztemp, 0, sizeof(char)*32);
// 	sprintf_s( sztemp, 32, "P5101=%d", nBlockNo);
// 	TerminalCommand( sztemp );
// 	Sleep(10);
// 	MacroCommand( "M752" );
	char szCmd[64];
	sprintf_s( szCmd, 64, "M752 %d", nBlockNo );
	SendCommand( szCmd, 0 );
}

void CPncIpcClient::ALR_LoadingBlockToXYStage( int nLoadUnload )
{
// 	MacroCommand( "M785" );
// 	SendCommand( "M785", 0 );
	char szCmd[64];
	sprintf_s( szCmd, 64, "M785 %d", nLoadUnload );
	SendCommand( szCmd, 0 );
}

void CPncIpcClient::ALR_ReturnArmToAcssette( int nLoadUnload )
{
// 	MacroCommand( "M786" );
//	SendCommand( "M786", 0 );
	char szCmd[64];
	sprintf_s( szCmd, 64, "M786 %d", nLoadUnload );
	SendCommand( szCmd, 0 );
}

void CPncIpcClient::ALR_ExtendArmToStage( int nLoadUnload )
{
// 	MacroCommand( "M785" );
// 	SendCommand( "M785", 0 );
	char szCmd[64];
	sprintf_s( szCmd, 64, "M785 %d", nLoadUnload );
	SendCommand( szCmd, 0 );
}

void CPncIpcClient::ALR_BlockSensingPosition( int nBlockNo )
{
	// M753 
	char szCmd[64];
	sprintf_s( szCmd, 64, "M753 %d", nBlockNo );
	SendCommand( szCmd, 0 );
}

void CPncIpcClient::ALR_BlockBarcodeReadPosition( int nBlockNo )
{
	// M754
	char szCmd[64];
	sprintf_s( szCmd, 64, "M754 %d", nBlockNo );
	SendCommand( szCmd, 0 );
}

void CPncIpcClient::ALR_ReadBarcode()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUNCMD_READ_BARCODE;

	pIpcClient_->Write( &cmd );
}

//////////////////////////////////////////////////////////////////////////

void CPncIpcClient::XYS_Gripper_Grip()
{
// 	MacroCommand( "M781" );
	SendCommand( "M781", 0 );
}

void CPncIpcClient::XYS_Geipper_Ungrip()
{
// 	MacroCommand( "M782" );
	SendCommand( "M782", 0 );
}

void CPncIpcClient::XYS_Gripper_Up()
{
// 	MacroCommand( "M779" );
	SendCommand( "M779", 0 );
}

void CPncIpcClient::XYS_Gripper_Donw()
{
// 	MacroCommand( "M780" );
	SendCommand( "M780", 0 );
}

void CPncIpcClient::XYS_MoveBlockLoadPos( int nLoadUnload )
{
// 	MacroCommand( "M783" );
//	SendCommand( "M783", 0 );
	char szCmd[32];
	sprintf_s( szCmd, 32, "M783 %d", nLoadUnload );
	SendCommand( szCmd, 0 );
}

void CPncIpcClient::XYS_MoveBlockWorkPos()
{
// 	MacroCommand( "M784" );
	SendCommand( "M784", 0 );
}

void CPncIpcClient::MoveReadyPosition()
{
// 	SendMDACommand( "M801" );
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUNCMD_MOVE_READYPOS;

	pIpcClient_->Write( &cmd );
}


void CPncIpcClient::SetHomeOffset()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_SET_HOME_OFFSET;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::SetHomeOffsetAB()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_SET_HOME_OFFSET_AB;

	pIpcClient_->Write( &cmd );
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void CPncIpcClient::SaveLeftToolTotalTime()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_SAVE_LEFT_TOOL_TOTAL_TIME;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::SaveRightToolTotalTime()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_SAVE_RIGHT_TOOL_TOTAL_TIME;

	pIpcClient_->Write( &cmd );
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void CPncIpcClient::Start_AutoCal_CoordinateOffset()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_START_AUTOCAL_COORDINATE_OFFSET;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Stop_AutoCal_CoordinateOffset()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_STOP_AUTOCAL_COORDINATE_OFFSET;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Start_SA_AutoCal_CoordinateOffset_A()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_START_SA_AUTOCAL_COORDINATE_OFFSET_A;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Stop_SA_AutoCal_CoordinateOffset_A()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_STOP_SA_AUTOCAL_COORDINATE_OFFSET_A;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Start_SA_AutoCal_CoordinateOffset_XYZ()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_START_SA_AUTOCAL_COORDINATE_OFFSET;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Stop_SA_AutoCal_CoordinateOffset_XYZ()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_STOP_SA_AUTOCAL_COORDINATE_OFFSET;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Start_AutoTeach_ToolPocket()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_START_AUTOTEACH_TOOLPOCKET;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Stop_AutoTeach_ToolPocket()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_STOP_AUTOTEACH_TOOLPOCKET;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::CleanRoom( BOOL bOn )
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_CLEANROOM;
	cmd.param.sp.param[0]	= bOn ? 1:0;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::ChangeIPAddress( char cBoard )
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_CHANGE_IP_ADDRESS;
	cmd.param.sp.param[0]	= cBoard;

	pIpcClient_->Write( &cmd );
}

// ip주소를 읽어서 저장한다 
void CPncIpcClient::UploadIPAddress()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_UPLOAD_IP_ADDRESS;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::TestLCD()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_TEST_LCD;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::UpdateLCD()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_UPDATE_LCD;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Start_ATCTest()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_START_ATC_TEST;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::Stop_ATCTest()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_STOP_ATC_TEST;

	pIpcClient_->Write( &cmd );
}

// 원점 재설정 기능
// => 함수 호출 후 500msec 이후 부터 
// => PAThreadStatus::nCompleteOriginComplete == True 일때 까지 대기 한다 
void CPncIpcClient::ResetOrigin( int axis_no )
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_RESET_ORIGIN;
	cmd.param.sp.param[0]	= axis_no;

	pIpcClient_->Write( &cmd );
}


void CPncIpcClient::ReturnItems()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;

	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_RETURN_ITEMS;

	pIpcClient_->Write( &cmd );
}

//

void CPncIpcClient::SendSALF()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_SEND_SALF;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::SendSFSF()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_SEND_SFSF;

	pIpcClient_->Write( &cmd );
}

void CPncIpcClient::SendWPAR()
{
	pa::SIpcCommCommand cmd;

	ASSERT( pIpcClient_ );

	cmd.m_cmd				= pa::IPC_COMMAND_MANUAL;
	cmd.param.sp.s_cmd		= pa::IPC_SUBCMD_SEND_WPAR;

	pIpcClient_->Write( &cmd );
}

