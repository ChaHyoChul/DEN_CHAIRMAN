#include "StdAfx.h"
#include "PTool.h"

pa::CPTool::CPTool(void)
{
	pShMem_		= NULL;
	pToolData_	= NULL;
}

pa::CPTool::~CPTool(void)
{
	Destroy();
}

BOOL pa::CPTool::Initialize( CString& strErrMsg )
{
	//////////////////////////////////////////////////////////////////////////
	// 파일이 있는지 확인
	CString strTemp = CString(IPC_FILEPATH) + CString(_T("\\SHM_")) + CString(TOOL_OBJECT_NAME);
	BOOL bReset = hcutil::IsExistFile( strTemp ) == TRUE ? FALSE : TRUE;
	//////////////////////////////////////////////////////////////////////////

	pShMem_ = new hcipc::CSharedMem();
	if( pShMem_ == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pa::CPTool::pShMem_ object") );
		return FALSE;
	}
// 	pToolData_ = (SToolData*)pShMem_->Create( IPC_FILEPATH, TOOL_OBJECT_NAME, sizeof(SToolData), FALSE, 0 );
	pToolData_ = (SToolData*)pShMem_->Create( IPC_FILEPATH, TOOL_OBJECT_NAME, sizeof(SToolData), bReset, 0 );
	if( pToolData_ == NULL ) {
		strErrMsg.Format( _T("create shared memory error for pa::CPTool::pToolData_ object") );
		return FALSE;
	}
	
	if( bReset == TRUE ) {
		pShMem_->Flush();
	}

	// ALWAYS CHECK FOR TOOL USAGE TIME
	if( pToolData_->bEnableToolUsageTime != TRUE ) {
		pToolData_->bEnableToolUsageTime = TRUE;
		pShMem_->Flush( (void*)&(pToolData_->bEnableToolUsageTime), sizeof(BOOL) );
	}

	return TRUE;
}

void pa::CPTool::Destroy()
{
	if( pShMem_ ) {
		delete pShMem_;
		pShMem_ = NULL;
		pToolData_ = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////

void pa::CPTool::IncToolUsingTime( int nToolNo, DWORD dwTime )
{
	pToolData_->hTool[nToolNo].dwUsingTime += dwTime;
	if( pToolData_->hTool[nToolNo].dwMaximumTime > 0 ) {
		pToolData_->hTool[nToolNo].fUsingRate = 
			( (double)pToolData_->hTool[nToolNo].dwUsingTime / (double)pToolData_->hTool[nToolNo].dwMaximumTime ) * 100.0;
	} else {
		pToolData_->hTool[nToolNo].fUsingRate = 0;
	}
}

void pa::CPTool::ResetToolUsingTime( int nToolNo )
{
	pToolData_->hTool[nToolNo].dwUsingTime		= 0;
	//pToolData_->hTool[nToolNo].dwMaximumTime	= dwMaximumTime;
	pToolData_->hTool[nToolNo].fUsingRate		= 0.0;
	pToolData_->hTool[nToolNo].dwErrCode		= 0;


	// 2015.04.21 
	//PTool->SaveToolUsingTime( nToolNo );
	// SaveToolUsingTime( nToolNo );
	SaveToolUsingTimeAll();
}

// set maximum time
void pa::CPTool::SetToolMaximumTime( int nToolNo, DWORD dwMaximumTime )
{
	DWORD ERR_TOOL_OVERTIME = 105;
	
	// set maximum time
	pToolData_->hTool[nToolNo].dwMaximumTime = dwMaximumTime;
	
	// update using rate
	if( pToolData_->hTool[nToolNo].dwMaximumTime > 0)
	{
		pToolData_->hTool[nToolNo].fUsingRate = pToolData_->hTool[nToolNo].dwUsingTime * 100 / pToolData_->hTool[nToolNo].dwMaximumTime;
	}
	else
	{
		pToolData_->hTool[nToolNo].fUsingRate = 0.0;
	}
	
	// check and throw ERR_TOOL_OVERTIME if time reached 
	if(pToolData_->hTool[nToolNo].dwErrCode == 0 && GetEnableToolUsageTime() && pToolData_->hTool[nToolNo].dwUsingTime >= pToolData_->hTool[nToolNo].dwMaximumTime){
		pToolData_->hTool[nToolNo].dwErrCode = ERR_TOOL_OVERTIME;
	}
	
	// check and remove ERR_TOOL_OVERTIME if time ok 
	if(pToolData_->hTool[nToolNo].dwErrCode == ERR_TOOL_OVERTIME && pToolData_->hTool[nToolNo].dwUsingTime < pToolData_->hTool[nToolNo].dwMaximumTime){
		pToolData_->hTool[nToolNo].dwErrCode = 0;
	}

	SaveToolUsingTimeAll();
}

// set actual - doesnt reset all errors!
void pa::CPTool::SetToolUsingTime( int nToolNo, DWORD dwUsingTime )
{
	DWORD ERR_TOOL_OVERTIME = 105;
	
	// update tool using time
	pToolData_->hTool[nToolNo].dwUsingTime	= dwUsingTime;

	// update using rate
	if( pToolData_->hTool[nToolNo].dwMaximumTime > 0)
	{
		pToolData_->hTool[nToolNo].fUsingRate = pToolData_->hTool[nToolNo].dwUsingTime * 100 / pToolData_->hTool[nToolNo].dwMaximumTime;
	}
	else
	{
		pToolData_->hTool[nToolNo].fUsingRate = 0.0;
	}
	
	// check and remove ERR_TOOL_OVERTIME if time ok 
	if(pToolData_->hTool[nToolNo].dwErrCode == ERR_TOOL_OVERTIME && pToolData_->hTool[nToolNo].dwUsingTime < pToolData_->hTool[nToolNo].dwMaximumTime){
		pToolData_->hTool[nToolNo].dwErrCode = 0;
	}
	
	SaveToolUsingTimeAll();
}

void pa::CPTool::SetToolErrCode( int nToolNo, DWORD dwErrCode )
{
	pToolData_->hTool[nToolNo].dwErrCode = dwErrCode;
}

// 해당 툴의 사용시간 정보를 저장 (Flush 명령 사용)
void pa::CPTool::SaveToolUsingTime( int nToolNo )
{
	pShMem_->Flush( (void*)&(pToolData_->hTool[nToolNo]), sizeof(STool) );
}

void pa::CPTool::SaveToolUsingTimeAll()
{
	pShMem_->Flush( (void*)&(pToolData_->hTool), sizeof(STool)*MAX_TOOL_NUM );
}

//////////////////////////////////////////////////////////////////////////

void pa::CPTool::SetEnableToolUsageTime( BOOL b )
{
	const DWORD ERR_TOOL_OVERTIME = 105;
	
	if( pToolData_->bEnableToolUsageTime != b ) {
		pToolData_->bEnableToolUsageTime = b;
		pShMem_->Flush( (void*)&(pToolData_->bEnableToolUsageTime), sizeof(BOOL) );
	}
	
	// clear overtime error if needed
	UpdateToolOvertimeError();
}

BOOL pa::CPTool::GetEnableToolUsageTime()
{
	return pToolData_->bEnableToolUsageTime;
}

void pa::CPTool::SetEnableRelatedTool( BOOL b )
{
	if( pToolData_->bEnableRelatedTool != b ) {
		pToolData_->bEnableRelatedTool = b;
		pShMem_->Flush( (void*)&(pToolData_->bEnableRelatedTool), sizeof(BOOL) );
	}
}

BOOL pa::CPTool::GetEnableRelatedTool()
{
	return pToolData_->bEnableRelatedTool;
}

void pa::CPTool::SetTimeCountZAxisPos( double fZAxisPos )
{
	pToolData_->fTimeCountZAxisPos = fZAxisPos;
	pShMem_->Flush( (void*)&(pToolData_->fTimeCountZAxisPos), sizeof(double) );
}

double pa::CPTool::GetTimeCountZAxisPos()
{
	return pToolData_->fTimeCountZAxisPos;
}

void pa::CPTool::UpdateToolOvertimeError()
{
	const DWORD ERR_TOOL_OVERTIME = 105;
	for(int i=1; i <= 6; i++)	// chairman has 6 tools
	{
		if(pToolData_->bEnableToolUsageTime)
		{
			if(pToolData_->hTool[i].dwErrCode == 0 && pToolData_->hTool[i].dwUsingTime >= pToolData_->hTool[i].dwMaximumTime){
				pToolData_->hTool[i].dwErrCode = ERR_TOOL_OVERTIME;
			}		
		}
		else
		{
			if(pToolData_->hTool[i].dwErrCode == ERR_TOOL_OVERTIME){
				pToolData_->hTool[i].dwErrCode = 0;
			}	
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 연관툴 관련 함수 
//////////////////////////////////////////////////////////////////////////

int  pa::CPTool::GetNumRelatedToolData()
{
	return pToolData_->nNumDataForRelatedTool;
}

// M140+M141+ 처럼. 맨 아래 추가  
BOOL pa::CPTool::AddRelatedToolData( const char* p )
{
	if( pToolData_->nNumDataForRelatedTool > MAX_TOOL_NUM ) {
		return FALSE;
	}

	memset( (void*)(pToolData_->szRelateTool[pToolData_->nNumDataForRelatedTool]), 0, sizeof(char)*64 );
	memcpy( (void*)(pToolData_->szRelateTool[pToolData_->nNumDataForRelatedTool]), p, strlen(p) );

	pToolData_->nNumDataForRelatedTool++;

	int len = sizeof(int) + ( sizeof(char) * (MAX_TOOL_NUM+1) * 64 );
	pShMem_->Flush( (void*)&(pToolData_->nNumDataForRelatedTool), len );

	return TRUE;
}

// nIndex 번째 데이터를 삭제 하고, 이 하 데이터를 하나씩 올린다 
void pa::CPTool::RemoveRelatedToolData( int nIndex )
{
	if( nIndex >= pToolData_->nNumDataForRelatedTool ) {
		return ;
	}

	int	len = ( pToolData_->nNumDataForRelatedTool - ( nIndex + 1 ) ) * 64;
	memcpy(	(void*)(pToolData_->szRelateTool[nIndex]), 
			(const void*)(pToolData_->szRelateTool[nIndex+1]), 
			sizeof(char)*len );

	pToolData_->nNumDataForRelatedTool--;

	len = sizeof(int) + ( sizeof(char) * (MAX_TOOL_NUM+1) * 64 );
	pShMem_->Flush( (void*)&(pToolData_->nNumDataForRelatedTool), len );
}

// nIndex 번째 데이터를 리턴한다 
void pa::CPTool::GetRelatedToolData( int nIndex, char* pRet, int len )
{
	if( nIndex < pToolData_->nNumDataForRelatedTool ) {
		memset( (void*)pRet, 0, sizeof(char)*len );
		memcpy( (void*)pRet, (const void*)(pToolData_->szRelateTool[nIndex]), sizeof(char)*strlen(pToolData_->szRelateTool[nIndex]) );
	}
}

// Tool 번호는 1부터 시작. 0은 툴이 없음 
int pa::CPTool::GetNextRelatedToolNo( int nToolNo )
{
	int		index;
	int		toolNo = 0;
	char	szTemp[32];
	BOOL	bFind = FALSE;

	memset((void*)szTemp, 0, sizeof(char)*32);
	sprintf_s( szTemp, 32, "M%d", 140+nToolNo-1 );

	int nNumRelatedToolData = pa::PTool->GetNumRelatedToolData();
	for( index = 0; index<nNumRelatedToolData; index++ )
	{
		if( pToolData_->szRelateTool[index][0]==szTemp[0] &&
			pToolData_->szRelateTool[index][1]==szTemp[1] &&
			pToolData_->szRelateTool[index][2]==szTemp[2] &&
			pToolData_->szRelateTool[index][3]==szTemp[3] )
		{
			bFind = TRUE;
			break;
		}
	}

	if( bFind == FALSE ) {
		// 연관 툴이 없음 
		return 0;
	}
	
	// index 번째 데이터에서 다음 툴 번호를 찾는다 
	int len = strlen( pToolData_->szRelateTool[index] );
	int t = 6;

	if( len > t ) {
		while( TRUE ) {
			memset((void*)szTemp, 0, sizeof(char)*32);
			szTemp[0] = pToolData_->szRelateTool[index][t++];
			szTemp[1] = pToolData_->szRelateTool[index][t++];
			szTemp[2] = pToolData_->szRelateTool[index][t++];

			toolNo = (int)atoi(szTemp) - 140 + 1;

			// toolNo에 해당하는 툴의 사용율을 확인 한다 
			if( pToolData_->hTool[toolNo].dwErrCode == 0 && 
				pToolData_->hTool[toolNo].fUsingRate < 99.0 ) {
					break;
			}

			// 다음 연관 툴을 확인한다 
			t += 2;
			if( t > len ) {
				toolNo = 0;
				break;
			}
		}
	}
	else {
		toolNo = 0;
	}

	return toolNo;
}

BOOL pa::CPTool::IsOverTime( int nToolNo )
{
	if( GetEnableToolUsageTime() && (pToolData_->hTool[nToolNo].dwUsingTime > pToolData_->hTool[nToolNo].dwMaximumTime) ) {
		return TRUE;
	}
	
	return FALSE;
}

BOOL pa::CPTool::IsPickable( int nToolNo )
{
	DWORD ERR_TOOL_OVERTIME = 105;
	
	if( pToolData_->hTool[nToolNo].dwErrCode == 0 || pToolData_->hTool[nToolNo].dwErrCode == ERR_TOOL_OVERTIME ) {
		return TRUE;
	}
	
	return FALSE;
}
