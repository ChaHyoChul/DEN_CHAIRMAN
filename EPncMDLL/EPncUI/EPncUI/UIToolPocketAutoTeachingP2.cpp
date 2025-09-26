#include "StdAfx.h"
#include "UIToolPocketAutoTeachingP2.h"

pa::CUIToolPocketAutoTeachingP2::CUIToolPocketAutoTeachingP2(void)
{
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_TOOL_DIAMETER_LEFT] = _T("Tool_Diameter_Left"); 
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_TOOL_DIAMETER_RIGHT] = _T("Tool_Diameter_Right");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_TOOL_NUMBER_LEFT] = _T("Tool_Number_Left");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_TOOL_NUMBER_RIGHT] = _T("Tool_Number_Right");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_MEASURE_COUNT] = _T("Measure_Count");

	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_1_X_OFFSET] = _T("Left_Tool1_X (G54)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_1_Y_OFFSET] = _T("Left_Tool1_Y (G54)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_2_X_OFFSET] = _T("Left_Tool2_X (G54)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_2_Y_OFFSET] = _T("Left_Tool2_Y (G54)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_X_OFFSET] = _T("Left_Tool3_X (G54)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_Y_OFFSET] = _T("Left_Tool3_Y (G54)");

	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_4_X_OFFSET] = _T("Right_Tool4_X (G54)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_4_Y_OFFSET] = _T("Right_Tool4_Y (G54)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_5_X_OFFSET] = _T("Right_Tool5_X (G54)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_5_Y_OFFSET] = _T("Right_Tool5_Y (G54)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_X_OFFSET] = _T("Right_Tool6_X (G54)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_Y_OFFSET] = _T("Right_Tool6_Y (G54)");

	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_X_OFFSET_FROM_BASE] = _T("Left_Tool3_X_Offset_From_Base");		// 11 
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_Y_OFFSET_FROM_BASE] = _T("Left_Tool3_Y_Offset_From_Base");		// 17
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_Y_PITCH]			= _T("Left_Tool_Y_Pitch");			// 8.5 
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_Y_SENSOR_OFFSET]	= _T("Left_Tool_Y_Sensor_Offset");	// 

	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_X_OFFSET_FROM_BASE]	= _T("Right_Tool6_X_Offset_From_Base");		// 7
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_Y_OFFSET_FROM_BASE]	= _T("Right_Tool6_Y_Offset_From_Base");		// 17 
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_Y_PITCH]			= _T("Right_Tool_Y_Pitch");			// 8.5
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_RIGHR_TOOL_Y_SENSOR_OFFSET]	= _T("Right_Tool_Y_Sensor_Offset");	// 

	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_CHK_XL_X_G53]	= _T("CHK_XL_X (G53)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_CHK_XL_Y1_G53]	= _T("CHK_XL_Y1 (G53)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_CHK_XL_Y2_G53]	= _T("CHK_XL_Y2 (G53)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_CHK_YL_X_G53]	= _T("CHK_YL_X (G53)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_CHK_YL_Y_G53]	= _T("CHK_YL_Y (G53)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_CHK_XYL_Z_G53]	= _T("CHK_XYL_Z (G53)");

	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_CHK_XR_X_G53]	= _T("CHK_XR_X (G53)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_CHK_XR_Y1_G53]	= _T("CHK_XR_Y1 (G53)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_CHK_XR_Y2_G53]	= _T("CHK_XR_Y2 (G53)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_CHK_YR_X_G53]	= _T("CHK_YR_X (G53)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_CHK_YR_Y_G53]	= _T("CHK_YR_Y (G53)");
	strParamsName[CUIToolPocketAutoTeachingP2::PARAM_CHK_XYR_Z_G53]	= _T("CHK_XYR_Z (G53)");
}

pa::CUIToolPocketAutoTeachingP2::~CUIToolPocketAutoTeachingP2(void)
{
}

BOOL pa::CUIToolPocketAutoTeachingP2::SaveParamsData(void)
{
	CCEIniFile iniFile;
	double val;

	if (!iniFile.Open(strFilePath_)) return FALSE;

	for (int i = 0; i<CUIToolPocketAutoTeachingP2::PARAM_NUM; i++)
	{
		val = pUIToolPocketAutoTeachingParam_->fParam[i];
		iniFile.SetValue(
			_T("AT_ToolPocket"), 
			strParamsName[i], 
			(double)val);
	}

	iniFile.Close();

	return TRUE;
}

BOOL pa::CUIToolPocketAutoTeachingP2::LoadParamsData(void)
{
	CCEIniFile iniFile;
	double val;

	if (!iniFile.Open(strFilePath_)) return FALSE;

	for (int i = 0; i<CUIToolPocketAutoTeachingP2::PARAM_NUM; i++)
	{
		val = 0.0;
		iniFile.GetValue(
			_T("AT_ToolPocket"), 
			strParamsName[i], 
			(double*)&val);

		pUIToolPocketAutoTeachingParam_->fParam[i] = val;
	}

	iniFile.Close();

	return TRUE;
}

int pa::CUIToolPocketAutoTeachingP2::GetParamsNum(void)
{
	return PARAM_NUM;
}

CString pa::CUIToolPocketAutoTeachingP2::GetParamName(UINT nID)
{
	return strParamsName[nID];
}

CString pa::CUIToolPocketAutoTeachingP2::GetParamDataS(UINT nID)
{
	CString str;
	switch (nID)
	{
	case pa::CUIToolPocketAutoTeachingP2::PARAM_TOOL_DIAMETER_LEFT: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_TOOL_DIAMETER_RIGHT: 

	case pa::CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_1_X_OFFSET:
	case pa::CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_1_Y_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_2_X_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_2_Y_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_X_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_Y_OFFSET: 

	case pa::CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_4_X_OFFSET:
	case pa::CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_4_Y_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_5_X_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_5_Y_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_X_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_Y_OFFSET: 

	case pa::CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_X_OFFSET_FROM_BASE:			// 11 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_3_Y_OFFSET_FROM_BASE:			// 17
	case pa::CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_Y_PITCH:			// 8.5 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_LEFT_TOOL_Y_SENSOR_OFFSET:	// 

	case pa::CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_X_OFFSET_FROM_BASE:		// 7
	case pa::CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_6_Y_OFFSET_FROM_BASE:		// 17 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_RIGHT_TOOL_Y_PITCH:			// 8.5
	case pa::CUIToolPocketAutoTeachingP2::PARAM_RIGHR_TOOL_Y_SENSOR_OFFSET:	// 

	case pa::CUIToolPocketAutoTeachingP2::PARAM_CHK_XL_X_G53:
	case pa::CUIToolPocketAutoTeachingP2::PARAM_CHK_XL_Y1_G53: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_CHK_XL_Y2_G53: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_CHK_YL_X_G53:
	case pa::CUIToolPocketAutoTeachingP2::PARAM_CHK_YL_Y_G53: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_CHK_XYL_Z_G53: 

	case pa::CUIToolPocketAutoTeachingP2::PARAM_CHK_XR_X_G53:
	case pa::CUIToolPocketAutoTeachingP2::PARAM_CHK_XR_Y1_G53: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_CHK_XR_Y2_G53: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_CHK_YR_X_G53:
	case pa::CUIToolPocketAutoTeachingP2::PARAM_CHK_YR_Y_G53: 
	case pa::CUIToolPocketAutoTeachingP2::PARAM_CHK_XYR_Z_G53:
		str.Format(_T("%.3f"), pUIToolPocketAutoTeachingParam_->fParam[nID]);
		break;

	case pa::CUIToolPocketAutoTeachingP2::PARAM_TOOL_NUMBER_LEFT:
	case pa::CUIToolPocketAutoTeachingP2::PARAM_TOOL_NUMBER_RIGHT:
	case pa::CUIToolPocketAutoTeachingP2::PARAM_MEASURE_COUNT:
		str.Format(_T("%d"), (int)(pUIToolPocketAutoTeachingParam_->fParam[nID]));
		break;
	}

	return str;
}

double pa::CUIToolPocketAutoTeachingP2::GetParamDataD(UINT nID)
{
	return (double)(pUIToolPocketAutoTeachingParam_->fParam[nID]);
}

double* pa::CUIToolPocketAutoTeachingP2::GetParamsPtr(UINT nID)
{
	return (double*)&(pUIToolPocketAutoTeachingParam_->fParam[nID]);
}

void pa::CUIToolPocketAutoTeachingP2::SetParamDataD(UINT nID, double val)
{
	pUIToolPocketAutoTeachingParam_->fParam[nID] = val;
}

// 툴 개수 리턴 
int pa::CUIToolPocketAutoTeachingP2::GetToolsNum(void)
{
	return 6;
}

// 툴 영역의 위치 계산 
void pa::CUIToolPocketAutoTeachingP2::InitToolPocketLayout()
{

}

CRect pa::CUIToolPocketAutoTeachingP2::GetToolPos(UINT inex)
{
	return CRect(0, 0, 0, 0);
}

CRect pa::CUIToolPocketAutoTeachingP2::GetToolSelPos(UINT index)
{
	return CRect(0, 0, 0, 0);
}

CRect pa::CUIToolPocketAutoTeachingP2::GetToolErrorMsgPos(UINT index)
{
	return CRect(0, 0, 0, 0);
}

