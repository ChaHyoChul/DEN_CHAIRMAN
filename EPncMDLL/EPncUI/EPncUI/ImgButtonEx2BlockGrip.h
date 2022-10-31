#pragma once


// CImgButtonEx2BlockGrip

class CImgButtonEx2BlockGrip : public CImgButtonEx2
{
	DECLARE_DYNAMIC(CImgButtonEx2BlockGrip)

	CRect	rcArea_;
	int		nLayerNo_;
	CBrush	hBrhOn_, hBrhOff_, *pOIdBrh_;
	CPen	hPenOn_, hPenOff_, *pOldPen_;
	int		nPrevDetectBlock_;

public:
	CImgButtonEx2BlockGrip();
	virtual ~CImgButtonEx2BlockGrip();

protected:
	virtual void after_create();
	virtual void update();

protected:
	DECLARE_MESSAGE_MAP()
};


