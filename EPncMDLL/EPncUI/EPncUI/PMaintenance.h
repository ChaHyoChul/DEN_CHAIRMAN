#pragma once
#include <ctime> 

namespace pa 
{
//////////////////////////////////////////////////////////////////////////

class CPMaintenance
{
private:
	hcipc::CSharedMem*	pShMem_;
	SMaintenanceData*	pMaintenanceData_;
	SToolData*			pToolData_;

public:
	BOOL Initialize( CString& strErrMsg );
	void Destroy();

	void SetLastMaintenance( int ID );							// 
	CTime GetLastMaintenance( int ID );							// 

	void SetMaximumTime( int ID, DWORD dwMaximumTime );			// 
	DWORD GetMaximumTime( int ID );								// 

	void SetEnableChecking( int ID, BOOL b );
	BOOL GetEnableChecking( int ID);

	void SaveAll();

	
	SMaintenanceData* GetMaintenanceData() {
		return pMaintenanceData_;
	}
	
	STool* GetToolData( int nToolNo ) {
		return &(pToolData_->hTool[nToolNo]);
	}

public:
	CPMaintenance(void);
	~CPMaintenance(void);
};

//////////////////////////////////////////////////////////////////////////
}

