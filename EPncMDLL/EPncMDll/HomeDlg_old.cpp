// HomeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncMDll.h"
#include "HomeDlg.h"


// CHomeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHomeDlg, CDialog)

CHomeDlg::CHomeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHomeDlg::IDD, pParent)
{

}

CHomeDlg::~CHomeDlg()
{
}

void CHomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHomeDlg, CDialog)
END_MESSAGE_MAP()


// CHomeDlg 메시지 처리기입니다.
