#pragma once

namespace hcutil
{
//////////////////////////////////////////////////////////////////////////

/** 
 * Ascii 문자열을 Unicode 문자열로 변경 
 */ 
void ASCII_TO_UNICODE( char* pAscii, wchar_t* pUnicode, int nBufferLen );

/** 
 * Ascii 문자열을 CString 문자열로 변경 
 */ 
CString ASCII_TO_CSTRING( char* pAscii );

/** 
 * Unicode 문자열을 Ascii 문자열로 변경 
 */
void UNICODE_TO_ASCII( wchar_t* pUnicode, char* pAscii, int nBufferLen );

/** 
 * Unicode 문자열을 Utf-8 문자열로 변환 
 */
void UNICODE_TO_UTF8( wchar_t* pUnicode, wchar_t* pUtf8, int nBufferLen );

/** 
 * Utf-8 문자열을 Unicode 문자열로 변환
 */
CString UTF8_TO_CSTRING( LPCSTR pUtf8 );

/** 
 * CString 문자열을 Ascii 문자열로 변경 
 */
void CSTRING_TO_ASCII( CString& str, char* pAscii, int nBufferLen );

/** 
 * 파일의 유무를 확인해서 있으면 TREU 리턴 
 */
BOOL IsExistFile( CString& strFilePath );

BOOL IsExistDir( CString& strDir );

BOOL IsExistFile( TCHAR *pFilePath );

BOOL IsExistDir( TCHAR *pDir );

int GetFileNames( CString strFilesPath, CString fileNames[] );

/** 
 * 파일의 크기를 byte 단위로 리턴한다. 파일이 없으면 FALSE 
 */
BOOL GetFileSize( CString& strFilePath, DWORD* pdwRetFileSize, CString& strErrMsg );

/** 
 * 파일을 삭제 한다 
 */
BOOL DeleteFile( CString& strFilePath, CString& strErrMsg );

/** 
 * 시스템 에러 메시지를 리턴한다 
 */
void GET_SYSTEM_ERROR_MESSAGE( DWORD dwErrorCode, CString& strErrorMessage );

/** 
 * Dialog 리소스의 위치/크기를 알아 낸다 
 */
void GetControlPos( UINT nID, CWnd* pParet, CRect* pRect, BOOL bRemoveCtrl );

void GetControlPos2( UINT nID, CWnd* pParet, CRect* pRect, CRect* pRect2, BOOL bRemoveCtrl );

void reposbutton( CButton* pCbuttn, CWnd* pParet, CRect* pRect, CRect* pRect2);

void reposstatic( CStatic* pStatic, CWnd* pParet, CRect* pRect, CRect* pRect2);

void reposlist( CListBox* pList, CWnd* pParet, CRect* pRect, CRect* pRect2);

void reposedit( CEdit * pEdit, CWnd* pParet, CRect* pRect, CRect* pRect2);

void reposcombo( CComboBox * pCombo, CWnd* pParet, CRect* pRect, CRect* pRect2);

/** 
 * 문자열을 숫자 값으로 변환한다
 * Value가 Ascii 코드이면 bAscii==TRUE
 */
int ToInt( TCHAR *pValue, BOOL isAscii );
double ToDouble( wchar_t *pValue, BOOL isAscii );

//////////////////////////////////////////////////////////////////////////
}

