// RaceConditionTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "time.h"

#include "Header.h"




HANDLE hSubDir = 0;




int _tmain(int argc, _TCHAR* argv[])
{
	srand( time(NULL) );

	ulong PrefProc = 0;
	if(!SetThreadAffinityMask(GetCurrentThread(),(ulong)(1 << PrefProc)))
	{
		printf("Warning: Error setting affinity mask\r\n");
		return -1;
	}
	if(SetThreadIdealProcessor(GetCurrentThread(),PrefProc) == -1)
	{
		printf("Warning: Error setting ideal processor, Err: %X\r\n", GetLastError());
	}

	SprayLinkTargets();
	Sleep(-1);
	return 0;
}



