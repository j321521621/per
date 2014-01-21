#pragma once
#include "constdef.h"
#include "RAVRender.h"


class ExportTable
{
public:
	BOOL Init()
	{
		memset(&(m_Characteristics),0,12);
		m_Base=0;
		return TRUE;
	}

	BOOL Init(PCHAR base,DWORD size,RAVRender* render)
	{
		if(base==0)
		{
			return Init();
		}

		memcpy(&(m_Characteristics),base,12);
		m_Name=render->RavToString(*(DWORD*)(base+12));
		m_Base=*(DWORD*)(base+16);
		
		DWORD numberOfFunction=*(DWORD*)(base+20);
		DWORD numberOfName=*(DWORD*)(base+24);
		
		PCHAR pExportFunction=render->RavToImage(*(DWORD*)(base+28),4);
		for(int i=0;i<numberOfFunction;++i)
		{
			m_ExportFunction.push_back(*((DWORD*)pExportFunction+i));
		}

		PCHAR pExportName=render->RavToImage(*(DWORD*)(base+32),4);
		PCHAR pExportNameOrdinal=render->RavToImage(*(DWORD*)(base+36),4);
		for(int i=0;i<numberOfFunction;++i)
		{
			m_ExportName.push_back(render->RavToString(*((DWORD*)pExportName+i)));
			m_ExportNamesOrdinal.push_back(*((WORD*)pExportNameOrdinal+i));
		}

		return TRUE;
	}

	void Print()
	{
		if(m_ExportFunction.size()==0)
		{
			return;
		}
		wprintf(L"---------------导出符号表--------------\n");
		printf("内部名称: %s\n",m_Name.c_str());
		for(int i=0;i<m_ExportFunction.size();++i)
		{
			bool foundname=false;
			for(int j=0;j<m_ExportNamesOrdinal.size();++j)
			{
				if(m_ExportNamesOrdinal[j]==i)
				{
					foundname=true;
					printf("    0x%04x: 0x%08x %s\n",m_Base+i,m_ExportFunction[i],m_ExportName[j].c_str());
					break;
				}
			}
			if(!foundname)
			{
				printf("    0x%04x: 0x%08x\n",m_Base+i,m_ExportFunction[i]);
			}

		}
	}

public:
	DWORD   m_Characteristics;
	DWORD   m_TimeDateStamp;
	WORD    m_MajorVersion;
	WORD    m_MinorVersion;
	string   m_Name;
	DWORD   m_Base;
	vector<DWORD> m_ExportFunction;
	vector<string> m_ExportName;
	vector<DWORD> m_ExportNamesOrdinal;
};
