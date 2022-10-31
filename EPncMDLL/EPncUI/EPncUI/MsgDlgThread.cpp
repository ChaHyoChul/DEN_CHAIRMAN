// MsgDlgThread.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "MsgDlgThread.h"

// CMsgDlgThread

IMPLEMENT_DYNCREATE(CMsgDlgThread, CWinThread)

CMsgDlgThread::CMsgDlgThread()
{
	pDlg_ = NULL;
}

CMsgDlgThread::~CMsgDlgThread()
{

}

BOOL CMsgDlgThread::InitInstance()
{
	pDlg_ = new CMsgDlg();
	pDlg_->UpdateWindow();
	pDlg_->ShowWindow( SW_HIDE );
	
	return TRUE;
}

int CMsgDlgThread::ExitInstance()
{
	if( pDlg_ ) {
		pDlg_->DestroyWindow();
		delete pDlg_;
		pDlg_ = NULL;
	}

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMsgDlgThread, CWinThread)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CMsgDlgThread 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

CMsgDlgThread* CMsgDlgThread::GetInstance()
{
	static CMsgDlgThread *obj = NULL;

	if( obj == NULL ) {
		obj = new CMsgDlgThread();
		obj->CreateThread();
	}

	return obj;
}

