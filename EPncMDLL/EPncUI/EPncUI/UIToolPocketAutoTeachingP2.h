#pragma once

namespace pa
{
	class CUIToolPocketAutoTeachingP2 : public CUIToolPocketAutoTeachingBase
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
			PARAM_RIGHR_TOOL_Y_SENSOR_OFFSET,	// 

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

	public:
		virtual BOOL SaveParamsData(void);
		virtual BOOL LoadParamsData(void);

		virtual int GetParamsNum(void);
		virtual CString GetParamName(UINT nID);
		virtual CString GetParamDataS(UINT nID);
		virtual double GetParamDataD(UINT nID);
		virtual double* GetParamsPtr(UINT nID);
		virtual void SetParamDataD(UINT nID, double val);

	public:
		virtual int GetToolsNum(void);			// 툴 개수 리턴 
		virtual void InitToolPocketLayout();	// 툴 영역의 위치 계산 
		virtual CRect GetToolPos(UINT inex);
		virtual CRect GetToolSelPos(UINT index);
		virtual CRect GetToolErrorMsgPos(UINT index);

	public:
		CUIToolPocketAutoTeachingP2(void);
		~CUIToolPocketAutoTeachingP2(void);
	};

}
