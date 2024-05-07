#pragma once

#include "afxwin.h"

#include "PointDataListBox.h"
#include "TitleBarWnd.h"

class CSetupAutoCalibrationDlg : public CDialogListPage
{
public:
	CSetupAutoCalibrationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupAutoCalibrationDlg();

	void SetParentWnd( CWnd* p ) { pParentWnd_ = p; }

	virtual void StartPageWork();
	virtual void StopPageWork();

	enum { IDD = IDD_DIALOG_SETUP_TEACHING };
    
private:
	DECLARE_DYNCREATE(CSetupAutoCalibrationDlg)
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	
    // Controls
	CWnd				*pParentWnd_;

	TCHAR*				pResourcePath_;
	hcutil::CCanvasCE*	pCanvasCE_;

	CDialogMap			*pDlgMap_;	

	CFont	fntMenuButton_;
	CFont	fntTitleBar;
	CFont	fntStepNoDisplay_;
    
	CBrush	brhTitle_;
	CBrush	brhBkgnd_;
	CBrush	brhNextPrevButtons_;
	CBrush	brhBackButton_;
    
    CButton btnBack_;

	CRect	PCUIrectST;

    // Control variables
    int nCurrSelectedPage_;

	enum EN_SUB_PAGE
	{
		SUB_PAGE_STEP1 = 0,
		SUB_PAGE_STEP2,
		SUB_PAGE_STEP3,
		SUB_PAGE_STEP4,
		SUB_PAGE_STEP5,
		SUB_PAGE_STEP6,
		SUB_PAGE_NUM
	};
    
    static const CString GetPageID(EN_SUB_PAGE pgno) {
		static const CString pages[] = {
			_T("STEP1"),
			_T("STEP2"),
			_T("STEP3"),
			_T("STEP4"),
			_T("STEP5"),
			_T("STEP6"),
            _T("N/A")
		};
		return pages[pgno];
	}
    
    // Window function utilities
	void initialize_DlgMap();
    
	void destroy_DlgMap();

	void updateState_MenuButton();
	void updateScreen();
	void check_LCD_Event();
    
	void writeLog( LPCTSTR log_msg );
    
    // Button handlers
	afx_msg void OnBnClickedButtonNext();
    afx_msg void OnBnClickedButtonPrevious();	
	afx_msg void OnBnClickedRadioStep1();
	afx_msg void OnBnClickedRadioStep2();
	afx_msg void OnBnClickedRadioStep3();
	afx_msg void OnBnClickedRadioStep4();
	afx_msg void OnBnClickedRadioStep5();
	afx_msg void OnBnClickedRadioStep6();
    afx_msg void OnBnClickedButtonClose();

	afx_msg LRESULT OnLCDAutoCalStartStop(WPARAM wparam, LPARAM lparam);
    
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void PreInitDialog();
	DECLARE_MESSAGE_MAP()
};
