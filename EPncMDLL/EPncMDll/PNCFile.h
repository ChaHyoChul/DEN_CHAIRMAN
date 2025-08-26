#pragma once

/** 
 * NC 파일을 한 라인씩 저장하고 있다 
 */

namespace pa
{
//////////////////////////////////////////////////////////////////////////

#define MAX_NCFILE_LINES	(10000000)		// maximum line 개수를 천만 라인으로 설정 
// #define MAX_NCFILE_LINES	(110000)		// maximum line 개수를 천만 라인으로 설정 
// #define MAX_NCFILE_LINES	(2000)
#define ETX_STR				("\r\n")
#define ETX_STR_LEN			(2)

class CPNCFile
{
private:
	// 이 부분은 GUI 프로그램과 공유하지 않는다 
	hcipc::CSharedMem*	pShMem_;			// NC 파일을 MMF로 연다 
	char*	pFile_;							// 파일 데이터의 포인터 (공유 메모리의 포인터)
	char*	pLines_[MAX_NCFILE_LINES];		// 파일 각 라인의 포인터 저장. 100만 라인까지 지원 

	int		nNumTotalLines_;				// NC 파일의 라인 개수 
	int		nWorkLine_;						// 현재 라인 번호 (0-base)

public:
	BOOL Open( CString& strNcFilePath, CString& strErrMsg );
	BOOL Open2( CString& strNcFilePath, CString& strErrMsg );
	BOOL Open3( CString& strNcPath, CString& strNcFileName, CString& strErrMsg );
	void Close();
	BOOL IsOpen();

	int  GetNumTotalLines() {
		int n = nNumTotalLines_;
		return n; 
	}
	
	int  GetWorkLine() {
		int n = nWorkLine_;	
		return n; 
	}
	
	void ResetWorkLine() { nWorkLine_ = 0; }
	
	void SetWorkLine( int nWorkLine ) { nWorkLine_ = nWorkLine; }

	BOOL GetLine( int index, BOOL bSetWorkLine, char* pNCCode );	// index 번째 NCCode를 리턴 한다. 문자열은 NULL로 끝난다  

	BOOL GetNextLine( char* pNCCode );
	
	double GetFirstX( );

	BOOL GetBlockLength(double* fBlockLength);		// (LENGTH=) 값을 리턴한다. 찾으면 True, 없으면 False를 리턴한다 
	BOOL GetBlockCheckYPosition(double* fYPos);		// 첫번째 (G01)의 Y 값을 찾아, 17<Y0<37=27, -37<Y0<-17=-27, 나머지=0

public:
	CPNCFile(void);
	~CPNCFile(void);
};

//////////////////////////////////////////////////////////////////////////
}

