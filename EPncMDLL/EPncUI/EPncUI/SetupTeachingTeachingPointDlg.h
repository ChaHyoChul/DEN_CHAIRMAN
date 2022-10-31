#pragma once

#include "PointDataListBox.h"
#include "TitleBarWnd.h"

// CSetupTeachingTeachingPointDlg 대화 상자입니다.

class CSetupTeachingTeachingPointDlg : public CDialogListPage
{
	DECLARE_DYNCREATE(CSetupTeachingTeachingPointDlg)

	CTitleBarWnd*		pTitleBarWnd_;
	CPointDataListBox*	pTeachingPointListBox_;

	CRect	CUIrectSTTP; 

	void update_TeachingPointListBox();

public:
	CSetupTeachingTeachingPointDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupTeachingTeachingPointDlg();

	virtual void StartPageWork();
	virtual void StopPageWork();
	virtual void UpdatePage();

	void UpdateTeachingPointListBox() {
		update_TeachingPointListBox();
	}

	pa::EN_TEACHING_POINT GetCurrentSelectedTeachingPoint();

	void ToolPositionAutoCalc();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_TEACHING_TEACHING_POINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	LRESULT OnNotifyPointDataListBox(WPARAM wparam, LPARAM lparam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
