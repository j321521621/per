#pragma once
#include "constdef.h"
#include "RAVRender.h"

class ImageBaseRelocation
{
public:
	DWORD Init(PCHAR base,DWORD size)
	{
		assert(size>=8,L"大小不够",FALSE);
		m_Rav=*(DWORD*)(base);
		DWORD usedsz=*(DWORD*)(base+4);
		assert(size>=usedsz,L"大小不够",FALSE);
		m_Count=(usedsz-8)/2;

		return usedsz;
	}

	void Print()
	{
		printf("    0x%08x %d\n",m_Rav,m_Count);
	}

public:
	DWORD m_Rav;
	DWORD m_Count;

};

class Relocation
{
public:
	BOOL Init()
	{
		return TRUE;
	}

	BOOL Init(PCHAR base,DWORD size,RAVRender* render)
	{
		if(base==0)
		{
			return Init();
		}

		while(size>0)
		{
			ImageBaseRelocation ibr;
			DWORD usedsz=ibr.Init(base,size);
			m_ImageBaseRelocation.push_back(ibr);
			base+=usedsz;
			size-=usedsz;
		}

		return TRUE;
	}

	void Print()
	{
		wprintf(L"---------------重定位--------------\n");
		for(vector<ImageBaseRelocation>::iterator it=m_ImageBaseRelocation.begin();it!=m_ImageBaseRelocation.end();++it)
		{
			it->Print();
		}
	}

public:
	vector<ImageBaseRelocation> m_ImageBaseRelocation;
};