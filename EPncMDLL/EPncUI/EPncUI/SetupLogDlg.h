#pragma once
#include "afxwin.h"
#include "TitleBarWnd.h"

// CSetupLogDlg 대화 상자입니다.

class CSetupLogDlg : public CDialogListPage
{
	DECLARE_DYNCREATE(CSetupLogDlg)

	CWnd			*pParentWnd_;

	TCHAR*				pResourcePath_;
	hcutil::CCanvasCE*	pCanvasCE_;

	CTitleBarWnd	*pTitleBarWnd_;
	CFileListBox*	pFileListBoxForSD_;

	CBrush	brhBkgnd_;
	CBrush	brhBackButton_;

	CFont	fntMenuButton_;

	CRect	CUIrectSL;

	void initialzie_titlebar();
	void destroy_titlebar();

	void initialize_FileListBox();
	void destroy_FileListBox();

	void writeLog( LPCTSTR log_msg );

	void updateState_MenuButton();

public:
	CSetupLogDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupLogDlg();

	void SetParentWnd( CWnd* p ) { pParentWnd_ = p; }

	virtual void StartPageWork();
	virtual void StopPageWork();
	virtual void UpdatePage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonBack();
//	afx_msg void OnBnClickedButtonEmoReset();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonCopyToUsb();
	afx_msg void OnBnClickedButtonSelectAll();
	afx_msg void OnBnClickedButtonUnselectAll();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual void PreInitDialog();
public:
	afx_msg void OnPaint();
};
