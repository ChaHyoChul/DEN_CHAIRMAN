#pragma once

#include "PointDataListBox.h"
#include "TitleBarWnd.h"

// CSetupAutoCalibrationStep1Dlg 대화 상자입니다.

class CSetupAutoCalibrationStep1Dlg : public CDialogListPage
{
	DECLARE_DYNCREATE(CSetupAutoCalibrationStep1Dlg)

    CBrush	brhBkgnd_;
    
	CRect	PCUIrectST;
    
    CFont fntSubstep_;
    CFont fntSubstepDesc_;

private:
	CSetupAutoCalibrationStep1Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupAutoCalibrationStep1Dlg();

	virtual void StartPageWork();
	virtual void StopPageWork();
    

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_TEACHING_COORD_OFFSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
    
private:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
