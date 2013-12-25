#pragma once
#include "constdef.h"

class MSDOSStub
{
public:
	MSDOSStub():m_peheader_off(0)
	{

	}

	BOOL Init(PCHAR base,long size)
	{
		assert(size>=0x3c+2,L"�ļ����Ȳ���(>=0x3c+2),�޷���ʼ��MSDOSStub",FALSE);
		m_peheader_off=*(PDWORD(base+0x3c));
		return TRUE;
	}

	void Print()
	{
		wprintf(L"---------------MSDOSStub--------------\n");
		wprintf(L"peͷƫ��: 0x%x\n",m_peheader_off);
	}

	DWORD m_peheader_off;
};
