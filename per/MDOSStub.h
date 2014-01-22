#pragma once
#include "constdef.h"

class MSDOSStub
{
public:
	MSDOSStub():m_peheader_off(0)
	{

	}

	DWORD Init(PCHAR base,DWORD size)
	{
		assert(size>=0x3c+4,L"�ļ����Ȳ���(>=0x3c+4),�޷���ʼ��MSDOSStub",FALSE);
		m_peheader_off=*(PDWORD(base+0x3c));
		return m_peheader_off;
	}

	void Print()
	{
		wprintf(L"---------------MSDOSStub--------------\n");
		wprintf(L"peͷƫ��: [%8x]\n",m_peheader_off);
	}

	DWORD m_peheader_off;
};