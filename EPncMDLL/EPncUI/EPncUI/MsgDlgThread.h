#pragma once

//////////////////////////////////////////////////////////////////////////
// CMsgDlgThread
//////////////////////////////////////////////////////////////////////////
#include "MsgDlg.h"
class CMsgDlgThread : public CWinThread
{
	DECLARE_DYNCREATE(CMsgDlgThread)

	CMsgDlg*	pDlg_;

protected:
public:	
	CMsgDlgThread();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMsgDlgThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void SetType( CMsgDlg::EN_TYPE hType ) {
		pDlg_->SetType( hType );
	}

	void SetLeftIcon( CMsgDlg::EN_ICON hIcon ) {
		pDlg_->SetLeftIcon( hIcon );
	}

	void SetMessage( CString& strMsg ) {
		pDlg_->SetMsg( strMsg );
	}

	void Show() {
		pDlg_->ShowWindow( SW_SHOW );
	}

	void Show( CMsgDlg::EN_TYPE hType, CMsgDlg::EN_ICON hIcon, CString& strMsg ) {
		pDlg_->SetType( hType );
		pDlg_->SetLeftIcon( hIcon );
		pDlg_->SetMsg( strMsg );
		//pDlg_->ShowWindow( SW_SHOW );
		pDlg_->Show();
	}

	void Hide() {
		//pDlg_->ShowWindow( SW_HIDE );
		pDlg_->Hide();
	}

	CMsgDlg* GetDlg() {
		return pDlg_;
	}

	CMsgDlg::EN_RET Wait() {
		MSG msg;
		while( pDlg_->IsWindowVisible() ) 
		{
			if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );	
			}
			Sleep( 10 );
		}
		return pDlg_->GetRetCode();
	}

protected:
	DECLARE_MESSAGE_MAP()

public:

	static CMsgDlgThread* GetInstance();
};

