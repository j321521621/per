#pragma once
#include "constdef.h"
#include "MDOSStub.h"
#include "Signature.h"
#include "COFFheader.h"
#include "OptionHeader.h"
#include "SectionHeader.h"
#include "ImportTable.h"
#include "ExportTable.h"
#include "Resource.h"
#include "Relocation.h"
#include "RAVRender.h"

class File:
	public RAVRender
{
public:
	File():m_size(0),m_base(0),m_path(L"")
	{

	}

	BOOL Init(CString path)
	{
		m_path=path;

		HANDLE hfile=CreateFile(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		assert(hfile!=INVALID_HANDLE_VALUE);
		m_size=GetFileSize(hfile,NULL);
		assert(m_size>0,L"文件内容不能为空");
		HANDLE hfmp=CreateFileMapping(hfile,NULL,PAGE_READONLY,NULL,NULL,NULL);
		assert(hfmp!=INVALID_HANDLE_VALUE);
		m_base=(PCHAR)MapViewOfFileEx(hfmp,FILE_MAP_READ,0,0,0,NULL);
		assert(m_base!=NULL);
		return TRUE;
	}

	operator char* const()
	{
		return m_base;
	}


protected:
	CString m_path;
	DWORD m_size;
	PCHAR m_base;

private:

};
class PEFile: public File
{
public:
	BOOL Init(LPWSTR path)
	{
		File::Init(path);
		PCHAR base=m_base;
		DWORD size=m_size;
		DWORD off;
		off=m_dosstub.Init(base,size);
		base+=off;
		subsize(size,off);

		off=m_signature.Init(base,size);
		base+=off;
		subsize(size,off);

		off=m_coffheader.Init(base,size);
		base+=off;
		subsize(size,off);
        m_petype=m_coffheader.GetPetype();

        off=m_optionalheader.Init(base,m_coffheader.m_SizeOfOptionalHeader,m_petype);
        base+=off;
        subsize(size,off);

		for(int i=0;i<m_coffheader.m_NumberOfSections;i++)
		{
			SectionHeader sh;
			sh.Init(base,size,m_petype);
			m_sectiontheader.push_back(sh);
			base+=40;
			size-=40;
		}

		if(m_optionalheader.m_Export.VirtualAddress)
		{
			m_exporttable.Init(RavToImage(m_optionalheader.m_Export.VirtualAddress,m_optionalheader.m_Export.Size),m_optionalheader.m_Export.Size,this);
		}
		else
		{
			m_exporttable.Init();
		}

		if(m_optionalheader.m_Import.VirtualAddress)
		{
			m_importtable.Init(RavToImage(m_optionalheader.m_Import.VirtualAddress,m_optionalheader.m_Import.Size),m_optionalheader.m_Import.Size,this);
		}
		else
		{
			m_importtable.Init();
		}

		if(m_optionalheader.m_Resource.VirtualAddress)
		{
			m_resourse.Init(RavToImage(m_optionalheader.m_Resource.VirtualAddress,m_optionalheader.m_Resource.Size),m_optionalheader.m_Resource.Size,this);
		}
		else
		{
			m_resourse.Init();
		}

		if(m_optionalheader.m_Resource.VirtualAddress)
		{
			m_relocation.Init(RavToImage(m_optionalheader.m_BaseRelocation.VirtualAddress,m_optionalheader.m_BaseRelocation.Size),m_optionalheader.m_BaseRelocation.Size,this);
		}
		else
		{
			m_relocation.Init();
		}

		return TRUE;
	}



	void Print()
	{
		wprintf(L"================PE文件解析================\n");
		wprintf(L"路径:%s\n",m_path);
		wprintf(L"大小:0x%x \n",m_size);

		if (m_petype==PETYPE_EXCUTE)
		{
			wprintf(L"类型:IMAGE\n");
			m_dosstub.Print();
			m_signature.Print();
			m_coffheader.Print();
			m_optionalheader.Print();
			m_exporttable.Print();
			//m_importtable.Print();
			m_resourse.Print();
			m_relocation.Print();
			for(vector<SectionHeader>::iterator it=m_sectiontheader.begin();it!=m_sectiontheader.end();it++)
			{
				it->Print();
			}
		}
		else if(m_petype==PETYPE_OBJECT)
		{
			wprintf(L"类型:OBJECT\n");
			m_coffheader.Print();
			for(vector<SectionHeader>::iterator it=m_sectiontheader.begin();it!=m_sectiontheader.end();it++)
			{
				it->Print();
			}
		}
		else
		{
			wprintf(L"类型:UNKNOWN\n");
		}
	}

	virtual PCHAR RavToImage(DWORD rav,DWORD size)
	{
		for(vector<SectionHeader>::iterator it=m_sectiontheader.begin();it!=m_sectiontheader.end();++it)
		{
			if(rav>=it->m_VirtualAddress && rav<it->m_VirtualAddress+it->m_VirtualSize)
			{
				assert(rav+size<=it->m_VirtualAddress+it->m_VirtualSize,L"RAV请求跨节",FALSE);
				return m_base+rav-it->m_VirtualAddress+it->m_PointerToRawData;
			}
		}
		assert(FALSE,L"RAV请求找不到合适的节",FALSE);
		return 0;
	}

	virtual string RavToString(DWORD rav)
	{
		string ret;
		for(int i=0;;i++)
		{
			char c=*(RavToImage(rav+i,1));
			if(c==0)
			{
				break;
			}
			ret.push_back(c);
		}
		return ret;
	}





private:
    PETYPE m_petype;
	MSDOSStub m_dosstub;
	Signature m_signature;
	COFFHeader m_coffheader;
	OptionalHeader m_optionalheader;
	vector<SectionHeader> m_sectiontheader;
	ImportTable m_importtable;
	ExportTable m_exporttable;
	Resource m_resourse;
	Relocation m_relocation;
};
