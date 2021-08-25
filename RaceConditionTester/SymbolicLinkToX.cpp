#include "stdafx.h"
#include "time.h"
#include "Header.h"


extern HANDLE hSubDir;


HANDLE AddSymbolicLinkToDirectory(HANDLE hDir,wchar_t* SymName,wchar_t* TargetName)
{
	if(!hDir) return 0;



	_UNICODE_STRING UniSS_slX = {0};

	if(SymName)
	{
		UniSS_slX.Length = wcslen(SymName)*2;
		UniSS_slX.MaxLength = UniSS_slX.Length + 2;
		UniSS_slX.Buffer = SymName;
	}

	_OBJECT_ATTRIBUTES ObjAttr_slX = {0};
	ObjAttr_slX.Length = sizeof(ObjAttr_slX);
	ObjAttr_slX.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttr_slX.ObjectName = & UniSS_slX;
	ObjAttr_slX.RootDirectory = hDir;
	

	_UNICODE_STRING uniTargetX = {0};
	if(TargetName)
	{
		uniTargetX.Length = wcslen(TargetName)*2;
		uniTargetX.MaxLength = uniTargetX.Length + 2;
		uniTargetX.Buffer = TargetName;
	}


	
	
	HANDLE hSymLinkX = 0;

	int retValue;
	retValue = ZwCreateSymbolicLinkObject(&hSymLinkX,
											SYMBOLIC_LINK_ALL_ACCESS,
											&ObjAttr_slX,
											&uniTargetX);

	printf("ZwCreateSymbolicLinkObject, ret: %X, hSymLink: %I64X\r\n",retValue,hSymLinkX);

	if(retValue >= 0)
	{
		return hSymLinkX;
	}

	return 0;
}


HANDLE AddSymbolicLinkToDirectory_same(HANDLE hDir,wchar_t* SymName,wchar_t* TargetName)
{
	if(!hDir) return 0;



	_UNICODE_STRING UniSS_slX = {0};

	if(SymName)
	{
		UniSS_slX.Length = wcslen(SymName)*2;
		UniSS_slX.MaxLength = UniSS_slX.Length + 2;
		UniSS_slX.Buffer = SymName;
	}

	_OBJECT_ATTRIBUTES ObjAttr_slX = {0};
	ObjAttr_slX.Length = sizeof(ObjAttr_slX);
	ObjAttr_slX.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttr_slX.ObjectName = & UniSS_slX;
	ObjAttr_slX.RootDirectory = hDir;
	

	_UNICODE_STRING uniTargetX = {0};
	if(TargetName)
	{
		uniTargetX.Length = wcslen(TargetName)*2;
		uniTargetX.MaxLength = uniTargetX.Length;
		uniTargetX.Buffer = TargetName;
	}


	
	
	HANDLE hSymLinkX = 0;

	int retValue;
	retValue = ZwCreateSymbolicLinkObject(&hSymLinkX,
											SYMBOLIC_LINK_ALL_ACCESS,
											&ObjAttr_slX,
											&uniTargetX);

	printf("ZwCreateSymbolicLinkObject, ret: %X, hSymLink: %I64X\r\n",retValue,hSymLinkX);

	if(retValue >= 0)
	{
		return hSymLinkX;
	}

	return 0;
}

