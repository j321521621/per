// per.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PE.h"



int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),HWIHTE);
	setlocale( LC_ALL,"chs");

	if(argc<2)
	{
		printf("È±ÉÙ²ÎÊý\n");
		exit(1);
	}
	PEFile pe;
	pe.Init(argv[1]);
	pe.Print();
	char s=pe[0];
	return 0;
}