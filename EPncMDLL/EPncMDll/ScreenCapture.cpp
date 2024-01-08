#include "StdAfx.h"
#include "ScreenCapture.h"

CScreenCapture* P_SCRN_CAPTURE = NULL;

CScreenCapture::CScreenCapture(void)
{
}

CScreenCapture::~CScreenCapture(void)
{
	Destroy();
}

void CScreenCapture::Initialize()
{
	szScreen_.cx = GetSystemMetrics( SM_CXSCREEN );
	szScreen_.cy = GetSystemMetrics( SM_CYSCREEN );
	szTile_.cx = szScreen_.cx / 8;
	szTile_.cy = szScreen_.cy / 8;
	szNumOfTile_.cx = 8;
	szNumOfTile_.cy = 8;

	pImageBuffer_ = NULL;
	pPrevImageBuffer_ = new char [szScreen_.cx * szScreen_.cy * 3 + 1];
	memset((void*)pPrevImageBuffer_, NULL, szScreen_.cx*szScreen_.cy*3 + 1);
	ASSERT(pPrevImageBuffer_);
	pTileImage_ = new char [szTile_.cx * szTile_.cy * 3 + 1];
	memset((void*)pTileImage_, NULL, szTile_.cx*szTile_.cy*3 + 1);
	ASSERT(pTileImage_);
	pJpgBuffer_ = new char [szTile_.cx * szTile_.cy * 3 + 1];
	memset((void*)pJpgBuffer_, NULL, szTile_.cx*szTile_.cy*3 + 1);
	ASSERT(pJpgBuffer_);
	pSendImageBuffer_ = new char [szScreen_.cx * szScreen_.cy * 3 + 1];
	memset((void*)pSendImageBuffer_, NULL, szScreen_.cx*szScreen_.cy*3 + 1);
	ASSERT(pSendImageBuffer_);
	nSendImageIndex_ = 0;
	nSendNumOfTile_ = 0;

	BITMAPINFO dibInfo;
	dibInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	dibInfo.bmiHeader.biWidth = szScreen_.cx;
	dibInfo.bmiHeader.biHeight = szScreen_.cy;
	dibInfo.bmiHeader.biPlanes = 1;
	dibInfo.bmiHeader.biBitCount = 24;
	dibInfo.bmiHeader.biCompression = BI_RGB;
	dibInfo.bmiHeader.biSizeImage = szScreen_.cx * szScreen_.cy * 3; 
	dibInfo.bmiHeader.biXPelsPerMeter = 0;
	dibInfo.bmiHeader.biYPelsPerMeter = 0;
	dibInfo.bmiHeader.biClrImportant = 0;
	dibInfo.bmiHeader.biClrUsed = 0;

	HDC hScrDC = CreateDC( _T("DISPLAY"), NULL, NULL, NULL );
	HBITMAP hbitmap = ::CreateDIBSection(hScrDC, &dibInfo, DIB_RGB_COLORS, (void **)&pImageBuffer_, 0, 0);
	hDeskTopDC_.Attach( hScrDC );
	hBitmap_.Attach( hbitmap );
	hMemDC_.CreateCompatibleDC( &hDeskTopDC_ );
	hMemDC_.SelectObject( hBitmap_ );

	//////////////////////////////////////////////////////////////////////////
	// 초기화 할때 이미지를 저장해 둔다 
	hMemDC_.StretchBlt( 0, 0, szScreen_.cx, szScreen_.cy, &hDeskTopDC_, 
						0, 0, szScreen_.cx, szScreen_.cy, SRCCOPY );
	//////////////////////////////////////////////////////////////////////////
}

void CScreenCapture::Destroy()
{
	HDC hTempDC = hDeskTopDC_.Detach();
	DeleteDC( hTempDC );

	if( pPrevImageBuffer_  != NULL ) {
		delete [] pPrevImageBuffer_;
		pPrevImageBuffer_ = NULL;
	}
	if( pTileImage_ != NULL ) {
		delete [] pTileImage_;
		pTileImage_ = NULL;
	}
	if( pJpgBuffer_ != NULL ) {
		delete [] pJpgBuffer_;
		pJpgBuffer_ = NULL;
	}
	if( pSendImageBuffer_ != NULL ) {
		delete [] pSendImageBuffer_;
		pSendImageBuffer_ = NULL;
	}
}

void CScreenCapture::Reset()
{
	TRACE( _T("CScreenCapture::Reset() -> Reset PrevImage !\n") );
	memset((void*)pPrevImageBuffer_, NULL, szScreen_.cx*szScreen_.cy*3);
}

void CScreenCapture::Capture()
{
	DWORD	dwTime = GetTickCount();
	BOOL	bIsSame = TRUE;
	int		nNumOfTile = 0;
	int		nStartX = 0;
	int		nEndX = 0;

	hMemDC_.StretchBlt( 0, 0, szScreen_.cx, szScreen_.cy, &hDeskTopDC_, 0, 0, szScreen_.cx, szScreen_.cy, SRCCOPY );

	nSendNumOfTile_ = 0;
	nSendImageIndex_ = 0;		// 전송 데이터 버퍼 인덱스 변수 초기화 
	nSendImageIndex_ += 4;		// Total Byte
	nSendImageIndex_ += 4;		// 조각 개수 

	for( int ty = 0; ty<szNumOfTile_.cy; ty++ )
	{
		for( int tx = 0; tx<szNumOfTile_.cx; tx++ )
		{
			bIsSame = TRUE;

			// 현재 타일의 시작 위치 저장 
			char* pTile = pImageBuffer_ + (ty * szScreen_.cx*3 * szTile_.cy) + (tx*szTile_.cx*3);
			char* pPrevTile = pPrevImageBuffer_ + (ty * szScreen_.cx*3 * szTile_.cy) + (tx*szTile_.cx*3);

			for( int i = 0; i<szTile_.cy; i++ )
			{
				// 한 라인(CX_TILE * 3 byte)씩 복사 
				char* pSrc = pTile + (szScreen_.cx * 3 * i);
				char* pPre = pPrevTile + (szScreen_.cx * 3 * i);

				memcpy( (void*)(pTileImage_ + i*szTile_.cx*3), pSrc, szTile_.cx*3 );

				// 이미지 비교 
				if( bIsSame == TRUE && (i%4 == 0) ) 
				{
					for( int j = 0; j<szTile_.cx; j+=4 )
					{
						if( pSrc[j*3+0]!=pPre[j*3+0] || 
							pSrc[j*3+1]!=pPre[j*3+1] || 
							pSrc[j*3+2]!=pPre[j*3+2] )
						{
							bIsSame = FALSE;
						}
					}
				}
			}

			// bIsSame==FALSE 이면 Tile 이미지를 전송 한다 
			if( bIsSame == FALSE )
			{
				CSize StartPos;
				StartPos.cx = tx * szTile_.cx;		// 화면 전환 시작점 (X축)
				StartPos.cy = ty * szTile_.cy;		// 화면 전환 시작점 (Y축)

				// 이미지 압축 
				DWORD dw = GetTickCount();
				hEncoder_.CompressImage( (unsigned char*)pTileImage_, (unsigned char*)pJpgBuffer_, szTile_.cx, szTile_.cy, nJpgImageSize_ );
				dw = GetTickCount() - dw;

// #ifdef _DEBUG 
// 				CString str;
// 				str.Format(_T("compress(%d, %d) => %d msec\n"), tx, ty, dw );
// 				TRACE( str );
// #endif 

				// 이미지 헤더 저장 
				memcpy((void*)(pSendImageBuffer_+nSendImageIndex_), (void*)&nJpgImageSize_, 4);	// 조각 이미지 크기
 				nSendImageIndex_ += 4;
				memcpy((void*)(pSendImageBuffer_+nSendImageIndex_), (void*)&szTile_.cx, 4);	// 조각 크기 (x)	
				nSendImageIndex_ += 4;
				memcpy((void*)(pSendImageBuffer_+nSendImageIndex_), (void*)&szTile_.cy, 4);	// 조각 크기 (y)	
				nSendImageIndex_ += 4;
				memcpy((void*)(pSendImageBuffer_+nSendImageIndex_), (void*)&StartPos.cx, 4);	// 조각 시작 위치 (x)	
				nSendImageIndex_ += 4;
				memcpy((void*)(pSendImageBuffer_+nSendImageIndex_), (void*)&StartPos.cy, 4);	// 조각 시작 위치 (y)	
				nSendImageIndex_ += 4;

				// 이미지 데이터 저장 
				memcpy((void*)(pSendImageBuffer_+nSendImageIndex_), (void*)pJpgBuffer_, nJpgImageSize_);
				nSendImageIndex_ += nJpgImageSize_;

				// 조각 개수 증가 
				nSendNumOfTile_ += 1;
			}
		}
	}

	// Header 
	memcpy((void*)(pSendImageBuffer_+0), (void*)&nSendImageIndex_, 4);		// 전체 데이터 크기
	memcpy((void*)(pSendImageBuffer_+4), (void*)&nSendNumOfTile_, 4);		// 조각 개수 

	// 이미지 백업 
	memcpy( (void*)pPrevImageBuffer_, (const void*)pImageBuffer_, szScreen_.cx*szScreen_.cy*3 );

// #ifdef _DEBUG 
// 	// 로그 출력
// 	dwTime = GetTickCount() - dwTime;
// 	CString strdbg;
// 	strdbg.Format(_T("CTestCERemoteServerDlg::send_screen_image3() => %dmsec \n"), dwTime );
// 	TRACE( strdbg );
// #endif 
}
