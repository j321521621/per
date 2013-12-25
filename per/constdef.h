#pragma once
#include <windows.h>
#include <atlstr.h>

#define HWIHTE 15
#define DWIHTE 7

enum PETYPE
{
	PETYPE_UNKNOWN,
	PETYPE_IMAGE,
	PETYPE_OBJECT
};

void assert(BOOL ok,CString info=L"",BOOL winerr=TRUE)
{
	if(!ok)
	{
		DWORD err=321521621;
		if(winerr)
		{
			err=GetLastError();
			LPWSTR buff=L"";
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,NULL,err,NULL,LPWSTR(&buff),0,NULL);
			info.Format(L"%s %s(%d)",info,buff,err);
		}
		wprintf(L"%s\n",info);
		exit(err);
	}
}



#define wprintfc(color,format,...) {\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);\
	wprintf(format,__VA_ARGS__);\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),HWIHTE);\
}
