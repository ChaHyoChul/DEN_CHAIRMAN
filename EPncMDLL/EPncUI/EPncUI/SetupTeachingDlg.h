#pragma once
#include "afxwin.h"

// CSetupTeachingDlg 대화 상자입니다.
#include "PointDataListBox.h"
#include "PositionWnd.h"
#include "TitleBarWnd.h"

class CSetupTeachingDlg : public CDialogListPage
{
public:
	enum EN_SUB_PAGE
	{
		SUB_PAGE_COORDINATE = 0,
		SUB_PAGE_TEACHING,
		SUB_PAGE_OPTION,
		SUB_PAGE_AT_COORDINATE_OFFSET,		//
		SUB_PAGE_AT_TOOL_TEACHING,			//
		SUB_PAGE_NUM
	};

private:
	DECLARE_DYNCREATE(CSetupTeachingDlg)

	CWnd				*pParentWnd_;

	TCHAR*				pResourcePath_;
	hcutil::CCanvasCE*	pCanvasCE_;

	CDialogMap			*pDlgMap_;	

	EN_SUB_PAGE			hCurrSelectedSubPage_;

	CFont	fntMenuButton_;		// 매뉴 버튼 폰트 
	CFont	fntManualButton_;	// 매뉴얼 버튼 폰트 
	CFont	fntJogButton_;		// 조그 버튼 
	CFont	fntJogSpd_;			// 조그 속도

	CBrush	brhBkgnd_;
	CBrush	brhEmoResetButton_;
	CBrush	brhBackButton_;
	CBrush	brhJogButton_;

	CTitleBarWnd		*pTitlePositionWnd_;
	CPositionWnd		*pPositionWnd_;

	CRect				PCUIrectST;

	double			fJogStep_[10];	// 0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1.0, 2.0
	int				nJogStep_;
	int				nJogMode_;		// 0:Step, 1:Contiue
	int				nPressedJogButton_;
	BOOL			bkeyboard_jog_btn_down_;

	BOOL			bDontMoveJOG_AB_;		// A.B축 Jog 버튼 Interlock

// 	int				nJogSpeedRate_;	// 초기 100% 만든 후 up/down 버튼이 누렸을때, 이 값을 기준으로 변경한다 
// 									// 화면에 들어올때 마다 100% 초기화 한다 

	BOOL	bSaveToFileButtonState_[SUB_PAGE_NUM];	// SaveToFile 버튼의 상태, 값을 수정하면 FALSE, 다운로드 및 저장하면 TRUE 

	BOOL isJogging;

	void initialize_DlgMap();
	void destroy_DlgMap();

	void initialize_PositionWnd();
	void destroy_PositionWnd();
	void update_PositionWnd();

	void initialize_JogButton();
	void destroy_JogButton();
	void updateState_JogButton();

	void updateState_ManualButton();

	void updateState_MenuButton();

	void updateJogStepVal( int nJogStep );

	void writeLog( LPCTSTR log_msg );

	void startJog( pa::EN_AXIS axis, BOOL dir );

	void startJog( pa::EN_AXIS axis, BOOL dir, int is_step );

	void stopJog( pa::EN_AXIS axis, BOOL dir );

//	void setMotorOverride( int nNewOvr );
	void setJogSpeedRate( int nNewSpdRate );

	void upload_to_pc();		// 제어기에서 데이터를 읽어 화면에 표시 한다 
	
	void computeCubeOffset();

	void ResetSaveToFileButtonState( int nMode ) {					// 0:CoordinateOffset, 1:TeachingPoint, 2:Option
		bSaveToFileButtonState_[nMode] = FALSE;
	}

	BOOL save_to_file_coordinate_offset( CString& strErrMsg );					// coordinate offset 데이터를 파일에 저장 
	BOOL load_from_file_coordinate_offset( CString& strErrMsg );
	BOOL save_to_file_teaching_point( CString& strErrMsg );						// teaching point 데이터를 파일에 저장 
	BOOL load_from_file_teaching_point( CString& strErrMsg );
	BOOL save_to_file_tp_option( CString& strErrMsg );							// tp option 데이터를 파일에 저장 
	BOOL load_from_file_tp_option( CString& strErrMsg );
	BOOL save_to_file_auto_taching_for_coord_offset( CString& strErrMsg );		//
	BOOL load_from_file_auto_taching_for_coord_offset( CString& strErrMsg );
	BOOL save_to_file_auto_teaching_for_toolpocket( CString& strErrMsg );		//
	BOOL load_from_file_auto_teaching_for_toolpocket( CString& strErrMsg );


	void reposbutton(CButton* Cbuttn);
	void reposstatic(CStatic* pstatic);

	void JOG_START_STOP( int button_id, pa::EN_AXIS axis_no, BOOL dir, MSG *pMsg );
public:
	CSetupTeachingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupTeachingDlg();

	void SetParentWnd( CWnd* p ) { pParentWnd_ = p; }

	virtual void StartPageWork();
	virtual void StopPageWork();
	virtual void UpdatePage();

	void ShowModeSelectRadioButton();
	void HideModeSelectRadioButton();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_TEACHING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonClose();
	afx_msg LRESULT OnNotifyButton(WPARAM wparam, LPARAM lparam);
	CButton chkJogModeStep_;
	afx_msg void OnBnClickedCheckJogMode();
	CButton btnJogStepVal_;
	afx_msg void OnBnClickedButtonStepVal();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonDownloadToCtrl();
	CButton btnDownLoadToController_;
	CButton	btnLoadFromFile_;
	CButton btnSaveToFile_;
	CButton btnBack_;
	CButton btnSetCurrentPosition_;
	afx_msg void OnBnClickedButtonG53();
	afx_msg void OnBnClickedButtonG54();
	CButton btnG53_;
	CButton btnG54_;
	CButton btnG55_;
	CButton btnSpindleClamp_;
	CButton btnSpindle2Clamp_;
	CButton btnBlockClamp_;
	afx_msg void OnBnClickedButtonSpindleClamp();
	afx_msg void OnBnClickedRadioCoordinateOffset();
	afx_msg void OnBnClickedRadioTeachingPoint();
	afx_msg void OnBnClickedRadioOption();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonM28();
	CButton btnM28M29_;
	CButton btnMoveXY_;
	afx_msg void OnBnClickedButtonMoveXy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonJogSpeedUp();
	afx_msg void OnBnClickedButtonJogSpeedDown();
	afx_msg LRESULT OnHcchaKeyMeg(WPARAM wparam, LPARAM lparam);
	CButton btnToolPosAutoCalc_;
	CButton chkJogModeCont_;
	afx_msg void OnBnClickedCheckJogMode2();
	CButton btnJogModeStep_;
	CButton btnJogModeContinue_;
	afx_msg void OnBnClickedButtonJogModeStep();
	afx_msg void OnBnClickedButtonJogModeContinue();
	CButton btnSRV_ON_;
	afx_msg void OnBnClickedButtonServoOn();
protected:
	virtual void PreInitDialog();
public:
	afx_msg void OnPaint();
	int nCurrSelectedPage_;
	afx_msg void OnBnClickedButtonAutoCalcToolpos();
	CButton btnServoOnOff_;
	afx_msg void OnBnClickedButtonServoOnoff();
	afx_msg void OnBnClickedButtonSav();
	afx_msg void OnBnClickedButtonUpoadToPc();
	afx_msg void OnBnClickedButtonSpindle2Clamp();
};
