#pragma once
#include "afxwin.h"


// CNumericInputTimeDlg 대화 상자입니다.

class CNumericInputTimeDlg : public CDialog
{
	DECLARE_DYNAMIC(CNumericInputTimeDlg)

private:
	//BOOL	bIsFloatType_;		// 실수 형이 아니면, Point를 Disable 
	CString strPrevNum_;		// 이전 숫자 저장 변수 
	CString strNum_;			// 숫자 저장 변수 
	
	int prevTime;
	int currTime;
	
	CFont	fntNum_;
	CFont	fntOldNum_;
	CFont	fntBtn_;
	CFont	fntUpDownBtn_;
	CFont	fntTime_;
	
	void updateTime();

public:
	CNumericInputTimeDlg(CWnd* pParent = NULL);		// 표준 생성자입니다.
	virtual ~CNumericInputTimeDlg();

	void SetTimePerMilling( int newTime );
	int GetTimePerMilling();
	
	
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_NUMERIC_INPUT_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

	afx_msg void OnBnClickedButtonIncMin();
	afx_msg void OnBnClickedButtonDecMin();
	afx_msg void OnBnClickedButtonIncSec();
	afx_msg void OnBnClickedButtonDecSec();
	

	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonEnter();
};
