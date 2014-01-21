#pragma once
#include "constdef.h"
#include "RAVRender.h"

wstring DataToWString(PCHAR base,DWORD size)
{
	wstring ret;
	assert(size>=2,L"DataToWString大小不足2",FALSE);
	WORD count=*(WORD*)(base);
	assert(size>=2+2*count,L"DataToWString大小不足count",FALSE);
	for(int i=0;i<count;i++)
	{
		ret.push_back(*(WCHAR*)(base+2+2*i));
	}
	return ret;
}

string IdToType(DWORD id)
{
	switch(id)
	{
	case(1): return "RT_CURSOR";
	case(2): return "RT_BITMAP";
	case(3): return "RT_ICON";
	case(4): return "RT_MENU";
	case(5): return "RT_DIALOG";
	case(6): return "RT_STRING";
	case(7): return "RT_FONTDIR";
	case(8): return "RT_FONT";
	case(9): return "RT_ACCELERATOR";
	case(10): return "RT_RCDATA";
	case(11): return "RT_MESSAGETABLE";
	case(12): return "RT_GROUP_CURSOR";
	case(14): return "RT_GROUP_ICON";
	case(16): return "RT_VERSION";
	case(17): return "RT_DLGINCLUDE";
	case(19): return "RT_PLUGPLAY";
	case(20): return "RT_VXD";
	case(21): return "RT_ANICURSOR";
	case(22): return "RT_ANIICON";
	case(23): return "RT_HTM";
	case(24): return "RT_MANIFEST";
	default: return "";
	}
	return "";
}

class ImageResourceDirectoryEntra
{
public:
	BOOL Init(PCHAR base,DWORD size,RAVRender* render)
	{
		memcpy(this,base,8);
		return TRUE;
	}
public:
	DWORD m_Name;
	DWORD m_OffsetToData;
};

class ImageResource
{
public:
	wstring m_Name;
	WORD m_Id;
	virtual void Print(int indent)
	{
		for(int i=0;i<indent;++i)
		{
			printf(" ");
		}

		if(m_Name.size())
		{
			wprintf(L"\"%s\"",m_Name.c_str());
		}
		else
		{
			printf("%d",m_Id);
		}
	}
};

class ImageResourceFile:public ImageResource
{
public:
	BOOL Init(PCHAR base,DWORD size,RAVRender* render)
	{
		DWORD OffsetToData=*(DWORD*)(base+0);
		DWORD Size=*(DWORD*)(base+4);
		m_Data=string(render->RavToImage(OffsetToData,Size),Size);
		m_CodePage=*(DWORD*)(base+8);
		return TRUE;
	}

	virtual void Print(int indent)
	{
		__super::Print(indent);
		printf(" CP:%d L:0x%08x\n",m_CodePage,m_Data.size());
	}
public:	
	string m_Data;
	DWORD m_CodePage;
};

class ImageResourceDirectory:public ImageResource
{
public:
	BOOL Init(PCHAR base,DWORD size,RAVRender* render,PCHAR rsrcbase,DWORD rsrcsize)
	{
		m_Characteristics=*(DWORD*)(base+0);
		m_TimeDateStamp=*(WORD*)(base+4);
		m_MajorVersion=*(WORD*)(base+8);
		m_MinorVersion=*(WORD*)(base+10);
		DWORD NumberOfNamedEntries=*(WORD*)(base+12);
		DWORD NumberOfIdEntries=*(WORD*)(base+14);

		for(int i=0;i<NumberOfNamedEntries+NumberOfIdEntries;i++)
		{
			DWORD name=*(DWORD*)(base+16+8*i);
			DWORD data=*(DWORD*)(base+16+8*i+4);
			
			wstring wname;
			DWORD id=0;
			if(name & 0x80000000)
			{	
				DWORD off=(name & ~0x80000000);
				wname=DataToWString(rsrcbase+off,rsrcsize-off);
			}
			else
			{
				id=name;
			}

			if(data & 0x80000000)
			{
				ImageResourceDirectory *ird=new ImageResourceDirectory();
				ird->m_Name=wname;
				ird->m_Id=id;
				DWORD off=(data & ~0x80000000);
				ird->Init(rsrcbase+off,rsrcsize-off,render,rsrcbase,rsrcsize);
				m_ImageResource.push_back(ird);
			}
			else
			{
				ImageResourceFile *irf=new ImageResourceFile();
				irf->m_Name=wname;
				irf->m_Id=id;
				DWORD off=data;
				irf->Init(rsrcbase+off,rsrcsize-off,render);
				m_ImageResource.push_back(irf);
			}
		}
		return TRUE;
	}

	virtual void Print(int indent)
	{

		__super::Print(indent);
		if(indent==4)
		{
			string type=IdToType(m_Id);
			printf(" %s\n",type.c_str());
		}
		else
		{
			printf("\n");
		}

		for(vector<ImageResource*>::iterator it=m_ImageResource.begin();it!=m_ImageResource.end();++it)
		{
			(*it)->Print(indent+4);
		}
	}

public:
	DWORD m_Characteristics;
	DWORD m_TimeDateStamp;
	WORD m_MajorVersion;
	WORD m_MinorVersion;
	vector<ImageResource*> m_ImageResource;
};

class Resource
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

		root.m_Id=0;
		root.Init(base,size,render,base,size);

		return TRUE;
	}

	void Print()
	{
		wprintf(L"---------------资源--------------\n");
		root.Print(0);
	}

public:
	ImageResourceDirectory root;
};
