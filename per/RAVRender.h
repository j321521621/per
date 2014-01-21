#pragma once

typedef void (*CbWhenVisit)(PBYTE base,DWORD size);

class RAVVisitor
{
public:
	virtual BOOL VisitRav(DWORD rav,DWORD size,PCHAR base,LPVOID data)=0;
};

class RAVRender
{
public:
	virtual PCHAR RavToImage(DWORD rav,DWORD size)=0;
	virtual string RavToString(DWORD rav)=0;
};