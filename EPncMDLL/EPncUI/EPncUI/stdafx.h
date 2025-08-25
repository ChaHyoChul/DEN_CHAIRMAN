
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원


#include <afxsock.h>            // MFC 소켓 확장


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
#define __USE_EPNCM_DLL__

#ifdef __USE_EPNCM_DLL__
	#include "../../EPncMDll/EPncMDLLHeader.h"
	#pragma comment (lib, "EPncMDll.lib")
#endif 

//////////////////////////////////////////////////////////////////////////

#include <math.h>

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#define WM_USB_MEMORY		    (WM_USER+160)       // wparam : 0(not connect), 1(connect)
#define WM_NOTIFY_WINDOW_MOVE   (WM_USER+180) 
//////////////////////////////////////////////////////////////////////////

#define WM_SETUP            (WM_USER+120)
#define WM_EPNCUI_QUIT		(WM_USER+121)

#define SETUP_EXIT          (0)
#define SETUP_BACK          (1)
#define SETUP_TEACHING      (2)
#define SETUP_TOOL          (3)

#define SETUP_OPTION        (5)
#define SETUP_IO            (6)
#define SETUP_TERM          (7)
#define SETUP_LOG           (8)
#define SETUP_SYSTEM        (9)

#define SETUP_MAINTENANCE   (11)
#define SETUP_AUTOCAL       (12)
#define SETUP_AUTOTEACH     (13)

#define WM_LCD_AUTOICAL_START_STOP	(WM_USER+13)	// WPARAM: 0:Stop, 1:Start 

#include <crtdbg.h>

#include <vector>
#include <algorithm>

#include "CanvasCE.h"

#include "ImgButtonEx2.h"
#include "ImgButtonEx2Run.h"
#include "ImgButtonEx2BlockGrip.h"
#include "ImgButtonEx2ToolClamp.h"

#include "JogButton.h"
#include "NotifyButton.h"
#include "NotifyButton2.h"
#include "DlgList.h"

#include "../../Common/CEIniFile.h"
#include "../../Common/hcutil.h"
#include "../../Common/IpcLib.h"
#include "../../Common/EPACommonHeader.h"

#include "FtpClient.h"
#include "PncIpcClient.h"
#include "PAStatus.h"
#include "PNCFile.h"
#include "PTool.h"
#include "PMAintenance.h"
#include "PNCFileMgr.h"
#include "PSWConfig.h"

#include "UIToolPocketAutoTeachingBase.h"
#include "UIToolPocketAutoTeachingP1.h"
#include "UIToolPocketAutoTeachingP2.h"

#include "FileListBox.h"
#include "NCStatusBarWnd.h"

#include "SplashDlg.h"
#include "ConnectErrorDlg.h"

namespace pa
{
    class CPNCFile;
    class CPNCFileMgr;
    class CPAStatus;
    class CPTool;
	class CPMaintenance;
    class CPSWConfig;

    extern CPAStatus*       PPAStatus;
    extern CPNCFile*        PNCFile;
    extern CPNCFileMgr*     PNCFileMgr;
    extern CPTool*          PTool;
	extern CPMaintenance*   PMaintenance;
    extern CPSWConfig*      PSWConfig;

	extern CUIToolPocketAutoTeachingBase* PUIToolPocketAutoTeaching;
}

