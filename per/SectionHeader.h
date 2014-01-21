#pragma once
#include "constdef.h"


#define IMAGE_SCN_TYPE_NO_PAD	0x00000008
#define IMAGE_SCN_CNT_CODE	0x00000020
#define IMAGE_SCN_CNT_INITIALIZED_DATA	0x00000040
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA	0x00000080
#define IMAGE_SCN_LNK_OTHER	0x00000100
#define IMAGE_SCN_LNK_INFO	0x00000200
#define IMAGE_SCN_LNK_REMOVE	0x00000800
#define IMAGE_SCN_LNK_COMDAT	0x00001000
#define IMAGE_SCN_GPREL	0x00008000
#define IMAGE_SCN_MEM_PURGEABLE	0x00020000
#define IMAGE_SCN_MEM_16BIT	0x00020000
#define IMAGE_SCN_MEM_LOCKED	0x00040000
#define IMAGE_SCN_MEM_PRELOAD	0x00080000
#define IMAGE_SCN_ALIGN_1BYTES	0x00100000
#define IMAGE_SCN_ALIGN_2BYTES	0x00200000
#define IMAGE_SCN_ALIGN_4BYTES	0x00300000
#define IMAGE_SCN_ALIGN_8BYTES	0x00400000
#define IMAGE_SCN_ALIGN_16BYTES	0x00500000
#define IMAGE_SCN_ALIGN_32BYTES	0x00600000
#define IMAGE_SCN_ALIGN_64BYTES	0x00700000
#define IMAGE_SCN_ALIGN_128BYTES	0x00800000
#define IMAGE_SCN_ALIGN_256BYTES	0x00900000
#define IMAGE_SCN_ALIGN_512BYTES	0x00A00000
#define IMAGE_SCN_ALIGN_1024BYTES	0x00B00000
#define IMAGE_SCN_ALIGN_2048BYTES	0x00C00000
#define IMAGE_SCN_ALIGN_4096BYTES	0x00D00000
#define IMAGE_SCN_ALIGN_8192BYTES	0x00E00000
#define IMAGE_SCN_LNK_NRELOC_OVFL	0x01000000
#define IMAGE_SCN_MEM_DISCARDABLE	0x02000000
#define IMAGE_SCN_MEM_NOT_CACHED	0x04000000
#define IMAGE_SCN_MEM_NOT_PAGED	0x08000000
#define IMAGE_SCN_MEM_SHARED	0x10000000
#define IMAGE_SCN_MEM_EXECUTE	0x20000000
#define IMAGE_SCN_MEM_READ	0x40000000
#define IMAGE_SCN_MEM_WRITE	0x80000000

class SectionHeader
{
public:
	SectionHeader():m_Name()
	{}

	BOOL Init(PCHAR base,DWORD size,PETYPE petype)
	{
		m_petype=petype;

		assert(size>=40,L"文件长度不够(>=40),无法初始化节头",FALSE);

		memcpy_s(&m_Name,8,base,8);
		memcpy_s(&m_VirtualSize,32,base+8,32);
		return TRUE;
	}

	void Print()
	{
		wprintf(L"---------------节头--------------\n");
		printf("名字: %s\n",m_Name);
		wprintf(L"虚拟: 0x%08x+0x%08x\n",m_VirtualAddress,m_VirtualSize);
		wprintf(L"数据: 0x%08x+0x%08x\n",m_PointerToRawData,m_SizeOfRawData);

		wprintfc(((m_Characteristics&IMAGE_SCN_CNT_CODE)?HWIHTE:DWIHTE),L"    执行代码",);
		wprintfc(((m_Characteristics&IMAGE_SCN_CNT_INITIALIZED_DATA)?HWIHTE:DWIHTE),L" 初始化数据",);
		wprintfc(((m_Characteristics&IMAGE_SCN_CNT_UNINITIALIZED_DATA)?HWIHTE:DWIHTE),L" 未初始化数据",);
		wprintfc(((m_Characteristics&IMAGE_SCN_LNK_INFO)?HWIHTE:DWIHTE),L" 链接信息",);
		wprintfc(((m_Characteristics&IMAGE_SCN_GPREL)?HWIHTE:DWIHTE),L" 全局指针",);
		wprintfc(((m_Characteristics&IMAGE_SCN_LNK_NRELOC_OVFL)?HWIHTE:DWIHTE),L" 重定位信息\n",);
		if(m_petype==PETYPE_OBJECT)
		{
			wprintfc(((m_Characteristics&IMAGE_SCN_LNK_REMOVE)?HWIHTE:DWIHTE),L"    不可链接",);
			wprintfc(((m_Characteristics&IMAGE_SCN_LNK_COMDAT)?HWIHTE:DWIHTE),L" 去重链接",);
			wprintf(L" 数据对齐:2^%d\n",(m_Characteristics&0x00E0000)>>20);
		}

		wprintfc(((m_Characteristics&IMAGE_SCN_MEM_SHARED)?HWIHTE:DWIHTE),L"    共享",);
		wprintfc(((m_Characteristics&IMAGE_SCN_MEM_EXECUTE)?HWIHTE:DWIHTE),L" 执行",);
		wprintfc(((m_Characteristics&IMAGE_SCN_MEM_READ)?HWIHTE:DWIHTE),L" 读",);
		wprintfc(((m_Characteristics&IMAGE_SCN_MEM_WRITE)?HWIHTE:DWIHTE),L" 写",);
		wprintfc(((m_Characteristics&IMAGE_SCN_MEM_DISCARDABLE)?HWIHTE:DWIHTE),L" 禁止加载",);
		wprintfc(((m_Characteristics&IMAGE_SCN_MEM_NOT_CACHED)?HWIHTE:DWIHTE),L" 禁止缓存",);
		wprintfc(((m_Characteristics&IMAGE_SCN_MEM_NOT_PAGED)?HWIHTE:DWIHTE),L" 禁止换页\n",);
	}

public:

	enum PETYPE m_petype;

	char m_Name[9];
	DWORD m_VirtualSize;
	DWORD m_VirtualAddress;
	DWORD m_SizeOfRawData;
	DWORD m_PointerToRawData;
	DWORD m_PointerToRelocations;
	DWORD m_PointerToLinenumbers;
	WORD m_NumberOfRelocations;
	WORD m_NumberOfLinenumbers;
	DWORD m_Characteristics;
};
