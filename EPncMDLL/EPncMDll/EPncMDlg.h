#pragma once


class CLCDAliveSender : public CGeneralThread
{
private:
	virtual void Execute();
};

// CEPncMDlg 대화 상자입니다.

class CEPncMDlg : public CDialog
{
	DECLARE_DYNAMIC(CEPncMDlg)

private:
	HANDLE hEvent_;
	int isInitSuccess_;

	CLCDAliveSender hLCDAliveSender_;


public:
	CEPncMDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEPncMDlg();

	int GetInitSuccess()
	{
		return isInitSuccess_;
	}

// 대화 상자 데이터입니다.
	enum { IDD = 106 }; //IDD_DIALOG_EPNCM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	static void WRITE_DIFF_TIME_LOG(DWORD dwDiffTick);
};


