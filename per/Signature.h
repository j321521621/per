#pragma once
#include "constdef.h"

class Signature
{
public:
	Signature():m_sig()
	{

	}
	BOOL Init(PCHAR base,DWORD size)
	{
		assert(size>=4,L"�ļ����Ȳ���(>=4)�޷���ʼ��Signature",FALSE);
		m_sig[0]=base[0];
		m_sig[1]=base[1];
		m_sig[2]=base[2];
		m_sig[3]=base[3];
		assert(m_sig[0]=='P' && m_sig[1]=='E'&& m_sig[2]==0 && m_sig[3]==0,L"�Ƿ���image�ļ�,SignatureУ��ʧ��",FALSE);
		return TRUE;
	}

	void Print()
	{
		wprintf(L"---------------Signature--------------\n");
		wprintf(L"PE��־: PE00\n");
	}

	UCHAR m_sig[4];
};
