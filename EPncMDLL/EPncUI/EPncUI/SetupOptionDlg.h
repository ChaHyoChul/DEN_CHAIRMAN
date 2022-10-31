#pragma once
#include "afxwin.h"
#include "TitleBarWnd.h"

// CSetupOptionDlg 대화 상자입니다.

class CSetupOptionDlg : public CDialogListPage
{
	DECLARE_DYNCREATE(CSetupOptionDlg)

	CWnd* pParentWnd_;

	TCHAR*				pResourcePath_;
	hcutil::CCanvasCE*	pCanvasCE_;

	BOOL	bEnableDownloadButton_;

	CBrush	brhBkgnd_;
	CBrush	brhBackButton_;

	CFont	fntMenuButton_;
	CFont	fntCheckBox_;
	CFont	fntEditBox_;

	CRect CUIrectSetT;


    CToolTipCtrl        m_tip_ctrl;
    BOOL                m_bShowToolTips;

    static const CString ToolTipText(int index) {
        switch (index) {
            case IDC_CHECK_USING_LCD:
                return _T("Enables communication with embedded LCD.");
            case IDC_CHECK_USING_OP_PANEL:
                return _T("Enables safety stop when the safety door is opened during machine operation.");
            case IDC_CHECK_USING_AIR_PRESSURE_LIMIT:
                return _T("Enables pressure checking function, which reads from the sensors on the side panel of the machine.");
            case IDC_BUTTON_AIR_PRESSURE_INTERVAL:
                return _T("Specifies how long does the reading need to be wrong so the insufficient pressure is confirmed.");
            case IDC_CHECK_USING_SPINDLE_AIR_PURGE:
                return _T("Setting these parameters will cut off the air purge supply after last machine’s action to reduce consumption of the compressed air.");
            case IDC_BUTTON_PURGE_AIR_HOLD_TIME:
                return _T("Specifies when to cut off the air purge after the last usage.");
            case IDC_CHECK_USING_FLOW_SENSOR:
                return _T("Specifies whether the water flow is verified by sensory data.");
            case IDC_BUTTON_FLOW_SENSOR_TIMEOUT:
                return _T("Specifies how long does the flow sensor needs to be OFF when water flow is expected to confirm a water flow failure during operation of the water pump.");
            case IDC_BUTTON_FLOW_SENSOR_START_TIMEOUT:
                return _T("Specifies how long does the flow sensor needs to be OFF when water flow is expected to confirm a water flow failure on start of the water pump.");
            case IDC_CHECK_USING_LOGGING:
                return _T("Enables logging function. When enables, operational details can be traced from .log and .err files. ");
            case IDC_CHECK_DEMO_MODE:
                return _T("Demonstration mode for exhibition shows. Executes the NC file over and over again.");
            case IDC_CHECK_INVALID_NC_CODE:
                return _T("Verify the machine can read the NC code on file open");
            case IDC_CHECK_NC_FILE_TAG:
                return _T("Checks for start/end program tag (%) to make sure the file was fully uploaded.");
            case IDC_CHECK_MACHINE_ID:
                return _T("Verifies machine type and MACHINE ID to prevent collision.");
            case IDC_CHECK_SPINDLE_OFFSET:
                return _T("Compares spindle offset in the NC file (if present) with spindle offset of the machine.");
            case IDC_CHECK_TRANSFORM_COORDINATE:
                return _T("Removes duplicate lines or close-position movement which may reduce vibrations and milling time.");
            case IDC_CHECK_BLCHK:
                return _T("Enables a block-size-checking sequence at the start of an NC-file.");
            case IDC_STATIC_TOOL_TIME:
                return _T("Time per milling used for computation of approximate number of millings before tool replacement.");
            case IDC_BUTTON_TOOLS14:
                return _T("Set time-per-milling for tools #1, #4");
            case IDC_BUTTON_TOOLS25:
                return _T("Set time-per-milling for tools #2, #5");
            case IDC_BUTTON_TOOLS3:
                return _T("Set time-per-milling for tool #3");
            case IDC_BUTTON_TOOLS6:
                return _T("Set time-per-milling for tool #6");
            default:
                return _T("");
        }
	}

	void updateState();

	void updateControlState();

	void updateState_MenuButton();

	void writeLog( LPCTSTR log_msg );

	void upload_to_pc();

	BOOL save_sw_config_data( CString& strErrMsg );
	BOOL load_sw_config_data( CString& strErrMsg );
	int dToolTimes[6];

public:
	CSetupOptionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupOptionDlg();

	void SetParentWnd( CWnd* pParent ) { pParentWnd_ = pParent; }

	virtual void StartPageWork();
	virtual void StopPageWork();
	virtual void UpdatePage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonClose();
	CButton chkUsingLCD_;
	afx_msg void OnBnClickedCheckUsingLCD();
	CButton chkUsingAirPressureLimit_;
	CButton chkDemoMode_;
	afx_msg void OnBnClickedButtonSave();
	CButton btnAirPressureInterval_;
	afx_msg void OnBnClickedButtonAirPressureInterval();
	afx_msg void OnBnClickedCheckDemoMode();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton chkUsingOpPanel_;
	int nToolErrOccureHandingCode_;
	CButton chkUsingFlowSensor_;
	CButton btnFlowSensorTimeout_;
	afx_msg void OnBnClickedButtonFlowSensorTimeout();
	afx_msg void OnBnClickedButtonDownloadToColtrollor();
	afx_msg void OnBnClickedCheckUsingFlowSensor();
	afx_msg void OnBnClickedCheckBlchk();
	afx_msg void OnBnClickedButtonBlchkSpeed();
	afx_msg void OnBnClickedButtonBlchkTorque();
	CButton chkInvalidNcCode_;
	CButton chkNcFileTag_;
	CButton chkMachineID_;
	CButton chkSpindleOffset_;
	CButton chkTransformCoord_;
	CButton chkBlchk_;
	CButton chkBlchkSpeed_;
	CButton chkBlchkTorque_;
	afx_msg void OnBnClickedCheckInvalidNcCode();
	afx_msg void OnBnClickedCheckNcFileTag();
	afx_msg void OnBnClickedCheckMachineID();
	afx_msg void OnBnClickedCheckSpindleOffset();
	afx_msg void OnBnClickedCheckTransformCoordinate();
	CButton chkUsingSpindleAirPurge_;
	afx_msg void OnBnClickedCheckUsingSpindleAirPurge();
protected:
	virtual void PreInitDialog();
public:
	afx_msg void OnPaint();
//	afx_msg void OnBnClickedButtonLoad();
	CButton chkUsingLogging_;
	afx_msg void OnBnClickedButtonLoadFromFile();
	afx_msg void OnBnClickedCheckUsingOpPanel();
	afx_msg void OnBnClickedCheckUsingAirPressureLimit();
	afx_msg void OnBnClickedCheckUsingLogging();
	afx_msg void OnBnClickedButtonFlowSensorStartTimeout();
	CButton btnFlowSensorStartTimeout_;
	CButton btnPurgeAirHoldTime_;
	afx_msg void OnBnClickedButtonPurgeAirHoldTime();
	
	afx_msg void OnBnClickedButtonTool14();
	afx_msg void OnBnClickedButtonTool25();
	afx_msg void OnBnClickedButtonTool3();
	afx_msg void OnBnClickedButtonTool6();
	
	CButton btnTool14_;
	CButton btnTool25_;
	CButton btnTool3_;
	CButton btnTool6_;

private:
	
	CString parseSecToTimeStr(int seconds);

};
