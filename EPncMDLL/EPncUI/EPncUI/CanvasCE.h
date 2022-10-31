#pragma once

//////////////////////////////////////////////////////////////////////////
#include <memory>

#define MaxNumCanvasLayers	(24)

namespace hcutil
{
//////////////////////////////////////////////////////////////////////////

class CCanvasCELayer;
class CCanvasCELayerMgr;
class CCanvasCE;

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

class CCanvasCELayer : public CDC 
{
	// Attribute
private:
	CCanvasCELayerMgr*	pParent_;

	CBitmap*	pBitmap_;
	CBitmap*	pOldBmp_;

	BOOL		bUsingTransparentClr_;	// 투명색 사용 여부를 저장한다
	COLORREF	clrTransparent_;		// 투명색을 저장한다
	int			nWidth_;				// Layer의 크기 
	int			nHeight_;				
	BOOL		bIsDrawing_;			// 화면에 그릴지 여부를 저장한다



	// Implement 
private:
	void init( CCanvasCELayerMgr* pParent );
	void destroy( void );

public:
	/** 
	 * CanvasLayer 객체를 생성한다
	 */
	BOOL Create( BOOL bUsingTransparentClr, COLORREF clrTransparent );

	/** 
	 * Layer의 이미지를 Bitmap 파일에서 읽어서 Layer에 그린다 
	 *	- strBitmapImageFilePath : 읽을 비트맵 이미지의 파일 경로 
	 *	- ptDest : 이미지를 출력할 위치 
	 *	- szDest : 이미지를 그릴 영역의 크기  
	 *	- bTransparent : True이면 투명 칼라를 사용해서, 그린다
	 *	- clrTransparent : 투명 칼라 값 
	 */
	BOOL LoadImageFormFile( LPCTSTR strBitmapImageFilePath, CPoint ptDest, CSize szDest );

	/** 
	 * CanvasLayer 영역을 클리어 한다
	 */
	void Clear( BOOL bRedraw );
	void Clear( CRect& rc, BOOL bRedraw );

	/** 
	 * CanvasLayer의 rect 영역을 갱신한다
	 */
	void Update( const CRect& rect, BOOL bRedraw );

	/** 
	 * 투명색 사용 여부를 설정한다
	 */
	void SetUsingTransparentClr( BOOL bUsingTransparentClr ) {
		bUsingTransparentClr_ = bUsingTransparentClr;
	}
	BOOL GetUsingTransparentClr( void ) {
		return bUsingTransparentClr_;
	} 

	/** 
	 * Layer를 화면에 그릴지 설정한다
	 */
	void SetIsDrawing( BOOL bIsDrawing ) {
		bIsDrawing_ = bIsDrawing;
	}
	BOOL GetIsDrawing( void ) {
		return bIsDrawing_;
	}

	/** 
	 * 투명색을 리턴 한다
	 */
	COLORREF GetTransparentColor( void ) {
		return clrTransparent_;
	}

	// Constructor & Destructor
public:
	CCanvasCELayer( CCanvasCELayerMgr* pParent );
	~CCanvasCELayer( void );

	friend class CCanvasCELayerMgr;
};

//////////////////////////////////////////////////////////////////////////
// class LayerMgr
//////////////////////////////////////////////////////////////////////////

class CCanvasCELayerMgr
{
private:
	CCanvasCE*		pParent_;
	CCanvasCELayer*	pCanvasCELayers_[MaxNumCanvasLayers];
	int				nLayerCount_;		// Add() 함수에서 Layer를 Count 한다

private:
	void init( CCanvasCE* pCanvas );
	void destroy( void );

public:
	// 새로운 Layer를 추가 한다
	// Layer Index를 리턴. 만약 에러가 발생했을 경우 -1 리턴 
	int Add( BOOL bUsingTransparentClr, COLORREF clrTransparent );

	// Layer를 삭제 한다
	void RemoveAll( void );

	// CanvasLayer 정보를 리턴한다
	int GetCount( void );
	CCanvasCELayer* Get( int nIndex );

	// 
	void Draw( CDC* pDC, const CRect& rcRedraw );


public:
	CCanvasCELayerMgr( CCanvasCE* pCanvas );
	~CCanvasCELayerMgr( void );

	friend CCanvasCELayer;
};

//////////////////////////////////////////////////////////////////////////
//	class Canvas 
//////////////////////////////////////////////////////////////////////////

class CCanvasCE : public CDC
{
	// Attribute
private:
	CWnd*		pParent_;
	CBitmap*	pBitmap_;
	CBitmap*	pOldBmp_;
	int			nWidth_, nHeight_;
	COLORREF	clrBackground_;

	CCanvasCELayerMgr*		pCanvasCELayerMgr_;

	// Double Buffer 
	CDC			hDoubleBufferedMemDC_;		// 더블 버퍼로 사용하는 Memory DC
	CBitmap		hDoubleBufferedBmp_;		// 더블 버퍼로 사용하는 Bitmap 
	CBitmap*	pOldDoubleBufferedBmp_;

protected:
public:

	// Implement 
private:
	void init( void );
	void destroy( void );

	void update_CanvasCELayer( const CRect& rect, BOOL bRedraw );

public:
	/** 
	 * Canvas 객체를 만들고 초기화 한다
	 */
 	BOOL Create( CWnd* pParent_, HDC hdc, int nWidth, int nHeight, COLORREF clrBackground );

	/** 
	 * Parent Window를 리턴한다
	 */
	CWnd* GetParent( void ) { return pParent_; }

	/** 
	 * Canvas 영역을 모두 지운다
	 */
	void Clear( void ) {
		FillSolidRect( 0, 0, nWidth_, nHeight_, clrBackground_ );
	}
	void Clear( COLORREF color ) {
		FillSolidRect( 0, 0, nWidth_, nHeight_, color );
	}
	void Clear( const CRect& rc ) {
		FillSolidRect( rc.left, rc.top, rc.right, rc.bottom, clrBackground_ );
	}

	/** 
	 * CanvasCELayerMgr을 리턴한다
	 */
	CCanvasCELayerMgr* GetCanvasCELayerMgr( void ) {
		return pCanvasCELayerMgr_;
	}

	/** 
	 * 특정 영역을 새로 그린다.
	 */ 
	void Draw( HDC hdc, const CRect& rcRedraw );

	/** 
	 *
	 */
	int GetWidth( void ) { return nWidth_; }
	int GetHeight( void ) { return nHeight_; }


// Constructor & Destructor
public:
	CCanvasCE( void );
	virtual ~CCanvasCE( void );

	friend CCanvasCELayerMgr;
	friend CCanvasCELayer;
};

// typedef std::tr1::shared_ptr<CCanvasCELayer>	SpCanvasCELayer;
// typedef std::tr1::weak_ptr<CCanvasCELayer>		WpCanvsdCELayer;
// 
// typedef std::tr1::shared_ptr<CCanvasCELayerMgr>	SpCanvasCELayerMgr;
// typedef std::tr1::weak_ptr<CCanvasCELayerMgr>	WpCanvasCELayerMgr;
// 
// typedef std::tr1::shared_ptr<CCanvasCE>			SpCanvasCE;
// typedef std::tr1::weak_ptr<CCanvasCE>			WpCanvasCE;

//////////////////////////////////////////////////////////////////////////
}

