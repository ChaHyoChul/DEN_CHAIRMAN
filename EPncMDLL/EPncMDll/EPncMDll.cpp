// EPncMDll.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"
#include "EPncMDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 유일한 응용 프로그램 개체입니다.
// 아래 객체가 없으면, DLL 리소스를 사용할 수 없어서
// 다이얼로그 등 생성할 수 없다
CWinApp theSystemApp;

using namespace std;

//////////////////////////////////////////////////////////////////////////

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 오류 코드를 필요에 따라 수정합니다.
		_tprintf(_T("심각한 오류: MFC를 초기화하지 못했습니다.\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 응용 프로그램의 동작은 여기에서 코딩합니다.
	}

	return nRetCode;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

EPNCMDLL_API int INITIALIZE_EPNCM_DLL(void)
{
	int nRet = 0;
	TRACE(_T("START : void INITIALIZE_EPNCM_DLL(void)\n"));

	//////////////////////////////////////////////////////////////////////////
	// 코드가 없으면, 다이얼로그를 만들 수 없다
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//////////////////////////////////////////////////////////////////////////
	
	if (theApp.Initialize())
	{
		nRet = 1;
		return nRet;
	}

	//////////////////////////////////////////////////////////////////////////

	TRACE(_T("STOP : void INITIALIZE_EPNCM_DLL(void)\n"));
	return nRet;
}

EPNCMDLL_API void DESTORY_EPNCM_DLL(void)
{
	TRACE(_T("START : void DESTORY_EPNCM_DLL(void)\n"));

	//////////////////////////////////////////////////////////////////////////
	// 코드가 없으면, 다이얼로그를 만들 수 없다
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//////////////////////////////////////////////////////////////////////////

	theApp.Destory();

	//////////////////////////////////////////////////////////////////////////

	TRACE(_T("STOP : void DESTORY_EPNCM_DLL(void)\n"));
}

EPNCMDLL_API void WriteLog( CLog::EN_TYPE type, int code, LPCTSTR log_msg )
{
	if (P_LOG)
	{
		P_LOG->WriteLog(type, code, log_msg);
	}
}

EPNCMDLL_API void WriteLog_EXT( LPCTSTR log_msg, TCHAR* extension )
{
	if (P_LOG)
	{
		P_LOG->WriteLog_EXT( log_msg, extension );
	}
} 