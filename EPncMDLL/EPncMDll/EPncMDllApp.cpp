#include "StdAfx.h"
#include "EPncMDllApp.h"

//////////////////////////////////////////////////////////////////////////
//
EPNCMDLL_API CEPncMDllApp theApp;

CEPncMDllApp::CEPncMDllApp(void)
{
	pMainDlg = NULL;
}

CEPncMDllApp::~CEPncMDllApp(void)
{
}

// App 객체를 초기화 한다 
int CEPncMDllApp::Initialize()
{
	//////////////////////////////////////////////////////////////////////////
	// 코드가 없으면, 다이얼로그를 만들 수 없다
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//////////////////////////////////////////////////////////////////////////

//	P_VERSION = _T("DS-20170901-5m_DLL-test-release");
	P_VERSION = _T("CM-20211210-001");	

	//////////////////////////////////////////////////////////////////////////
	// 소켓을 초기화 한다 
	CString strErrMsg(_T(""));
	hcsock::STARTUP_SOCKET_COMM(strErrMsg);

	//////////////////////////////////////////////////////////////////////////
	// 모델 정보를 읽는다
	pa::MODEL_INFO.Load();

	//////////////////////////////////////////////////////////////////////////
	// Main Dialog를 생성 한다 
	pMainDlg = new CEPncMDlg();
	ASSERT(pMainDlg);
	BOOL b = pMainDlg->Create(106/*IDD_DIALOG_EPNCM*/, NULL);
// 	pMainDlg->ShowWindow(SW_SHOW);

	if (pMainDlg->GetInitSuccess() != 0)
	{
		pMainDlg->ShowWindow(SW_HIDE);
	}

	return pMainDlg->GetInitSuccess();
}

// App 객체를 해제 한다
void CEPncMDllApp::Destory()
{
	if (pMainDlg != NULL)
	{
		pMainDlg->DestroyWindow();
		delete pMainDlg;
		pMainDlg = NULL;
	}

	hcsock::CLEANUP_SOCKET_COMM();
}

