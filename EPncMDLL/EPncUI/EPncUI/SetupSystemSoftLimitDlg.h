#pragma once

// CSetupSystemSoftLimitDlg 대화 상자입니다.
#include "OptionDataListBox.h"
#include "TitleBarWnd.h"

class CSetupSystemSoftLimitDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupSystemSoftLimitDlg)

	CTitleBarWnd*		pTitleBar_;
	COptionDataListBox*	pDataListBox_;

	CFont	fntButton_;

	double	fSoftLimitData_[pa::AXIS_NUM][2];			// 각 축의 +/- soft-limit 

	void writeLog( LPCTSTR log_msg );

public:
	CSetupSystemSoftLimitDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupSystemSoftLimitDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_SYSTEM_SOFTLIMIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonUpload();
	afx_msg void GetLimits();
	afx_msg void OnBnClickedButtonDownloadSave();
	afx_msg void OnBnClickedButtonClose();
	LRESULT OnNotifyPointDataListBox(WPARAM wparam, LPARAM lparam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
