#pragma once

namespace pa 
{
	class CToolPocketAutoTeachingP2 : public CToolPocketAutoTeachingBase 
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

			PARAM_LEFT_TOOL_3_X_OFFSET_FROM_BASE,			// 11 
			PARAM_LEFT_TOOL_3_Y_OFFSET_FROM_BASE,			// 17
			PARAM_LEFT_TOOL_Y_PITCH,			// 8.5 
			PARAM_LEFT_TOOL_Y_SENSOR_OFFSET,	// 

			PARAM_RIGHT_TOOL_6_X_OFFSET_FROM_BASE,		// 7
			PARAM_RIGHT_TOOL_6_Y_OFFSET_FROM_BASE,		// 17 
			PARAM_RIGHT_TOOL_Y_PITCH,			// 8.5
			PARAM_RIGHT_TOOL_Y_SENSOR_OFFSET,	// 

			PARAM_CHK_XL_X_G53, 
			PARAM_CHK_XL_Y1_G53, 
			PARAM_CHK_XL_Y2_G53, 
			PARAM_CHK_YL_X_G53, 
			PARAM_CHK_YL_Y_G53, 
			PARAM_CHK_XYL_Z_G53, 

			PARAM_CHK_XR_X_G53, 
			PARAM_CHK_XR_Y1_G53, 
			PARAM_CHK_XR_Y2_G53, 
			PARAM_CHK_YR_X_G53, 
			PARAM_CHK_YR_Y_G53, 
			PARAM_CHK_XYR_Z_G53, 

			PARAM_NUM
		};

		CString strParamsName[PARAM_NUM];

	private:
		void calcLeftToolPocket();
		void calcRightToolPocket();

	public:
		virtual BOOL LoadParams(TCHAR* pFileParh, CString& strErrMsg);
		virtual BOOL SaveParams(TCHAR* pFileParh, CString& strErrMsg);

		virtual CString GetParamName(UINT nID);
		virtual double GetParamValue(UINT nID);
		virtual void SetParamValue(UINT nID, double fValue);

		virtual BOOL RunForReCalc(BOOL bResetStep);
		virtual BOOL Run(BOOL bResetStep);


	public:
		CToolPocketAutoTeachingP2(void);
		~CToolPocketAutoTeachingP2(void);
	};

}
