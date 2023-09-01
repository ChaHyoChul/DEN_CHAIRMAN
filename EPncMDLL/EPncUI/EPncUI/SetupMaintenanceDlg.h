#pragma once

#include "afxwin.h"
#include "TitleBarWnd.h"

// CSetupMaintenanceDlg 대화 상자입니다.

class CSetupMaintenanceDlg : public CDialogListPage
{
	DECLARE_DYNCREATE(CSetupMaintenanceDlg)

public:
	enum EN_SELECT_MODE
	{
		SELECT_NONE = 0, 
		SELECT_COOLANT, 
		SELECT_FILTER, 
		SELECT_NOZZLE, 
		SELECT_COLLET, 
	};

private:
	CTitleBarWnd*	pTitleBarWnd_;
	EN_SELECT_MODE	selectedMode_;

	BOOL	bDirectAccess_;					// Operation 화면에서 직접 접근 
	
	BOOL	bEnableChecking[pa::MAINTENANCE_NUM];
	DWORD	dwMaximumTime[pa::MAINTENANCE_NUM];
	BOOL	bTimeReached[pa::MAINTENANCE_NUM+1];
	int SEC_HOUR;
	int SEC_DAY;

	CBrush	brhBkgnd_;						// 다이얼로그 배경 색 
	CBrush	brhATC_;						// background of ATC
	CBrush	brhBackButton_;
	CBrush	brhDark_;
	CBrush	brhToolBkgnd_;					// tool screen background

	CFont	fntButton_;						// 버튼의 폰트 
	CFont	fntToolError_;				// error font
	CFont	fntMenuButton_;					// 매뉴 버튼 
	CFont	fntSpindleRPM_;
	CFont	fntToolNo_;
	CFont	fntToolNoLabel_;
	CFont	fntTime_;
	CFont	fntHeadings_;
	
	CRect	CUIrectSetT; 

    CToolTipCtrl        m_tip_ctrl;
    BOOL                m_bShowToolTips;

    static const CString ToolTipText(int index) {
        switch (index) {
            case IDC_BUTTON_MAINTENANCE1:
            case IDC_BUTTON_MAINTENANCE2:
            case IDC_BUTTON_MAINTENANCE3:
            case IDC_BUTTON_MAINTENANCE4:
                return _T("Set the time of the last maintenance to current time");
            case IDC_BUTTON_UP_D1:
            case IDC_BUTTON_UP_D2:
            case IDC_BUTTON_UP_D3:
            case IDC_BUTTON_UP_D4:
                return _T("Increase");
            case IDC_BUTTON_DOWN_D1:
            case IDC_BUTTON_DOWN_D2:
            case IDC_BUTTON_DOWN_D3:
            case IDC_BUTTON_DOWN_D4:
                return _T("Decrease");
            case IDC_BUTTON_SAVE1:
            case IDC_BUTTON_SAVE2:
            case IDC_BUTTON_SAVE3:
            case IDC_BUTTON_SAVE4:
                return _T("Save the new settings");
            case IDC_BUTTON_TOOL_MANAGER:
                return _T("Open tool manager panel to reset tool timer after replacement");
            case IDC_STATIC_COOLANT:
                return _T("Prevent clogging of the circulation system by regular coolant exchanges.");
            case IDC_STATIC_FILTER:
                return _T("Exchange filter regularly to ensure satisfactory filtering of the coolant.");
            case IDC_STATIC_NOZZLES:
                return _T("Clean nozzles regularly to prevent clogging from solid particles of the milling waste.");
            case IDC_STATIC_COLLET:
                return _T("Operation of the spindle's collet might be compromised if milling material enters the inside of the clamping mechanism.");
            case IDC_STATIC_TOOL:
                return _T("Replace tools regularly to maintain good milling quality.");
            default:
                return _T("");
        }
	}


	CWnd* pParentWnd_;

	TCHAR*				pResourcePath_;
	hcutil::CCanvasCE*	pCanvasCE_;
	int					nBackgroundLayerIndex_;
	int					nStatusLayerIndex_;

	void updateButtonState();
	void updateState_MenuButton();

	void writeLog( LPCTSTR log_msg );

public:
	CSetupMaintenanceDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMaintenanceDlg();

	virtual void StartPageWork();
	virtual void StopPageWork();
	
	void SetParentWnd( CWnd* p ) { pParentWnd_ = p; }

	void SetDirectAccess( BOOL b ) { bDirectAccess_ = b; };

	void SetSelectMode(EN_SELECT_MODE mode) { selectedMode_= mode; }

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnImgButtonExClicked(WPARAM wparam, LPARAM lparam); // WM_IMGBUTTONEX_CLICKED
	afx_msg void OnDestroy();
	
	afx_msg void OnBnClickedMaintenance1();
	afx_msg void OnBnClickedMaintenance2();
	afx_msg void OnBnClickedMaintenance3();
	afx_msg void OnBnClickedMaintenance4();
	
	afx_msg void OnBnClickedUpD1();
	afx_msg void OnBnClickedUpD2();
	afx_msg void OnBnClickedUpD3();
	afx_msg void OnBnClickedUpD4();
	
	afx_msg void OnBnClickedDownD1();
	afx_msg void OnBnClickedDownD2();
	afx_msg void OnBnClickedDownD3();
	afx_msg void OnBnClickedDownD4();
	
	afx_msg void OnBnClickedSave1();
	afx_msg void OnBnClickedSave2();
	afx_msg void OnBnClickedSave3();
	afx_msg void OnBnClickedSave4();
	
	afx_msg void OnBnClickedButtonTool();
	afx_msg void OnBnClickedButtonClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnPaint();
protected:
	virtual void PreInitDialog();
	
private:

	void SetLastMaintenance(int ID);
	
	void incMax(int ID, int amount);
	void saveMax(int ID);
	
	void updateMaximumTime();
	void updateMaximumTime(int ID);
	void updateRemainingTime();
	void updateRemainingTime( int ID );
	void updateRemainingToolTime();
	void updateCurrentTime();
	
	
	CRect rcStatusArea_[pa::MAINTENANCE_NUM + 1];
	COLORREF CLR_BACKGROUND; //= RGB(255, 255 ,255);
	COLORREF CLR_NG;// = RGB(255, 235 ,235);
	
	BOOL initialize_SetupDlg();
	
	CStatic	txtMaxDays[pa::MAINTENANCE_NUM];
	CStatic	txtLastDate[pa::MAINTENANCE_NUM];
	CStatic	txtNextDate[pa::MAINTENANCE_NUM];
};

