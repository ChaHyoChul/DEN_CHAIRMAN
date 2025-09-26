#include "StdAfx.h"
#include "UIToolPocketAutoTeachingP1.h"

pa::CUIToolPocketAutoTeachingP1::CUIToolPocketAutoTeachingP1(void)
{
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_TOOL_DIAMETER_LEFT] = _T("Tool_Diameter_Left");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_TOOL_DIAMETER_RIGHT] = _T("Tool_Diameter_Right"); 
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_TOOL_NUMBER_LEFT] = _T("Tool_Number_Left");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_TOOL_NUMBER_RIGHT] = _T("Tool_Number_Right");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_MEASURE_COUNT] = _T("Measure_Count");

	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_1_X_OFFSET] = _T("Left_Tool1_X (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_1_Y_OFFSET] = _T("Left_Tool1_Y (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_2_X_OFFSET] = _T("Left_Tool2_X (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_2_Y_OFFSET] = _T("Left_Tool2_Y (G54)"); 
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_3_X_OFFSET] = _T("Left_Tool3_X (G54)"); 
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_3_Y_OFFSET] = _T("Left_Tool3_Y (G54)"); 

	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_4_X_OFFSET] = _T("Right_Tool4_X (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_4_Y_OFFSET] = _T("Right_Tool4_Y (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_5_X_OFFSET] = _T("Right_Tool5_X (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_5_Y_OFFSET] = _T("Right_Tool5_Y (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_6_X_OFFSET] = _T("Right_Tool6_X (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_6_Y_OFFSET] = _T("Right_Tool6_Y (G54)"); 

	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_JIG_TOOL3_X_OFFSET] = _T("Left_JIG_to_T3_X");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_JIG_TOOL3_Y_OFFSET] = _T("Left_JIG_to_T3_Y");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_JIG_TOOL6_X_OFFSET] = _T("Right_JIG_to_T6_X");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_JIG_TOOL6_Y_OFFSET] = _T("Right_JIG_to_T6_Y");

	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XL_X_UP_G54] = _T("CHK_XL_X_UP (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XL_X_DOWN_G54] = _T("CHK_XL_X_DOWN (G54)"); 
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XL_Y_G54] = _T("CHK_XL_Y (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XL_Z_G54] = _T("CHK_XL_Z (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_YL_Y_G54] = _T("CHK_YL_Y (G54)"); 
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_YL_Z_G54] = _T("CHK_YL_Z (G54)"); 

	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XR_X_UP_G54] = _T("CHK_XR_X_UP (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XR_X_DOWN_G54] = _T("CHK_XR_X_DOWN (G54)"); 
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XR_Y_G54] = _T("CHK_XR_Y (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XR_Z_G54] = _T("CHK_XR_Z (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_YR_Y_G54] = _T("CHK_YR_Y (G54)");
	strParamsName[pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_YR_Z_G54] = _T("CHK_YR_Z (G54)"); 
}

pa::CUIToolPocketAutoTeachingP1::~CUIToolPocketAutoTeachingP1(void)
{
}

BOOL pa::CUIToolPocketAutoTeachingP1::SaveParamsData(void)
{
	CCEIniFile iniFile;
	double val;

	if (!iniFile.Open(strFilePath_)) return FALSE;

	for (int i = 0; i<UIToolPocketAutoTeachingParam::PARAM_NUM; i++)
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

BOOL pa::CUIToolPocketAutoTeachingP1::LoadParamsData(void)
{
	CCEIniFile iniFile;
	double val;

	if (!iniFile.Open(strFilePath_)) return FALSE;

	for (int i = 0; i<UIToolPocketAutoTeachingParam::PARAM_NUM; i++)
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

int pa::CUIToolPocketAutoTeachingP1::GetParamsNum(void)
{
	return PARAM_NUM;
}

CString pa::CUIToolPocketAutoTeachingP1::GetParamName(UINT nID)
{
	return strParamsName[nID];
}

CString pa::CUIToolPocketAutoTeachingP1::GetParamDataS(UINT nID)
{
	CString str;

	switch (nID)
	{
	case pa::CUIToolPocketAutoTeachingP1::PARAM_TOOL_DIAMETER_LEFT: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_TOOL_DIAMETER_RIGHT: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_1_X_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_1_Y_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_2_X_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_2_Y_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_3_X_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_LEFT_TOOL_3_Y_OFFSET: 

	case pa::CUIToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_4_X_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_4_Y_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_5_X_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_5_Y_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_6_X_OFFSET: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_RIGHT_TOOL_6_Y_OFFSET: 

	case pa::CUIToolPocketAutoTeachingP1::PARAM_JIG_TOOL3_X_OFFSET:
	case pa::CUIToolPocketAutoTeachingP1::PARAM_JIG_TOOL3_Y_OFFSET:
	case pa::CUIToolPocketAutoTeachingP1::PARAM_JIG_TOOL6_X_OFFSET:
	case pa::CUIToolPocketAutoTeachingP1::PARAM_JIG_TOOL6_Y_OFFSET:

	case pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XL_X_UP_G54:
	case pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XL_X_DOWN_G54:
	case pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XL_Y_G54:
	case pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XL_Z_G54: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_YL_Y_G54: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_YL_Z_G54: 

	case pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XR_X_UP_G54:
	case pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XR_X_DOWN_G54: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XR_Y_G54:
	case pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_XR_Z_G54: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_YR_Y_G54: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_CHK_YR_Z_G54: 
	default:
		str.Format(_T("%.3f"), pUIToolPocketAutoTeachingParam_->fParam[nID]);
		break;

	case pa::CUIToolPocketAutoTeachingP1::PARAM_TOOL_NUMBER_LEFT:
	case pa::CUIToolPocketAutoTeachingP1::PARAM_TOOL_NUMBER_RIGHT: 
	case pa::CUIToolPocketAutoTeachingP1::PARAM_MEASURE_COUNT:
		str.Format(_T("%d"), (int)(pUIToolPocketAutoTeachingParam_->fParam[nID]));
		break;
	}

	return str;
}

double pa::CUIToolPocketAutoTeachingP1::GetParamDataD(UINT nID)
{
	return (double)(pUIToolPocketAutoTeachingParam_->fParam[nID]);
}

double* pa::CUIToolPocketAutoTeachingP1::GetParamsPtr(UINT nID)
{
	return (double*)&(pUIToolPocketAutoTeachingParam_->fParam[nID]);
}

void pa::CUIToolPocketAutoTeachingP1::SetParamDataD(UINT nID, double val)
{
	pUIToolPocketAutoTeachingParam_->fParam[nID] = val;
}

// 툴 개수 리턴 
int pa::CUIToolPocketAutoTeachingP1::GetToolsNum(void)
{
	return 6;
}

// 툴 영역의 위치 계산 
void pa::CUIToolPocketAutoTeachingP1::InitToolPocketLayout()
{

}

CRect pa::CUIToolPocketAutoTeachingP1::GetToolPos(UINT inex)
{
	return CRect(0, 0, 0, 0);
}

CRect pa::CUIToolPocketAutoTeachingP1::GetToolSelPos(UINT index)
{
	return CRect(0, 0, 0, 0);
}

CRect pa::CUIToolPocketAutoTeachingP1::GetToolErrorMsgPos(UINT index)
{
	return CRect(0, 0, 0, 0);
}

