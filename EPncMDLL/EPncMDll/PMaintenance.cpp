#include "StdAfx.h"
#include "PMaintenance.h"

pa::CPMaintenance::CPMaintenance(void)
{
	pShMem_				= NULL;
	pMaintenanceData_	= NULL;
	pToolData_	= NULL;
}

pa::CPMaintenance::~CPMaintenance(void)
{
	Destroy();
}

BOOL pa::CPMaintenance::Initialize( CString& strErrMsg )
{
	//////////////////////////////////////////////////////////////////////////
	// 파일이 있는지 확인
	CString strTemp = CString(IPC_FILEPATH) + CString(_T("\\SHM_")) + CString(MAINTENANCE_OBJECT_NAME);
	BOOL bReset = hcutil::IsExistFile( strTemp ) == TRUE ? FALSE : TRUE;
	//////////////////////////////////////////////////////////////////////////

	pShMem_ = new hcipc::CSharedMem();
	if( pShMem_ == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pa::CPMaintenance::pShMem_ object") );
		return FALSE;
	}
	CString strObjectName = pa::GET_OBJECT_NAME_WITH_TAG(MAINTENANCE_OBJECT_NAME);
	pMaintenanceData_ = (SMaintenanceData*)pShMem_->Create( IPC_FILEPATH, (TCHAR*)(LPCTSTR)strObjectName, sizeof(SMaintenanceData), bReset, 0 );
	if( pMaintenanceData_ == NULL ) {
		strErrMsg.Format( _T("create shared memory error for pa::CPMaintenance::pMaintenanceData_ object") );
		return FALSE;
	}

	if( bReset == TRUE ) {
		pShMem_->Flush();
	}

	return TRUE;
}

void pa::CPMaintenance::Destroy()
{
	if( pShMem_ ) {
		delete pShMem_;
		pShMem_ = NULL;
		pMaintenanceData_ = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// LAST MAINTENANCE

void pa::CPMaintenance::SetLastMaintenance( int ID )
{
	// get current date
	pMaintenanceData_->tmLastMaintenance[ID] = CTime::GetCurrentTime(); //current timestamp;
	
	SaveAll();
}

CTime pa::CPMaintenance::GetLastMaintenance( int ID )
{
	return pMaintenanceData_->tmLastMaintenance[ID]; //last timestamp;
}

//////////////////////////////////////////////////////////////////////////
// MAXIMUM TIME

void pa::CPMaintenance::SetMaximumTime( int ID, DWORD dwMaximumTime )
{
	if(dwMaximumTime >= 0 && ID >= 0 && ID < MAINTENANCE_NUM){
		pMaintenanceData_->dwMaximumTime[ID] = dwMaximumTime;
	}

	SaveAll();
}

DWORD pa::CPMaintenance::GetMaximumTime( int ID )
{
	return pMaintenanceData_->dwMaximumTime[ID];
}

//////////////////////////////////////////////////////////////////////////
// ENABLE CHECKING

void pa::CPMaintenance::SetEnableChecking( int ID, BOOL bEnable )
{
	pMaintenanceData_->bEnableChecking[ID] = bEnable;
	SaveAll();
//		pShMem_->Flush( (void*)&(pMaintenanceData_->bEnableToolUsageTime), sizeof(BOOL) );
}

BOOL pa::CPMaintenance::GetEnableChecking( int ID )
{
	return pMaintenanceData_->bEnableChecking[ID];
}

//////////////////////////////////////////////////////////////////////////

void pa::CPMaintenance::SaveAll()
{
	pShMem_->Flush( (void*)&(pMaintenanceData_), sizeof(SMaintenanceData) );
}
