#pragma once

namespace pa 
{
//////////////////////////////////////////////////////////////////////////

/** 
 * Config 데이터를 INI 파일과 공유 메모리를 같이 사용하는 구조
 *	- INI 파일에서 읽은 데이터를 공유 메모리에 저장하고,
 *	- 공유 메모리에 변경된 데이터를 INI 파일에 저장한다 
 */ 

class CPConfig
{
private:
	CString strFilePath;

public:

private:
	hcipc::CSharedMem	*pShMem_;

	BOOL load_from_file_coordinate_offset( CString& strErrMsg );
	BOOL load_from_file_teaching_point( CString& strErrMsg );
	BOOL load_from_file_tp_option( CString& strErrMsg );
	BOOL load_from_file_sw_config_data( CString& strErrMsg );


public:
	SConfigData			*pConfig_;

public:
	BOOL Initialize( CString& strConfigFilePath, CString& strErrMsg );
	void Destroy();

	BOOL Load( CString& strErrMsg );
	BOOL Save( CString& strErrMsg );

	BOOL SaveCoordOffset( CString& strErrMsg );
	BOOL SaveTeachingPoint( CString& strErrMsg );
	BOOL SaveOption( CString& strErrMsg );

	BOOL SaveUsingDetectBlock( BOOL b, CString& strErrMsg );
	BOOL SaveUsingAirLimitSensor( BOOL b, CString& strErrMsg );
	BOOL SaveDelayGripBlock( int i, CString& strErrMsg );
	BOOL SaveAirLimitInterval( int i, CString& strErrMsg );
	BOOL SaveToolErrorOccureHandlingCode( int i, CString& strErrMsg );
	BOOL SaveUsingLCD( BOOL b, CString& strErrMsg );
	BOOL SaveUsingOpPanel( BOOL b, CString& strErrMsg );
	BOOL SaveUsingFlowSensor( CString& strErrMsg );
	BOOL SaveNcFileChecker( CString& strErrMsg );
	BOOL SaveSpindleAirPurge( CString& strErrMsg );
	BOOL SaveUsingLog( CString& strErrMsg );
	BOOL SaveToolTimesPerMilling( int newTimes[pa::MAX_TOOL_NUM], CString& strErrMsg );

public:
	CPConfig(void);
	~CPConfig(void);
};

//////////////////////////////////////////////////////////////////////////
}

