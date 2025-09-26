#pragma once

namespace pa 
{
	struct SToolPocketAutoTeachingParam 
	{
		enum EN_PARAM 
		{
			NUM = 50
		};
		double fParam[NUM];

	};

	class CToolPocketAutoTeachingBase
	{
	protected:
		// 공유 메모리 선언 
		hcipc::CSharedMem *pShMem_;
		SToolPocketAutoTeachingParam* pToolPocketAutoTeachingParam_;

		int		nStepNo_;
		double	fMeasureResult_[2][3];
		char	szCommand_[256];
		double	fMeasurePoint_[2];
		int		nProgressRate_;

		void writeLog(LPCTSTR logMsg, BOOL bExt);

	private:
		BOOL create_shared_mem_object();

	public:
		BOOL Initialize(CString strFilePath, CString& strErrMsg);
		void Destroy();

		virtual BOOL LoadParams(TCHAR* pFileParh, CString& strErrMsg) = 0;
		virtual BOOL SaveParams(TCHAR* pFileParh, CString& strErrMsg) = 0;

		virtual CString GetParamName(UINT nID) = 0;
		virtual double GetParamValue(UINT nID) = 0;
		virtual void SetParamValue(UINT nID, double fValue) = 0;

		virtual BOOL RunForReCalc(BOOL bResetStep) = 0;
		virtual BOOL Run(BOOL bResetStep) = 0;


	public:
		CToolPocketAutoTeachingBase(void);
		virtual ~CToolPocketAutoTeachingBase(void);
	};

}
