#include "StdAfx.h"
#include "ToolPocketAutoTeachingP1.h"

#define SEND_CMD_MDA(StepVar, StepNo, NextStepNo, LineCmd)					\
	case StepNo:															\
		PAMotion->MDA(FALSE, LineCmd);										\
		Sleep(300);															\
		StepVar = NextStepNo;												\
		break;																\

#define MOVE_DNE_MDA(StepVar, StepNo, NextStepNo)							\
	case StepNo:															\
		if( PAMotion->MotionDone(CPAAsyncComm::CMD_RND_MDA, TRUE) )			\
			StepVar = NextStepNo;											\
		break;																\


pa::CToolPocketAutoTeachingP1::CToolPocketAutoTeachingP1(void)
{
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_TOOL_DIAMETER_LEFT] = _T("Tool_Diameter_Left");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_TOOL_DIAMETER_RIGHT] = _T("Tool_Diameter_Right"); 
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_TOOL_NUMBER_LEFT] = _T("Tool_Number_Left");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_TOOL_NUMBER_RIGHT] = _T("Tool_Number_Right");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_MEASURE_COUNT] = _T("Measure_Count");

	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_1_X_OFFSET] = _T("Left_Tool1_X (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_1_Y_OFFSET] = _T("Left_Tool1_Y (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_2_X_OFFSET] = _T("Left_Tool2_X (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_2_Y_OFFSET] = _T("Left_Tool2_Y (G54)"); 
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_3_X_OFFSET] = _T("Left_Tool3_X (G54)"); 
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_3_Y_OFFSET] = _T("Left_Tool3_Y (G54)"); 

	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_4_X_OFFSET] = _T("Right_Tool4_X (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_4_Y_OFFSET] = _T("Right_Tool4_Y (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_5_X_OFFSET] = _T("Right_Tool5_X (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_5_Y_OFFSET] = _T("Right_Tool5_Y (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_6_X_OFFSET] = _T("Right_Tool6_X (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_6_Y_OFFSET] = _T("Right_Tool6_Y (G54)"); 

	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_JIG_TOOL3_X_OFFSET] = _T("Left_JIG_to_T3_X");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_JIG_TOOL3_Y_OFFSET] = _T("Left_JIG_to_T3_Y");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_JIG_TOOL6_X_OFFSET] = _T("Right_JIG_to_T6_X");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_JIG_TOOL6_Y_OFFSET] = _T("Right_JIG_to_T6_Y");

	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_CHK_XL_X_UP_G54] = _T("CHK_XL_X_UP (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_CHK_XL_X_DOWN_G54] = _T("CHK_XL_X_DOWN (G54)"); 
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_CHK_XL_Y_G54] = _T("CHK_XL_Y (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_CHK_XL_Z_G54] = _T("CHK_XL_Z (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_CHK_YL_Y_G54] = _T("CHK_YL_Y (G54)"); 
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_CHK_YL_Z_G54] = _T("CHK_YL_Z (G54)"); 

	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_CHK_XR_X_UP_G54] = _T("CHK_XR_X_UP (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_CHK_XR_X_DOWN_G54] = _T("CHK_XR_X_DOWN (G54)"); 
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_CHK_XR_Y_G54] = _T("CHK_XR_Y (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_CHK_XR_Z_G54] = _T("CHK_XR_Z (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_CHK_YR_Y_G54] = _T("CHK_YR_Y (G54)");
	strParamsName[pa::CToolPocketAutoTeachingP1::PARAM_CHK_YR_Z_G54] = _T("CHK_ZR_Z (G54)"); 
}

pa::CToolPocketAutoTeachingP1::~CToolPocketAutoTeachingP1(void)
{
}

//////////////////////////////////////////////////////////////////////////

BOOL pa::CToolPocketAutoTeachingP1::LoadParams(TCHAR* pFilePath, CString& strErrMsg)
{
	CCEIniFile hIniFile;
	CString strKeyName; 
	CString strFilePath;
	double fTemp = 0.0;

	strFilePath.Format(_T("%s"), pFilePath);

	if (!hcutil::IsExistFile(strFilePath)) 
	{
		strErrMsg.Format(_T("file not found"));
		return FALSE;
	}

	hIniFile.Open(pFilePath);

	strKeyName.Format(_T("AT_ToolPocket"));
	for (int i = 0; i<ToolPocketAutoTeachingParam::PARAM_NUM; i++)
	{
		hIniFile.GetValue(strKeyName, strParamsName[i], (double*)&fTemp);
		pToolPocketAutoTeachingParam_->fParam[i] = fTemp;
	}

	return TRUE;
}

BOOL pa::CToolPocketAutoTeachingP1::SaveParams(TCHAR* pFileParh, CString& strErrMsg)
{
	CCEIniFile hIniFile;
	CString strKeyName;
	CString strFilePath;
	double fTemp = 0.0;

	strFilePath.Format(_T("%s"), pFileParh);

	hIniFile.Open(pFileParh);
	strKeyName.Format(_T("AT_ToolPocket"));
	for (int i = 0; i<ToolPocketAutoTeachingParam::PARAM_NUM; i++)
	{
		fTemp = pToolPocketAutoTeachingParam_->fParam[i];
		hIniFile.SetValue(strKeyName, strParamsName[i], (double)fTemp);
	}

	return TRUE;
}

CString pa::CToolPocketAutoTeachingP1::GetParamName(UINT nID)
{
	return strParamsName[nID];
}

double pa::CToolPocketAutoTeachingP1::GetParamValue(UINT nID)
{
	if (pToolPocketAutoTeachingParam_ == NULL) return 0.0;
	return pToolPocketAutoTeachingParam_->fParam[nID];
}

void pa::CToolPocketAutoTeachingP1::SetParamValue(UINT nID, double fValue)
{
	if (pToolPocketAutoTeachingParam_ == NULL) return ;
	pToolPocketAutoTeachingParam_->fParam[nID] = fValue;
}

BOOL pa::CToolPocketAutoTeachingP1::RunForReCalc(BOOL bResetStep)
{
	int& step = nStepNo_;
	CString strLog;
	pa::SConfigData* pSCD = (pa::PConfig->pConfig_);
//	pa::SToolPocketAutoTeachingParam* params = (pa::SAutoCalCoordinateOffsetParam*)(pToolPocketAutoTeachingParam_);
	SToolPocketAutoTeachingParam* params = (SToolPocketAutoTeachingParam*)(pToolPocketAutoTeachingParam_);

	step = bResetStep ? 1 : step;

	switch (step)
	{
	case 1: 
		step = 100;
		break; 

		//////////////////////////////////////////////////////////////////////////
		// Offset 적용만 한다 
		//////////////////////////////////////////////////////////////////////////
	case 100:
		strLog.Format(_T("start. auto taching for tool pocket (type=3"));
		writeLog(strLog, FALSE);
		step = 110;
		break; 

	case 110:
		// Left tool pocket 
		if (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[0] != 0) 
		{
			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1][pa::AXIS_X] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + GetParamValue(PARAM_LEFT_TOOL_1_X_OFFSET);
			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1][pa::AXIS_Y] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + GetParamValue(PARAM_LEFT_TOOL_1_Y_OFFSET);

			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2][pa::AXIS_X] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + GetParamValue(PARAM_LEFT_TOOL_2_X_OFFSET);
			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2][pa::AXIS_Y] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + GetParamValue(PARAM_LEFT_TOOL_2_Y_OFFSET);

			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_X] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + GetParamValue(PARAM_LEFT_TOOL_3_X_OFFSET);
			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_Y] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + GetParamValue(PARAM_LEFT_TOOL_3_Y_OFFSET);

			pa::PAMotion->WTCP(0, 1, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1]);
			pa::PAMotion->WTCP(1, 2, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2]);
			pa::PAMotion->WTCP(2, 3, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3]);
		}
		step = 120;
		break; 

	case 120:
		// Right tool pocket 
		if (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[1] != 0)
		{
			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4][pa::AXIS_X] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + GetParamValue(PARAM_RIGHT_TOOL_4_X_OFFSET);
			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4][pa::AXIS_Y] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + GetParamValue(PARAM_RIGHT_TOOL_4_Y_OFFSET);

			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5][pa::AXIS_X] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + GetParamValue(PARAM_RIGHT_TOOL_5_X_OFFSET);
			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5][pa::AXIS_Y] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + GetParamValue(PARAM_RIGHT_TOOL_5_Y_OFFSET);

			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_X] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + GetParamValue(PARAM_RIGHT_TOOL_6_X_OFFSET);
			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_Y] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + GetParamValue(PARAM_RIGHT_TOOL_6_Y_OFFSET);

			pa::PAMotion->WTCP(3, 4, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4]);
			pa::PAMotion->WTCP(4, 5, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5]);
			pa::PAMotion->WTCP(5, 6, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6]);
		}
		step = 140;
		break; 

	case 140:
		strLog.Format(_T("stop. auto taching for tool pocket (type=3"));
		writeLog(strLog, FALSE);
		step = 0;
		break;
	}

	return (step == 0) ? TRUE : FALSE;
}


BOOL pa::CToolPocketAutoTeachingP1::Run(BOOL bResetStep)
{
	int& step = nStepNo_;
	CString strLog;
	pa::SConfigData* pSCD = (pa::PConfig->pConfig_);
	SToolPocketAutoTeachingParam* params = (SToolPocketAutoTeachingParam*)(pToolPocketAutoTeachingParam_);

	// Tool number
	static int TOOL_NO = 0;
	static int TOOL2_NO = 0;
	// Tool diameter 
	const double F_TOOL_DIAMETER = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_TOOL_DIAMETER_LEFT);
	const double F_TOOL2_DIAMETER = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_TOOL_DIAMETER_RIGHT);
	// Offsets of tool pockets #3 and #6 from left/right pins
	const double OFF_T3_XL = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_JIG_TOOL3_X_OFFSET);
	const double OFF_T3_YL = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_JIG_TOOL3_Y_OFFSET);
	const double OFF_T6_XR = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_JIG_TOOL6_X_OFFSET);
	const double OFF_T6_YR = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_JIG_TOOL6_Y_OFFSET);
	// Left pin measurement positions
	const double CHK_XL_X_UP = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_CHK_XL_X_UP_G54);
	const double CHK_XL_X_DOWN = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_CHK_XL_X_DOWN_G54);
	const double CHK_XL_Y = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_CHK_XL_Y_G54);
	const double CHK_XL_Z_G54 = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_CHK_XL_Z_G54);
	const double CHK_YL_Y = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_CHK_YL_Y_G54);
	const double CHK_YL_Z_G54 = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_CHK_YL_Z_G54);
	// Right pin measurement positions
	const double CHK_XR_X_UP = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_CHK_XR_X_UP_G54);
	const double CHK_XR_X_DOWN = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_CHK_XR_X_DOWN_G54);
	const double CHK_XR_Y = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_CHK_XR_Y_G54);
	const double CHK_XR_Z_G54 = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_CHK_XR_Z_G54);
	const double CHK_YR_Y = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_CHK_YR_Y_G54);
	const double CHK_YR_Z_G54 = GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_CHK_YR_Z_G54);
	//
	const double SAFETY_OFFSET = 0.0;

	step = bResetStep ? 1 : step;

	switch (step)
	{
	case 1: 
		step = 1000;
		break; 

	case 1000:
		TOOL_NO = (int)(GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_TOOL_NUMBER_LEFT));
		TOOL2_NO = (int)(GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_TOOL_NUMBER_RIGHT));
		pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable = 0;
		pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable2 = 0;
		// set tool numbers manually
		PAMotion->SetToolNumber_CMD_WCTN( TOOL_NO, TOOL2_NO );
		//
		strLog.Format(_T("---> Tool lengths before: %.3f, %.3f"), pa::PPAStatus->GetPAStatus()->fCurrentToolLenght, pa::PPAStatus->GetPAStatus()->fCurrentTool2Lenght);
		writeLog(strLog, FALSE);
		step = 1010;
		break;
	
		SEND_CMD_MDA(step, 1010, 1020, "M207")
		MOVE_DNE_MDA(step, 1020, 2000)

		//////////////////////////////////////////////////////////////////////////
		// 측정 Left - Bottom 
	case 2000:
		step = (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[0] != 0) ? 2010 : 3000;
		break;
	case 2010:
		// ZL and ZR to safe position
		sprintf_s( szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 2020;
		break;
		SEND_CMD_MDA( step, 2020, 2030, szCommand_ ) 
		MOVE_DNE_MDA( step, 2030, 2040 )

	case 2040:
		// X, YL into position
		sprintf_s( szCommand_, 256, "G00 G90 G54 X%.3f Y%.3f",
			CHK_XL_X_UP,
			CHK_XL_Y-SAFETY_OFFSET);
		step = 2050;
		break;
		SEND_CMD_MDA( step, 2050, 2060, szCommand_ )
		MOVE_DNE_MDA( step, 2060, 2070 )

	case 2070:
		// Extend ZL
		sprintf_s( szCommand_, 256, "G00 G90 G54 Z%.3f",
			CHK_XL_Z_G54);
		step = 2080;
		break;
		SEND_CMD_MDA( step, 2080, 2090, szCommand_ ) 
		MOVE_DNE_MDA( step, 2090, 2100 )												

	case 2100:		
		// Detect in X axis DOWN
		PAMotion->SCAL( FALSE, 
			pa::AXIS_X, 
			1,
			0.01, -0.001,
			100,
			//(int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_MEASURE_COUNT]), 
			(int)(GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_MEASURE_COUNT)), 
			10,
			-0.4 );

		Sleep( 100 );
		step = 2110;
		break;

	case 2110:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) ) {
			step = 2120;
		}
		break;

	case 2120:
		// Get measurement
		PAMotion->GET_MEASURE_RESULT();
		step = 2130;
		break;

	case 2130:	
		fMeasureResult_[0][0] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format(_T("---> Measured result XL1 = %.3f"), fMeasureResult_[0][0]);
		writeLog(strLog, FALSE);
		step = 2140;
		break;

	case 2140:		
		// ZL and ZR to safe position
		sprintf_s( szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 2150;
		break;		
		SEND_CMD_MDA( step, 2150, 2160, szCommand_ ) 
		MOVE_DNE_MDA( step, 2160, 2170 )	

		// Left - Top 
	case 2170:
		// ZL and ZR to safe position
		sprintf_s( szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 2180;
		break;
		SEND_CMD_MDA( step, 2180, 2190, szCommand_ ) 
		MOVE_DNE_MDA( step, 2190, 2200 )

	case 2200:
		// X, YL into position
		sprintf_s( szCommand_, 256, "G00 G90 G54 X%.3f Y%.3f",
			CHK_XL_X_DOWN,
			CHK_XL_Y-SAFETY_OFFSET);
		step = 2210;
		break;
		SEND_CMD_MDA( step, 2210, 2220, szCommand_ ) 
		MOVE_DNE_MDA( step, 2220, 2230 )

	case 2230:
		// Extend ZL
		sprintf_s( szCommand_, 256, "G00 G90 G54 Z%.3f",
			CHK_XL_Z_G54);
		step = 2240;
		break;
		SEND_CMD_MDA( step, 2240, 2250, szCommand_ ) 
		MOVE_DNE_MDA( step, 2250, 2260 )												

	case 2260:		
		// Detect in X axis UP
		PAMotion->SCAL( FALSE, 
			pa::AXIS_X, 
			1,
			-0.01, 0.001,
			100,
			//(int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_MEASURE_COUNT]), 
			GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_MEASURE_COUNT), 
			-10,
			0.4 );

		Sleep( 100 );
		step = 2270;
		break;

	case 2270:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) ) {
			step = 2280;
		}
		break;

	case 2280:
		// Get measurement
		PAMotion->GET_MEASURE_RESULT();
		step = 2290;
		break;

	case 2290:
		fMeasureResult_[0][1] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;	
		strLog.Format(_T("---> Measured result XL2 = %.3f"), fMeasureResult_[0][1]);
		writeLog(strLog, FALSE);
		step = 2300;
		break;

	case 2300:		
		// ZL and ZR to safe position
		sprintf_s( szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 2310;
		break;		
		SEND_CMD_MDA( step, 2310, 2320, szCommand_ ) 
		MOVE_DNE_MDA( step, 2320, 2330 )	

		// Left - Y축 
	case 2330:
		// ZL and ZR to safe position
		sprintf_s( szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 2340;
		break;
		SEND_CMD_MDA( step, 2340, 2350, szCommand_ ) 
		MOVE_DNE_MDA( step, 2350, 2360 )

	case 2360:
		// YL into position
		sprintf_s( szCommand_, 256, "G00 G90 G54 Y%.3f", CHK_YL_Y-SAFETY_OFFSET);
		step = 2370;
		break;
		SEND_CMD_MDA( step, 2370, 2380, szCommand_ ) 
		MOVE_DNE_MDA( step, 2380, 2390 )

	case 2390:
		// X into position
		{
			double Xmeas = (fMeasureResult_[0][0] + fMeasureResult_[0][1]) / 2;
			sprintf_s( szCommand_, 256, "G00 G90 G53 X%.3f", Xmeas);
			step = 2400;
		}
		break;
		SEND_CMD_MDA( step, 2400, 2410, szCommand_ ) 
		MOVE_DNE_MDA( step, 2410, 2420 )

	case 2420:
		// Extend ZL
		sprintf_s( szCommand_, 256, "G00 G90 G54 Z%.3f", CHK_YL_Z_G54);
		step = 2430;
		break;
		SEND_CMD_MDA( step, 2430, 2440, szCommand_ ) 
		MOVE_DNE_MDA( step, 2440, 2450 )												

	case 2450:		
		// Detect in YL axis back
		PAMotion->SCAL( FALSE, 
			pa::AXIS_Y, 
			1,
			0.01, -0.001,
			100,
			//(int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_MEASURE_COUNT]), 
			(int)(GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_MEASURE_COUNT)), 
			6,
			-0.4 );
		Sleep( 100 );
		step = 2460;
		break;

	case 2460:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) ) {
			step = 2470;
		}
		break;

	case 2470:
		// Get measurement
		PAMotion->GET_MEASURE_RESULT();
		step = 2480;
		break;

	case 2480:	
		fMeasureResult_[0][2] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;	
		strLog.Format(_T("---> Measured result YL1 = %.3f"), fMeasureResult_[0][2]);
		writeLog(strLog, FALSE);
		step = 2490;
		break;

	case 2490:
		// ZL and ZR to safe position
		sprintf_s( szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 2500;
		break;		
		SEND_CMD_MDA( step, 2500, 2510, szCommand_ ) 
		MOVE_DNE_MDA( step, 2510, 3000 )	


		//////////////////////////////////////////////////////////////////////////
		// 측정 Right - Bottom
	case 3000:
		step = (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[1] != 0) ? 3010 : 4000;
		break;

	case 3010:
		// ZL and ZR to safe position
		sprintf_s( szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 3020;
		break;
		SEND_CMD_MDA( step, 3020, 3030, szCommand_ ) 
		MOVE_DNE_MDA( step, 3030, 3040 )

	case 3040:
		// X, YR into position
		sprintf_s( szCommand_, 256, "G00 G90 G54 X%.3f A%.3f",
			CHK_XR_X_UP,
			CHK_XR_Y-SAFETY_OFFSET);
		step = 3050;
		break;
		SEND_CMD_MDA( step, 3050, 3060, szCommand_ ) 
		MOVE_DNE_MDA( step, 3060, 3070 )

	case 3070:
		// Extend ZR
		sprintf_s( szCommand_, 256, "G00 G90 G54 B%.3f",
			CHK_XR_Z_G54);
		step = 3080;
		break;
		SEND_CMD_MDA( step, 3080, 3090, szCommand_ ) 
		MOVE_DNE_MDA( step, 3090, 3100 )												

	case 3100:		
		// Detect in X axis DOWN
		PAMotion->SCAL( FALSE, 
			pa::AXIS_X, 
			1,
			0.01, -0.001,
			100,
			// (int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_MEASURE_COUNT]), 
			(int)(GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_MEASURE_COUNT)), 
			10,
			-0.4 );
		Sleep( 100 );
		step = 3110;
		break;

	case 3110:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) ) {
			step = 3120;
		}
		break;

	case 3120:
		// Get measurement
		PAMotion->GET_MEASURE_RESULT();
		step = 3130;
		break;

	case 3130:	
		fMeasureResult_[1][0] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format( _T("---> Measured result XR1 = %.3f"), fMeasureResult_[1][0] );
		writeLog(strLog, FALSE);
		step = 3140;
		break;

	case 3140:		
		// ZL and ZR to safe position
		sprintf_s( szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 3150;
		break;		
		SEND_CMD_MDA( step, 3150, 3160, szCommand_ ) 
		MOVE_DNE_MDA( step, 3160, 3170 )	

		// Right Top
	case 3170:
		// ZL and ZR to safe position
		sprintf_s( szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 3180;
		break;
		SEND_CMD_MDA( step, 3180, 3190, szCommand_ ) 
		MOVE_DNE_MDA( step, 3190, 3200 )

	case 3200:
		// X, YR into position
		sprintf_s( szCommand_, 256, "G00 G90 G54 X%.3f A%.3f",
			CHK_XR_X_DOWN,
			CHK_XR_Y-SAFETY_OFFSET);
		step = 3210;
		break;
		SEND_CMD_MDA( step, 3210, 3220, szCommand_ ) 
		MOVE_DNE_MDA( step, 3220, 3230 )

	case 3230:
		// Extend ZR
		sprintf_s( szCommand_, 256, "G00 G90 G54 B%.3f",
			CHK_XR_Z_G54);
		step = 3240;
		break;
		SEND_CMD_MDA( step, 3240, 3250, szCommand_ ) 
		MOVE_DNE_MDA( step, 3250, 3260 )												

	case 3260:		
		// Detect in X axis UP
		PAMotion->SCAL( FALSE, 
			pa::AXIS_X, 
			1,
			-0.01, 0.001,
			100,
			//(int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_MEASURE_COUNT]), 
			(int)(GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_MEASURE_COUNT)), 
			-10,
			0.4 );
		Sleep( 100 );
		step = 3270;
		break;

	case 3270:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) ) {
			step = 3280;
		}
		break;

	case 3280:
		// Get measurement
		PAMotion->GET_MEASURE_RESULT();
		step = 3290;
		break;

	case 3290:
		fMeasureResult_[1][1] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format(_T("---> Measured result XR2 = %.3f"), fMeasureResult_[1][1]);
		writeLog(strLog, FALSE);
		step = 3300;
		break;

	case 3300:		
		// ZL and ZR to safe position
		sprintf_s( szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 3310;
		break;		
		SEND_CMD_MDA( step, 3310, 3320, szCommand_ ) 
		MOVE_DNE_MDA( step, 3320, 3340 )	

		// Right Y
	case 3340:
		// ZL and ZR to safe position
		sprintf_s( szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 3350;
		break;
		SEND_CMD_MDA( step, 3350, 3360, szCommand_ ) 
		MOVE_DNE_MDA( step, 3360, 3370 )

	case 3370:
		// YR into position
		sprintf_s( szCommand_, 256, "G00 G90 G54 A%.3f", CHK_YR_Y-SAFETY_OFFSET);
		step = 3380;
		break;
		SEND_CMD_MDA( step, 3380, 3390, szCommand_ ) 
		MOVE_DNE_MDA( step, 3390, 3400 )

	case 3400:
		// X into position
		{
			double Xmeas = (fMeasureResult_[1][0] + fMeasureResult_[1][1]) / 2;
			sprintf_s( szCommand_, 256, "G00 G90 G53 X%.3f", Xmeas);
			step = 3410;
		}
		break;
		SEND_CMD_MDA( step, 3410, 3420, szCommand_ ) 
		MOVE_DNE_MDA( step, 3420, 3430 )

	case 3430:
		// Extend ZR
		sprintf_s( szCommand_, 256, "G00 G90 G54 B%.3f",
			CHK_YR_Z_G54);
		step = 3440;
		break;
		SEND_CMD_MDA( step, 3440, 3450, szCommand_ ) 
		MOVE_DNE_MDA( step, 3450, 3460 )												

	case 3460:		
		// Detect in YR axis back
		PAMotion->SCAL( FALSE, 
			pa::AXIS_A, 
			2,
			0.01, -0.001,
			100,
			//(int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_MEASURE_COUNT]), 
			(int)(GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_MEASURE_COUNT)), 
			6,
			-0.4 );
		Sleep( 100 );
		step = 3470;
		break;

	case 3470:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) ) {
			step = 3480;
		}
		break;

	case 3480:
		// Get measurement
		PAMotion->GET_MEASURE_RESULT();
		step = 3490;
		break;

	case 3490:	
		fMeasureResult_[1][2] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format(_T("---> Measured result YR1 = %.3f"), fMeasureResult_[1][2]);
		writeLog(strLog, FALSE);
		step = 3500;
		break;

	case 3500:		
		// ZL and ZR to safe position
		sprintf_s( szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 3510;
		break;		
		SEND_CMD_MDA( step, 3510, 3520, szCommand_ ) 
		MOVE_DNE_MDA( step, 3520, 3530 )	

			// 초기 위치로 이동
	case 3530:
		sprintf_s( szCommand_, 256, "G00 G90 G53 X0 Y0 Z0 A0 B0" );
		step = 3540;
		break;
		SEND_CMD_MDA( step, 3540, 3550, szCommand_ )
		MOVE_DNE_MDA( step, 3550, 4000 )


		// 결과 계산 
	case 4000:
		// Left tool pocket 
		if (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[0] != 0) 
		{
			// #3 
			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_X] = (fMeasureResult_[0][0] + fMeasureResult_[0][1]) / 2.0 + OFF_T3_XL;
			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_Y] = fMeasureResult_[0][2] + OFF_T3_YL + (F_TOOL_DIAMETER / 2.0);
			// #1
			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1][pa::AXIS_X] = 
				pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_X] 
				+ GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_1_X_OFFSET) // param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL1_X_OFFSET] - 
				- GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_3_X_OFFSET);  // param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL3_X_OFFSET];
			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1][pa::AXIS_Y] = 
				pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_Y] 
				+ GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_1_Y_OFFSET)	//+ param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL1_Y_OFFSET] 
				- GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_3_Y_OFFSET); //- param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL3_Y_OFFSET];
			// #2
			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2][pa::AXIS_X] = 
				pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_X] 
				+ GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_2_X_OFFSET)	// + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL2_X_OFFSET] 
				- GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_3_X_OFFSET);	// - param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL3_X_OFFSET];
			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2][pa::AXIS_Y] = 
				pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_Y] 
				+ GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_2_Y_OFFSET)	// + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL2_Y_OFFSET] 
				- GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_3_Y_OFFSET);	// - param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL3_Y_OFFSET];

			pa::PAMotion->WTCP(0, 1, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1]);
			pa::PAMotion->WTCP(1, 2, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2]);
			pa::PAMotion->WTCP(2, 3, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3]);
		}
		step = 4010;
		break;

	case 4010:
		// Right tool pocket 
		if (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[1] != 0)
		{
			// #6 
			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_X] = (fMeasureResult_[1][0] + fMeasureResult_[1][1])/2 + OFF_T6_XR;
			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_A] = fMeasureResult_[1][2] + OFF_T6_YR + F_TOOL2_DIAMETER/2;
			// #4 
			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4][pa::AXIS_X] = 
				pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_X] 
				+ GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_4_X_OFFSET)	// + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL4_X_OFFSET] 
				- GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_6_X_OFFSET);	// - param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL6_X_OFFSET];
			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4][pa::AXIS_A] = 
				pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_A] 
				+ GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_4_Y_OFFSET)	// + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL4_Y_OFFSET] 
				- GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_6_Y_OFFSET);	// - param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL6_Y_OFFSET];
			// #5 
			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5][pa::AXIS_X] = 
				pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_X] 
				+ GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_5_X_OFFSET)	// + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL5_X_OFFSET] 
				- GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_6_X_OFFSET);	// - param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL6_X_OFFSET];
			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5][pa::AXIS_A] = 
				pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_A] 
				+ GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_5_Y_OFFSET)	// + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL5_Y_OFFSET] 
				- GetParamValue(pa::CToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_6_Y_OFFSET); 	// - param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL6_Y_OFFSET];

			pa::PAMotion->WTCP(3, 4, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4]);
			pa::PAMotion->WTCP(4, 5, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5]);
			pa::PAMotion->WTCP(5, 6, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6]);
		}
		step = 4020;
		break;


	case 4020:
		strLog.Format( _T("Stop AUTO TEACHING (ATC pins)") );
		writeLog( strLog, FALSE );
		step = 0;
		break;
	}

	return (step == 0) ? TRUE : FALSE;
}

