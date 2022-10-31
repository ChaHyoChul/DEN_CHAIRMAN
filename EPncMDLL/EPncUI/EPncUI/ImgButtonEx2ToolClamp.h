#pragma once


// CimgButtonEx2ToolClamp

class CImgButtonEx2ToolClamp : public CImgButtonEx2
{
	DECLARE_DYNAMIC(CImgButtonEx2ToolClamp)

	int		nPrevToolNo_;
	CRect	rcArea_;
	int		nLayerNo_;
	CBrush	hBrhOn_, hBrhOff_, *pOIdBrh_;
	CPen	hPenOn_, hPenOff_, *pOldPen_;
	CFont	fntToolNo_;

public:
	CImgButtonEx2ToolClamp();
	virtual ~CImgButtonEx2ToolClamp();

	virtual void after_create();
	virtual void update();

protected:
	DECLARE_MESSAGE_MAP()
};


