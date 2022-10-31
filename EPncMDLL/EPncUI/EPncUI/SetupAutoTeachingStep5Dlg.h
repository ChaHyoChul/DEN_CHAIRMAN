#pragma once

#include "OptionDataListBox.h"
#include "TitleBarWnd.h"

// CSetupAutoTeachingStep5Dlg 대화 상자입니다.

class CSetupAutoTeachingStep5Dlg : public CDialogListPage
{
	DECLARE_DYNCREATE(CSetupAutoTeachingStep5Dlg)

	CWnd*				pParent_;

	CBrush	brhBkgnd_;		// 배경색 
	CRect	PCUIrectST;
	
	CFont	fntBtn_;		// Run 버튼
	CFont	fntSubstep_;		// Run 버튼
	CFont	fntSubstepDesc_;		// Run 버튼
    
    int		nIsPressStopButton_;

	void updateButtonState();

private:


public:
	CSetupAutoTeachingStep5Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupAutoTeachingStep5Dlg();

	virtual void StartPageWork();
	virtual void StopPageWork();
	virtual void UpdatePage();

	void SetParentWnd( CWnd* pParent ) { pParent_ = pParent; }

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStartStop();
};
