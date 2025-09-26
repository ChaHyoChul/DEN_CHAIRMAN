#pragma once

namespace pa
{
	struct SUIToolPocketAutoTeachingParam 
	{
		enum EN_PARAM 
		{
			PARAM_NUM = 50
		};
		double fParam[PARAM_NUM];
	};

	class CUIToolPocketAutoTeachingBase
	{
	protected:
		CString strFilePath_;
		hcipc::CSharedMem *pShMem_;
		SUIToolPocketAutoTeachingParam* pUIToolPocketAutoTeachingParam_;

	private:
		BOOL create_shared_mem_object(void);

	public:
		virtual BOOL Initialize(CString strFilePath);
		void Destroy(void);
		
		virtual BOOL SaveParamsData(void) = 0;
		virtual BOOL LoadParamsData(void) = 0;

		virtual int GetParamsNum(void) = 0;
		virtual CString GetParamName(UINT nID) = 0;
		virtual CString GetParamDataS(UINT nID) = 0;
		virtual double GetParamDataD(UINT nID) = 0;
		virtual double* GetParamsPtr(UINT nID) = 0;
		virtual void SetParamDataD(UINT nID, double val) = 0;

	public:
		virtual int GetToolsNum(void) = 0;			// 툴 개수 리턴 
		virtual void InitToolPocketLayout() = 0;	// 툴 영역의 위치 계산 
		virtual CRect GetToolPos(UINT inex) = 0;
		virtual CRect GetToolSelPos(UINT index) = 0;
		virtual CRect GetToolErrorMsgPos(UINT index) = 0;
 
	public:
		CUIToolPocketAutoTeachingBase(void);
		virtual ~CUIToolPocketAutoTeachingBase(void);
	};
}
