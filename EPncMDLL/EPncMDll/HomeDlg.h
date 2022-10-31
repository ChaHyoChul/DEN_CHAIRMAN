#pragma once

#include "resource.h"
#include "afxwin.h"

// CHomeDlg 대화 상자입니다.

class CHomeDlg : public CDialog
{
	DECLARE_DYNAMIC(CHomeDlg)

	CBrush	brhBkgnd_;
	CBrush	brhHome_[2];
	CBrush	brhStop_;
	CFont	fntButton_;

	void updateButtonState();

public:
	CHomeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHomeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = 102 }; //IDD_DIALOG_HOME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

public:
	static CHomeDlg*	P_HOME_DLG;

	static void INITIALIZE_DLG();
	static void DELETE_DLG();
	static void SHOW_DLG();
	static void HIDE_DLG();
	static BOOL IS_SHOW();


	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonHome();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	CButton btnHome_;
};
