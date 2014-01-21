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
		assert(size>=4,L"文件长度不够(>=4)无法初始化Signature",FALSE);
		m_sig[0]=base[0];
		m_sig[1]=base[1];
		m_sig[2]=base[2];
		m_sig[3]=base[3];
		assert(m_sig[0]=='P' && m_sig[1]=='E'&& m_sig[2]==0 && m_sig[3]==0,L"非法的image文件,Signature校验失败",FALSE);
		return TRUE;
	}

	void Print()
	{
		wprintf(L"---------------Signature--------------\n");
		wprintf(L"PE标志: PE00\n");
	}

	UCHAR m_sig[4];
};
