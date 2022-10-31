#pragma once

#include "ToolTimeDispWnd.h"
#include "afxwin.h"
#include "TitleBarWnd.h"

// CSetupAutoTeachingStep7Dlg대화 상자입니다.

class CSetupAutoTeachingStep7Dlg: public CDialogListPage
{
	DECLARE_DYNCREATE(CSetupAutoTeachingStep7Dlg)

	CTitleBarWnd*	pTitleBarWnd_;

	int		nNumTools_;
	BOOL	bDirectAccess_;					// Operation 화면에서 직접 접근 

	CBrush	brhBkgnd_;						// 다이얼로그 배경 색 
	CBrush	brhATC_;						// background of ATC
	CBrush	brhBackButton_;
	CBrush	brhDark_;

	CFont	fntButton_;						// 버튼의 폰트 
	CFont	fntToolError_;				// error font
	CFont	fntMenuButton_;					// 매뉴 버튼
	CFont	fntToolNo_;
	CFont	fntSubstep_;
	CFont	fntToolNoLabel_;
	
	CRect	PCUIrectST; 

	CToolTimeDispWnd*	pToolTimeDispWnd_[pa::MAX_TOOL_NUM+1];

	CWnd* pParentWnd_;

	TCHAR*				pResourcePath_;
	hcutil::CCanvasCE*	pCanvasCE_;
	int					nBackgroundLayerIndex_;
	int					nStatusLayerIndex_;

	int		nSpindleOverride_;

	void updateButtonState();
	void updateToolErrMsg();
	void doResetTool( int nToolNo );
	void doTimeTool( int nToolNo );

	void updateState_MenuButton();
	
	void updateToolNo();

	void writeLog( LPCTSTR log_msg );

	enum EN_OPER_BUTTON
	{
		OPER_BTN_T1 = 0,
		OPER_BTN_T2,
		OPER_BTN_T3,
		OPER_BTN_T4,
		OPER_BTN_T5,
		OPER_BTN_T6,

		OPER_BTN_NUM
	};

private:
	CImgButtonEx2*			pOperButtonsEx_[OPER_BTN_NUM];
	void initialize_OperButtons();
	void destroy_OperButtons();

public:
	CSetupAutoTeachingStep7Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupAutoTeachingStep7Dlg();

	virtual void StartPageWork();
	virtual void StopPageWork();
	virtual void UpdatePage();

	void SetParentWnd( CWnd* p ) { pParentWnd_ = p; }

	void SetDirectAccess( BOOL b ) { bDirectAccess_ = b; };

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnImgButtonExClicked(WPARAM wparam, LPARAM lparam); // WM_IMGBUTTONEX_CLICKED
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonGetTool1();
	afx_msg void OnBnClickedButtonGetTool2();
	afx_msg void OnBnClickedButtonGetTool3();
	afx_msg void OnBnClickedButtonGetTool4();
	afx_msg void OnBnClickedButtonGetTool5();
	afx_msg void OnBnClickedButtonGetTool6();
	afx_msg void OnBnClickedButtonToolReturn();
	afx_msg void OnBnClickedButtonTool2Return();
	afx_msg void OnBnClickedButtonResetToolNo();
	afx_msg void OnBnClickedButtonResetTool2No();
	afx_msg void OnBnClickedButtonSetToolNo();
	afx_msg void OnBnClickedButtonSetTool2No();
	afx_msg void OnBnClickedButtonTimeTool1();
	afx_msg void OnBnClickedButtonTimeTool2();
	afx_msg void OnBnClickedButtonTimeTool3();
	afx_msg void OnBnClickedButtonTimeTool4();
	afx_msg void OnBnClickedButtonTimeTool5();
	afx_msg void OnBnClickedButtonTimeTool6();
	afx_msg void OnBnClickedButtonResetTool1();
	afx_msg void OnBnClickedButtonResetTool2();
	afx_msg void OnBnClickedButtonResetTool3();
	afx_msg void OnBnClickedButtonResetTool4();
	afx_msg void OnBnClickedButtonResetTool5();
	afx_msg void OnBnClickedButtonResetTool6();
	afx_msg void OnBnClickedCheckToolClampUnclamp();
	afx_msg void OnBnClickedCheckTool2ClampUnclamp();
	CButton chkBtnToolClamp_;
	CButton chkBtnTool2Clamp_;
	afx_msg void OnPaint();
protected:
	virtual void PreInitDialog();
};

