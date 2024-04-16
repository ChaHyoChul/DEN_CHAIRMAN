#pragma once

namespace pa
{
//////////////////////////////////////////////////////////////////////////

class CGCodeHelper
{
private:
	//////////////////////////////////////////////////////////////////////////
	// Replace Command 
	static int		nNumReplaceCommand;				// 교환 명령 개수 
	static char		szSourceCommand[32][32];		// 원본 명령 
	static char		szReplaceCommand[32][32];		// 교환 명령 
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Check Command 
	static int		nNumCheckCommand;
	static char		szCheckCommands[32];			// 검사할 명령 코드 
	static double	fCheckCommandData[2][32];		// 검사할 명령의 데이터 상/하한 
	//////////////////////////////////////////////////////////////////////////

public:

public:
	/** 
	 * Replace 명령을 파일에서 읽는다 
	 */
	static BOOL LoadReplaceCommand( CString strConfigFilePath, CString& strErrMsg );

	/** 
	 * Check Command 명령을 파일에서 읽는다 
	 */
	static BOOL LoadCheckCommand( CString strConfigFilePath, CString& strErrMsg );

	/** 
	 * GCode 에서 주석문을 제거 한다 
	 */
	static void RemoveCommentFromGCode( char* pGCode );

	static void RemoveCommantFromGCode2( char* pGCode, char* char_for_command[], int command_count );

	/** 
	 * GCode에 M30 명령이 있는지 확인한다 
	 */
	static BOOL CheckM30_Stop( char* pGCode );

	/** 
	 * GCode에 M47 명령이 있는지 확인한다 
	 */
	static BOOL CheckM47_Repeat( char* pGCode );

	/** 
	 * GCode에 M140-M147까지 툴 체인지 명령이 있느면, Tool의 사용율을 확인한다 
	 */
	static BOOL CheckM140_M147_ToolChange( char* pNCCode, int* pErrorToolNo, int *pnToolNo );

	/** 
	 * 
	 */
	static int CheckM140_M145_Restart(int nToolNo );

	/** 
	 * GCode에 명령 교환
	 */
	static void CheckReplaceCommand( char* pGCode );

	/** 
	 * Nc File Check 함수 
	 */
	static BOOL CheckNcFile( char* pNCCode, CString& refErrorMsg );

};

//////////////////////////////////////////////////////////////////////////
}

