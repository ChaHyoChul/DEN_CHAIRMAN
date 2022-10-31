#pragma once

#include "IconWnd.h"

// CEPncUIDlg2 대화 상자입니다.

class CEPncUIDlg2 : public CDialog
{
	DECLARE_DYNAMIC(CEPncUIDlg2)
	//const static CString msgStatus[];
	static const CString GetStatusMsg(int index) {
		static const CString a[] = {
			_T("Replace the coolant!"),
			_T("Rinse the filter!"),
			_T("Replace the filter!"),
			_T("Clean the nozzles!"),
			_T("Clean the collets!"),
			_T("Close the safety door!"),
			_T("Connect Chairman 5X!"),
			_T("Replace tool:")
		};
		return a[index];
	}

public:
	enum EN_OPER_BUTTON
	{
		OPER_BTN_EMO = 0,
		OPER_BTN_SETUP,
		OPER_BTN_START,
		OPER_BTN_STOP,
		OPER_BTN_OPEN,
		OPER_BTN_READY_POS,
		OPER_BTN_TOOLS,
		OPER_BTN_ICN_TOOLS,
		OPER_BTN_ICN_COLLETS,
		OPER_BTN_ICN_COOLANT,
		OPER_BTN_ICN_FILTER,
		OPER_BTN_ICN_NOZZLE,
		OPER_BTN_ICN_DOOR,
		OPER_BTN_ICN_CONNECTION,
		OPER_BTN_ICN_MAINTENANCE,
		OPER_BTN_ICN_READY,
		OPER_BTN_SPINDLE_RPM,
		OPER_BTN_SPINDLE_RPM2,
		OPER_BTN_NC_LOADED,
		OPER_BTN_NC_RUNNING,
		OPER_BTN_NC_FINISHED,
		OPER_BTN_PROGRAM_COLLAPSE,
		OPER_BTN_PROGRAM_CLOSE,
		OPER_BTN_DART,

		OPER_BTN_NUM
	};

	enum EN_STATUS {
		STATUS_TOOL_TITLE,
		STATUS_TOOL_INFO1,		// NO.1 
		STATUS_TOOL_INFO2_1,
		STATUS_TOOL_INFO2_2,
		STATUS_TOOL_INFO2_3,
		STATUS_TOOL_INFO3,		// [55%]
		STATUS_SPINDLE_TITLE,
		STATUS_SPINDLE,			// 1800 RPM
		STATUS_FEEDRATE_TITLE,
		STATUS_FEEDRATE,		// 1000
		STATUS_MILLING_TIME_TITLE,
		STATUS_MILLING_TIME_STATUS1,
		STATUS_MILLING_TIME_STATUS2,
		STATUS_MILLING_TIME_STATUS1_1,
		STATUS_MILLING_TIME_STATUS1_2,
		STATUS_MILLING_TIME_STATUS1_3,
		STATUS_MILLING_TIME_STATUS2_1,
		STATUS_MILLING_TIME_STATUS2_2,
		STATUS_MILLING_TIME_STATUS2_3,

		STATUS_PROG_RATE1,		// progress bar
		
		STATUS_TOOL_HEALTH_1,
		STATUS_TOOL_HEALTH_2,
		STATUS_TOOL_HEALTH_3,
		STATUS_TOOL_HEALTH_4,
		STATUS_TOOL_HEALTH_5,
		STATUS_TOOL_HEALTH_6,
		
		STATUS_TOOL_HEALTH_LEFT,
		STATUS_TOOL_HEALTH_RIGHT,

		STATUS_NUM
	};

	enum EN_STATUS_ICON {
		STATUS_ICON_USB = 0,
		STATUS_ICON_REMOTE,
		STATUS_ICON_BLOCK,	
		STATUS_ICON_NUM
	};
	
private:
//	HANDLE		hEvent_;
	enum EN_MSG_STATUS{
		MSG_COOLANT = 0,
		MSG_COOLANT_2,
		MSG_FILTER,
		MSG_NOZZLES,
		MSG_COLLET,
		MSG_DOOR,
		MSG_CONNECT,
		MSG_TOOLS,	
		MSG_NUM
	};
	
//	int		msgStatusID[MSG_NUM];
	
	CString strImageFilePath;
	CString strImageFilePathL;
	CString strImageFilePathR;
	CString strImageFilePathCurrL;
	CString strImageFilePathCurrR;

	TCHAR*				pResourcePath_;
	hcutil::CCanvasCE*	pCanvasCE_;
	int					nBackgroundLayerIndex_;
	int					nToolBkgLayerIndex_;
	int					nStatusLayerIndex_;
	int					nToolOutlineLayerIndex_;
	BOOL 				PREV_SHOWN_SPINDLE1;
	BOOL 				PREV_SHOWN_SPINDLE2;
	// BOOL 				PREV_SHOWN_SPINDLE2;

	CImgButtonEx2*			pOperButtonsEx_[OPER_BTN_NUM];

	CFont	hFntRate_;
	CFont	hFntStatusTitle_;
	CFont	hFntStatus_;
	CFont	hFntNcFile_;
	CFont	hFntModel_;
	CFont	hFntSystemStatus_;
	CFont	hFntDeviceStatus_;
	CFont	hFntMainStatus_;
	CFont	hFntToolNo_;
	CFont	hFntSpindleRPM_;
	CFont	hFntMillingTime_;
	CFont	hFntProgRate_;
	CFont	hFntToolTime_;
	CFont	hFntToolStatus_;
	CFont	hFntMillingStatus_;
	CFont	hFntMillingClock_;
	CFont	hFntMaintenanceStatus_;
	CFont	hFntVersion_;
	CFont	hFntMachineID_;

	CBrush	m_brModelInfo;
	CBrush	m_brModelID;
	CBrush	m_brNCFileInfo;
	CBrush	m_brMaintenanceStatus;

// 	CFont	hFntToolStatus_;
// 	CBrush	hBrhToolStatusWhite_;
// 	CBrush	hBrhToolStatusOrange_;
// 	CBrush	hBrhToolStatusRed_;
// 	CPen	hPenToolStatus_;

	BOOL	bConnectedUsbMemory_;

	BOOL	bToolButtonPressed_;						// 툴 버튼이 눌렸을 경우, TRUE

	CIconWnd*	pIconWnd_[STATUS_ICON_NUM];
	CRect		rcStatusArea_[STATUS_NUM];				// 상태 출력 영역 
	CRect		rcStatusIconArea_[STATUS_ICON_NUM];		// 상태 출력 영역 

	int			nPREV_MILLING_TIME_;					// 가공 남은 시간
	int			nESTIMATED_TIME;						// Total time estimation
	int			nBEFORE_TOOLCHANGE_RUNNINGTIME_;		// 툴 교체전 까지 가공시간

	int			millingStartLine;
	BOOL		isUSBConnected_;
	CString		strDriverName_;

	CString strNCFileName;

	CRect	CUICRect_m;
	LPPOINT NewPos;
	
	CTime	timeStart;
	CTime	timeFinish;

private:
	void init_status_layout();

	void draw_font();

	void initialize_OperButtons();
	void destroy_OperButtons();

	BOOL preinitialize_pmac_object( CString& strErrMsg );

	BOOL initialize_pmac_object( CString& strErrMsg );
	void destroy_pmac_object();

	BOOL initialize_ErrorDlg();
	void destroy_ErrorDlg();

	BOOL initialize_SetupDlg();
	void destroy_SetupDlg();

	BOOL initialize_StatusIconWnd();
	void destroy_StatusIconWnd();

	void updateButtonState();						// 화면중 버튼 상태를 갱신 한다

//	void updateStatus();							// tool info, spindle, feedrate 출력. 아래 4개의 함수로 대체 함 

	void updateToolInfoDisp();						// 현재 잡고있는 툴 정보를 화면에 표시
	void updateSpindleInfoDisp();					// 스핀들 정보를 화면에 표시 
	void updateFeedRateInfoDisp();					// FeedRate 정보를 화면에 표시 
	void updateMillingTimeDisp();					// 가공 시간 정보를 화면에 표시 
	void updateNumSelectNcFileDisp();				// 선택된 NC 파일 개수를 화면에 표시 

	void updateRemoteModeDisp();					// remote mode 표시 출력 
	void updateUsbMemConnectDisp();					// usb memory 표시 출력
	void updateBlockExistDisp();					// 블록 유/무 표시 출력 
	void updateProgressBarStatus();					// 작업 진행상태 표시 출력  
	void updateToolHealthStatus();

	void updateNCFile();

	void updateSystemStatus();
	void updateDeviceStatus();

	void updateToolTime();
	void updateNCStatus();

	//
	void doButtonEMO();
	void doButtonConnect();
	void doButtonSetup();
	void doButtonRunPause();
	void doButtonStop();
	void doButtonOpen();
	void doButtonReadyPos();
	void doButtonTools();
	void doButtonMaintenance();

	void doButtonProgramCollapse();
	void doButtonProgramClose();

	void writeLog( LPCTSTR log_msg );

	void updateDemoModeDisp();						// demo mode 표시 출력 

	int calcExcpetTime( DWORD dwRunningTime, int nTotalLine, int nCurrLine );
	int calcExcpetTime2( DWORD dwRunningTime, int nTotalLine, int nCurrLine );
	int findRemainingTime( int nTotalLine, int nCurrLine );

	bool WaitForFileToOpen(DWORD dwTimeout) ;

	void DeleteLeftOverNCFiles();

	void AddCommaToData(CString data, CString &commaData);

	BOOL getToolError();	// 6개중 한개라도, 에러상태면 TRUE 리턴 
	int  getToolAlarm();	// 6개중 한개라도, 90% 이상이면 1, 100% 이면 2, 아니면 0 리턴 

public:
	BOOL Splash_Call_Init(int stepNo);	// Splash 다이얼로그에서, 프로그램을 초기화 하기 위해 호출 (step => 0 ~ 20)
	void Quit();

public:
	CEPncUIDlg2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEPncUIDlg2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EPNCUI_DIALOG2 };

protected:
    HICON m_hIcon;
    
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void PreInitDialog();
    afx_msg HCURSOR OnQueryDragIcon();
    
public:
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnImgButtonExClicked(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnUsbMemory(WPARAM wparam, LPARAM lparam);

	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);

	afx_msg LRESULT OnEPncUIDlgQuit(WPARAM wparam, LPARAM lparam);

	static double F_CURRENT_RUN_RATE;

#ifdef _SAVE_RUNTIME_UI_
	int DBG_SAVE_RUNNING_TIME;
	void SAVE_RUNNING_TIME(BOOL b, TCHAR* pFilePath, int nLineNo, int nRunningTime, int nRemainTime, int nRemainTime2 );
#endif
	//////////////////////////////////////////////////////////////////////////
	afx_msg void OnBnClickedButtonV2Emo2();
};
