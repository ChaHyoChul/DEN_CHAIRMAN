#pragma once

#include "OptionDataListBox.h"
#include "TitleBarWnd.h"

// CSetupTeachingAutoTeachingDlg 대화 상자입니다.

class CSetupTeachingAutoTeachingDlg : public CDialogListPage
{
	DECLARE_DYNCREATE(CSetupTeachingAutoTeachingDlg)

	CWnd*				pParent_;
	CTitleBarWnd*		pTitleBarWnd_;
	COptionDataListBox*	pParamListBox_;

	CBrush	brhBkgnd_;		// 배경색 
	CFont	fntBtn_;		// Run 버튼  
	CRect	CUIrectSTAT; 
	
	int		nIsPressStopButton_;

	void updateCheckBoxState();

	void updateButtonState();

public:
	CSetupTeachingAutoTeachingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupTeachingAutoTeachingDlg();

	virtual void StartPageWork();
	virtual void StopPageWork();
	virtual void UpdatePage();

	void SetParentWnd( CWnd* pParent ) { pParent_ = pParent; }

	void UpdateParamListBox();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_TEACHING_AUTO_TEACHING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreInitDialog();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	LRESULT OnNotifyPointDataListBox(WPARAM wparam, LPARAM lparam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStartStop();
	int nSelectScope_;
	afx_msg void OnBnClickedRadioSelectFull();
	afx_msg void OnBnClickedRadioSelectOnly();
};
