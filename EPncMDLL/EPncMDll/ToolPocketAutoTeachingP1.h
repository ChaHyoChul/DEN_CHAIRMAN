#pragma once

namespace pa
{
	class CToolPocketAutoTeachingP1 : public CToolPocketAutoTeachingBase
	{
	public:
		enum ToolPocketAutoTeachingParam
		{
			PARAM_TOOL_DIAMETER_LEFT = 0, 
			PARAM_TOOL_DIAMETER_RIGHT, 
			PARAM_TOOL_NUMBER_LEFT, 
			PARAM_TOOL_NUMBER_RIGHT, 
			PARAM_MEASURE_COUNT, 

			PARAM_LEFT_TOOL_1_X_OFFSET, 
			PARAM_LEFT_TOOL_1_Y_OFFSET, 
			PARAM_LEFT_TOOL_2_X_OFFSET, 
			PARAM_LEFT_TOOL_2_Y_OFFSET, 
			PARAM_LEFT_TOOL_3_X_OFFSET, 
			PARAM_LEFT_TOOL_3_Y_OFFSET, 

			PARAM_RIGHT_TOOL_4_X_OFFSET, 
			PARAM_RIGHT_TOOL_4_Y_OFFSET, 
			PARAM_RIGHT_TOOL_5_X_OFFSET, 
			PARAM_RIGHT_TOOL_5_Y_OFFSET, 
			PARAM_RIGHT_TOOL_6_X_OFFSET, 
			PARAM_RIGHT_TOOL_6_Y_OFFSET, 

			PARAM_JIG_TOOL3_X_OFFSET,
			PARAM_JIG_TOOL3_Y_OFFSET,
			PARAM_JIG_TOOL6_X_OFFSET,
			PARAM_JIG_TOOL6_Y_OFFSET,

			PARAM_CHK_XL_X_UP_G54, 
			PARAM_CHK_XL_X_DOWN_G54, 
			PARAM_CHK_XL_Y_G54, 
			PARAM_CHK_XL_Z_G54, 
			PARAM_CHK_YL_Y_G54, 
			PARAM_CHK_YL_Z_G54, 

			PARAM_CHK_XR_X_UP_G54, 
			PARAM_CHK_XR_X_DOWN_G54, 
			PARAM_CHK_XR_Y_G54, 
			PARAM_CHK_XR_Z_G54, 
			PARAM_CHK_YR_Y_G54, 
			PARAM_CHK_YR_Z_G54, 

			PARAM_NUM
		};

		CString strParamsName[PARAM_NUM];

	public:
		virtual BOOL LoadParams(TCHAR* pFileParh, CString& strErrMsg);
		virtual BOOL SaveParams(TCHAR* pFileParh, CString& strErrMsg);

		virtual CString GetParamName(UINT nID);
		virtual double GetParamValue(UINT nID);
		virtual void SetParamValue(UINT nID, double fValue);

		virtual BOOL RunForReCalc(BOOL bResetStep);
		virtual BOOL Run(BOOL bResetStep);

	public:
		CToolPocketAutoTeachingP1(void);
		~CToolPocketAutoTeachingP1(void);
	};
}