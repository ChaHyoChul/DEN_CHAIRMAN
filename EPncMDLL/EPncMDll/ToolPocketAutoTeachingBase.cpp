#include "StdAfx.h"
#include "ToolPocketAutoTeachingBase.h"

pa::CToolPocketAutoTeachingBase::CToolPocketAutoTeachingBase(void)
{
	pShMem_ = NULL;
}

pa::CToolPocketAutoTeachingBase::~CToolPocketAutoTeachingBase(void)
{
}

BOOL pa::CToolPocketAutoTeachingBase::create_shared_mem_object()
{
	pShMem_ = new hcipc::CSharedMem();
	if (pShMem_ == NULL) 
	{
		return FALSE;
	}

	CString strObjectName = pa::GET_OBJECT_NAME_WITH_TAG(AUTOTEACH_TOOLPOCKET_PARAM_OBJECT_NAME);

	pToolPocketAutoTeachingParam_ = (SToolPocketAutoTeachingParam*)pShMem_->Create(
		NULL, 
		(TCHAR*)(LPCTSTR)strObjectName, 
		sizeof(SToolPocketAutoTeachingParam), 
		TRUE, 
		0);

	if (pToolPocketAutoTeachingParam_ == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL pa::CToolPocketAutoTeachingBase::Initialize(CString strFilePath, CString& strErrMsg)
{
	if (create_shared_mem_object() == FALSE)
	{
		strErrMsg.Format(_T("create shared memory error for tool pocket auto teaching"));
		return FALSE;
	}

	return LoadParams((TCHAR*)(LPCTSTR)strFilePath, strErrMsg);
}

void pa::CToolPocketAutoTeachingBase::Destroy()
{
	if (pShMem_)
	{
		delete pShMem_;
		pShMem_ = NULL;
		pToolPocketAutoTeachingParam_ = NULL;
	}
}

void pa::CToolPocketAutoTeachingBase::writeLog(LPCTSTR logMsg, BOOL bExt)
{
	if (!bExt)
	{
		P_LOG->WriteLog(CLog::TYPE_OPER, 99, logMsg);
	
	}
	else 
	{
		P_LOG->WriteLog_EXT(logMsg);
	}
}
