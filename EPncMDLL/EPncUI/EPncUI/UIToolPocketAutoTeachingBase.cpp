#include "StdAfx.h"
#include "UIToolPocketAutoTeachingBase.h"

pa::CUIToolPocketAutoTeachingBase::CUIToolPocketAutoTeachingBase(void)
{
	pShMem_ = NULL;
}

pa::CUIToolPocketAutoTeachingBase::~CUIToolPocketAutoTeachingBase(void)
{
}

BOOL pa::CUIToolPocketAutoTeachingBase::Initialize(CString strFilePath)
{
	strFilePath_ = strFilePath;

	create_shared_mem_object();

	LoadParamsData();

	return TRUE;
}

void pa::CUIToolPocketAutoTeachingBase::Destroy()
{
	if (pShMem_)
	{
		delete pShMem_;
		pShMem_ = NULL;
		pUIToolPocketAutoTeachingParam_ = NULL;
	}
}

BOOL pa::CUIToolPocketAutoTeachingBase::create_shared_mem_object()
{
	pShMem_ = new hcipc::CSharedMem();

	if (pShMem_ == NULL) 
	{
		return FALSE;
	}

	CString strObjectName = pa::GET_OBJECT_NAME_WITH_TAG(AUTOTEACH_TOOLPOCKET_PARAM_OBJECT_NAME);

	pUIToolPocketAutoTeachingParam_ = (SUIToolPocketAutoTeachingParam*)pShMem_->Create(
		NULL, 
		//_T("TOOL_POCKET"), 
		(TCHAR*)(LPCTSTR)strObjectName, 
		sizeof(SUIToolPocketAutoTeachingParam), 
		FALSE, // TRUE
		0);

	if (pUIToolPocketAutoTeachingParam_ == NULL)
	{
		return FALSE;
	}

	return TRUE;	
}
