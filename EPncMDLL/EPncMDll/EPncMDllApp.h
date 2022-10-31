#pragma once

#ifdef EPNCMDLL_EXPORTS
#define EPNCMDLL_API __declspec(dllexport)
#else
#define EPNCMDLL_API __declspec(dllimport)
#endif

class CEPncMDllApp
{
public:
	TCHAR		*P_VERSION;

	//////////////////////////////////////////////////////////////////////////
	// 아래 변수는 CMainDlg::OnInitDialog() 함수에서 초기화 된다 
	CString		strIpAddr;				// unicode 
	char		szIpAddr[64];			// ascii
	int			nPortNo1;				// 
	int			nPortNo2;				//
	int			nPortNoForStream;		// 
	//////////////////////////////////////////////////////////////////////////

	CEPncMDlg*	pMainDlg;				// 메인 다이얼로그 객체 생성 

public:
	CEPncMDllApp(void);
	~CEPncMDllApp(void);

	int Initialize();					// App 객체를 초기화 한다 
	void Destory();						// App 객체를 해제 한다
};

extern EPNCMDLL_API CEPncMDllApp theApp;
