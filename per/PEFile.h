#pragma once
#include "constdef.h"
#include "MDOSStub.h"
#include "Signature.h"
#include "COFFheader.h"
#include "OptionHeader.h"
#include "SectionHeader.h"
#include <vector>
using std::vector;

class File{
public:
	File():m_size(0),m_base(0),m_petype(PETYPE_UNKNOWN),m_path(L"")
	{

	}

	BOOL Init(CString path)
	{
		m_path=path;
		if(path.Right(4)==L".exe" || path.Right(4)==L".dll")
		{
			m_petype=PETYPE_IMAGE;
		}
		else if(path.Right(4)==L".obj")
		{
			m_petype=PETYPE_OBJECT;
		}
		else
		{
			assert(FALSE,L"未识别的文件扩展名",FALSE);
		}
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
	long m_size;
	PCHAR m_base;
	PETYPE m_petype;

private:

};
class PEFile: public File
{
public:
	BOOL Init(LPWSTR path)
	{
		File::Init(path);
		PCHAR base=m_base;
		long size=m_size;
		if (m_petype==PETYPE_IMAGE)
		{
			m_dosstub.Init(base,size);
			base+=m_dosstub.m_peheader_off;
			size-=m_dosstub.m_peheader_off;

			m_signature.Init(base,size);
			base+=4;
			size-=4;

			m_coffheader.Init(base,size,m_petype);
			base+=20;
			size-=20;

			m_optionalheader.Init(base,size,m_petype);
			base+=m_coffheader.m_SizeOfOptionalHeader;
			size-=m_coffheader.m_SizeOfOptionalHeader;

			for(int i=0;i<m_coffheader.m_NumberOfSections;i++)
			{
				SectionHeader sh;
				sh.Init(base,size,m_petype);
				m_sectiontable.push_back(sh);
				base+=40;
				size-=40;
			}
		}
		else if(m_petype==PETYPE_OBJECT)
		{

			m_coffheader.Init(base,size,m_petype);
			base+=20;
			size-=20;

			for(int i=0;i<m_coffheader.m_NumberOfSections;i++)
			{
				SectionHeader sh;
				sh.Init(base,size,m_petype);
				m_sectiontable.push_back(sh);
				base+=40;
				size-=40;
			}
		}
		else
		{
			assert(FALSE,L"未识别的PE文件类型",FALSE);
		}
		return TRUE;
	}

	void Print()
	{
		wprintf(L"================PE文件解析================\n");
		wprintf(L"路径:%s\n",m_path);
		wprintf(L"大小:0x%x \n",m_size);

		if (m_petype==PETYPE_IMAGE)
		{
			wprintf(L"类型:IMAGE\n");
			m_dosstub.Print();
			m_signature.Print();
			m_coffheader.Print();
			m_optionalheader.Print();
			for(vector<SectionHeader>::iterator it=m_sectiontable.begin();it!=m_sectiontable.end();it++)
			{
				it->Print();
			}
		}
		else if(m_petype==PETYPE_OBJECT)
		{
			wprintf(L"类型:OBJECT\n");
			m_coffheader.Print();
			for(vector<SectionHeader>::iterator it=m_sectiontable.begin();it!=m_sectiontable.end();it++)
			{
				it->Print();
			}
		}
		else
		{
			wprintf(L"类型:UNKNOWN\n");
		}
	}


private:
	MSDOSStub m_dosstub;
	Signature m_signature;
	COFFHeader m_coffheader;
	OptionalHeader m_optionalheader;
	vector<SectionHeader> m_sectiontable;

};
