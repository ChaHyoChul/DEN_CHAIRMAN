#include "StdAfx.h"
#include "PAMotion.h"
#include "EPncMDLL.h"

//////////////////////////////////////////////////////////////////////////
//
BOOL pa::CPAMotion::NOT_CHECK_STATUS = FALSE;
BOOL pa::CPAMotion::STREAM_MODE_ERROR= FALSE;

//////////////////////////////////////////////////////////////////////////



pa::CPAMotion::CPAMotion(void)
{
	InitializeCriticalSection( &hCS_ );

	hSockStream_	= INVALID_SOCKET;
	dwStreamTimeout_= 2000;
}

pa::CPAMotion::~CPAMotion(void)
{
	DeleteCriticalSection( &hCS_ );
}

// 1:RND_HALT
void pa::CPAMotion::HALT()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_HALT );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_HALT );
}

// 0:RND_RST
void pa::CPAMotion::RST()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_RST );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_RST, 30*1000 );
}

// 0:RND_INIT
void pa::CPAMotion::INIT()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_INIT );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_INIT );
}

// 0:RND_MODE
void pa::CPAMotion::MODE( CPAAsyncComm::EN_PA_RUNMODE hRunMode )
{
	char*	P_PA_RUNMODE[] = { "OFF", "AUTO", "STEP", "MDA" };

	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_MODE, P_PA_RUNMODE[hRunMode] );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_MODE );
}

// 0:RND_STREAM
void pa::CPAMotion::STREAM()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_STREAM );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_STREAM );
}

void pa::CPAMotion::PAUSE_MCODE_THREAD()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_MCODE_THREAD_PAUSE );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_MCODE_THREAD_PAUSE );
}

void pa::CPAMotion::RESUME_MCODE_THREAD()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_MCODE_THREAD_RESUME );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_MCODE_THREAD_RESUME );
}

// 1:RND_STOP
void pa::CPAMotion::STOP( int is_stream /*=0*/ )
{
	try {
		if( is_stream == 0 ) 
		{
			PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RND_STOP );
			PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RND_STOP, 30000 );
		}
		else 
		{
			PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RND_STOP, "1" );
			PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RND_STOP, 30000 );
		}
	}
	catch( CPException& e ) {
		throw e;
	}
}

// 1:RND_PAUSE
void pa::CPAMotion::PAUSE()
{
	try {
		PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RND_PAUSE );
		PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RND_PAUSE, 120*1000 );
		TRACE( _T("void pa::CPAMotion::PAUSE()\n") );
	}
	catch( CPException& e ) {
		throw e;
	}
}

// 0:RND_CONTINUE
void pa::CPAMotion::CONTINUE()
{
	try {
		PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_CONTINUE );
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_CONTINUE );
		TRACE( _T("void pa::CPAMotion::CONTINUE()\n") );
	}
	catch( CPException& e ) {
		throw e;
	}
}

// 1:RND_STATUS
void pa::CPAMotion::STATUS()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RND_STATUS );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RND_STATUS, 5000 );
}

// 0:RND_MDA
pa::CPAAsyncComm::EN_COMMAND pa::CPAMotion::MDA( BOOL blocking, char* pCommand )
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_MDA, pCommand );
	if( blocking ) 
	{
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_MDA );
	}
	return CPAAsyncComm::CMD_RND_MDA;
}

// 0:RND_ATTACH
void pa::CPAMotion::ATTACH( int mode )
{
	char param[32];
	if( mode == 0 ) {
		sprintf_s( param, 32, "0" );
	} else {
		sprintf_s( param, 32, "", mode );
	}

	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_ATTACH, param );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_ATTACH );
}

// 0:RND_COMMAND => 사용 안함
pa::CPAAsyncComm::EN_COMMAND pa::CPAMotion::COMMAND( BOOL blocking, char* pcommand )
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_COMMAND, pcommand );
	if( blocking )
	{
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_COMMAND, 0 );
	}
	return CPAAsyncComm::CMD_RND_COMMAND;
}

// 0:RND_HOME
pa::CPAAsyncComm::EN_COMMAND pa::CPAMotion::HOME( BOOL blocking )
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_HOME );
	if( blocking ) 
	{
		// 2016.09.21. 
		//	4WA 장비가 INC 모터를 쓰는 관계로 Timeout 이 필요. 
		//	기존에는 INFINITE를 썼지만, 그럴 수 없어 시간을 크게 입력 (3분)
		// PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_HOME, 180*1000 );
		//	기존에는 INFINITE를 썼지만, 그럴 수 없어 시간을 크게 입력 (5분)
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_HOME, 300*1000 );
	}
	return CPAAsyncComm::CMD_RND_HOME;
}

// 0:RND_ORG
pa::CPAAsyncComm::EN_COMMAND pa::CPAMotion::ORG( BOOL blocking )
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_ORG );
	if( blocking )
	{
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_ORG );
	}
	return CPAAsyncComm::CMD_RND_ORG;
}

// 0:RND_BLCHK	- check block length for chairman
void pa::CPAMotion::BLCHK()
{
	char param[64];
	int checkSpeed = PConfig->pConfig_->nCheckBlockSizeSpeed;
	int checkTorque = PConfig->pConfig_->nCheckBlockSizeTorque;
	int timeout = 60 * 1000;	// PA timeout is 40 seconds
	
	if( checkSpeed != 0 && checkTorque != 0 )
	{
		sprintf_s( param, 64, "%d,%d", checkSpeed, checkTorque );
		PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_BLCHK, param );
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_BLCHK, timeout );
	}
	else
	{
		PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_BLCHK );
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_BLCHK, timeout );
	}
}

// 0:JOG 
//	> jog_mode : step / cont 
void pa::CPAMotion::JOG( EN_AXIS hAxis, BOOL bDir, BOOL bStep, double fStepDist )
{
	char param[64];

	if( !bStep ) {
		// Continuous
		sprintf_s( param, 64, "%d,%d", (int)(hAxis+1), (bDir ? 1 : -1) );
		PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_JOG, param );
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_JOG );
	}
	else {
		// Step 
		double fPos[pa::AXIS_NUM+1] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };	// AutoLoader 축 추가. C축 
		fPos[hAxis] = fStepDist * ( bDir ? 1.0 : -1.0 );
		switch( pa::MODEL_INFO.GetNumAxis() )
		{
		case 3:
			sprintf_s( param, 64, "%.3f,%.3f,%.3f", fPos[0], fPos[1], fPos[2] );
			break;
		case 4:
			sprintf_s( param, 64, "%.3f,%.3f,%.3f,%.3f", fPos[0], fPos[1], fPos[2], fPos[3] );
			break;
		case 5:
			sprintf_s( param, 64, "%.3f,%.3f,%.3f,%.3f,%.3f", fPos[0], fPos[1], fPos[2], fPos[3], fPos[4] );
			break;
		default:
			return ;
		}
		PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_MMI, param );
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_MMI );
	}
}

void pa::CPAMotion::JSTOP()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_JSTOP );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_JSTOP );
}

// 0:Read Jog Speed 
void pa::CPAMotion::RJSS()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RJSS );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RJSS );
}

// 0:Set Jog Speed
void pa::CPAMotion::WJSS( int jog_speed )
{
	char param[64];

	sprintf_s( param, 64, "%d", jog_speed );
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_WJSS, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_WJSS );
	//////////////////////////////////////////////////////////////////////////
	// 변수에 저장 
// 	CPAAsyncComm::N_TEMP_JOG_SPEED = jog_speed;
	pa::PPAStatus->GetThreadState()->nJogSpeed_ = jog_speed;
	//////////////////////////////////////////////////////////////////////////
}

// 0:Get Coordinate Offset 
void pa::CPAMotion::RCFG( EN_COORDINATE hCoordNo )
{
	char param[64];

	sprintf_s( param, 64, "%d", 53+hCoordNo );
	CPAAsyncComm::H_TEMP_COORDINATE_NO = hCoordNo; 
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RCFG, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RCFG );
}

// 0:Set Coordinate Offset 
void pa::CPAMotion::WCFG( int coordno, double val[] )
{
	char param[128];

	sprintf_s( param, 128, "1,%d,%.3f,%.3f,%.3f,0.0,0.0,0.0,%.3f,%.3f", 53+coordno, val[0], val[1], val[2], val[3], val[4] );
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_WCFG, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_WCFG );
	//////////////////////////////////////////////////////////////////////////
	// 변수에 저장 
	for( int i = 0; i<pa::MODEL_INFO.GetNumAxis(); i++ ) {
		PConfig->pConfig_->fCoordOffset[coordno][i] = val[i];
	}
	//////////////////////////////////////////////////////////////////////////
}

// 0:Get Teaching Point 
void pa::CPAMotion::RTCP( EN_TEACHING_POINT hTeachingPoint )
{
	char param[32];
	int  tp_no = N_TEACHING_POINT_NO[(int)hTeachingPoint];

	//////////////////////////////////////////////////////////////////////////
	// 2016.09.06.
	//	ready position은 업로드 하지 않는다 (B축이 <0일 경우, A축이 -180 으로 바뀌는 현상 있음) 
	/*
	enum EN_TEACHING_POINT 
	{
	TEACHING_POINT_TOOL1 = 0,		// point no. 1
	TEACHING_POINT_TOOL2,			// point no. 2
	TEACHING_POINT_TOOL3,			// point no. 3
	TEACHING_POINT_TOOL4,			// point no. 4
	TEACHING_POINT_TOOL5,			// point no. 5
	TEACHING_POINT_TOOL6,			// point no. 6
	TEACHING_POINT_TOOL7,			// point no. 7
	TEACHING_POINT_TOOL8,			// point no. 8
	TEACHING_POINT_READYPOS,		// point no. 9
	TEACHING_POINT_BLOCK_LOADER,	// point no. 10	=> autoloader 작업 영역. 추가  
	TEACHING_POINT_BLOCK_UNLOADER,	// point no. 11	=> autoloader 작업 영역. 추가  
	TEACHING_POINT_BLOCK_WORK,		// point no. 12
	TEACHING_POINT_SENSING_UP,		// point no. 19
	TEACHING_POINT_SENSING_DN,		// point no. 20
	TEACHING_POINT_NUM
	};
	*/
	//////////////////////////////////////////////////////////////////////////
	// 2016.09.06.
	//	ready position은 업로드 하지 않는다 (B축이 <0일 경우, A축이 -180 으로 바뀌는 현상 있음) 
	// 2017.05.16 
	//	block-loader, block-work 위치도 업로드 하지 않는다 
	if( tp_no == 9 ) //|| tp_no == 10 || tp_no == 11 ) 
	{
		return ;
	}

	//////////////////////////////////////////////////////////////////////////

	sprintf_s( param, 32, "%d", tp_no );

	CPAAsyncComm::H_TEMP_TEACHING_POINT_NO = hTeachingPoint;
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RTCP, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RTCP );
}

// 0:Set Teaching Point 
void pa::CPAMotion::WTCP( int index, int tp_no, double val[] )
{
	char param[256];

	sprintf_s( param, 256, "%d,%.3f,%.3f,%.3f,%.3f,%.3f", tp_no, val[0], val[1], val[2], val[3], val[4] );

	//////////////////////////////////////////////////////////////////////////
	// 2016.09.06.
	//	ready position은 다운로드 하지 않는다 (B축이 <0일 경우, A축이 -180 으로 바뀌는 현상 있음) 
	// 2017.05.16 
	//	block-loader, block-work 위치도 다운로드 하지 않는다 	
	if( tp_no != 9 ) //&& tp_no !=10 && tp_no != 11 )
	{
		PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_WTCP, param );
		PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_WTCP );
	}
	else
	{
		// 파일에 저장한다 
		TCHAR*		pFilePath =  INI_TEACHING_POINT_PATH;
		CString		strAxisName[] = { _T("_X"), _T("_Y"), _T("_Z"), _T("_A"), _T("_B") }; 
		CCEIniFile	hIniFile;
		CString		strValueName;

		if( hIniFile.Open( pFilePath ) ) {
			for( int j = 0; j<pa::AXIS_NUM; j++ ) {
				strValueName = pa::STR_TEACHING_POINT[pa::TEACHING_POINT_READYPOS] + strAxisName[j];
				hIniFile.SetValue( _T("TeachingPoint"), strValueName, (double)(pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_READYPOS][j]) );
			}		
			hIniFile.Close();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 변수에 저장 
	for( int i = 0; i<pa::MODEL_INFO.GetNumAxis(); i++ ) {
		PConfig->pConfig_->fTeachingPoint[index][i] = val[i];
	}
	//////////////////////////////////////////////////////////////////////////
}

// 0:Get Z-Offset 
void pa::CPAMotion::RZOO()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RZOO );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RZOO );
}

void pa::CPAMotion::RTOO(int nDirection)
{
	char param[64];

	sprintf_s( param, 64, "%d", nDirection);
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RTOO, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RTOO );
}

void pa::CPAMotion::RTDATA(char* szDataname)
{
	char param[64];

	sprintf_s( param, 64, "%s", szDataname );
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RTDATA, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RTDATA );
}

// 0:Set Z-Offset 
void pa::CPAMotion::WZOO( double z_offset )
{
	char param[64];

	sprintf_s( param, 64, "%.3f", z_offset );
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_WZOO, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_WZOO );
	//////////////////////////////////////////////////////////////////////////
	// 변수에 저장 
	PConfig->pConfig_->fOptionData[OPTION_Z1AXIS_ORIGIN_OFFSET] = z_offset;
	//////////////////////////////////////////////////////////////////////////
}

void pa::CPAMotion::WTOO( int nDirection, double z_offset )
{
	char param[64];

	sprintf_s( param, 64, "%d,%.3f", nDirection, z_offset );
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_WTOO, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_WTOO );

	if (nDirection == 1)
	{
		PConfig->pConfig_->fOptionData[OPTION_Z1AXIS_ORIGIN_OFFSET] = z_offset;
	}

	else if (nDirection == 2)
	{
		PConfig->pConfig_->fOptionData[OPTION_Z2AXIS_ORIGIN_OFFSET] = z_offset;
	}
}

void pa::CPAMotion::WTDATA( char* szDataname, double dwData )
{
	char param[64];

	sprintf_s( param, 64, "%s,%.3f", szDataname, dwData );
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_WTDATA, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_WTDATA );

	if (strcmp(szDataname, "spindleoffset") == 0)
	{
		PConfig->pConfig_->fOptionData[OPTION_SPINDLE_OFFSET] = dwData;

	}
}

// 0:Get Tool Sensing High Speed 
void pa::CPAMotion::RTHS()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RTHS );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RTHS );
}

// 0:Set Tool Sensing High Speed 
void pa::CPAMotion::WTHS( int high_speed )
{
	char param[64];

	sprintf_s( param, 64, "%d", high_speed );
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_WTHS, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_WTHS );
	//////////////////////////////////////////////////////////////////////////
	// 변수에 저장 
	PConfig->pConfig_->fOptionData[OPTION_TOOL_SENSING_HIGHSPEED] = high_speed;
	//////////////////////////////////////////////////////////////////////////
}

// 0:Get Tool Sensing Low Speed
void pa::CPAMotion::RTLS()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RTLS );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RTLS );
}

// 0:Set Tool Sensing Low Speed 
void pa::CPAMotion::WTLS( int low_speed )
{
	char param[64];

	sprintf_s( param, 64, "%d", low_speed );
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_WTLS, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_WTLS );
	//////////////////////////////////////////////////////////////////////////
	// 변수에 저장 
	PConfig->pConfig_->fOptionData[OPTION_TOOL_SENSING_LOWSPEED] = low_speed;
	//////////////////////////////////////////////////////////////////////////
}

// 0:Output Signal 
void pa::CPAMotion::IOT( int bit, int signal )
{
	char param[64];

	bit = pa::SPAStatus::OUTPUT_BIT_NO[bit];

	sprintf_s( param, 64, "%d,%d", bit, signal );
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_IOT, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_IOT, 3000 );
}

// 1:RND_STATE 
void pa::CPAMotion::RND_STATE()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RND_STATE );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RND_STATE, 5000 ); // 0 );		// 2017.05.20. 0으로 했을 경우, 응답이 없으면 무한 대기 한다  0 );
																			// - 5초로 수정했을때 RND_STATUS[8]에서 timeout 에러가 발생해서 10초로 수정해 본다 
																			// - 10초로 수정했을때 RND_STATUS[8]에서 timeout 에러가 발생해서 30초로 수정해 본다 
																			// - 30초로 수정했을때 RND_STATUS[8]에서 timeout 에러가 발생해서 0초로 수정해 본다 
																			// - log 수정 후 10초로 변경 
}

// 1:RND_AES
void pa::CPAMotion::RND_AES()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RND_AES );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RND_AES );
}

// 1:RND_ASS
void pa::CPAMotion::RND_ASS()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RND_ASS );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RND_ASS );
}

// 0:RND_MMA
pa::CPAAsyncComm::EN_COMMAND pa::CPAMotion::RND_MMA( BOOL blocking, double position[] )
{
	char param[128];

	sprintf_s( param, 128, "%.3f,%.3f,%.3f,%.3f,%.3f", position[0], position[1], position[2], position[3], position[4] );
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_MMA, param );
	if( blocking )
	{
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_MMA );
	}

	return CPAAsyncComm::CMD_RND_MMA;
}

// 0:RND_MMI
pa::CPAAsyncComm::EN_COMMAND pa::CPAMotion::RND_MMI( BOOL blocking, double position[] )
{
	char param[128];

	sprintf_s( param, 128, "%.3f,%.3f,%.3f,%.3f,%.3f", position[0], position[1], position[2], position[3], position[4] );
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_MMI, param );
	if( blocking )
	{
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_MMI );
	}

	return CPAAsyncComm::CMD_RND_MMI;
}

// 0:DO_MEASURE
pa::CPAAsyncComm::EN_COMMAND pa::CPAMotion::DO_MEASURE( 
	BOOL blocking, pa::EN_AXIS hAxis, 
	double in_pitch, double out_pitch, int speed_for_measure_pos, int measure_count, 
	double f1st_max_distance, double f2st_measure_offset )
{
	char	param[256];
	int		no_axis = (int)hAxis + 1;

	sprintf_s( param, 256, "%d,%.3f,%.3f,%d,%d,%.3f,%.3f", no_axis, in_pitch, out_pitch, speed_for_measure_pos, measure_count, f1st_max_distance, f2st_measure_offset );
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_DO_MEASURE, param );
	if( blocking )
	{
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_DO_MEASURE );
	}

	CString strDbg;
	strDbg.Format( _T("%d SEND : DO_MEASURE\n"), GetTickCount() );
	TRACE( strDbg );

	CString strLog;
	strLog.Format( _T("do_measure : %d,%.3f,%.3f,%d,%d,%.3f,%.3f"), 
		no_axis, in_pitch, out_pitch, speed_for_measure_pos, measure_count, f1st_max_distance, f2st_measure_offset );
	PThread->writeLog_AutoCal( strLog, TRUE );

	return CPAAsyncComm::CMD_RND_DO_MEASURE;
}

pa::CPAAsyncComm::EN_COMMAND pa::CPAMotion::SCAL(
	BOOL blocking, pa::EN_AXIS hAxis,
	int spindle_no, double in_pitch, double out_pitch, int speed_for_measure_pos, int measure_count,
	double f1st_max_distance, double f2st_measure_offset)
{
	char	param[256];
	int		no_axis = (int)hAxis + 1;

	sprintf_s( param, 256, "%d,%d,%.3f,%.3f,%d,%d,%.3f,%.3f", spindle_no, no_axis, in_pitch, out_pitch, speed_for_measure_pos, measure_count, f1st_max_distance, f2st_measure_offset );
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_SCAL, param );
	if( blocking )
	{
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_SCAL );
	}

	CString strDbg;
	strDbg.Format( _T("%d SEND : SCAL\n"), GetTickCount() );
	TRACE( strDbg );

	CString strLog;
	strLog.Format( _T("SCAL : %d,%d,%.3f,%.3f,%d,%d,%.3f,%.3f"), 
		spindle_no, no_axis, in_pitch, out_pitch, speed_for_measure_pos, measure_count, f1st_max_distance, f2st_measure_offset );
	PThread->writeLog_AutoCal( strLog, TRUE );

	return CPAAsyncComm::CMD_RND_SCAL;
}

// 0:GET_MEASURE_RESULT
//	> 결과 데이터는 Receive 함수에서 저장한다 
//	> SThreadState::fTempMeasureResult  
void pa::CPAMotion::GET_MEASURE_RESULT()
{
	CPAAsyncComm::F_TEMP_MEASURE_RESULT = 0.0;	// 초기화 

	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RND_GET_MEASURE_RESULT );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RND_GET_MEASURE_RESULT );

	CString strLog;
	strLog.Format( _T("measure result : %.5f"), CPAAsyncComm::F_TEMP_MEASURE_RESULT );
	PThread->writeLog_AutoCal( strLog, TRUE );
}

// Get Tool Sensing Margin
void pa::CPAMotion::RTMG()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RTMG );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RTMG );
}

// Set Tool Sensing Margin
void pa::CPAMotion::WTMG( double fval )
{
	char param[64];

	sprintf_s( param, 64, "%.3f", fval );
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_WTMG, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_WTMG );
	//////////////////////////////////////////////////////////////////////////
	// 변수에 저장 
	PConfig->pConfig_->fOptionData[OPTION_TOOL_SENSING_MARGIN] = fval;
	//////////////////////////////////////////////////////////////////////////
}

// Get S/W Limit+ 
void pa::CPAMotion::RMAXL()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RMAXL );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RMAXL );
}

// Set Soft-Limit 
void pa::CPAMotion::WMAXL( int axis, double fmax, double fmin )
{
	char param[256];

	sprintf_s( param, 256, "%d,%.3f,%.3f", axis, fmax, fmin );

	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_WMAXL, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_WMAXL, 10000 ); // 0 );
}

// Get S/W Limit-
void pa::CPAMotion::RMINL()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RMINL );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RMINL );
}

void pa::CPAMotion::RENABLE()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RENABLE );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RENABLE, 30000 ); //0 );
}

void pa::CPAMotion::RDISABLE()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RDISABLE );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RDISABLE, 30000 ); //0 );
}

void pa::CPAMotion::S_INIT()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_S_INIT );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_S_INIT );
}

void pa::CPAMotion::toolReturn_148()
{
	SendMotionCommand( CPAAsyncComm::CMD_RND_M148, FALSE );
}

// Spindle Stop
void pa::CPAMotion::M05()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_M05 );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_M05 );
}

// 집진기/물펌프 Stop
void pa::CPAMotion::M29()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_M29 );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_M29 );
}

//
void pa::CPAMotion::RND_SUHO()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_SUHO );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_SUHO );
}

//
void pa::CPAMotion::RND_SABHO()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_SABHO );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_SABHO );
}

// 
void pa::CPAMotion::VER()
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_VER );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_VER );
}

//
void pa::CPAMotion::RND_STIN( int tool_no, double tool_length, int tool_length_update_flag )
{
	char	param[64];

	sprintf_s( param, 64, "%d,%.5f,%d", tool_no, tool_length, tool_length_update_flag );

	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RND_STIN, param );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RND_STIN );
}

//
void pa::CPAMotion::WDSSZ( double disk_thiness )
{
	char	param[32];

	sprintf_s( param, 32, "%.3f", disk_thiness );

	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_WDSSZ, param );
	PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_WDSSZ );
}

//
void pa::CPAMotion::SORZ( BOOL blocking )
{
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_SORZ );

	if( blocking )
	{
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_SORZ, 30000 );	// 30초 
	}
}

//////////////////////////////////////////////////////////////////////////
// 동작이 완료면 TRUE
BOOL pa::CPAMotion::MotionDone( CPAAsyncComm::EN_COMMAND hCommand, BOOL isMotorStop )
{
	BOOL bRet = TRUE;
	bRet = PPAAsyncComm[0]->IsComplete(hCommand);		// !B_IS_RUNNING_COMPLETE[hCommand]; SEND : TRUE, RECV : FALSE
	return bRet;
}

// 모터 Stop 일 때, TRUE
BOOL pa::CPAMotion::IsMotorStop()
{
	BOOL bRet = (pa::PPAStatus->GetPAStatus()->nMotorMovingFlag == 0) ? TRUE : FALSE;
	return bRet;
}

void pa::CPAMotion::EMO( EN_PNC_ERR hErrCode )
{
	CString strDbg;

	//////////////////////////////////////////////////////////////////////////
	// RST 명령이 실행중이면 리턴 한다 
	if( CPAAsyncComm::B_IS_RUNNING_COMPLETE[CPAAsyncComm::CMD_RND_RST] == TRUE ) 
	{
		return ;
	}
	//////////////////////////////////////////////////////////////////////////

	if( PPAStatus->GetThreadState()->hRunMode == pa::RUNMODE_RUN ) 
	{
		int nCurrNCFileIndex = PNCFileMgr->GetCurrentWorkNCFileIndex();
		int nTempLineNo = 0;
		if( nCurrNCFileIndex != -1 ) {
			// 현재 라인번호 저장 
			nTempLineNo = PPAStatus->GetThreadState()->nCurrentNCCodeStepNo;
			pa::PThread->SET_ERROR_LINENO( nTempLineNo );
		}
		
		pa::PThread->bIsEMOError_ = TRUE;	// just for NC file management

		STOP( 1 );
		Sleep( 50 );

		STOP( 1 );
		Sleep( 50 );
	}
	else
	{
		STOP();
		Sleep( 50 );
		
		STOP();
		Sleep( 50 );
	}
	
	// Servo-Off 
	RDISABLE();

	throw CPException( pa::ERR_PNC, hErrCode, _T("") );
}

// 제어기 에러 리셋 
void pa::CPAMotion::ErrorReset()
{
	CString strLog;

	ASSERT( PPAAsyncComm[0] );
	ASSERT( PPAAsyncComm[1] );

	int nRetryCount = 0;
	bool reconnect = false;

	/////////////////////////////////
	// NEW, DOING ALL IN ONE LOOP
	DWORD dwTime = GetTickCount();
	while(PPAAsyncComm[0]->GetConnectState() != hcsock::ISocket::CONNECTED || PPAAsyncComm[1]->GetConnectState() != hcsock::ISocket::CONNECTED)
	{
		reconnect = true;
		hcsock::ISocket::EN_CONNECT_STATE conn_status0 = PPAAsyncComm[0]->GetConnectState();
		hcsock::ISocket::EN_CONNECT_STATE conn_status1 = PPAAsyncComm[1]->GetConnectState();
		
		// Timeout Checking
		if( (GetTickCount() - dwTime) > 20*1000 ){
			///////////LOG////////////
			CString logMsg;
			logMsg.Format( _T("Connection to PA controller failed on error reset!") );
			P_LOG->WriteLog_EXT( logMsg );
			//////////////////////////
			
			pa::PPAStatus->GetThreadState()->bConnectionFailed = TRUE;
			return;
		}

		// 연결 될때까지 대기
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
	
	//Test LCD connection
	if( PConfig->pConfig_->bUsingLCD )
	{
		pa::PPAStatus->GetPAStatus()->bLCDAlive = FALSE;	//reset to false, always sends alive when changing page
		pGLCD->SendCommand(_T("page 0"));
		Sleep(100);		
		
		
		if( reconnect || pa::PPAStatus->GetPAStatus()->bLCDAlive == FALSE ) 
		{

			dwTime = GetTickCount();			// timeout for connection
			pa::PPAStatus->GetPAStatus()->bLCDAlive = FALSE;
			pGLCD->StartComm();
			DWORD dwTime2 = GetTickCount();		// timeout for response
			Sleep(100);

			while ((GetTickCount() - dwTime) < 10*1000)
			{
				if (pa::PPAStatus->GetPAStatus()->bLCDAlive == TRUE){
					break;	//success
				} else {
					// try again after 2 seconds
					if ((GetTickCount() - dwTime2) > 2*1000){
						pGLCD->StartComm();
						dwTime2 = GetTickCount();		// reset timeout for response
					}
				}
				pGLCD->SendCommand(_T("page 0"));		// go to loading page and wait for result (alive signal)
				Sleep(100);
			}
			
			// handle failure
			if (pa::PPAStatus->GetPAStatus()->bLCDAlive == FALSE)
			{
				///////////LOG////////////
				CString logMsg;
				logMsg.Format( _T("Connection to LCD failed on error reset or old LCD software!") );
				P_LOG->WriteLog_EXT( logMsg );	
				//////////////////////////
			}
			
			////////////////////
			// UPDATE EVERYTHING
			////////////////////
			// update milling time
			DWORD dwRunningTime = pa::PPAStatus->GetThreadState()->dwRunningTime;
			CTimeSpan	tms( dwRunningTime );
			int			milling_time[3] = { tms.GetHours(), tms.GetMinutes(), tms.GetSeconds() };
			CString strSendmessage;
			strSendmessage.Format(_T("pgMain.txtTimer.txt=\"%02d:%02d:%02d\""), milling_time[0], milling_time[1], milling_time[2]);
			pGLCD->SendCommand(strSendmessage);
			strSendmessage.Format(_T("pgMainRunning.txtTimer.txt=\"%02d:%02d:%02d\""), milling_time[0], milling_time[1], milling_time[2]);
			pGLCD->SendCommand(strSendmessage);
			
			// update current filename if there is one
			int nCurrNCFileIndex = pa::PNCFileMgr->GetCurrentWorkNCFileIndex();
			if( nCurrNCFileIndex != -1 )
			{
				SNCFileInfo* pInfo = pa::PNCFileMgr->GetNCFileInfo( nCurrNCFileIndex );
				strSendmessage.Format(_T("pgMain.sctxtFilename.txt=\"%s\""), pInfo->file_name);
				pGLCD->SendCommand(strSendmessage);
				strSendmessage.Format(_T("pgMainRunning.sctxtFilename.txt=\"%s\""), pInfo->file_name);
				pGLCD->SendCommand(strSendmessage);
			} 
			else 
			{
				strSendmessage.Format(_T("pgMain.sctxtFilename.txt=\"\""));
				pGLCD->SendCommand(strSendmessage);
				strSendmessage.Format(_T("pgMainRunning.sctxtFilename.txt=\"\""), nCurrNCFileIndex);
				pGLCD->SendCommand(strSendmessage);
			}

			// reset collet states
			pGLCD->SendCommand(_T("pgMain.btnColletR.picc=0"));
			pGLCD->SendCommand(_T("pgMain.btnColletR.picc2=0"));
			pGLCD->SendCommand(_T("pgMain.btnColletL.picc=0"));
			pGLCD->SendCommand(_T("pgMain.btnColletL.picc2=0"));
			PPAStatus->GetPAStatus()->bLCDRefresh=TRUE;
		}
	}

	if( PPAStatus->GetPAStatus()->nSpindle_Board_Status != 1 ) {
		S_INIT();
	}
	
	if( pa::CPAMotion::STREAM_MODE_ERROR == TRUE )
	{
		pa::CPAMotion::STREAM_MODE_ERROR = FALSE;
		RST();
		Sleep(100);
		STOP(1);
	}
	
	PPAStatus->GetThreadState()->nIsConnectedPAController=1;	//force is connected if the connections was successful
	
	RST();
	Sleep(100);
	STOP();

	PPAAsyncComm[0]->Reset();
	PPAAsyncComm[1]->Reset();
	
	//////////////////////////////////////////////////////////////////////
	// 2020.03.19. 제어기 시간 불러와서 PC시간으로 설정
	DWORD	dwErr = 0;
	int		nRespErrCode = 0;
	try {
#ifdef _USE_PA_
		PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_RDT );		// Command 전송
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_RDT, 10000 );
		dwErr = CPAAsyncComm::DW_RESPONSE_RDT_CMD;
		nRespErrCode = CPAAsyncComm::N_ERRORCODE_RDT_CMD;
#endif 
	}
	catch ( pa::CPException& e ) {
		dwErr = 0;
	}
	
	PThread->changeRunMode( RUNMODE_STOP, TRUE );
}

// Tool Clamp/Unclamp
void pa::CPAMotion::ToolClamp( BOOL bDirection, EN_STATE hState )
{
	int	nSignal = 0;

	switch( hState )
	{
	case CLAMP:		nSignal = 0; break;
	case UNCLAMP:	nSignal = 1; break;
	default:
		ASSERT( FALSE );
		return ;
	}
	
	if ( bDirection == 0)		// Left Tool
	{
		if (nSignal == 0)
		{
			MDA( TRUE, "M911" );
		}
		else
		{
			MDA( TRUE, "M910");
		}
	}
	else		// Right Tool
	{
		if (nSignal == 0)
		{
			MDA( TRUE, "M921" );
		}
		else
		{
			MDA( TRUE, "M920");
		}
	}
}

// Block Clamp/Unclamp
void pa::CPAMotion::BlockClamp( EN_STATE hState )
{
	switch( hState )
	{
	case CLAMP: default:
		PPAAsyncComm[0]->SendCommand( (CPAAsyncComm::EN_COMMAND)CPAAsyncComm::CMD_M912 );
		PPAAsyncComm[0]->Wait((CPAAsyncComm::EN_COMMAND)CPAAsyncComm::CMD_M912);
		break;
	case UNCLAMP:
		PPAAsyncComm[0]->SendCommand( (CPAAsyncComm::EN_COMMAND)CPAAsyncComm::CMD_M913 );
		PPAAsyncComm[0]->Wait((CPAAsyncComm::EN_COMMAND)CPAAsyncComm::CMD_M913);
		break;
	}
}

// Door Open/Close
void pa::CPAMotion::ATCDoor( EN_STATE hState )
{
	int nSignal = 0;

	switch( hState )
	{
	case OPEN:	nSignal = 1; break;
	case CLOSE:	nSignal = 0; break;
	default: 
		ASSERT( FALSE );
		return ;
	}

	if( pa::MODEL_INFO.IsUsingATCDoor() )
	{
		if( nSignal == 0 ) {
			MDA( TRUE, "M702" );	// Close
		} else {
			MDA( TRUE, "M701" );	// Open
		}
	}
}

//	M28/M29 매크로 사용으로 수정 한다 
void pa::CPAMotion::ClearRoom( BOOL bOn )
{
	if( bOn )
	{
		Sleep( 500 );
		// M28
		MDA( TRUE, "MM28"); //	"M28" ); => 2018.07.06. 매뉴얼로 M28명령을 보낼때는 MM28을 사용한다 
	}
	else 
	{
		// M29 
		MDA( TRUE, "M29" );
		Sleep( 500 );
	}
}

void pa::CPAMotion::SendCommand( char* command, char* response, int len, DWORD dwTimeout  )
{
	char*	pcmd = NULL;
	char*	pdata= NULL;
	int		command_index = 0;

	for( int i = 0; i<CPAAsyncComm::CMD_NUM; i++ )
	{
		pcmd = strstr( command, CPAAsyncComm::STR_COMMAND[i] );
		if( pcmd ) {
			command_index = i;
			break;
		}
	}
 	if( pcmd == NULL ) {
		sprintf_s( response, len, "command does not exist!" );
		return ;
	}
	else
	{
		pdata = strstr( command, " " );
		if( pdata ) {
			pdata += 1;	// skip white space 
		}
	}

	if( pdata ) {
		PPAAsyncComm[0]->SendCommand( (CPAAsyncComm::EN_COMMAND)command_index, pdata );
	} else {
		PPAAsyncComm[0]->SendCommand( (CPAAsyncComm::EN_COMMAND)command_index );
	}
	PPAAsyncComm[0]->Wait( (CPAAsyncComm::EN_COMMAND)command_index, dwTimeout );
	
	// 응답 데이터 저장 
	if( response )
	{
		sprintf_s( response, len, "%s", PPAAsyncComm[0]->GetResponse() );
	}
}

void pa::CPAMotion::SendMDACommand( char* mda_command )
{
	MDA( FALSE, mda_command );
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void pa::CPAMotion::UploadSoftLimit()
{
	RMAXL();
	RMINL();
}

void pa::CPAMotion::DownloadSoftLimit()
{
	double	ftemp[20][2];
	int		nNumAxis = pa::MODEL_INFO.GetNumAxis();

	NOT_CHECK_STATUS = TRUE;

	Sleep( 3000 );

	try {
		RDISABLE();
	}
	catch( CPException& e ) {

	}

	for( int i = 0; i<nNumAxis; i++ ) 
	{
		int		axis = i+1;
		double	fTempMax = PPAStatus->GetThreadState()->fSoftLimit_[i][1];
		double	fTempMin = PPAStatus->GetThreadState()->fSoftLimit_[i][0];
		WMAXL( axis, fTempMax, fTempMin );
		Sleep( 100 );
	}

	Sleep( 1000 );

	RENABLE();

	Sleep( 3000 );

	NOT_CHECK_STATUS = FALSE;
}

// Z up
// AB 수평 
// XY 이동 
// AB 이동 
void pa::CPAMotion::MoveReadyPos()
{
	char szCommand[256];
	double fAPos;
	double fBPos;
	BOOL bMoveAB  = pa::MODEL_INFO.IsUsingABMoveWhenReadyPosMove() == 0 ? FALSE : TRUE;	// 4WAS만 FALSE

	// ZUp 
	sprintf_s( szCommand, 256, "G00 G90 G53 X%.3f Y%.3f Z%.3f A%.3f B%.3f",
		PPAStatus->GetPAStatus()->fPosition[pa::AXIS_X],
		PPAStatus->GetPAStatus()->fPosition[pa::AXIS_Y],
		0.0,
		PPAStatus->GetPAStatus()->fPosition[pa::AXIS_A],
		0.0 );

	MDA( FALSE, szCommand );
	PPAAsyncComm[0]->Wait( pa::CPAAsyncComm::CMD_RND_MDA, 10000 ); //0 );
	Sleep( 100 );

	// XY 이동 
	sprintf_s( szCommand, 256, "G00 G90 G53 X%.3f Y%.3f Z%.3f A%.3f B%.3f",
		PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_READYPOS][pa::AXIS_X],
		PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_READYPOS][pa::AXIS_Y],
		0.0,
		PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_READYPOS][pa::AXIS_A],
		0.0);

	MDA( FALSE, szCommand );
	PPAAsyncComm[0]->Wait( pa::CPAAsyncComm::CMD_RND_MDA, 10000 ); //0 );
	Sleep( 100 );
}

// stream 모드를 시작하고, stream용 scoket 을 연결한다.  
void pa::CPAMotion::StartStreamMode()
{
	int errcode = 0;

	TRACE( _T("pa::CPAMotion::StartStreamMode()\n") );

	STOP();
	Sleep( 100 );
	MODE( CPAAsyncComm::PA_RUNMODE_AUTO );
	Sleep( 100 );
	STREAM();

#ifdef _USE_PA_
 
 	try {
 		// 1. socket 생성 
 		hSockStream_ = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
 
 		if( hSockStream_ == INVALID_SOCKET ) {
 			errcode = GetLastError();
 			throw errcode;
 		}
 
 		// 2. 연결 
 		SOCKADDR_IN server_addr;
 		server_addr.sin_family		= AF_INET;
		server_addr.sin_port		= htons( theApp.nPortNoForStream );
		server_addr.sin_addr.s_addr = inet_addr( theApp.szIpAddr );
 
 		int conn = connect( hSockStream_, (SOCKADDR*)&server_addr, sizeof(server_addr) );
 		if( conn != 0 ) {
 			errcode = GetLastError();
 			throw errcode;
 		}
 
 		// 3. 소켓을 비동기 소켓으로 만든다 
 		unsigned long ul = 1;
 		if( ioctlsocket( hSockStream_, FIONBIO, (unsigned long *)&ul ) == SOCKET_ERROR ) {
 			errcode = GetLastError();
 			throw errcode;
 		}
 	}
 	catch( int& )
 	{
 		if( hSockStream_ != INVALID_SOCKET ) 
 		{
 			closesocket( hSockStream_ );
 			hSockStream_ = INVALID_SOCKET;
 		}

		throw CPException( pa::ERR_PNC, pa::PNC_ERR_STREAM_SOCKET_CONNECT_FAIL, _T("") );
 	}
 
#endif
 
 	PPAStatus->GetPAStatus()->nStreamStatusCode = 0;
 	PPAStatus->GetPAStatus()->nStreamLineNumber = 0;
 	PPAStatus->GetPAStatus()->nStreamBufferCount= 0;
}

// stream 모드를 종료 한다 
void pa::CPAMotion::StopStreamMode()
{
	if( hSockStream_ != INVALID_SOCKET ) {
		closesocket( hSockStream_ );
		hSockStream_ = INVALID_SOCKET;
	}

	PPAStatus->GetThreadState()->nStartingNCCodeStepNo = 0;
	PPAStatus->GetThreadState()->nNumberOfPreparingStep= 0;

	TRACE( _T("pa::CPAMotion::StopStreamMode()\n") );
}

BOOL pa::CPAMotion::IsStopStreamMode()
{
	BOOL bRet = ( hSockStream_ == INVALID_SOCKET ) ? TRUE : FALSE;

	return bRet;
}

// stream 모드로 gcode를 전송 한다. b가 false일 경우, ?를 전송한다		
int  pa::CPAMotion::SendStreamCommand( char* pCommand, BOOL bPrepareCmd )
{
 	int status_code = 0;
 	int line_number = 0;
	int buffer_count = 0;
 
	status_code = 0;
	line_number = 0;
	buffer_count = 0;

	// 2017.08.14. 가공중 EMO 걸렸을 때, 멈추는 현상으로 수정 해 봄
	try {
		// command 전송 
		stream_send_and_receive( pCommand, &status_code, &line_number, &buffer_count );
	}
	catch( CPException& e ) {
		throw e;
	}

	// 응답 저장 
	PPAStatus->GetPAStatus()->nStreamStatusCode = status_code;
	PPAStatus->GetPAStatus()->nStreamLineNumber = line_number;
	PPAStatus->GetPAStatus()->nStreamBufferCount= buffer_count;
		 
	if( bPrepareCmd ) {
		PPAStatus->GetThreadState()->nNumberOfPreparingStep += 1;
	}
		 
	// 에러 처리 
	if( status_code != 0 ) {
		// status 
		throw CPException( ERR_PA_STREAM, status_code, _T("") );
	} 

	return buffer_count;
}

void pa::CPAMotion::stream_send_and_receive( char* pCommand, int* state_code, int* line_no, int* buffer_count )
{
	static char szReveive[128];
	static int  receive_index;
	DWORD	dwTime;
	int ret = 0;
	int retry_count = 0;

	CString strTrace;
	strTrace.Format(_T("stream_send_and_receive : %s\n"), pCommand);
	TRACE(strTrace);

HC_RETRY:

	if( hSockStream_ == INVALID_SOCKET ) {
		for( int i = 0; i<20; i++ ) {
			PThread->checkState();
			PThread->checkLimitSensor();
			Sleep(10);
		}
		throw CPException( ERR_PNC, PNC_ERR_STREAM_SOCKET_DISCONNECT, _T("1. [Invalid-socket]") );
	}

	memset( (void*)szReveive, 0, sizeof(char)*128 );
	receive_index = 0;

	ret = send( hSockStream_, pCommand, strlen(pCommand), 0 );

	dwTime = GetTickCount();

	// \r\n 까지 읽는고, 결과를 파싱해서 리턴 한다 
	while( TRUE )
	{
		if( PPAStatus->GetPAStatus()->nRunStatus == 0 ) { *state_code=0; *line_no=0; *buffer_count=0; return; }

		ret = recv( hSockStream_, (char*)(szReveive + receive_index), 128, 0 );

		if( ret <= 0 ) 
		{	
			DWORD dwErrCode = WSAGetLastError();
			if( dwErrCode == 0 || dwErrCode == WSAEWOULDBLOCK )
			{
				if( GetTickCount() - dwTime > dwStreamTimeout_ ) 
				{
					TRACE( _T("*****") );

					//////////////////////////////////////////////////////////////////////////
					// 2016.05.31 
					//	- stream timeout일 경우, 제어기 상태를 읽어 본다 
					try 
					{
						//	for( int i = 0; i<20; i++ ) {
						for( int i = 0; i<10; i++ ) 
						{
							// 에러 중 check state
							PThread->checkState();
							// 리밋 센서 check 
							PThread->checkLimitSensor();
							//
							Sleep(10);
							//
							CString strTemp;
							strTemp.Format( _T("*** %d\n"), i );
							TRACE( strTemp );
						}
					}
					catch( CPException& e ) 
					{
						throw e;
					}

					CString strCmd;
					strCmd = hcutil::ASCII_TO_CSTRING( pCommand );

					// 2016.10.12. StreamSocketTimeout 디버깅 테스트 코드 
					if( pCommand[0] == '?' ) 
					{
						retry_count += 1;

						if( retry_count >= 10 )
						{
							CString strErrCode;
							strErrCode.Format( _T("2: SEC[%d]"), dwErrCode );
							throw CPException( ERR_PNC, PNC_ERR_STREAM_SOCKET_TIMEOUT, strErrCode ); //(LPCTSTR)strCmd );
						}

						CString logMsg;
						logMsg.Format( _T(">>>>> STREAM_SOCKET_TIMEOUT and RETRY") );
						P_LOG->WriteLog_EXT( _T(">>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<") );		
						P_LOG->WriteLog_EXT( logMsg );
						goto HC_RETRY;
					}
					else 
					{
						try {
							for (int i = 0; i<10; i++) {
								PThread->checkState();
								PThread->checkLimitSensor();
							}
						}
						catch (CPException& e) {
							throw e;
						}
						
						CString strErrCode;
						strErrCode.Format( _T("3: SEC[%d]"), dwErrCode );
						throw CPException( ERR_PNC, PNC_ERR_STREAM_SOCKET_TIMEOUT, strErrCode );
					}

				}
				else 
				{
// 					TRACE( _T("retry !!!\n") );
					// 다시 읽는다 
					Sleep( 0 );	// 2017.03.14	// 2017.03.15	// 이거 없으면 화면 깜밖임이 심해짐 
					continue;
				}
			}
			else 
			{
				try {
					for( int i = 0; i<10; i++ ) {
						PThread->checkState();
						PThread->checkLimitSensor();
						Sleep(10);
					}
				}
				catch (CPException& e) {
					throw e;
				}

				// 통신 에러 
				// socket 닫고, exception 
				closesocket( hSockStream_ );
				hSockStream_ = INVALID_SOCKET;

				CString strErrCode;
				strErrCode.Format( _T("4: SEC[%d]"), dwErrCode );
				throw CPException( ERR_PNC, PNC_ERR_STREAM_SOCKET_DISCONNECT, strErrCode );
			}
		}

		receive_index += ret;

		// 응답 완료 확인 
		if( szReveive[receive_index-2] == '\r' && szReveive[receive_index-1] == '\n' ) 
		{
			break;
		}
	}


	// 응답 코드에서 "\n\r"  제거 하고, 마지막에 ' '를 추가 한다  
	szReveive[receive_index-2] = ' ';
	szReveive[receive_index-1] = 0;
	receive_index -= 1;

	// 응답 코드 분석 : "status_code line_number buffer_count"
	int  ret_value[3] = { 0, 0, 0};

	// 2017.03.15
	char* p = strchr( szReveive, ' ' );	// start line_number 
	if( p ) {
		p = strchr( p+1, ' ' );				// start buffer_count 
		if( p ) {
			ret_value[2] = atoi( p+1 );
		}
	} 
	
	*state_code		= 0;
	*line_no		= 0;
	*buffer_count	= ret_value[2];
}

//////////////////////////////////////////////////////////////////////////
// 2017.01.11 추가 
void pa::CPAMotion::RTPPO()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RTPPO );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RTPPO );
}

void pa::CPAMotion::WTPPO( double offset )
{
	char param[64];

	sprintf_s( param, 64, "%.3f", offset );
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_WTPPO, param );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_WTPPO );
	//////////////////////////////////////////////////////////////////////////
	// 변수에 저장 
	PConfig->pConfig_->fOptionData[OPTION_TOOL_POCKET_PUT_OFFSET] = offset;
	//////////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////
// 2017.01.12 추가 
void pa::CPAMotion::RADR()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RADR );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RADR );
}

void pa::CPAMotion::WADR( char* pNewAddress )
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_WADR, pNewAddress );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_WADR, 10*1000 );
}

void pa::CPAMotion::RRIOADR()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RRIOADR );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RRIOADR );
}

void pa::CPAMotion::WRIOADR( char* pNewAddress )
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_WRIOADR, pNewAddress );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_WRIOADR, 10*1000 );
}

void pa::CPAMotion::ChangeIPAddress( char cBoard )
{
	try 
	{
		PPAStatus->GetThreadState()->bCheckStatus = FALSE;

		CString strLog;
		strLog.Format( _T("start change ip address - %s"), cBoard==0 ? _T("PA") : _T("IO") );
		PThread->writeLog_AutoCal( strLog, FALSE );

		if( cBoard == 0 ) 
		{
			// PA 제어기 
			CString strPAAddr = hcutil::ASCII_TO_CSTRING( pa::PPAStatus->GetThreadState()->szNEW_PA_IP_ADDR );
			strLog.Format( _T("pa controller %s -> %s"),
				pa::PPAStatus->GetThreadState()->szPA_IP_ARRD,
				strPAAddr );
			PThread->writeLog_AutoCal( strLog, TRUE );
			WADR( pa::PPAStatus->GetThreadState()->szNEW_PA_IP_ADDR );
			Sleep(100);

			// PAConfig.ini 설정 변경 
			//	KeyName = PA
			//	ValueName = IpAddr 
			CString strConfigFilePath;
			strConfigFilePath.Format( INI_PA_CONFIG_PATH ); //_T("\\SD Card\\EPnc\\Config\\PAConfig.ini") );

			CCEIniFile hIniFile;
			hIniFile.Open( strConfigFilePath );
			hIniFile.SetValue( _T("PA"), _T("IpAddr"), strPAAddr );
			hIniFile.Close();
		}
		else if( cBoard == 1 )
		{
			// Cantops I/O 보드 
			CString strIOAddr = hcutil::ASCII_TO_CSTRING( pa::PPAStatus->GetThreadState()->szNEW_CANTOPS_IP_ADDR );
			strLog.Format( _T("io controller %s -> %s"),
				pa::PPAStatus->GetThreadState()->szCANTOPS_IP_ADDR, 
				strIOAddr );
			PThread->writeLog_AutoCal( strLog, TRUE );
			WRIOADR( pa::PPAStatus->GetThreadState()->szNEW_CANTOPS_IP_ADDR );
			Sleep(100);
		}

		strLog.Format( _T("end change ip address") );
		PThread->writeLog_AutoCal( strLog, FALSE );

		PPAStatus->GetThreadState()->bCheckStatus = TRUE;
	}
	catch (CPException& e)
	{
		PPAStatus->GetThreadState()->bCheckStatus = TRUE;
		throw e;
	}
}

void pa::CPAMotion::UploadIPAddress()
{
	CString strTemp[2];
	memset((void*)(pa::PPAStatus->GetThreadState()->szIpAddress), 0, sizeof(TCHAR)*64);
	_stprintf_s( pa::PPAStatus->GetThreadState()->szIpAddress, 63, _T("%s\n\n"), theApp.strIpAddr );
	TRACE( pa::PPAStatus->GetThreadState()->szIpAddress );
}

// 2021.07.14 Manually select tool
void pa::CPAMotion::SetToolNumber_CMD_WCTN( int toolNoLeft, int toolNoRight )
{
	char* argBuff;	
	argBuff = new char[16];
	sprintf_s( argBuff, 16, "%d, %d", toolNoLeft, toolNoRight );
	
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_WCTN, argBuff );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_WCTN, 10*1000 );
}

// 2021.08.31 Set spindle RPM
void pa::CPAMotion::SetSpindleRPM()
{
	char* argBuff;	
	argBuff = new char[16];
	sprintf_s( argBuff, 16, "S%d", pa::PPAStatus->GetPAStatus()->nSpindleSpeedSetting );
	
	MDA( TRUE, argBuff );
}

//////////////////////////////////////////////////////////////////////////
// Helpers
//////////////////////////////////////////////////////////////////////////

void pa::CPAMotion::SendMotionCommand( pa::CPAAsyncComm::EN_COMMAND cmd, BOOL blocking )
{
	PPAAsyncComm[0]->SendCommand( cmd );

	if( blocking ) 
	{
		PPAAsyncComm[0]->Wait( cmd, 0 );
	}
}

void pa::CPAMotion::SendMotionCommand( pa::CPAAsyncComm::EN_COMMAND cmd, BOOL blocking, int param )
{
	char szParam[64];

	sprintf_s( szParam, 64, "%d", param );

	PPAAsyncComm[0]->SendCommand( cmd, szParam );

	if( blocking ) 
	{
		PPAAsyncComm[0]->Wait( cmd, 0 );
	}
}

//////////////////////////////////////////////////////////////////////////
// Select M28/M29 
//////////////////////////////////////////////////////////////////////////
void pa::CPAMotion::GWVF( BOOL blocking )
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_GWVF );

	if( blocking ) 
	{
		PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_GWVF );
	}
}

void pa::CPAMotion::SWVF( BOOL blocking, int nSelectM28 )
{
	char szParam[64];

	sprintf_s( szParam, 64, "%d", nSelectM28 );

	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_SWVF, szParam );

	if( blocking )
	{
		PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_SWVF );
	}
}

//////////////////////////////////////////////////////////////////////////
// Barcode
//////////////////////////////////////////////////////////////////////////

int pa::CPAMotion::ReadBarcode()
{
	int barcode = -1;
	return barcode;
}

void pa::CPAMotion::RND_MSC()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RND_MSC );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RND_MSC, 3000 );
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void pa::CPAMotion::ResetOrigin( pa::EN_AXIS hAxis, BOOL blocking )
{
	char szParam[32];

	PPAStatus->GetThreadState()->bCompleteResetOrigin_ = FALSE;

	sprintf_s( szParam, 32, "%d", (int)(hAxis)+1 );
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_FZS, szParam );

	if( blocking )
	{
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_FZS, 300*1000 );	// 5분  
	}
}

//////////////////////////////////////////////////////////////////////////
// 2018.02.27 SALF 명령 추가 
//////////////////////////////////////////////////////////////////////////
void pa::CPAMotion::RND_SALF( BOOL blocking, int using_air, int interval, int sensor_type )
{
	char szParam[64];

	sprintf_s( szParam, 64, "%d,%d,%d", using_air, interval, sensor_type );
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_SALF, szParam );

	if( blocking )
	{
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_SALF, 3000 );
	}
}

void pa::CPAMotion::RND_CDTEX()
{
	PPAAsyncComm[1]->SendCommand( CPAAsyncComm::CMD_RND_CDTEX );
	PPAAsyncComm[1]->Wait( CPAAsyncComm::CMD_RND_CDTEX, 5000 );
}

void pa::CPAMotion::RND_SFSF( BOOL blocking, int using_waterflow_senor, int startTimeout, int sensingTimeout, int using_water_level_sensor )
{
	char szParam[64];

	sprintf_s( szParam, 64, "%d,%d,%d,%d", using_waterflow_senor, startTimeout, sensingTimeout, using_water_level_sensor );
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_SFSF, szParam );

	if( blocking )
	{
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_SFSF, 3000 );
	}
}

void pa::CPAMotion::RND_WPAR( BOOL blocking, int purge_air_hold_time )
{
	char szParam[64];

	sprintf_s( szParam, 64, "%d", purge_air_hold_time );
	PPAAsyncComm[0]->SendCommand( CPAAsyncComm::CMD_WPAR, szParam );

	if( blocking )
	{
		PPAAsyncComm[0]->Wait( CPAAsyncComm::CMD_WPAR, 3000 );
	}
}