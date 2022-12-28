#include "StdAfx.h"
#include "PConfig.h"
//#include "EPncM.h"
#include "EPncMDLL.h"

//////////////////////////////////////////////////////////////////////////

pa::CPConfig::CPConfig(void)
{
	pShMem_		= NULL;
	pConfig_	= NULL;
}

pa::CPConfig::~CPConfig(void)
{
	Destroy();
}

BOOL pa::CPConfig::Initialize( CString& strConfigFilePath, CString& strErrMsg )
{
	strFilePath = strConfigFilePath;

	pShMem_ = new hcipc::CSharedMem();
	if( pShMem_ == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pmac::CPConfig::pShMem_ object" ) );
		return FALSE;
	} 
	pConfig_ = (SConfigData*)pShMem_->Create( NULL, CONFIG_OBJECT_NAME, sizeof(SConfigData), TRUE, 0 );
	if( pConfig_ == NULL ) {
		strErrMsg.Format( _T("create shared memory error for pa::CPConfig::pConfig_ object") );
		return FALSE;
	}

	return TRUE;
}

void pa::CPConfig::Destroy()
{
	if( pShMem_ ) {
		delete pShMem_;
		pShMem_ = NULL;
		pConfig_= NULL;
	}
}

BOOL pa::CPConfig::Load( CString& strErrMsg )
{
	CString		strAxisName[]	= { _T("_X"), _T("_Y"), _T("_Z"), _T("_A"), _T("_B") }; 
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;
	int			nTemp;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail. (pmac::CPConfig::Load())") );
		return FALSE;
	}

	hIniFile.Close();

	//////////////////////////////////////////////////////////////////////////

	if( load_from_file_coordinate_offset( strErrMsg ) == FALSE ) {
		return FALSE;
	}

	if( load_from_file_teaching_point( strErrMsg ) == FALSE ) {
		return FALSE;
	}

	if( load_from_file_tp_option( strErrMsg ) == FALSE ) {
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	if( load_from_file_sw_config_data( strErrMsg ) == FALSE ) {
		return FALSE;
	}

	return TRUE;
}

// 이 함수를 사용하지 않음 
// => 각각 종류별로 저장하도록 수정 되어 있음 
// => coordinate offset 화면에서 저장 버튼을 누르면, coordinateoffset 만 파일에 저장 
// => teaching point 화면에서 저장 버튼을 누르면, teachingpoint 만 파일에 저장 
BOOL pa::CPConfig::Save( CString& strErrMsg ) 
{
	if( !SaveCoordOffset( strErrMsg ) ) {
		return FALSE;
	}

	if( !SaveTeachingPoint( strErrMsg ) ) {
		return FALSE;
	}

	if( !SaveOption( strErrMsg ) ) {
		return FALSE;
	}

	if( !SaveUsingDetectBlock( pConfig_->bUsingDetectBlock, strErrMsg ) ) {
		return FALSE;
	}

	if( !SaveUsingAirLimitSensor( pConfig_->bUsingAirLimitSensor, strErrMsg ) ) {
		return FALSE;
	}

	if( !SaveDelayGripBlock( pConfig_->nDelayGripBlock, strErrMsg ) ) {
		return FALSE;
	}

	if( !SaveAirLimitInterval( pConfig_->nAirLimitInterval, strErrMsg ) ) {
		return FALSE;
	}

	if( !SaveToolErrorOccureHandlingCode( pConfig_->nToolErrorOccure_HandlingCode, strErrMsg ) ) {
		return FALSE;
	}

	if( !SaveUsingLCD( pConfig_->bUsingLCD, strErrMsg ) ) {
		return FALSE;
	}
	
	if( !SaveUsingOpPanel( pConfig_->bUsingOpPanel, strErrMsg ) ) {
		return FALSE;
	}

	if( !SaveUsingFlowSensor( strErrMsg ) ) {
		return FALSE;
	}

	if( !SaveNcFileChecker( strErrMsg ) ) {
		return FALSE;
	}

	if( !SaveSpindleAirPurge( strErrMsg ) ) {
		return FALSE;
	}

	if( !SaveSpindleAirPurge( strErrMsg ) ) {
		return FALSE;
	}

	if( !pa::PPAStatus->GetAutoCalCoordinateOffsetParam()->Save( INI_AUTO_TEACH_COORD_OFFSET_PARAM_PATH, strErrMsg ) ) {
		return FALSE;
	}

	if( !pa::PPAStatus->GetAutoTeachToolPocketParam()->Save( INI_AUTO_TEACH_COORD_OFFSET_PARAM_PATH, strErrMsg ) ) {
		return FALSE;
	}

	// Log 파라메타 
	if( !SaveUsingLog( strErrMsg ) ) {
		return FALSE;
	}
	
	// Tool times per milling
	if( !SaveToolTimesPerMilling( pConfig_->nToolTimesPerMilling, strErrMsg ) ) {
		return FALSE;
	}

	return TRUE;
}


BOOL pa::CPConfig::SaveCoordOffset( CString& strErrMsg )
{
	CString		strAxisName[] = { _T("_X"), _T("_Y"), _T("_Z"), _T("_A"), _T("_B") }; 
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveCoordOffset())") );
		return FALSE;
	}

	// [CoordOffset]
	strKeyName.Format( _T("CoordOffset") );

	for( int i = 0; i<COORD_NUM; i++ ) {
		for( int j = 0; j<AXIS_NUM; j++ ) {
			strValueName = STR_COORDINATE[i] + strAxisName[j];
			hIniFile.SetValue( strKeyName, strValueName, (double)pConfig_->fCoordOffset[i][j] );
		}
	}
	
	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::SaveTeachingPoint( CString& strErrMsg )
{
	CString		strAxisName[] = { _T("_X"), _T("_Y"), _T("_Z"), _T("_A"), _T("_B") }; 
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveTeachingPoint())") );
		return FALSE;
	}

	// [TeachingPoint]
	strKeyName.Format( _T("TeachingPoint") );

	for( int i = 0; i<TEACHING_POINT_NUM; i++ ) {
		for( int j = 0; j<AXIS_NUM; j++ ) {
			strValueName = STR_TEACHING_POINT[i] + strAxisName[j];
			hIniFile.SetValue( strKeyName, strValueName, (double)(pConfig_->fTeachingPoint[i][j]) );
		}
	}

	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::SaveOption( CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveOption())") );
		return FALSE;
	}

	// [Option]
	strKeyName.Format( _T("Option" ) );

	for( int i = 0; i<OPTION_NUM; i++ ) {
		strValueName = STR_OPTION[i];
		hIniFile.SetValue( strKeyName, strValueName, (double)(pConfig_->fOptionData[i]) );
	}

	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::SaveUsingDetectBlock( BOOL b, CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;
	int		nTemp;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveUsingDetectBlock())") );
		return FALSE;
	}

	// [UsingMelodyForComplete]
	strKeyName.Format( _T("UsingDetectBlock") );
	strValueName.Format( _T("Data") );
	pConfig_->bUsingDetectBlock = b;
	nTemp = ( b == FALSE ) ? 0 : 1; 
	hIniFile.SetValue( strKeyName, strValueName, (int)nTemp );

	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::SaveUsingAirLimitSensor( BOOL b, CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;
	int		nTemp;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveUsingAirLimitSensor())") );
		return FALSE;
	}

	// [UsingAirLimitSensor]
	strKeyName.Format( _T("UsingAirLimitSensor") );
	strValueName.Format( _T("Data") );
	pConfig_->bUsingAirLimitSensor = b;
	nTemp = ( b == FALSE ) ? 0 : 1; 
	hIniFile.SetValue( strKeyName, strValueName, (int)nTemp );

	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::SaveDelayGripBlock( int i, CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveDelayGripBlock())") );
		return FALSE;
	}

	// [UsingAirLimitSensor]
	strKeyName.Format( _T("DelayGripBlock") );
	strValueName.Format( _T("Data") );
	pConfig_->nDelayGripBlock = i;
	hIniFile.SetValue( strKeyName, strValueName, (int)(pConfig_->nDelayGripBlock) );

	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::SaveAirLimitInterval( int i, CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveAirLimitInterval())") );
		return FALSE;
	}

	// [UsingAirLimitSensor]
	strKeyName.Format( _T("AirLimitInterval") );
	strValueName.Format( _T("Data") );
	pConfig_->nAirLimitInterval = i;
	hIniFile.SetValue( strKeyName, strValueName, (int)(pConfig_->nAirLimitInterval) );

	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::SaveToolErrorOccureHandlingCode( int i, CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveAirLimitInterval())") );
		return FALSE;
	}

	// [UsingAirLimitSensor]
	strKeyName.Format( _T("ToolErrOccure") );
	strValueName.Format( _T("HandlingCode") );
	pConfig_->nToolErrorOccure_HandlingCode = i;
	hIniFile.SetValue( strKeyName, strValueName, (int)(pConfig_->nToolErrorOccure_HandlingCode) );

	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::SaveUsingLCD( BOOL b, CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveUsingLCD())") );
		return FALSE;
	}

	// [UsingAirLimitSensor]
	strKeyName.Format( _T("LCD") );
	strValueName.Format( _T("Using") );
	pConfig_->bUsingLCD = b;
	hIniFile.SetValue( strKeyName, strValueName, (int)( ( pConfig_->bUsingLCD == FALSE ) ? 0 : 1) );
	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::SaveUsingOpPanel( BOOL b, CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveUsingOpPanel())") );
		return FALSE;
	}

	// [UsingAirLimitSensor]
	strKeyName.Format( _T("OpPanel") );
	strValueName.Format( _T("Using") );
	pConfig_->bUsingOpPanel = b;
	hIniFile.SetValue( strKeyName, strValueName, (int)( ( pConfig_->bUsingOpPanel == FALSE ) ? 0 : 1) );
	hIniFile.Close();

	return TRUE;
}

// 사용하지 않음 
BOOL pa::CPConfig::SaveUsingFlowSensor( CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;
	int		nTemp;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveUsingFlowSensor())") );
		return FALSE;
	}

	// [Flow Sensor]
	strKeyName.Format( _T("FlowSensor") );

	nTemp = (pConfig_->bUsingFlowSensor == TRUE) ? 1 : 0;
	hIniFile.SetValue( strKeyName, _T("Using"), (int)nTemp );

	nTemp = (pConfig_->nFlowSensorTimeout);
	hIniFile.SetValue( strKeyName, _T("Timeout"), (int)nTemp );

	nTemp = (pConfig_->nFlowSensorStartTimeout);
	hIniFile.SetValue( strKeyName, _T("StartTimeout"), (int)nTemp );

	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::SaveNcFileChecker( CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveNcFileCheck())") );
		return FALSE;
	}

	strKeyName.Format( _T("NcFileCheckWhenLoading") );
	hIniFile.SetValue( strKeyName, _T("CheckInvalidNcCode"), (int)( pConfig_->bCheckInvalidNcCode == FALSE ) ? 0 : 1 );
	hIniFile.SetValue( strKeyName, _T("TransformNcFile"), (int)( pConfig_->bTransformNcFile == FALSE ) ? 0 : 1 );
	hIniFile.SetValue( strKeyName, _T("CheckBlockSize"), (int)( pConfig_->bCheckBlockSize == FALSE ) ? 0 : 1 );
	hIniFile.SetValue( strKeyName, _T("CheckNcFileTag"), (int)( pConfig_->bCheckNcFileTag == FALSE ) ? 0 : 1 );

	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::SaveSpindleAirPurge( CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveSpindleAirPurge())") );
		return FALSE;
	}

	strKeyName.Format( _T("SpindleAirPurge") );
	hIniFile.SetValue( strKeyName, _T("Using"), (int)( pConfig_->bUsingSpindleAirPurge == FALSE ) ? 0 : 1 );

	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::SaveUsingLog( CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveSpindleAirPurge())") );
		return FALSE;
	}

	strKeyName.Format( _T("log") );
	hIniFile.SetValue( strKeyName, _T("operation_log"), (int)(pConfig_->nEnableOperationLog) );
	hIniFile.SetValue( strKeyName, _T("ipc_comm_log"), (int)(pConfig_->nEnableIpcCommLog) );
	hIniFile.SetValue( strKeyName, _T("thread_mode_log"), (int)(pConfig_->nEnableThreadModeLog) );
	hIniFile.SetValue( strKeyName, _T("oppanel_log"), (int)(pConfig_->nEnableOpPenalLog) );
	hIniFile.SetValue( strKeyName, _T("ext_log"), (int)(pConfig_->nEnableExtLog) );
	hIniFile.SetValue( strKeyName, _T("err_log"), (int)(pConfig_->nEnableErrLog) );

	hIniFile.Close();

	return TRUE;
}


// not used now
BOOL pa::CPConfig::SaveToolTimesPerMilling( int newTimes[pa::MAX_TOOL_NUM], CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( strFilePath ) ) {
		strErrMsg.Format( _T("config file open fail (pmac::CPConfig::SaveSpindleAirPurge())") );
		return FALSE;
	}
	
/*
	strKeyName.Format( _T("log") );
	hIniFile.SetValue( strKeyName, _T("operation_log"), (int)(pConfig_->nEnableOperationLog) );
	hIniFile.SetValue( strKeyName, _T("ipc_comm_log"), (int)(pConfig_->nEnableIpcCommLog) );
	hIniFile.SetValue( strKeyName, _T("thread_mode_log"), (int)(pConfig_->nEnableThreadModeLog) );
	hIniFile.SetValue( strKeyName, _T("oppanel_log"), (int)(pConfig_->nEnableOpPenalLog) );
	hIniFile.SetValue( strKeyName, _T("ext_log"), (int)(pConfig_->nEnableExtLog) );
	hIniFile.SetValue( strKeyName, _T("err_log"), (int)(pConfig_->nEnableErrLog) );
*/
	hIniFile.Close();

	return TRUE;
}


BOOL pa::CPConfig::load_from_file_coordinate_offset( CString& strErrMsg )
{
	TCHAR*		pFilePath = INI_COORD_OFFSET_PATH; //_T("\\SD Card\\EPnc\\Config\\CoordOffset.ini");
	CString		strAxisName[] = { _T("_X"), _T("_Y"), _T("_Z"), _T("_A"), _T("_B") }; 
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;
	double	fTemp;

	if( !hIniFile.Open( pFilePath ) ) {
		strErrMsg.Format( _T("ERROR : config file open fail (CSetupTeachingDlg::load_from_file_coordinate_offset())") );
		return FALSE;
	}

	// [CoordOffset]
	strKeyName.Format( _T("CoordOffset") );

	for( int i = 0; i<pa::COORD_NUM; i++ ) {
		for( int j = 0; j<pa::AXIS_NUM; j++ ) {
			strValueName = pa::STR_COORDINATE[i] + strAxisName[j];
			hIniFile.GetValue( strKeyName, strValueName, (double*)&fTemp );
			pConfig_->fCoordOffset[i][j] = fTemp;
		}
	}

	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::load_from_file_teaching_point( CString& strErrMsg )
{
	TCHAR*		pFilePath = INI_TEACHING_POINT_PATH; // _T("\\SD Card\\EPnc\\Config\\TeachingPoint.ini");
	CString		strAxisName[] = { _T("_X"), _T("_Y"), _T("_Z"), _T("_A"), _T("_B") }; 
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;
	double	fTemp;

	if( !hIniFile.Open( pFilePath ) ) {
		strErrMsg.Format( _T("ERROR : config file open fail (CSetupTeachingDlg::load_from_file_teaching_point())") );
		return FALSE;
	}

	// [TeachingPoint]
	strKeyName.Format( _T("TeachingPoint") );

	for( int i = 0; i<pa::TEACHING_POINT_NUM; i++ ) {
		for( int j = 0; j<pa::AXIS_NUM; j++ ) {
			strValueName = pa::STR_TEACHING_POINT[i] + strAxisName[j];
			hIniFile.GetValue( strKeyName, strValueName, (double*)&fTemp );
			pConfig_->fTeachingPoint[i][j] = fTemp;
		}
	}

	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::load_from_file_tp_option( CString& strErrMsg )
{
	TCHAR*		pFilePath = INI_TP_OPTION_PATH; // _T("\\SD Card\\EPnc\\Config\\TPOption.ini");
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;
	double	fTemp;

	if( !hIniFile.Open( pFilePath ) ) {
		strErrMsg.Format( _T("ERROR : config file open fail (CSetupTeachingDlg::load_from_file_tp_option())") );
		return FALSE;
	}

	// [Option]
	strKeyName.Format( _T("Option" ) );

	for( int i = 0; i<pa::OPTION_NUM; i++ ) {
		strValueName = pa::STR_OPTION[i];
		hIniFile.GetValue( strKeyName, strValueName, (double*)&fTemp );
		pConfig_->fOptionData[i] = fTemp;
	}

	hIniFile.Close();

	return TRUE;
}

BOOL pa::CPConfig::load_from_file_sw_config_data( CString& strErrMsg )
{
	TCHAR*		pFilePath = INI_SW_CONFIG_PATH; //_T("\\SD Card\\EPnc\\Config\\swconfig.ini");
	CString		strKeyName, strValueName, strTemp;
	CCEIniFile	hIniFile;
	int			nTemp;

	if( !hIniFile.Open( pFilePath ) ) {
		strErrMsg.Format( _T("ERROR : config file open fail (CSetupOptionDlg::save_sw_config_data()") );
		return FALSE;
	}

	strKeyName.Format( _T("SWConfig") );

	// 0.1. Using LCD 
	hIniFile.GetValue( strKeyName, _T("Using_LCD"), (int*)&nTemp );
	pConfig_->bUsingLCD = nTemp == 0 ? FALSE : TRUE;

	// 1. Using. External OpPanel 
	hIniFile.GetValue( strKeyName, _T("Using_OpPanel"), (int*)&nTemp );
	pConfig_->bUsingOpPanel = nTemp == 0 ? FALSE : TRUE;

	// 2. Using. Air Pressure Limit 
	hIniFile.GetValue( strKeyName, _T("Using_AirPressureLimit"), (int*)&nTemp );
	pConfig_->bUsingAirLimitSensor = nTemp == 0 ? FALSE : TRUE;

	// 5. Using. Auto Adjust Spindle Air Blow 
	hIniFile.GetValue( strKeyName, _T("Using_SpindleAirPurge"), (int*)&nTemp );
	pConfig_->bUsingSpindleAirPurge = nTemp == 0 ? FALSE : TRUE;

	// 6. Low Air Pressure Interval 
	hIniFile.GetValue( strKeyName, _T("AirPressureInterval"), (int*)&nTemp );
	strTemp.Format( _T("%d"), nTemp );
	pConfig_->nAirLimitInterval = nTemp;

	// 7. Using. Detect Block Sensor 
	hIniFile.GetValue( strKeyName, _T("Using_DetectBlockSensor"), (int*)&nTemp );
	pConfig_->bUsingDetectBlock = nTemp == 0 ? FALSE : TRUE;

	// 8. Block Grip Interval 
	hIniFile.GetValue( strKeyName, _T("BlockGripInterval"), (int*)&nTemp );
	pConfig_->nDelayGripBlock = nTemp;

	// 9. Using. Flow Sensor 
	hIniFile.GetValue( strKeyName, _T("Using_FlwoSensor"), (int*)&nTemp );
	pConfig_->bUsingFlowSensor = nTemp;

	// 10. Flow Sensor Timeout 
	hIniFile.GetValue( strKeyName, _T("FlowSensorTimeout"), (int*)&nTemp );
	pConfig_->nFlowSensorTimeout = nTemp;

	hIniFile.GetValue( strKeyName, _T("FlowSensorStartTimeout"), (int*)&nTemp );
	pConfig_->nFlowSensorStartTimeout = nTemp;

	// 
	hIniFile.GetValue(strKeyName, _T("Using_WaterLevelSensor"), (int*)&nTemp);
	pConfig_->bUsingWaterLevelSensor = nTemp;

	//
	hIniFile.GetValue( strKeyName, _T("PurgeAirHoldTime"), (int*)&nTemp );
	pConfig_->nPurgeAirHoldTime = nTemp;

	// 11. M28 Operation (WET / DRY)
	hIniFile.GetValue( strKeyName, _T("M28_Oprtation"), (int*)&nTemp );
	pConfig_->nSelectM28Operation = nTemp;

	// 12. Tool Error Occure...
	hIniFile.GetValue( strKeyName, _T("ToolErrorOccure"), (int*)&nTemp ); 
	pConfig_->nToolErrorOccure_HandlingCode = nTemp;

	// 13. When NC-File Loading...
	hIniFile.GetValue( strKeyName, _T("Check_InvalidNcCode"), (int*)&nTemp );
	pConfig_->bCheckInvalidNcCode = nTemp == 0 ? FALSE : TRUE;

	hIniFile.GetValue( strKeyName, _T("Check_NcFileTag"), (int*)&nTemp );
	pConfig_->bCheckNcFileTag = nTemp == 0 ? FALSE : TRUE;

	hIniFile.GetValue( strKeyName, _T("Check_MachineID"), (int*)&nTemp );
	pConfig_->bCheckMachineID = nTemp == 0 ? FALSE : TRUE;

	hIniFile.GetValue( strKeyName, _T("Check_SpindleOffset"), (int*)&nTemp );
	pConfig_->bCheckSpindleOffset = nTemp == 0 ? FALSE : TRUE;

	hIniFile.GetValue( strKeyName, _T("Check_TransformCoord"), (int*)&nTemp );
	pConfig_->bTransformNcFile = nTemp == 0 ? FALSE : TRUE;

	hIniFile.GetValue( strKeyName, _T("Check_CheckBlockSize"), (int*)&nTemp );
	pConfig_->bCheckBlockSize = nTemp == 0 ? FALSE : TRUE;

	hIniFile.GetValue( strKeyName, _T("CheckBlockSize_Speed"), (int*)&nTemp );
	pConfig_->nCheckBlockSizeSpeed = nTemp;

	hIniFile.GetValue( strKeyName, _T("CheckBlockSize_Torque"), (int*)&nTemp );
	pConfig_->nCheckBlockSizeTorque = nTemp;

	hIniFile.GetValue( strKeyName, _T("GraphicLCD_Port"), (int*)&nTemp );
	pConfig_->nGraphicLCD_Port = nTemp;
	
	// 14. Time usage per milling (approximation)
	hIniFile.GetValue( strKeyName, _T("ToolTimePerMilling14"), (int*)&nTemp );
	pConfig_->nToolTimesPerMilling[0] = nTemp;
	
	hIniFile.GetValue( strKeyName, _T("ToolTimePerMilling25"), (int*)&nTemp );
	pConfig_->nToolTimesPerMilling[1] = nTemp;
	
	hIniFile.GetValue( strKeyName, _T("ToolTimePerMilling3"), (int*)&nTemp );
	pConfig_->nToolTimesPerMilling[2] = nTemp;
	
	hIniFile.GetValue( strKeyName, _T("ToolTimePerMilling6"), (int*)&nTemp );
	pConfig_->nToolTimesPerMilling[5] = nTemp;
	
	

	// 
	hIniFile.GetValue( strKeyName, _T("Using_Logging"), (int*)&nTemp );
	pConfig_->nEnableOperationLog	= nTemp;			// Log option
	pConfig_->nEnableIpcCommLog		= nTemp;			// Log option
	pConfig_->nEnableThreadModeLog	= nTemp;			// Log option
	pConfig_->nEnableOpPenalLog		= nTemp;			// Log option
	pConfig_->nEnableExtLog			= nTemp;			// Log option
	pConfig_->nEnableErrLog			= nTemp;

	//////////////////////////////////////////////////////////////////////////
	// 청소 주기 Timeout 값 
	nTemp = 0;
	strKeyName.Format( _T("Timeout") );
	hIniFile.GetValue( strKeyName, _T("CleaningTimeout"), (int*)&nTemp );
	nTemp = (nTemp != 0) ? nTemp : 500;					// 디폴트 500시간  
	pConfig_->nCleaningTimeout_Hour = nTemp;

	//////////////////////////////////////////////////////////////////////////
	// 필터 교환 Timeout 값
	nTemp = 0;
	strKeyName.Format( _T("Timeout") );
	hIniFile.GetValue( strKeyName, _T("FilterTimeout"), (int*)&nTemp );
	nTemp = (nTemp != 0) ? nTemp : 500 * 3600;					// 디폴트 500시간
	pConfig_->nFilterTimeout_sec = nTemp;			

	//
	hIniFile.Close();

	return TRUE;
}

