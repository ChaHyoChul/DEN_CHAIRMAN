#pragma once
#include "afxwin.h"

// CSetupToolResetDlg 대화 상자입니다.

class CSetupToolResetDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupToolResetDlg)

	int		nToolNo_;
	DWORD	dwCurrentTime_;
	DWORD	dwMaximumTime_;
	double	fCurrentUsagePct;
	
	bool	prevUseMaxTimeCheck;

	CFont	fntUpDownBtn_;	// = heading 1
	CFont	fntOkCancelBtn_;
	CFont	fntTime_;
	CFont	fntHeading2_;
	CFont	fntToolNo_;

	BOOL	bIsPressButton_;
	int		nInc_;

	void incHour();
	void decHour();
	void incMinute();
	void decMinute();
	void incSecond();
	void decSecond();
	void updateCurrentTime();
	void updateMaximumTime();
	void updateToolNo();
	void writeLog( LPCTSTR log_msg );
	
public:
	CSetupToolResetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupToolResetDlg();

	void SetToolNo( int toolNo );

	DWORD GetUsingTime(); 
	DWORD GetMaximunTime(); 
	

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_TOOL_RESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
 	afx_msg void OnBnClickedButtonReset2();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonResetCur();
	afx_msg void OnBnClickedButtonResetMax();
	afx_msg void OnBnClickedCheckToolMaxUsageTime();
	CButton chkMaxToolUsageTime_;
	CNotifyButton btnHourUp_;
	CNotifyButton btnHourDown_;
	CNotifyButton btnMinuteUp_;
	CNotifyButton btnMinuteDown_;
	CNotifyButton btnSecondUp_;
	CNotifyButton btnSecondDown_;
	afx_msg LRESULT OnNotifyButton(WPARAM wparam, LPARAM lparam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
