// JogButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "JogButton.h"


// CJogButton

IMPLEMENT_DYNAMIC(CJogButton, CButton)

CJogButton::CJogButton()
{
	nID_ = 0;
}

CJogButton::~CJogButton()
{
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CJogButton, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CJogButton::OnBnClicked)
// 	ON_CONTROL_REFLECT(BN_DOUBLECLICKED, &CJogButton::OnBnDoubleclicked)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CJogButton 메시지 처리기입니다.

void CJogButton::OnBnClicked()
{
}

// void CJogButton::OnBnDoubleclicked()
// {
// }

void CJogButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage( WM_JOGBUTTON, (WPARAM)nID_, (LPARAM)1 );

	CButton::OnLButtonDown(nFlags, point);
}

void CJogButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage( WM_JOGBUTTON, (WPARAM)nID_, (LPARAM)0 );

	CButton::OnLButtonUp(nFlags, point);
}

