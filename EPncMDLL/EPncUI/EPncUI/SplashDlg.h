#pragma once

// CSplashDlg 대화 상자입니다.

class CSplashDlg : public CDialog
{
	DECLARE_DYNAMIC(CSplashDlg)

private:
	hcutil::CCanvasCE*	pCanvasCE_;
	int nBackgroundLayerIndex_;
	int nStatusLayerIndex_;
	CFont fontMsg_;
	CFont font_;

	void updateProgress(int stepNo);

public:
	CSplashDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSplashDlg();

// 대화 상자 데이터입니다.
	//enum { IDD = IDD_DIALOG_SPLASH };
	enum { IDD = 182 }; //IDD_DIALOG_SPLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	static CSplashDlg*	P_SPLASH_DLG;

	static void INITIALIZE_DLG();
	static void DELETE_DLG();
	static void SHOW_DLG();
	static void HIDE_DLG();
	static BOOL IS_SHOW();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreInitDialog();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
