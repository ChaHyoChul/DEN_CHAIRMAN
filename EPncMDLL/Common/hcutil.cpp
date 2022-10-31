#include "StdAfx.h"
#include "hcutil.h"

/** 
 * Ascii 문자열을 Unicode 문자열로 변경 
 */ 
void hcutil::ASCII_TO_UNICODE( char* pAscii, wchar_t* pUnicode, int nBufferLen )
{
	size_t numberOfConverted = 0;

	memset( (void*)pUnicode, 0, sizeof(TCHAR)*nBufferLen );

	mbstowcs_s( &numberOfConverted, pUnicode, nBufferLen, pAscii, _TRUNCATE );
}

/** 
 * Unicode 문자열을 Ascii 문자열로 변경 
 */
void hcutil::UNICODE_TO_ASCII( wchar_t* pUnicode, char* pAscii, int nBufferLen )
{
	size_t numberOfConverts = 0;

	memset( (void*)pAscii, 0, sizeof(char)*nBufferLen );

	wcstombs_s( &numberOfConverts, pAscii, nBufferLen, pUnicode, _TRUNCATE );
}

/** 
 * Unicode 문자열을 Utf-8 문자열로 변환 
 */
void hcutil::UNICODE_TO_UTF8( wchar_t* pUnicode, wchar_t* pUtf8, int nBufferLen )
{
	WideCharToMultiByte( CP_UTF8, 0, pUnicode, -1, (LPSTR)pUtf8, nBufferLen, NULL, NULL );	
}

/** 
 * Utf-8 문자열을 Unicode 문자열로 변환
 */
CString hcutil::UTF8_TO_CSTRING( LPCSTR pUtf8 )
{
	TCHAR szTemp[256];

	memset((void*)szTemp, 0, sizeof(TCHAR)*256);
	MultiByteToWideChar( CP_UTF8, 0, pUtf8, -1, (LPWSTR)szTemp, 255 );	

	CString strRet;
	strRet.Format( _T("%s"), szTemp );
	
	return strRet;
}

/** 
 * Ascii 문자열을 CString 문자열로 변경
 */
CString hcutil::ASCII_TO_CSTRING( char* pAscii )
{
	CString	strRet;
	wchar_t	szTemp[128];
	size_t	numberOfConverted = 0;

	memset( (void*)szTemp, 0, sizeof(wchar_t)*128 );

	mbstowcs_s( &numberOfConverted, szTemp, 128, pAscii, _TRUNCATE );

	strRet.Format( _T("%s"), szTemp ); 

	return strRet;
}

/** 
 * CString 문자열을 Ascii 문자열로 변경 
 */
void hcutil::CSTRING_TO_ASCII( CString& str, char* pAscii, int nBufferLen )
{
	size_t numberOfConverts = 0;

	memset( (void*)pAscii, 0, sizeof(char)*nBufferLen );

	wcstombs_s( &numberOfConverts, pAscii, nBufferLen, (const wchar_t *)(LPCTSTR)str, _TRUNCATE );
}

/** 
 * 파일의 유무를 확인해서, 있으면 TRUE 리턴 
 */
BOOL hcutil::IsExistFile( CString& strFilePath )
{
	WIN32_FIND_DATA	wfd;
	HANDLE	hr;
	BOOL	bRet = TRUE;

	hr = FindFirstFile( (LPCTSTR)strFilePath, &wfd );
	if( hr == INVALID_HANDLE_VALUE || hr == NULL ) {
		bRet = FALSE;
	}
	if( bRet ) {
		FindClose( hr );
	}

	return bRet;
}

BOOL hcutil::IsExistDir( CString& strDir )
{
	DWORD dwRet = GetFileAttributes( strDir );

	return (BOOL)(  dwRet != 0xFFFFFFFF );
}

BOOL hcutil::IsExistFile( TCHAR *pFilePath )
{
	WIN32_FIND_DATA	wfd;
	HANDLE	hr;
	BOOL	bRet = TRUE;

	hr = FindFirstFile( (LPCTSTR)pFilePath, &wfd );
	if( hr == INVALID_HANDLE_VALUE || hr == NULL ) {
		bRet = FALSE;
	}
	if( bRet ) {
		FindClose( hr );
	}

	return bRet;
}

BOOL hcutil::IsExistDir( TCHAR *pDir )
{
	DWORD dwRet = GetFileAttributes( (LPCTSTR)pDir );

	return (BOOL)(  dwRet != 0xFFFFFFFF );
}

/** 
 * 파일의 크기를 byte 단위로 리턴한다. 파일이 없으면 FALSE 
 */
BOOL hcutil::GetFileSize( CString& strFilePath, DWORD* pdwRetFileSize, CString& strErrMsg )
{
	// 파일 크기를 알아내고,
	HANDLE hTempFile = CreateFile( strFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

	if( hTempFile == INVALID_HANDLE_VALUE || hTempFile == NULL ) {
		DWORD dwErrCode = GetLastError();
		hcutil::GET_SYSTEM_ERROR_MESSAGE( dwErrCode, strErrMsg );
		return FALSE;
	}
	if( pdwRetFileSize ) {
		*pdwRetFileSize = (int)::GetFileSize( hTempFile, NULL );
	}

	CloseHandle( hTempFile );

	return TRUE;
}

/** 
 * 파일을 삭제 한다 
 */
BOOL hcutil::DeleteFile( CString& strFilePath, CString& strErrMsg )
{
	BOOL bRet = ::DeleteFile( strFilePath );

	if( !bRet ) {
		DWORD dwErrCode = GetLastError();
		GET_SYSTEM_ERROR_MESSAGE( dwErrCode, strErrMsg );
	} 
	
	return bRet;
}

int hcutil::GetFileNames( CString strFilesPath, CString fileNames[] ) {
	CWaitCursor waitCur;
	int nIndex = 0; 
	
	if(!strFilesPath.IsEmpty()) {
		CFileFind find;
		CString strFile;

		if(strFilesPath.Right(1) != _T("\\"))
			strFile = strFilesPath + _T("\\*.*");
		else
			strFile = strFilesPath + _T("*.*");
		
		BOOL bFound = find.FindFile(strFile);
		while(bFound)
		{
			bFound = find.FindNextFile();
			CString strFileName;

			if(find.IsDirectory())
				continue;

			strFileName = find.GetFileName();
			fileNames[nIndex++] = strFileName;
		}
	}

	return nIndex;
}

/** 
 * System 에러 코드를 메시지로 변환 
 */
void hcutil::GET_SYSTEM_ERROR_MESSAGE( DWORD dwErrorCode, CString& strErrorMessage )
{
	TCHAR szMsg[1024] = {0,};

	memset( (void*)szMsg, 0, sizeof(char)*1024 );

	// MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErrorCode, 0, szMsg, 1024, NULL );
	strErrorMessage.Format( _T("[%d] %s"), dwErrorCode, szMsg );

//////////////////////////////////////////////////////////////////////////
//	TEST-CODE
// 	DWORD dwTemp = FormatMessage( 
// 		// FORMAT_MESSAGE_FROM_SYSTEM 
// 		FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
// 		NULL, 
// 		dwErrorCode, 
// 		0, 
// 		(LPTSTR)&s, //szMsg, 
// 		0,	//1024, 
// 		NULL );
// 	strErrorMessage.Format( _T("[%d] %s"), dwErrorCode, s ); //szMsg );
//////////////////////////////////////////////////////////////////////////
}

/** 
 * Dialog 리소스의 위치/크기를 알아 낸다 
 */
void hcutil::GetControlPos( UINT nID, CWnd* pParet, CRect* pRect, BOOL bRemoveCtrl )
{
	CStatic* pStatic = (CStatic*)(pParet->GetDlgItem( nID ));
	ASSERT( pStatic );
	pStatic->GetWindowRect( pRect );
	pParet->ScreenToClient( pRect );
	if( bRemoveCtrl ) {
		pStatic->DestroyWindow();
	}
}

void hcutil::GetControlPos2( UINT nID, CWnd* pParet, CRect* pRect, CRect* pRect2, BOOL bRemoveCtrl )
{
	CStatic* pStatic = (CStatic*)(pParet->GetDlgItem( nID ));
	CRect RectUI;
	ASSERT( pStatic );
	pParet->GetWindowRect(RectUI);
	pParet->GetClientRect( RectUI );

	
	pStatic->GetWindowRect( pRect );
	pParet->ScreenToClient( pRect );


	int top=pRect->top;
	int bottom=pRect->bottom;
	int left=pRect->left;
	int right=pRect->right;
	double newheight=((double)(RectUI.bottom))/(pRect2->bottom);
	double newwidth=((double)(RectUI.right))/(pRect2->right);
	
	int newtop=(int)(top*newheight+0.5);
	int newbottom=(int)(bottom*newheight+0.5);

	int newleft=(int)(left*newwidth+0.5);
	int newright=(int)(right*newwidth+0.5);

//y = (int)(x + 0.5);

	pStatic->MoveWindow(newleft,newtop, newright-newleft,newbottom-newtop);
	pStatic->GetWindowRect( pRect );
	pParet->ScreenToClient( pRect );
	if( bRemoveCtrl ) {
		pStatic->DestroyWindow();
	}
}

void hcutil::reposbutton( CButton* pCbuttn, CWnd* pParet, CRect* pRect, CRect* pRect2)
{
	CRect RectUI;
	pParet->GetWindowRect(RectUI);
	pParet->GetClientRect( RectUI );

	
	pCbuttn->GetWindowRect( pRect );
	pParet->ScreenToClient( pRect );


	int top=pRect->top;
	int bottom=pRect->bottom;
	int left=pRect->left;
	int right=pRect->right;
	double newheight=((double)(RectUI.bottom))/(pRect2->bottom);
	double newwidth=((double)(RectUI.right))/(pRect2->right);
	
	int newtop=(int)(top*newheight+0.5);
	int newbottom=(int)(bottom*newheight+0.5);

	int newleft=(int)(left*newwidth+0.5);
	int newright=(int)(right*newwidth+0.5);

	pCbuttn->MoveWindow(newleft,newtop, newright-newleft,newbottom-newtop);
	pCbuttn->GetWindowRect( pRect );
	pParet->ScreenToClient( pRect );
}

void hcutil::reposstatic( CStatic* pStatic, CWnd* pParet, CRect* pRect, CRect* pRect2)
{
	CRect RectUI;
	pParet->GetWindowRect(RectUI);
	pParet->GetClientRect( RectUI );

	
	pStatic->GetWindowRect( pRect );
	pParet->ScreenToClient( pRect );


	int top=pRect->top;
	int bottom=pRect->bottom;
	int left=pRect->left;
	int right=pRect->right;
	double newheight=((double)(RectUI.bottom))/(pRect2->bottom);
	double newwidth=((double)(RectUI.right))/(pRect2->right);
	
	int newtop=(int)(top*newheight+0.5);
	int newbottom=(int)(bottom*newheight+0.5);

	int newleft=(int)(left*newwidth+0.5);
	int newright=(int)(right*newwidth+0.5);

	pStatic->MoveWindow(newleft,newtop, newright-newleft,newbottom-newtop);
	//pStatic->MoveWindow(18,176, 177-18,196-176);
	pStatic->GetWindowRect( pRect );
	pParet->ScreenToClient( pRect );
}

void hcutil::reposlist( CListBox* pList, CWnd* pParet, CRect* pRect, CRect* pRect2)
{
	CRect RectUI;
	pParet->GetWindowRect(RectUI);
	pParet->GetClientRect( RectUI );

	
	pList->GetWindowRect( pRect );
	pParet->ScreenToClient( pRect );


	int top=pRect->top;
	int bottom=pRect->bottom;
	int left=pRect->left;
	int right=pRect->right;
	double newheight=((double)(RectUI.bottom))/(pRect2->bottom);
	double newwidth=((double)(RectUI.right))/(pRect2->right);
	
	int newtop=(int)(top*newheight+0.5);
	int newbottom=(int)(bottom*newheight+0.5);

	int newleft=(int)(left*newwidth+0.5);
	int newright=(int)(right*newwidth+0.5);

//y = (int)(x + 0.5);

	pList->MoveWindow(newleft,newtop, newright-newleft,newbottom-newtop);
	//pStatic->MoveWindow(18,176, 177-18,196-176);
	pList->GetWindowRect( pRect );
	pParet->ScreenToClient( pRect );
}

void hcutil::reposedit( CEdit * pEdit, CWnd* pParet, CRect* pRect, CRect* pRect2)
{
	CRect RectUI;
	pParet->GetWindowRect(RectUI);
	pParet->GetClientRect( RectUI );

	
	pEdit->GetWindowRect( pRect );
	pParet->ScreenToClient( pRect );


	int top=pRect->top;
	int bottom=pRect->bottom;
	int left=pRect->left;
	int right=pRect->right;
	double newheight=((double)(RectUI.bottom))/(pRect2->bottom);
	double newwidth=((double)(RectUI.right))/(pRect2->right);
	
	int newtop=(int)(top*newheight+0.5);
	int newbottom=(int)(bottom*newheight+0.5);

	int newleft=(int)(left*newwidth+0.5);
	int newright=(int)(right*newwidth+0.5);

//y = (int)(x + 0.5);

	pEdit->MoveWindow(newleft,newtop, newright-newleft,newbottom-newtop);
	//pStatic->MoveWindow(18,176, 177-18,196-176);
	pEdit->GetWindowRect( pRect );
	pParet->ScreenToClient( pRect );
}

void hcutil:: reposcombo( CComboBox * pCombo, CWnd* pParet, CRect* pRect, CRect* pRect2)
{

	CRect RectUI;
	pParet->GetWindowRect(RectUI);
	pParet->GetClientRect( RectUI );

	
	pCombo->GetWindowRect( pRect );
	pParet->ScreenToClient( pRect );


	int top=pRect->top;
	int bottom=pRect->bottom;
	int left=pRect->left;
	int right=pRect->right;
	double newheight=((double)(RectUI.bottom))/(pRect2->bottom);
	double newwidth=((double)(RectUI.right))/(pRect2->right);
	
	int newtop=(int)(top*newheight+0.5);
	int newbottom=(int)(bottom*newheight+0.5);

	int newleft=(int)(left*newwidth+0.5);
	int newright=(int)(right*newwidth+0.5);

//y = (int)(x + 0.5);

	pCombo->MoveWindow(newleft,newtop, newright-newleft,newbottom-newtop);
	//pStatic->MoveWindow(18,176, 177-18,196-176);
	pCombo->GetWindowRect( pRect );
	pParet->ScreenToClient( pRect );

}

/** 
 * 문자열을 숫자 값으로 변환한다 
 */
int hcutil::ToInt( TCHAR *pValue, BOOL isAscii )
{
	int ret = 0;

	ret = isAscii ? atoi((const char *)pValue) : _ttoi(pValue);

	return ret;
}

double hcutil::ToDouble( wchar_t *pValue, BOOL isAscii )
{
	char	szAscii[64];
	size_t	numberOfConverts = 0;
	double	ret = 0.0;

	if( isAscii ) {
		ret = atof((const char *)pValue);
	} else {
		memset((void*)szAscii, 0, sizeof(char)*64);
		wcstombs_s( &numberOfConverts, szAscii, 64, pValue, _TRUNCATE );
	//	ret = atof((const char *)pValue);
		ret = atof((const char *)szAscii );
	}

	return ret;
}
