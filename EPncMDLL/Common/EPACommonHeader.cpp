//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "EPACommonHeader.h"

#include <ctime>

namespace pa 
{

double SThreadState::RUNNING_TIME_PER_LINE = 0.015;

CString STR_PGM_TAG(_T(""));
CString GET_OBJECT_NAME_WITH_TAG(CString strName)
{
	CString ret;
// 	ret.Format(_T("%s-%s"), strName, STR_PGM_TAG);
	ret.Format(_T("%s_%s"), strName, STR_PGM_TAG);
	return ret;
}

EN_USER_MODE	USER_MODE = USER_MODE_USR;
CString	STR_USER_MODE[USER_MODE_NUM] =
{
	_T("User"),
	_T("Manager"),
	_T("RND")
};

CString GET_RND_PASSWORD()
{
	CString strRndPassword;
	CTime time = CTime::GetCurrentTime();

	int month = time.GetMonth();

	int mTenth = month / 10 + 1;
	int mOneth = (month % 10 + 1) % 10;

	strRndPassword.Format(_T("%d%d%d%d"), mTenth, mOneth, mTenth, mOneth);

	return strRndPassword;
}

CString	STR_USER_MODE_PW[USER_MODE_NUM] =
{
	_T(""),
	_T("1234"),
	_T("100321")
};

void SET_CURRENT_USERMODE(EN_USER_MODE user_mode)
{
	pa::PPAStatus->GetThreadState()->hUserMode = user_mode;
}

EN_USER_MODE GET_CURRENT_USERMODE()
{
	return pa::PPAStatus->GetThreadState()->hUserMode;
}

CString STR_PA_RUN_STATUS[] = 
{
	_T("Idle"),
	_T("Run"),
	_T("Unknown"),
	_T("Error"),
	_T("Pause")
};

CString STR_COORDINATE[COORD_NUM] = 
{
	_T("G53"),
	_T("G54"),
};

CString STR_TEACHING_POINT[TEACHING_POINT_NUM] =
{
	_T("Left Tool No.1"),
	_T("Left Tool No.2"),
	_T("Left Tool No.3"),
	_T("Right Tool No.4"),
	_T("Right Tool No.5"),
	_T("Right Tool No.6"),
	_T("Ready Position"),
	_T("Left Tool Sensing Up"),
	_T("Left Tool Sensing Down"),
	_T("Right Tool Sensing Up"),
	_T("Right Tool Sensing Down"),
};

int N_TEACHING_POINT_NO[TEACHING_POINT_NUM] =
{
	1,
	2,
	3,
	4,
	5,
	6,
	9,
	19,
	20,
	21,
	22
};

CString STR_OPTION[OPTION_NUM] =
{
	_T("ZLAxis Origin Offset"),
	_T("ZRAxis Origin Offset"),
	_T("Designed Cube Size"),
	_T("Measured Cube Size"),
	_T("Spindle Offset(XR-XL)"),
	_T("Tool Sensing High Speed"),
	_T("Tool Sensing Low Speed"),
	_T("Tool Sensing Margin"),
	_T("Tool Pocket Put Offset")
};

CString STR_NC_FILESTATE[MAX_NCFILE_NUM] = 
{
	_T("Before"),
	_T("Run"),
	_T("Complete"),
	_T("Stop"),
	_T("Error")
};

COLORREF CLR_NC_FILESTATE[NCFILE_STATE_NUM] =
{
	RGB( 200, 120,  80 ),	// Before
	RGB( 120, 200, 120 ),	// Run
	RGB( 120, 120, 200 ),	// Complete
	RGB( 200, 120, 200 ),	// Stop
	RGB( 255, 100, 100 )	// Error
};

CString STR_RUNMODE[RUNMODE_NUM] = 
{
	_T("Stop"),
	_T("ToStop"),
	_T("ToRun"),
	_T("Run"),
	_T("Init."),
	_T("Error"),
	_T("Pause")
};

// input bit 번호 저장 
int SPAStatus::INPUT_BIT_NO[IN_NUM] = 
{
	10001, 10002, 10003, 10004, 
	10005, 10006, 10007, 10008,

	20001, 20002, 20003, 20004, 20005,
	20006, 20007, 20008, 20009, 20010,
	20011, 20012
};

// output bit 번호 저장 
int SPAStatus::OUTPUT_BIT_NO[OUT_NUM] = 
{
	13, 14, 15, 16, 
	17, 18, 19, 20,
	20033, 20034, 20035, 20036, 
	20037, 20038, 20039, 20040, 
	20041, 20042, 20043, 20044, 
	20045, 20046, 20047, 20048
};

// TODO: figure this out
TCHAR* P_USB_NCFILE_PATH = NCFILE_PATH;				// USB 메모리의 NC 파일 경로 ( \\하드 디스크 )

void GET_NCFILE_FULL_PATH( CString& strNCFileName )
{
	strNCFileName = CString( NCFILE_PATH ) + CString( _T("\\") ) + strNCFileName;
}

// FileMgr 화면 컬러 정의
COLORREF CLR_FILEMGR_MAIN		= RGB( 96, 96, 216 );
COLORREF CLR_FILEMGR_REGISTER	= RGB( 198, 255, 255 ); //RGB( 198, 255, 255 );
COLORREF CLR_FILEMGR_COPY		= RGB( 250, 224, 212 );

// Setup 화면 컬러 정의 
COLORREF CLR_SETUP_MAIN			    = RGB( 220, 220, 220 );
COLORREF CLR_SETUP_TEACHING		    = RGB( 198, 255, 255 );
COLORREF CLR_SETUP_TOOL			    = RGB( 209, 255, 202 );
COLORREF CLR_SETUP_ATC			    = RGB( 209, 209, 207 );
COLORREF CLR_SETUP_AUTO_CALIBRATION	= RGB( 255, 0, 0 );
COLORREF CLR_SETUP_AUTO_TEACHING	= RGB( 255, 0, 0 );
COLORREF CLR_SETUP_OPTION		    = RGB( 255, 198, 255 );
COLORREF CLR_BUTTON_EMO_RESET	    = RGB( 255, 64, 64 );
COLORREF CLR_BUTTON_BACK		    = RGB( 128, 128, 220 );
COLORREF CLR_SETUP_IO			    = RGB( 198, 255, 255 );
COLORREF CLR_SETUP_LOG			    = RGB( 250, 244, 192 );
COLORREF CLR_SETUP_BKGND		    = RGB( 190, 190, 190 );
COLORREF CLR_SETUP_MAINTENANCE	    = RGB( 255, 255, 255 );
COLORREF CLR_SOFT_LIMITS	        = RGB( 250, 224, 212 );


CString SAutoCalCoordinateOffsetParam::STR_PARAM_NAME[PARAM_NUM] =
{
	CString( _T("XLPosForXAxisMeasure") ),
	CString( _T("XRPosForXAxisMeasure") ),
	CString( _T("YLPosForXAxisMeasure") ),
	CString( _T("YRPosForXAxisMeasure") ),
	CString( _T("ZLPosForXAxisMeasure") ),
	CString( _T("ZRPosForXAxisMeasure") ),
	CString( _T("XLPosForYZAxisMeasure") ),
	CString( _T("XRPosForYZAxisMeasure") ),
	CString( _T("YLPosForYAxisMeasure") ),
	CString( _T("YRPosForYAxisMeasrue") ),
	CString( _T("ZLPosForYAxisMeasure") ),
	CString( _T("ZRPosForYAxisMeasure") ),
	CString( _T("ZLPosForZAxisMeasure") ),
	CString( _T("ZRPosForZAxisMeasure") ),

	CString( _T("Tool_Diameter_Left") ),
	CString( _T("Tool_Diameter_Right") ),
	CString( _T("Tool_Number_Left") ),
	CString( _T("Tool_Number_Right") ),
	CString( _T("Measure_Count") )
};

BOOL SAutoCalCoordinateOffsetParam::Load( TCHAR* pFilePath, CString& strErrMsg )
{
	CCEIniFile	hIniFile;
	CString		strKeyName;

	if( !hcutil::IsExistDir( CString(pFilePath) ) ) {
		strErrMsg.Format( _T("file not found !") );
		return FALSE;
	}

	hIniFile.Open( pFilePath );

	strKeyName.Format( _T("AutoCal") );

	for( int i = 0; i<(int)PARAM_NUM; i++ ) 
	{
		hIniFile.GetValue( strKeyName, STR_PARAM_NAME[i], (double*)&fParam[i] );
	}

	hIniFile.Close();

	return TRUE;
}

BOOL SAutoCalCoordinateOffsetParam::Save( TCHAR* pFilePath, CString& strErrMsg )
{
	CCEIniFile	hIniFile;
	CString		strKeyName;

	hIniFile.Open( pFilePath );

	strKeyName.Format( _T("AutoCal") );

	for( int i = 0; i<(int)PARAM_NUM; i++ ) 
	{
		hIniFile.SetValue( strKeyName, STR_PARAM_NAME[i], (double)fParam[i] );
	}

	hIniFile.Close();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

BOOL SAutoTeachToolPocketParam::Load(TCHAR* pFilePath, int nNumParams, CString strParamsName[], CString& strErrMsg)
{
	CCEIniFile	hIniFile;
	CString		strKeyName;
	CString		strFilePath;

	strFilePath.Format( _T("%s"), pFilePath );

	if( !hcutil::IsExistFile( strFilePath ) ) {
		strErrMsg.Format( _T("file not found !") );
		return FALSE;
	}

	hIniFile.Open( pFilePath );

	strKeyName.Format( _T("AT_ToolPocket") );

	for( int i = 0; i<nNumParams; i++ ) 
	{
		hIniFile.GetValue( strKeyName, strParamsName[i], (double*)&fParam[i] );
	}

	hIniFile.Close();

	return TRUE;
}

BOOL SAutoTeachToolPocketParam::Save( TCHAR* pFilePath, int nNumParams, CString strParamsName[], CString& strErrMsg )
{
	CCEIniFile	hIniFile;
	CString		strKeyName;

	hIniFile.Open( pFilePath );

	strKeyName.Format( _T("AT_ToolPocket") );

	for( int i = 0; i<nNumParams; i++ ) 
	{
		hIniFile.SetValue( strKeyName, strParamsName[i], (double)fParam[i] );
	}

	hIniFile.Close();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SCoordinateOffsetDataRange::Load( TCHAR* pFilePath, CString& strErrMsg )
{
	TCHAR*	P_AXIS_NAME[pa::AXIS_NUM] = {_T("X_AXIS"), _T("Y_AXIS"), _T("Z_AXIS"), _T("A_AXIS"), _T("B_AXIS") };
	CCEIniFile	hIniFile;
	CString strKeyname;

	hIniFile.Open( pFilePath );

	// 각 축별로 default, min, max값을 읽어 저장한다 
	for( int i = 0; i<pa::AXIS_NUM; i++ )
	{
		strKeyname.Format( _T("%s"), P_AXIS_NAME[i] );

		hIniFile.GetValue( strKeyname, _T("default"), (double*)&fDefault[i] );
		hIniFile.GetValue( strKeyname, _T("min"), (double*)&fMin[i] );
		hIniFile.GetValue( strKeyname, _T("max"), (double*)&fMax[i] );
	}

	hIniFile.Close();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SMeasureParamEtc::Load( TCHAR* pFilePath, CString& strErrMsg )
{
	CCEIniFile	hIniFile;
	CString strKeyname, strValuename;

	hIniFile.Open( pFilePath );

	// [Tool]
	strKeyname.Format( _T("Tool") );
	for( int i = 0; i<8; i++ )
	{
		strValuename.Format( _T("No%d"), i );
		hIniFile.GetValue( strKeyname, strValuename, (int*)&nUsingTool[i] );
	}
	hIniFile.GetValue( strKeyname, _T("DefaultDiameter"), (double*)&fDefaultToolDiameter );
	hIniFile.GetValue( strKeyname, _T("MinDiameter"), (double*)&fMinToolDiameter );
	hIniFile.GetValue( strKeyname, _T("MaxDiameter"), (double*)&fMaxToolDiameter );

	// [Disk]
	strKeyname.Format( _T("Disk") );
	hIniFile.GetValue( strKeyname, _T("DefaultThickness"), (double*)&fDefaultDiskThickness );
	hIniFile.GetValue( strKeyname, _T("MinThickness"), (double*)&fMinDiskThickness );
	hIniFile.GetValue( strKeyname, _T("MaxThickmess"), (double*)&fMaxDiskThickness );

	hIniFile.Close();

	return TRUE;
}

BOOL SMeasureParamEtc::CheckToolNo( BOOL bDirection, int nToolNo, CString& strErrMsg )
{
	BOOL bRet = TRUE; 

	if (bDirection == FALSE)	// 왼쪽이 FALSE, 오른쪽이 TRUE
	{
		if( nToolNo >= 1 && nToolNo <= 3 ) 
		{
			if( pa::PPAStatus->GetMeasureParamEtc()->nUsingTool[nToolNo-1] == 0 ) 
			{
				// 사용할 수 없는 툴 번호 
				bRet = FALSE;
				strErrMsg.Format( _T("Left tool number input error") );
			}
		}
		else 
		{
			bRet = FALSE;
			strErrMsg.Format( _T("Left tool number input error") );
		}
	}
	
	else
	{
		if( nToolNo >= 4 && nToolNo <= 5 ) 
		{
			if( pa::PPAStatus->GetMeasureParamEtc()->nUsingTool[nToolNo-1] == 0 ) 
			{
				// 사용할 수 없는 툴 번호 
				bRet = FALSE;
				strErrMsg.Format( _T("Right tool number input error") );
			}
		}
		else 
		{
			bRet = FALSE;
			strErrMsg.Format( _T("Right tool number input error") );
		}
	}

	return bRet;
}

BOOL SMeasureParamEtc::CheckToolDiameter( BOOL bDirection, double fTD, CString& strErrMsg )
{
	BOOL bRet = TRUE;
	double fMin = fMinToolDiameter;
	double fMax = fMaxToolDiameter;
	
	if (bDirection == FALSE)	// 왼쪽이 FALSE, 오른쪽이 TRUE
	{
		if( fTD < fMin || fTD > fMax ) 
		{
			// 툴 두께 입력 에러 
			bRet = FALSE;
			strErrMsg.Format( _T("Left tool diameter input error (%.3f ~ %.3f)"), fMin, fMax );
		}
	}

	else
	{
		if( fTD < fMin || fTD > fMax ) 
		{
			// 툴 두께 입력 에러 
			bRet = FALSE;
			strErrMsg.Format( _T("Right tool diameter input error (%.3f ~ %.3f)"), fMin, fMax );
		}
	}

	return bRet;
}

void DELETE_OLD_FILE( int nKeepingDays, TCHAR* extension, TCHAR* path )
{
	HANDLE			hr;
	WIN32_FIND_DATA	hWFD;
	TCHAR			szPath[512];

	memset((void*)szPath, 0, sizeof(TCHAR)*512);
	_stprintf( szPath, _T("%s\\*.%s"), path, extension );

	hr = FindFirstFile( szPath, &hWFD );

	while( hr != INVALID_HANDLE_VALUE ) 
	{
		{
			CTime tmBase;
			CTime tmFile;

			tmBase = CTime::GetCurrentTime() - CTimeSpan( nKeepingDays, 0, 0, 0 );
			tmFile = CTime( hWFD.ftLastWriteTime );

			if( tmBase > tmFile ) 
			{
				TCHAR szTemp[512];
				memset((void*)szTemp, 0, sizeof(TCHAR)*512);
				_stprintf( szTemp, _T("%s\\%s"), LOG_PATH, hWFD.cFileName );

				::DeleteFile( szTemp );
			}
		}

		if( FindNextFile( hr, &hWFD ) == FALSE )
		{
			break;
		}
	}

	FindClose( hr );
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void SPowerOnTestConfig::Load( TCHAR* pFileName )
{
	CCEIniFile	hIniFile;
	CString		strKeyName;

	if( pFileName != NULL && hcutil::IsExistFile( pFileName ) == TRUE ) {
		IsLoaded = 1;
	} else {
		IsLoaded = 0;
		return;
	}

	hIniFile.Open( pFileName );

	strKeyName.Format( _T("Tool") );
	
	hIniFile.GetValue( strKeyName, _T("tp_x_min"), (double*)&toolpocket_x_min );
	hIniFile.GetValue( strKeyName, _T("tp_x_max"), (double*)&toolpocket_x_max );
	hIniFile.GetValue( strKeyName, _T("tp_y_min"), (double*)&toolpocket_y_min );
	hIniFile.GetValue( strKeyName, _T("tp_y_max"), (double*)&toolpocket_y_max );

	hIniFile.Close();
}

//////////////////////////////////////////////////////////////////////////

SModelInfo2 MODEL_INFO;

SModelInfo2::SModelInfo2()
{
	memset((void*)this, 0, sizeof(SModelInfo2));
}

SModelInfo2::~SModelInfo2()
{

}

// 모델정보를 파일에서 읽어 초기화 한다 
void SModelInfo2::Load()
{
	CString		strFilePath(INI_MODEL_INFO_PATH);
	CCEIniFile	hIniFile;
	CString		strKeyName;
	int			num = 0;
	int			model_no = 0;
	int			ntemp;

	hIniFile.Open( strFilePath );

	hIniFile.GetValue( _T("Common"), _T("Num"), (int*)&num );
	hIniFile.GetValue( _T("Common"), _T("ModelNo"), (int*)&model_no );

	//////////////////////////////////////////////////////////////////////////

	strKeyName.Format( _T("Model%d"), model_no );

	hModelID = (EN_MODEL)model_no;

	hIniFile.GetValue( strKeyName, _T("NumAxis"), (int*)&nNumAxis );
	hIniFile.GetValue( strKeyName, _T("MachineName"), (LPTSTR)szMachineName, 64 );
	hIniFile.GetValue( strKeyName, _T("MachineID"), (int*)&nMachineID );			
	// maybe check whether the machine id is within limits?
	// if( nMachineID < 0 || nMachineID > 99 )
	// {
		// nMachineID = 0;
	// }
	hIniFile.GetValue( strKeyName, _T("ModelName"), (LPTSTR)szModelName, 64 );
	hIniFile.GetValue( strKeyName, _T("DisplayModelName"), (LPTSTR)szDisplayModelName, 64 );

	hIniFile.GetValue( strKeyName, _T("bk_image_name"), (LPTSTR)szBkgndImageFileName, 64 );
	hIniFile.GetValue( strKeyName, _T("mn_font_sx"), (int*)&nModelNameFontSX );
	hIniFile.GetValue( strKeyName, _T("mn_font_sy"), (int*)&nModelNameFontSY );
	hIniFile.GetValue( strKeyName, _T("mn_offset_x"), (int*)&nModelNameOffsetX );
	hIniFile.GetValue( strKeyName, _T("mn_offset_y"), (int*)&nModelNameOffsetY );
	hIniFile.GetValue( strKeyName, _T("using_atc_door"), (int*)&nUsingATCDoor );
	hIniFile.GetValue( strKeyName, _T("input_filename"), (LPTSTR)szInputFileName, 128 );
	hIniFile.GetValue( strKeyName, _T("output_filename"), (LPTSTR)szOutputFileName, 128 );
	hIniFile.GetValue( strKeyName, _T("m28_type"), (int*)&ntemp ); hM28Type = (EN_M28_TYPE)ntemp;
	hIniFile.GetValue( strKeyName, _T("pa_ctrl_type"), (int*)&ntemp ); hPaCtrlType = (EN_PA_CTRL_TYPE)ntemp;
	hIniFile.GetValue( strKeyName, _T("air_pressure_sensor_type"), (int*)&ntemp ); hAirPressureSensorType = (EN_AIR_PRESSURE_SENEOR_TYPE)ntemp;
	
	hIniFile.GetValue( strKeyName, _T("toolpocket_type"), (int*)&nToolPocketType); 
	hIniFile.GetValue( strKeyName, _T("ncfile_machine_name"), (LPTSTR)szNcFileMachineName, 64);
	//////////////////////////////////////////////////////////////////////////

	hIniFile.Close();
}

}
