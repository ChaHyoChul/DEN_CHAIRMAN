#pragma once
#include "afxwin.h"
#include "TitleBarWnd.h"
#include "IOListBoxEx.h"

// CSetupIODlg 대화 상자입니다.

class CSetupIODlg : public CDialogListPage
{
	DECLARE_DYNCREATE(CSetupIODlg)

	CWnd*			pParentWnd_;

	TCHAR*				pResourcePath_;
	hcutil::CCanvasCE*	pCanvasCE_;

	CTitleBarWnd	*pTitleBarWnd_;
	CIOListBoxEx	*pInputListBox_;
	CIOListBoxEx	*pOutputListBox_;

	CBrush	brhBkgnd_;
	CBrush	brhBackButton_;

	CFont	fntMenuButton_;

	CRect CUIrectSIO;

	void initialzie_titlebar();
	void destroy_titlebar();

	void initialize_inputlistbox();
	void destroy_inputlistbox();

	void initialize_outputlistbox();
	void destroy_outputlistbox();

	void updateState_MenuButton();

	void writeLog( LPCTSTR log_msg );

public:
	CSetupIODlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupIODlg();

	void SetParentWnd( CWnd* pParent ) { pParentWnd_ = pParent; }

	virtual void StartPageWork();
	virtual void StopPageWork();
	virtual void UpdatePage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_IO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonBack();
//	afx_msg void OnBnClickedButtonEmoReset();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

// 	static int *P_INPUT[64];
// 	static int *P_OUTPUT[64];
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual void PreInitDialog();
public:
	afx_msg void OnPaint();
};
