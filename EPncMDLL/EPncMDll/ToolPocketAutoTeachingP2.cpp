#include "StdAfx.h"
#include "ToolPocketAutoTeachingP2.h"

//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////

pa::CToolPocketAutoTeachingP2::CToolPocketAutoTeachingP2(void)
{
	strParamsName[CToolPocketAutoTeachingP2::PARAM_TOOL_DIAMETER_LEFT] = _T("Tool_Diameter_Left"); 
	strParamsName[CToolPocketAutoTeachingP2::PARAM_TOOL_DIAMETER_RIGHT] = _T("Tool_Diameter_Right");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_TOOL_NUMBER_LEFT] = _T("Tool_Number_Left");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_TOOL_NUMBER_RIGHT] = _T("Tool_Number_Right");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_MEASURE_COUNT] = _T("Measure_Count");

	strParamsName[CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_1_X_OFFSET] = _T("Left_Tool1_X (G54)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_1_Y_OFFSET] = _T("Left_Tool1_Y (G54)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_2_X_OFFSET] = _T("Left_Tool2_X (G54)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_2_Y_OFFSET] = _T("Left_Tool2_Y (G54)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_X_OFFSET] = _T("Left_Tool3_X (G54)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_Y_OFFSET] = _T("Left_Tool3_Y (G54)");

	strParamsName[CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_4_X_OFFSET] = _T("Right_Tool4_X (G54)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_4_Y_OFFSET] = _T("Right_Tool4_Y (G54)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_5_X_OFFSET] = _T("Right_Tool5_X (G54)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_5_Y_OFFSET] = _T("Right_Tool5_Y (G54)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_X_OFFSET] = _T("Right_Tool6_X (G54)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_Y_OFFSET] = _T("Right_Tool6_Y (G54)");

	strParamsName[CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_X_OFFSET_FROM_BASE] = _T("Left_Tool3_X_Offset_From_Base");		// 11 
	strParamsName[CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_Y_OFFSET_FROM_BASE] = _T("Left_Tool3_Y_Offset_From_Base");		// 17
	strParamsName[CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_Y_PITCH]			= _T("Left_Tool_Y_Pitch");			// 8.5 
	strParamsName[CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_Y_SENSOR_OFFSET]	= _T("Left_Tool_Y_Sensor_Offset");	// 

	strParamsName[CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_X_OFFSET_FROM_BASE]	= _T("Right_Tool6_X_Offset_From_Base");		// 7
	strParamsName[CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_Y_OFFSET_FROM_BASE]	= _T("Right_Tool6_Y_Offset_From_Base");		// 17 
	strParamsName[CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_Y_PITCH]			= _T("Right_Tool_Y_Pitch");			// 8.5
	strParamsName[CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_Y_SENSOR_OFFSET]	= _T("Right_Tool_Y_Sensor_Offset");	// 

	strParamsName[CToolPocketAutoTeachingP2::PARAM_CHK_XL_X_G53]	= _T("CHK_XL_X (G53)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_CHK_XL_Y1_G53]	= _T("CHK_XL_Y1 (G53)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_CHK_XL_Y2_G53]	= _T("CHK_XL_Y2 (G53)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_CHK_YL_X_G53]	= _T("CHK_YL_X (G53)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_CHK_YL_Y_G53]	= _T("CHK_YL_Y (G53)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_CHK_XYL_Z_G53]	= _T("CHK_XYL_Z (G53)");

	strParamsName[CToolPocketAutoTeachingP2::PARAM_CHK_XR_X_G53]	= _T("CHK_XR_X (G53)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_CHK_XR_Y1_G53]	= _T("CHK_XR_Y1 (G53)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_CHK_XR_Y2_G53]	= _T("CHK_XR_Y2 (G53)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_CHK_YR_X_G53]	= _T("CHK_YR_X (G53)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_CHK_YR_Y_G53]	= _T("CHK_YR_Y (G53)");
	strParamsName[CToolPocketAutoTeachingP2::PARAM_CHK_XYR_Z_G53]	= _T("CHK_XYR_Z (G53)");
}

pa::CToolPocketAutoTeachingP2::~CToolPocketAutoTeachingP2(void)
{
}

BOOL pa::CToolPocketAutoTeachingP2::LoadParams(TCHAR* pFilePath, CString& strErrMsg)
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
	for (int i = 0; i<CToolPocketAutoTeachingP2::PARAM_NUM; i++)
	{
		hIniFile.GetValue(strKeyName, strParamsName[i], (double*)&fTemp);
		pToolPocketAutoTeachingParam_->fParam[i] = fTemp;
	}

	return TRUE;
}

BOOL pa::CToolPocketAutoTeachingP2::SaveParams(TCHAR* pFileParh, CString& strErrMsg)
{
	CCEIniFile hIniFile;
	CString strKeyName;
	CString strFilePath;
	double fTemp = 0.0;

	strFilePath.Format(_T("%s"), pFileParh);

	hIniFile.Open(pFileParh);
	strKeyName.Format(_T("AT_ToolPocket"));
	for (int i = 0; i<CToolPocketAutoTeachingP2::PARAM_NUM; i++)
	{
		fTemp = pToolPocketAutoTeachingParam_->fParam[i];
		hIniFile.SetValue(strKeyName, strParamsName[i], (double)fTemp);
	}

	return TRUE;
}

CString pa::CToolPocketAutoTeachingP2::GetParamName(UINT nID)
{
	return strParamsName[nID];
}

double pa::CToolPocketAutoTeachingP2::GetParamValue(UINT nID)
{
	if (pToolPocketAutoTeachingParam_ == NULL) return 0.0;
	return pToolPocketAutoTeachingParam_->fParam[nID];
}

void pa::CToolPocketAutoTeachingP2::SetParamValue(UINT nID, double fValue)
{
	if (pToolPocketAutoTeachingParam_ == NULL) return ;
	pToolPocketAutoTeachingParam_->fParam[nID] = fValue;
}


BOOL pa::CToolPocketAutoTeachingP2::RunForReCalc(BOOL bResetStep)
{
	int& step = nStepNo_;
	CString strLog;
	pa::SConfigData* pSCD = (pa::PConfig->pConfig_);
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
		step = 140;
		break; 

	case 110:
		// Left tool pocket 
		if (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[0] != 0) 
		{
			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1][pa::AXIS_X] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + GetParamValue(CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_1_X_OFFSET);
			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1][pa::AXIS_Y] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + GetParamValue(CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_1_Y_OFFSET);

			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2][pa::AXIS_X] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + GetParamValue(CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_2_X_OFFSET);
			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2][pa::AXIS_Y] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + GetParamValue(CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_2_Y_OFFSET);

			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_X] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + GetParamValue(CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_X_OFFSET);
			pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_Y] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + GetParamValue(CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_Y_OFFSET);

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
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + GetParamValue(CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_4_X_OFFSET);
			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4][pa::AXIS_Y] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + GetParamValue(CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_4_Y_OFFSET);

			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5][pa::AXIS_X] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + GetParamValue(CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_5_X_OFFSET);
			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5][pa::AXIS_Y] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + GetParamValue(CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_5_Y_OFFSET);

			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_X] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + GetParamValue(CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_X_OFFSET);
			pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_Y] = 
				pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + GetParamValue(CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_Y_OFFSET);

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

BOOL pa::CToolPocketAutoTeachingP2::Run(BOOL bResetStep)
{
	int& step = nStepNo_;
	CString strLog;
	pa::SConfigData* pSCD = (pa::PConfig->pConfig_);
	SToolPocketAutoTeachingParam* params = (SToolPocketAutoTeachingParam*)(pToolPocketAutoTeachingParam_);

	// Tool number 
	static int TOOL_NO_L = 0;
	static int TOOL_NO_R = 0;
	// Tool diameter 
	const double L_TOOL_DIAMETER = GetParamValue(CToolPocketAutoTeachingP2::PARAM_TOOL_DIAMETER_LEFT);
	const double R_TOOL_DIAMETER = GetParamValue(CToolPocketAutoTeachingP2::PARAM_TOOL_DIAMETER_RIGHT);
	// 
	const int MEASURE_COUNT = (int)(GetParamValue(PARAM_MEASURE_COUNT));
	// 
	const double CHK_XL_X = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_XL_X_G53);
	const double CHK_XL_Y1 = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_XL_Y1_G53);
	const double CHK_XL_Y2 = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_XL_Y2_G53);
	const double CHK_YL_X = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_YL_X_G53);
	const double CHK_YL_Y = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_YL_Y_G53);
	const double CHK_XYL_Z = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_XYL_Z_G53);

	const double CHK_XR_X = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_XR_X_G53);
	const double CHK_XR_Y1 = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_XR_Y1_G53);
	const double CHK_XR_Y2 = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_XR_Y2_G53);
	const double CHK_YR_X = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_YR_X_G53);
	const double CHK_YR_Y = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_YR_Y_G53);
	const double CHK_XYR_Z = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_XYR_Z_G53);

	step = bResetStep ? 1 : step;

	switch (step)
	{
	case 1:
		step = 1000;
		break; 

	case 1000:
		TOOL_NO_L = (int)(GetParamValue(CToolPocketAutoTeachingP2::PARAM_TOOL_NUMBER_LEFT));
		TOOL_NO_R = (int)(GetParamValue(CToolPocketAutoTeachingP2::PARAM_TOOL_NUMBER_RIGHT));
		pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable = 0;
		pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable2 = 0;
		// set tool numbers manually 
		PAMotion->SetToolNumber_CMD_WCTN(TOOL_NO_L, TOOL_NO_R);
		// 
		strLog.Format(_T(""));
		writeLog(strLog, FALSE);
		step = 1010;
		step = 2000;
		break;

		SEND_CMD_MDA(step, 1010, 1020, "M207")
		MOVE_DNE_MDA(step, 1020, 2000)

		//////////////////////////////////////////////////////////////////////////
		// 측정 Left 
		//////////////////////////////////////////////////////////////////////////
	case 2000:
		step = (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[0] != 0) ? 2010: 3000;
		break; 

	case 2010:
		// ZL, ZR to safe position 
		sprintf_s(szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 2020;
		break; 
		SEND_CMD_MDA(step, 2020, 2030, szCommand_)
		MOVE_DNE_MDA(step, 2030, 2040)

	case 2040:
		// P1 측정 
		sprintf_s(szCommand_, 256, "G00 G90 G53 X%.3f Y%.3f", 
			CHK_XL_X, 
			CHK_XL_Y1); 
		step = 2050;
		break; 
		SEND_CMD_MDA(step, 2050, 2060, szCommand_)
		MOVE_DNE_MDA(step, 2060, 2070)

	case 2070:
		// Z축 접근 
		sprintf_s(szCommand_, 256, "G00 G90 G53 Z%.3f", CHK_XYL_Z);
		step = 2080; 
		break;
		SEND_CMD_MDA(step, 2080, 2090, szCommand_)
		MOVE_DNE_MDA(step, 2090, 2100)

	case 2100:
		// 측정: X축 다운  
		PAMotion->SCAL(FALSE, 
			pa::AXIS_X, 
			1, 
			0.01, -0.001, 
			500,  
			MEASURE_COUNT, 
			10, -0.4);
		Sleep(500);
		step = 2110;
		break; 

	case 2110:
		if (PAMotion->MotionDone(CPAAsyncComm::CMD_RND_SCAL, TRUE)) {
			PAMotion->GET_MEASURE_RESULT(); 
			step = 2120;
		}
		break; 
	case 2120:
		fMeasureResult_[0][0] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format(_T("---> Measured result XY1 = %.3f"), fMeasureResult_[0][1]);
		writeLog(strLog, FALSE);
		step = 2130;
		break; 

	case 2130:
		// ZL, ZR to safe position 
		sprintf_s(szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 2140;
		break; 
		SEND_CMD_MDA(step, 2140, 2150, szCommand_)
		MOVE_DNE_MDA(step, 2150, 2160)

	case 2160:
		// P2 측정 
		sprintf_s(szCommand_, 256, "G00 G90 G53 X%.3f Y%.3f", 
			CHK_XL_X, 
			CHK_XL_Y2);
		step = 2170;
		break; 
		SEND_CMD_MDA(step, 2170, 2180, szCommand_)
		MOVE_DNE_MDA(step, 2180, 2190)

	case 2190:
		// Z축 접근 
		sprintf_s(szCommand_, 256, "G00 G90 G53 Z%.3f", CHK_XYL_Z);
		step = 2200;
		break;
		SEND_CMD_MDA(step, 2200, 2210, szCommand_)
		MOVE_DNE_MDA(step, 2210, 2220)

	case 2220:
		// 측정: X축 다운 
		PAMotion->SCAL(FALSE, 
			pa::AXIS_X, 
			1, 
			0.01, -0.001, 
			500,  
			MEASURE_COUNT, 
			10, -0.4);
		Sleep(500);
		step = 2230;
		break; 

	case 2230:
		if (PAMotion->MotionDone(CPAAsyncComm::CMD_RND_SCAL, TRUE)) {
			PAMotion->GET_MEASURE_RESULT();
			step = 2240;
		}
		break; 

	case 2240:
		fMeasureResult_[0][1] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format(_T("---> Measured result XY2 = %.3f"), fMeasureResult_[0][2]);
		writeLog(strLog, FALSE);
		step = 2250;
		break; 

	case 2250:
		// ZL, ZR to safe position 
		sprintf_s(szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 2260;
		break;
		SEND_CMD_MDA(step, 2260, 2270, szCommand_)
		MOVE_DNE_MDA(step, 2270, 2280)

	case 2280:
		// P3 측정 
		sprintf_s(szCommand_, 256, "G00 G90 G53 X%.3f Y%.3f", 
			CHK_YL_X, 
			CHK_YL_Y);
		step = 2290;
		break;
		SEND_CMD_MDA(step, 2290, 2300, szCommand_)
		MOVE_DNE_MDA(step, 2300, 2310)

	case 2310:
		// Z축 접근 
		sprintf_s(szCommand_, 256, "G00 G90 G53 Z%.3f", CHK_XYL_Z); 
		step = 2320;
		break; 
		SEND_CMD_MDA(step, 2320, 2330, szCommand_)
		MOVE_DNE_MDA(step, 2330, 2340)

	case 2340:
		// 측정: Y축+ 방향 
		PAMotion->SCAL(FALSE, 
			pa::AXIS_Y, 
			1, 
			0.01, -0.001, 
			//-0.01, +0.001, 
			500, 
			MEASURE_COUNT, 
			10, 
			-0.4);
			//0.4);
		Sleep(500);
		step = 2350;
		break;

	case 2350:
		if (PAMotion->MotionDone(CPAAsyncComm::CMD_RND_SCAL, TRUE)) {
			PAMotion->GET_MEASURE_RESULT();
			step = 2360;
		}
		break; 
	case 2360:
		fMeasureResult_[0][2] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format(_T("---> Measured result XY2 = %.3f"), fMeasureResult_[0][2]);
		writeLog(strLog, FALSE);
		step = 2370;
		break; 

	case 2370:
		// ZL, SZR to safe position 
		sprintf_s(szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 2380;
		break; 
		SEND_CMD_MDA(step, 2380, 2390, szCommand_)
		MOVE_DNE_MDA(step, 2390, 2400)

	case 2400:
		step = 3000;
		break; 

		//////////////////////////////////////////////////////////////////////////
		// 측정 Right
		//////////////////////////////////////////////////////////////////////////
	case 3000:
		step = (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[1] != 0) ? 3010: 4000;  
		break; 
	
	case 3010:
		// ZL, ZR to safe position 
		sprintf_s(szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 3020;
		break;
		SEND_CMD_MDA(step, 3020, 3030, szCommand_)
		MOVE_DNE_MDA(step, 3030, 3040)

	case 3040:
		// P1 측정 
		sprintf_s(szCommand_, 256, "G00 G90 G53 X%.3f A%.3f", 
			CHK_XR_X, 
			CHK_XR_Y1);
		step = 3050;
		break; 
		SEND_CMD_MDA(step, 3050, 3060, szCommand_)
		MOVE_DNE_MDA(step, 3060, 3070)

	case 3070:
		// Z축 접근 
		sprintf_s(szCommand_, 256, "G00 G90 G53 B%.3f", CHK_XYR_Z);
		step = 3080;
		break; 
		SEND_CMD_MDA(step, 3080, 3090, szCommand_)
		MOVE_DNE_MDA(step, 3090, 3100)
		
	case 3100:
		// 측정: X축 다운 
		PAMotion->SCAL(FALSE, 
			pa::AXIS_X, 
			2, 
			0.01, -0.001, 
			250, 
			MEASURE_COUNT, 
			10, -0.4);
		Sleep(500);
		step = 3110;
		break; 

	case 3110:
		if (PAMotion->MotionDone(CPAAsyncComm::CMD_RND_SCAL, TRUE)) {
			PAMotion->GET_MEASURE_RESULT();
			step = 3120;
		}
		break;

	case 3120:
		fMeasureResult_[1][0] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format(_T("---> Measured result XY1 = %.3f"), fMeasureResult_[0][1]);
		writeLog(strLog, FALSE);
		step = 3130;
		break;

	case 3130:
		// ZL, ZR to safe position 
		sprintf_s(szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 3140;
		break;
		SEND_CMD_MDA(step, 3140, 3150, szCommand_)
		MOVE_DNE_MDA(step, 3150, 3160)

	case 3160:
		// P2 축정 
		sprintf_s(szCommand_, 256, "G00 G90 G53 X%.3f A%.3f", 
			CHK_XR_X, 
			CHK_XR_Y2); 
		step = 3170;
		break; 
		SEND_CMD_MDA(step, 3170, 3180, szCommand_)
		MOVE_DNE_MDA(step, 3180, 3190)

	case 3190:
		// Z축 접근 
		sprintf_s(szCommand_, 256, "G00 G90 G53 B%.3f", CHK_XYR_Z);
		step = 3200;
		break; 
		SEND_CMD_MDA(step, 3200, 3210, szCommand_)
		MOVE_DNE_MDA(step, 3210, 3220)

	case 3220:
		// 측정: X축 다운 
		PAMotion->SCAL(FALSE, 
			pa::AXIS_X, 
			2, 
			0.01, -0.001, 
			250, 
			MEASURE_COUNT, 
			10, -0.4);
		Sleep(500);
		step = 3230;
		break;

	case 3230:
		if (PAMotion->MotionDone(CPAAsyncComm::CMD_RND_SCAL, TRUE)) {
			PAMotion->GET_MEASURE_RESULT();
			step = 3240;
		}
		break; 

	case 3240:
		fMeasureResult_[1][1] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format(_T("---> Measured result XY2 = %.3f"), fMeasureResult_[0][2]);
		writeLog(strLog, FALSE);
		step = 3250;
		break; 

	case 3250:
		// ZL, ZR to safe positon 
		sprintf_s(szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 3260;
		break; 
		SEND_CMD_MDA(step, 3260, 3270, szCommand_)
		MOVE_DNE_MDA(step, 3270, 3280)

	case 3280:
		// P3 측정 
		sprintf_s(szCommand_, 256, "G00 G90 G53 X%.3f A%.3f", 
			CHK_YR_X, 
			CHK_YR_Y);
		step = 3290;
		break; 
		SEND_CMD_MDA(step, 3290, 3300, szCommand_)
		MOVE_DNE_MDA(step, 3300, 3310)

	case 3310:
		// Z축 접근 
		sprintf_s(szCommand_, 256, "G00 G90 G53 B%.3f", CHK_XYR_Z);
		step = 3320;
		break;
		SEND_CMD_MDA(step, 3320, 3330, szCommand_)
		MOVE_DNE_MDA(step, 3330, 3340) 

	case 3340:
		// 측정: Y축+ 방향 
		PAMotion->SCAL(FALSE, 
			pa::AXIS_A, 
			2, 
			0.01, -0.001, 
			250, 
			MEASURE_COUNT, 
			10, 
			-0.4);
		Sleep(500);
		step = 3350;
		break; 

	case 3350:
		if (PAMotion->MotionDone(CPAAsyncComm::CMD_RND_SCAL, TRUE)) {
			PAMotion->GET_MEASURE_RESULT();
			step = 3360;
		}
		break; 

	case 3360:
		fMeasureResult_[1][2] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format(_T("---> Measured result XY2 = %.3f"), fMeasureResult_[1][2]);
		writeLog(strLog, FALSE);
		step = 3370;
		break; 

	case 3370:
		// ZL, ZR to safe position 
		sprintf_s(szCommand_, 256, "G00 G90 G53 Z0 B0");
		step = 3380;
		break;
		SEND_CMD_MDA(step, 3380, 3390, szCommand_)
		MOVE_DNE_MDA(step, 3390, 3400)

	case 3400:
		step = 4000;
		break;

		//////////////////////////////////////////////////////////////////////////
		// 결과 계산 
		//////////////////////////////////////////////////////////////////////////
	case 4000:
		if (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[0] != 0)
		{
			// Left 
			calcLeftToolPocket();
		}
		if (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[1] != 0)
		{
			// Right 
			calcRightToolPocket();
		}
		strLog.Format( _T("Stop AUTO TEACHING (ATC pins)") );
		writeLog( strLog, FALSE );
		step = 4010;
		break; 
		
	case 4010:
		step = 0;
		break;

	}

	return (step == 0) ? TRUE : FALSE;
}

void pa::CToolPocketAutoTeachingP2::calcLeftToolPocket()
{
	pa::SConfigData* pSCD = (pa::PConfig->pConfig_);

	double x1 = fMeasureResult_[0][0];
	double y1 = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_XL_Y1_G53);
	double x2 = fMeasureResult_[0][1];
	double y2 = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_XL_Y2_G53);
	double x3 = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_YL_X_G53);
	double y3 = fMeasureResult_[0][2];

	double tool_diameter_helf = GetParamValue(CToolPocketAutoTeachingP2::PARAM_TOOL_DIAMETER_LEFT) / 2.0;

	// 틀어진 각도 
	double fdX = x1 - x2;
	double fdY = y1 - y2;
	double fRadian = atan(fdX / fdY);
	double fcos = cos(fRadian);
	double fsin = sin(fRadian);

	// 
	double p1 = y1 + (tool_diameter_helf) * fsin;
	double q1 = x1 + (tool_diameter_helf) * fcos;

	double p2 = y2 + (tool_diameter_helf) * fsin;
	double q2 = x2 + (tool_diameter_helf) * fcos;

	double p3 = y3 + (tool_diameter_helf) * fcos;
	double q3 = x3 + (tool_diameter_helf) * fsin;

	// 기준 모서리 위치 
	double p4 = ((p2 - p1)*(p2*q1 - p1*q2) + q3*(q2 - q1)*(q2 - q1) + p3*(p2 - p1)*(q2 - q1)) /
		((p2 - p1)*(p2 - p1) + (q2 - q1)*(q2 - q1));
	double q4 = ((q2 - q1)*(p1*q2 - p2*q1) + p3*(p2 - p1)*(p2 - p1) + q3*(p2 - p1)*(q2 - q1)) /
		((p2 - p1)*(p2 - p1) + (q2 - q1)*(q2 - q1));

	// 각 툴의 위치 
	double fToolPocket[3][2];
// 	double a = GetParamValue(CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_X_OFFSET_FROM_BASE);	//8.5;		// first offset x 
// 	double b = GetParamValue(CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_Y_OFFSET_FROM_BASE);	//11;		// first offset y
	double a = GetParamValue(CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_Y_OFFSET_FROM_BASE);	//8.5;		// first offset x 
	double b = GetParamValue(CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_X_OFFSET_FROM_BASE);	//11;		// first offset y
	double c = GetParamValue(CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_Y_PITCH);				//17.0;	// pitch y 

	// 2025.08.03 X축이 낮아서 Offset의 부호를 바꿈
	fToolPocket[0][0] = p4 + (a) * fcos + (b+c*2) * fsin;
	fToolPocket[0][1] = q4 + (a) * fsin + (b+c*2) * fcos;
	fToolPocket[1][0] = p4 + (a) * fcos + (b+c) * fsin;
	fToolPocket[1][1] = q4 + (a) * fsin + (b+c) * fcos;
	fToolPocket[2][0] = p4 + (a) * fcos + (b) * fsin;
	fToolPocket[2][1] = q4 + (a) * fsin + (b) * fcos;

	pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1][pa::AXIS_X] = fToolPocket[0][0];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1][pa::AXIS_Y] = fToolPocket[0][1];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2][pa::AXIS_X] = fToolPocket[1][0];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2][pa::AXIS_Y] = fToolPocket[1][1];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_X] = fToolPocket[2][0];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_Y] = fToolPocket[2][1];
	
	pa::PAMotion->WTCP(0, 1, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1]);
	pa::PAMotion->WTCP(1, 2, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2]);
	pa::PAMotion->WTCP(2, 3, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3]);

	//////////////////////////////////////////////////////////////////////////
	// 센서 
	double fSensor[2];
	double fSensorOffset = GetParamValue(CToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_Y_SENSOR_OFFSET);
	fSensor[0] = p4 + (a) * fcos + (fSensorOffset) * fsin;
	fSensor[1] = q4 - (a) * fsin + (fSensorOffset) * fcos;
	
	pSCD->fTeachingPoint[pa::TEACHING_POINT_SENSING_UP1][pa::AXIS_X] = fSensor[0];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_SENSING_UP1][pa::AXIS_Y] = fSensor[1];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_SENSING_DN1][pa::AXIS_X] = fSensor[0];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_SENSING_DN1][pa::AXIS_Y] = fSensor[1];
	pa::PAMotion->WTCP(7, 19, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_SENSING_UP1]);
	pa::PAMotion->WTCP(8, 20, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_SENSING_DN1]);

}

void pa::CToolPocketAutoTeachingP2::calcRightToolPocket()
{
	pa::SConfigData* pSCD = (pa::PConfig->pConfig_);

	double x1 = fMeasureResult_[1][0];
	double y1 = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_XL_Y1_G53);
	double x2 = fMeasureResult_[1][1];
	double y2 = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_XL_Y2_G53);
	double x3 = GetParamValue(CToolPocketAutoTeachingP2::PARAM_CHK_YL_X_G53);
	double y3 = fMeasureResult_[1][2];

	double tool_diameter_helf = GetParamValue(CToolPocketAutoTeachingP2::PARAM_TOOL_DIAMETER_LEFT) / 2.0;

	// 틀어진 각도 
	double fdX = x2 - x1;
	double fdY = y2 - y1;
	double fRadian = atan(fdX / fdY);
	double fcos = cos(fRadian);
	double fsin = sin(fRadian);

	// 
	double p1 = y1 + (tool_diameter_helf) * fsin;
	double q1 = x1 + (tool_diameter_helf) * fcos;

	double p2 = y2 + (tool_diameter_helf) * fsin;
	double q2 = x2 + (tool_diameter_helf) * fcos;

	double p3 = y3 + (tool_diameter_helf) * fcos;
	double q3 = x3 + (tool_diameter_helf) * fsin;

	// 기준 모서리 위치 
	double p4 = ((p2 - p1)*(p2*q1 - p1*q2) + q3*(q2 - q1)*(q2 - q1) + p3*(p2 - p1)*(q2 - q1)) /
		((p2 - p1)*(p2 - p1) + (q2 - q1)*(q2 - q1));
	double q4 = ((q2 - q1)*(p1*q2 - p2*q1) + p3*(p2 - p1)*(p2 - p1) + q3*(p2 - p1)*(q2 - q1)) /
		((p2 - p1)*(p2 - p1) + (q2 - q1)*(q2 - q1));

	// 각 툴의 위치 
	double fToolPocket[3][2];
// 	double a = GetParamValue(CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_X_OFFSET_FROM_BASE);	//8.5;		// first offset x 
// 	double b = GetParamValue(CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_Y_OFFSET_FROM_BASE);	//7;		// first offset y
	double a = GetParamValue(CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_Y_OFFSET_FROM_BASE);	//8.5;		// first offset x 
	double b = GetParamValue(CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_X_OFFSET_FROM_BASE);	//7;		// first offset y
	double c = GetParamValue(CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_Y_PITCH);				//17.0;	// pitch y 

	// 2025.08.03 X축이 낮아서 Offset의 부호를 바꿈
	fToolPocket[0][0] = p4 + (a) * fcos + (b+c*2) * fsin;
	fToolPocket[0][1] = q4 + (a) * fsin + (b+c*2) * fcos;
	fToolPocket[1][0] = p4 + (a) * fcos + (b+c) * fsin;
	fToolPocket[1][1] = q4 + (a) * fsin + (b+c) * fcos;
	fToolPocket[2][0] = p4 + (a) * fcos + (b) * fsin;
	fToolPocket[2][1] = q4 + (a) * fsin + (b) * fcos;

	pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4][pa::AXIS_X] = fToolPocket[0][0];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4][pa::AXIS_A] = fToolPocket[0][1];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5][pa::AXIS_X] = fToolPocket[1][0];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5][pa::AXIS_A] = fToolPocket[1][1];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_X] = fToolPocket[2][0];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_A] = fToolPocket[2][1];

	pa::PAMotion->WTCP(3, 4, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4]);
	pa::PAMotion->WTCP(4, 5, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5]);
	pa::PAMotion->WTCP(5, 6, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6]);

	//////////////////////////////////////////////////////////////////////////
	// 센서 
	double fSensor[2];
	double fSensorOffset = GetParamValue(CToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_Y_SENSOR_OFFSET);
	fSensor[0] = p4 + (a) * fcos + (fSensorOffset) * fsin;
	fSensor[1] = q4 - (a) * fsin + (fSensorOffset) * fcos;

	pSCD->fTeachingPoint[pa::TEACHING_POINT_SENSING_UP2][pa::AXIS_X] = fSensor[0];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_SENSING_UP2][pa::AXIS_Y] = fSensor[1];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_SENSING_DN2][pa::AXIS_X] = fSensor[0];
	pSCD->fTeachingPoint[pa::TEACHING_POINT_SENSING_DN2][pa::AXIS_Y] = fSensor[1];
	pa::PAMotion->WTCP(9, 21, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_SENSING_UP2]);
	pa::PAMotion->WTCP(10, 22, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_SENSING_DN2]);
}
