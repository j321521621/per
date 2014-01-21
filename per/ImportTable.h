#pragma once
#include "constdef.h"
#include "RAVRender.h"


class ImageImportByName
{
public:
	void Print()
	{
		printf("        0x%4x: %s\n",m_Hint,m_name.c_str());
	}

public:
	DWORD m_Hint;
	string m_name;
};

class ImageImportDescriptor
{
public:
	BOOL Init(PCHAR base,DWORD size,RAVRender* render)
	{
		assert(size==20,L"导入表描述符大小不是20,初始化失败",FALSE);
		m_OriginalFirstThunk=*((DWORD*)base+0);
		m_TimeDateStamp=*((DWORD*)base+1);
		m_ForwarderChain=*((DWORD*)base+2);
		DWORD name1=*((DWORD*)base+3);
		m_FirstThunk=*((DWORD*)base+4);
		for(int i=0;;i++)
		{
			PCHAR pch=render->RavToImage(name1+i,1);
			if(*pch==0)
			{
				break;
			}
			m_name.push_back(*pch);
		}
		for(int i=0;;i++)
		{
			PCHAR prntbase=render->RavToImage(m_OriginalFirstThunk+4*i,4);
			DWORD rntrav=*(DWORD*)prntbase;
			if(rntrav==0)
			{
				break;
			}
			if (rntrav&IMAGE_ORDINAL_FLAG32)
			{
				ImageImportByName iibn;
				iibn.m_Hint=rntrav&(~IMAGE_ORDINAL_FLAG32);
				iibn.m_name="";
				m_ImageImportByName.push_back(iibn);
				continue;
			}
			else
			{
				ImageImportByName iibn;
				iibn.m_Hint=*(WORD*)(render->RavToImage(rntrav,2));
				for(int j=0;;j++)
				{
					PCHAR pch=render->RavToImage(rntrav+2+j,1);
					if(*pch==0)
					{
						break;
					}
					iibn.m_name.push_back(*pch);
				}
				m_ImageImportByName.push_back(iibn);
			}
		}
		return true;
	}

	void Print()
	{
		printf("    %s\n",m_name.c_str());
		for(vector<ImageImportByName>::iterator it=m_ImageImportByName.begin();it!=m_ImageImportByName.end();++it)
		{
			it->Print();
		}
	}

public:
	DWORD m_OriginalFirstThunk;
	DWORD m_TimeDateStamp ;
	DWORD m_ForwarderChain;
	string m_name;
	DWORD m_FirstThunk;
	vector<ImageImportByName> m_ImageImportByName;
};


class ImportTable
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

		assert(size%20==0,L"导入表大小不是20的整倍数,导入表初始化失败",FALSE);

		for(int i=0;i<size/20-1;++i)
		{
			ImageImportDescriptor iid;
			iid.Init(base+i*20,20,render);
			m_descriptor.push_back(iid);
		}
		char buf[20];
		memset(buf,0,20);
		assert(memcmp(base+size-20,buf,20)==0,L"导入表最后一个成员不是0",FALSE);
		return true;
	}

	void Print()
	{
		if(m_descriptor.size()==0)
		{
			return;
		}
		wprintf(L"---------------导入符号表--------------\n");
		for(vector<ImageImportDescriptor>::iterator it=m_descriptor.begin();it!=m_descriptor.end();++it)
		{
			it->Print();
		}
	}

public:
	vector<ImageImportDescriptor> m_descriptor;
};