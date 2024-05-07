#pragma once

#include "OptionDataListBox.h"
#include "TitleBarWnd.h"

// CSetupAutoCalibrationStep5Dlg 대화 상자입니다.

class CSetupAutoCalibrationStep5Dlg : public CDialogListPage
{
	DECLARE_DYNCREATE(CSetupAutoCalibrationStep5Dlg)

	CWnd*				pParent_;

	CBrush	brhBkgnd_;		// 배경색 
	CFont	fntBtn_;		// Run 버튼
    CRect	PCUIrectST;
	
    CFont fntSubstep_;
    CFont fntSubstepDesc_;
    
	int		nIsPressStopButton_;

	void updateCheckBoxState();

	void updateButtonState();

private:


public:
	CSetupAutoCalibrationStep5Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupAutoCalibrationStep5Dlg();

	virtual void StartPageWork();
	virtual void StopPageWork();
	virtual void UpdatePage();

	void SetParentWnd( CWnd* pParent ) { pParent_ = pParent; }

	void StartAutoCal();
	void StopAutoCal();

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
