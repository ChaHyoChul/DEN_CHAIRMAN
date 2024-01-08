#pragma once

#include "TonyJpegEncoder.h"

class CScreenCapture
{
	CSize		szScreen_;			// 화면 크기 
	CSize		szTile_;			// 타일 이미지 크기 
	CSize		szNumOfTile_;		// 타일 개수 

	CBitmap		hBitmap_;
	CDC			hDeskTopDC_;
	CDC			hMemDC_;
	CTonyJpegEncoder	hEncoder_;

	char*		pImageBuffer_;		// 원본 이미지를 저장할 포인터 변수 
	char*		pPrevImageBuffer_;	// 이전 이미지를 저장할 메모리 변수 
	char*		pTileImage_;		// 조각 이미지를 Jpg로 변환하지 전 저장할 메모리 
	char*		pJpgBuffer_;		// 조각 이미지를 Jpg로 변환해서 저장할 메모리 
	int			nJpgImageSize_;
	char*		pSendImageBuffer_;	// 전송할 이미지 데이터 전체를 저장할 메모리 
	int			nSendImageIndex_;	// 
	int			nSendNumOfTile_;	// 전송할 타일 이미지 개수 

public: 
	void Initialize();
	void Destroy();

	void Reset();

	void Capture();					// 화면을 캡쳐하고 압축해서 저장한다 

	char* GetSendImageBuffer()
	{
		return pSendImageBuffer_;
	}

	int GetSendImageLen()
	{
		return nSendImageIndex_;
	}

	int GetSendNumOfTile()
	{
		return nSendNumOfTile_;
	}

public:
	CScreenCapture(void);
	~CScreenCapture(void);
};

extern CScreenCapture* P_SCRN_CAPTURE;
