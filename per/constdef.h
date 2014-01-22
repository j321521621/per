#pragma once
#include <windows.h>
#include <atlstr.h>

#define HWIHTE 15
#define DWIHTE 7

enum PETYPE
{
	PETYPE_EXCUTE,
	PETYPE_OBJECT
};

void assert(BOOL ok,CString info=L"assert错误",BOOL winerr=FALSE)
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
		::MessageBox(NULL,info,L"assert",MB_OK);
		exit(err);
	}
}



#define wprintfc(color,format,...) {\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);\
	wprintf(format,__VA_ARGS__);\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),HWIHTE);\
}

void subsize(DWORD &size,DWORD off)
{
	assert(size>=off,L"文件长度不够");
	size-=off;
}