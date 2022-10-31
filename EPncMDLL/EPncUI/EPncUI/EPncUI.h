// EPncUI.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
    #error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"

// CEPncUIApp:
// 이 클래스의 구현에 대해서는 EPncUI.cpp을 참조하십시오.
//

#define WM_HCCHA_KEY_MSG    (WM_USER+999)

class CEPncUIApp : public CWinApp
{
public:
    CEPncUIApp();

    HWND    hKeyboardMgeReceiveWnd_;    // Jog 메시지를 받을 윈도우의 핸들 

    TCHAR   *P_VERSION;

    BOOL    bNoNeedEnterPassword_;

    BOOL MotionDone( BOOL checkingMotorStop );

    BOOL IsMotorStop();

    void MsgPump();

    void LoadPW();
    void SavePW( pa::EN_USER_MODE hUserMode, CString strPW );

    BOOL isRunningProgram();

// 재정의입니다.
public:
    virtual BOOL InitInstance();

// 구현입니다.

    DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CEPncUIApp theApp;
