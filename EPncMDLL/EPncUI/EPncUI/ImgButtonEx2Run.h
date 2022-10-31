#pragma once


// CImgButtonEx2Run

class CImgButtonEx2Run : public CImgButtonEx2
{
	DECLARE_DYNAMIC(CImgButtonEx2Run)

	CRect	rcMSArea_;
	CRect	rcSSArea_;
//	CRect	rcBufArea_;
	CRect	rcDbgMsgArea_[5];
	CRect	rcTimeArea_;
	CFont	hFont_;
	CFont	hFont2_;
	CFont*	pOldFont_;
	int		nLayerNo_;
	CRect	rcProgressArea_;

public:
	CImgButtonEx2Run();
	virtual ~CImgButtonEx2Run();

protected:
	virtual void after_create();
	virtual void update();

protected:
	DECLARE_MESSAGE_MAP()
};


