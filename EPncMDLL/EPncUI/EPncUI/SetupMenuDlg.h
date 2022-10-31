#pragma once

// CSetupMenuDlg 대화 상자입니다.

class CSetupMenuDlg : public CDialogListPage
{
	DECLARE_DYNCREATE(CSetupMenuDlg)

	CWnd* pWndParent_;
    
    CRect	PCUIrectST;
	
    CToolTipCtrl        m_tip_ctrl;
    BOOL                m_bShowToolTips;

    static const CString ToolTipText(int index) {
        switch (index) {
            case IDC_BUTTON_AUTO_CALIBRATION:
                return _T("User's Auto Calibration Wizard");
            case IDC_BUTTON_AUTO_TEACHING:
                return _T("User's Auto Teaching Wizard");
            case IDC_BUTTON_TEACHING:
                return _T("Teaching Points, Coordinate Offsets, Jogging, etc.");
            case IDC_BUTTON_TOOL:
                return _T("Tool Management Panel");
            case IDC_BUTTON_OPTION:
                return _T("Options - Waterflow, Airflow, Milling, etc.");
            case IDC_BUTTON_IO:
                return _T("Input/Output Panel");
            case IDC_BUTTON_TERM:
                return _T("Command Line (Expert)");
            case IDC_BUTTON_LOG:
                return _T("Log File Management");
            case IDC_BUTTON_SYSTEM:
                return _T("About the System, Soft Limits, Spindle Runtime, IP Addresses, etc.");
            case IDC_BUTTON_MAINTENANCE:
                return _T("Maintenance Schedule Management");   
            default:
                return _T("");
        }
	}
    
    TCHAR*				pResourcePath_;
	hcutil::CCanvasCE*	pCanvasCE_;

    CButton btnAutoTeaching_;

	CFont	fntButton_;
	CFont	fntCheckBox_;

	CBrush	brhSetupMain_;
	CBrush	brhTeachingButton_;
	CBrush	brhToolButton_;
	CBrush	brhAutoCalibrationButton_;
	CBrush	brhAutoTeachingButton_;
	CBrush	brhOptionButton_;
	CBrush	brhIOButton_;
	CBrush	brhLogButton_;
	CBrush	brhBackButton_;
	CBrush	brhMaintenanceButton_;
	
	
	void writeLog( LPCTSTR log_msg );
	void update_service_button_state();

public:
	CSetupMenuDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMenuDlg();

	void SetParentWnd( CWnd* pParent ) { pWndParent_ = pParent; }

	virtual void StartPageWork();
	virtual void StopPageWork();
	virtual void UpdatePage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_MENU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonTeaching();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonTool();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonAutoCalibration();
	afx_msg void OnBnClickedButtonAutoTeaching();
	afx_msg void OnBnClickedButtonOption();
	afx_msg void OnBnClickedCheckNoNddePassword();
	afx_msg void OnBnClickedButtonIo();
	afx_msg void OnBnClickedButtonTerm();
	afx_msg void OnBnClickedButtonLog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonSystem();
	// afx_msg void OnToolTipTextAboutToShow();
protected:
	virtual void PreInitDialog();
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonMaintenance();
	afx_msg void OnBnClickedButtonService();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};
