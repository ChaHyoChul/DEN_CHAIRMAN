#pragma once

#include "OptionDataListBox.h"
#include "TitleBarWnd.h"

// CSetupTeachingOffsetDlg 대화 상자입니다.

class CSetupTeachingOffsetDlg : public CDialogListPage
{
	DECLARE_DYNCREATE(CSetupTeachingOffsetDlg)

	CTitleBarWnd*			pTitleBarWnd_;
	COptionDataListBox*		pOptionDataListBox_;

	CRect	CUIrectSTOff; 

	void update_OptionListBox();

public:
	CSetupTeachingOffsetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupTeachingOffsetDlg();

	virtual void StartPageWork();
	virtual void StopPageWork();
	virtual void UpdatePage();

	void UpdateOptionListBox() {
		update_OptionListBox();
	}

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_TEACHING_OFFSET };

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
