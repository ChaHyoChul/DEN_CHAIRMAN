#pragma once

/**
 * ex1>
 *	10x10 데이터 (모두 CString) 를 저장하는데 약 5초 
 *	10x10 데이터 (모두 CString) 를 읽는데 약 1.5초 
 * ex2>
 *	XYZ 티칭 데이터 10개 저장하는데 약 0.35초 
 *  XYZ 티칭 데이터 10개 읽는데 약 0.05초 
 *
 * ex3>
 *	XYX 티칭 데이터중 한개의 값을 수정 할 경우 0.015초 
 *
 * 쓸만 함 
 */

class CCEIniFile
{
private:
	CString strFilePath_;

public:
	BOOL Open( LPCTSTR strFilePath );	// 파일 경로를 저장 한다
	void Close( void );

	BOOL GetValue( CString strKeyName, CString strValueName, LPTSTR ret, int len );
	BOOL GetValue( CString strKeyName, CString strValueName, CString* ret );
	BOOL GetValue( CString strKeyName, CString strValueName, int* ret  );
	BOOL GetValue( CString strKeyName, CString strValueName, double* ret );

	BOOL SetValue( CString strKeyName, CString strValueName, CString val );
	BOOL SetValue( CString strKeyName, CString strValueName, int val  );
	BOOL SetValue( CString strKeyName, CString strValueName, double val );

public:
	CCEIniFile( void );
	~CCEIniFile( void );

	static DWORD GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName);
	static BOOL WritePrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName);
};

