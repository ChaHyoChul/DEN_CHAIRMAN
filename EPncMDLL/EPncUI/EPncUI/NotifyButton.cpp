// NotifyButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "NotifyButton.h"

IMPLEMENT_DYNAMIC(CNotifyButton, CButton)

CNotifyButton::CNotifyButton()
{
	bPressed_ = FALSE;
}

CNotifyButton::~CNotifyButton()
{
}

BEGIN_MESSAGE_MAP(CNotifyButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CNotifyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	TRACE( _T("OnLButtonDown() \n") );

	SetCapture();

	bPressed_ = TRUE;

	::PostMessage( GetParent()->GetSafeHwnd(), WM_NOTIFY_BUTTON, (WPARAM)nID_, (LPARAM)1 );

	CButton::OnLButtonDown(nFlags, point);
}

void CNotifyButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	TRACE( _T("OnLButtonUp() \n") );

	bPressed_ = FALSE;

// 	::SendMessage( GetParent()->GetSafeHwnd(), WM_NOTIFY_BUTTON, (WPARAM)nID_, (LPARAM)0 );
	::PostMessage( GetParent()->GetSafeHwnd(), WM_NOTIFY_BUTTON, (WPARAM)nID_, (LPARAM)0 );

	ReleaseCapture();

	CButton::OnLButtonUp(nFlags, point);
}
