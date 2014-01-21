#pragma once
#include "constdef.h"
#include <time.h>

#define IMAGE_FILE_RELOCS_STRIPPED	0x0001
#define IMAGE_FILE_EXECUTABLE_IMAGE	0x0002
#define IMAGE_FILE_LINE_NUMS_STRIPPED	0x0004
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED	0x0008
#define IMAGE_FILE_AGGRESSIVE_WS_TRIM	0x0010
#define IMAGE_FILE_LARGE_ADDRESS_AWARE	0x0020
#define IMAGE_FILE_BYTES_REVERSED_LO	0x0080
#define IMAGE_FILE_32BIT_MACHINE	0x0100
#define IMAGE_FILE_DEBUG_STRIPPED	0x0200
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP	0x0400
#define IMAGE_FILE_NET_RUN_FROM_SWAP	0x0800
#define IMAGE_FILE_SYSTEM	0x1000
#define IMAGE_FILE_DLL	0x2000
#define IMAGE_FILE_UP_SYSTEM_ONLY	0x4000
#define IMAGE_FILE_BYTES_REVERSED_HI	0x8000

CString GetMachineDesc(WORD code)
{
	if(code==0x0) return L"The contents of this field are assumed to be applicable to any machine type";
	if(code==0x1d3) return L"Matsushita AM33";
	if(code==0x8664) return L"x64";
	if(code==0x1c0) return L"ARM little endian";
	if(code==0x1c4) return L"ARMv7 (or higher) Thumb mode only";
	if(code==0xaa64) return L"ARMv8 in 64-bit mode";
	if(code==0xebc) return L"EFI byte code";
	if(code==0x14c) return L"Intel 386 or later processors and compatible processors";
	if(code==0x200) return L"Intel Itanium processor family";
	if(code==0x9041) return L"Mitsubishi M32R little endian";
	if(code==0x266) return L"MIPS16";
	if(code==0x366) return L"MIPS with FPU";
	if(code==0x466) return L"MIPS16 with FPU";
	if(code==0x1f0) return L"Power PC little endian";
	if(code==0x1f1) return L"Power PC with floating point support";
	if(code==0x166) return L"MIPS little endian";
	if(code==0x1a2) return L"Hitachi SH3";
	if(code==0x1a3) return L"Hitachi SH3 DSP";
	if(code==0x1a6) return L"Hitachi SH4";
	if(code==0x1a8) return L"Hitachi SH5";
	if(code==0x1c2) return L"ARM or Thumb (“interworking”)";
	if(code==0x169) return L"MIPS little-endian WCE v2";
	return "Unknown Machine";
}

class COFFHeader
{
public:
	COFFHeader():
	  m_Machine(0),
		  m_NumberOfSections(0),
		  m_TimeDateStamp(0),
		  m_PointerToSymbolTable(0),
		  m_NumberOfSymbols(0),
		  m_SizeOfOptionalHeader(0),
		  m_Characteristics(0)
	  {

	  }
	  BOOL Init(PCHAR base,DWORD size,PETYPE petype)
	  {
		  m_petype=petype;
		  assert(size>=20,L"文件长度不够(>=20)无法初始化COFFHeader",FALSE);
		  memcpy_s(&m_Machine,20,base,20);
		  if(petype==PETYPE_IMAGE)
		  {
			  assert(m_NumberOfSections<=96,L"NumberOfSections非法(<=96)无法初始化COFFHeader",FALSE);
		  }
		  assert(petype!=PETYPE_IMAGE || m_PointerToSymbolTable==0,L"m_PointerToSymbolTable非法(=0)无法初始化COFFHeader",FALSE);
		  assert(petype!=PETYPE_IMAGE || m_NumberOfSymbols==0,L"m_NumberOfSymbols非法(=0)无法初始化COFFHeader",FALSE);
		  assert(petype!=PETYPE_OBJECT || m_SizeOfOptionalHeader==0,L"m_SizeOfOptionalHeader非法(=0)无法初始化COFFHeader",FALSE);
		  return TRUE;
	  }

	  void Print()
	  {
		  wprintf(L"---------------COFF头--------------\n");
		  wprintf(L"机器: %s (0x%x)\n",GetMachineDesc(m_Machine),m_Machine);
		  wprintf(L"节数: %d\n",m_NumberOfSections);
		  printf("时间戳: %s",ctime((time_t*)&m_TimeDateStamp));
		  if(m_petype==PETYPE_OBJECT)
		  {
			  wprintf(L"符号表入口: 0x%x\n",m_PointerToSymbolTable);
			  wprintf(L"符号数: %d\n",m_NumberOfSymbols);

		  }
		  if(m_petype==PETYPE_IMAGE)
		  {
			  wprintf(L"可选头大小: 0x%x\n",m_SizeOfOptionalHeader);
		  }
		  wprintf(L"标志位: 0x%x\n",m_Characteristics);
		  if(m_petype==PETYPE_IMAGE)
		  {
			  wprintfc(((m_Characteristics&IMAGE_FILE_RELOCS_STRIPPED)?HWIHTE:DWIHTE),L"    消去重定位表\n",);
			  wprintfc(((m_Characteristics&IMAGE_FILE_EXECUTABLE_IMAGE)?HWIHTE:DWIHTE),L"    可运行\n",);
		  }
		  wprintfc(((m_Characteristics&IMAGE_FILE_LINE_NUMS_STRIPPED)?HWIHTE:DWIHTE),L"    消去行号表\n",);
		  wprintfc(((m_Characteristics&IMAGE_FILE_LARGE_ADDRESS_AWARE)?HWIHTE:DWIHTE),L"    大内存感知\n",);
		  wprintfc(((m_Characteristics&IMAGE_FILE_32BIT_MACHINE)?HWIHTE:DWIHTE),L"    32位程序\n",);
		  wprintfc(((m_Characteristics&IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP)?HWIHTE:DWIHTE),L"    临时介质缓存\n",);
		  wprintfc(((m_Characteristics&IMAGE_FILE_NET_RUN_FROM_SWAP)?HWIHTE:DWIHTE),L"    网络介质缓存\n",);
		  wprintfc(((m_Characteristics&IMAGE_FILE_SYSTEM)?HWIHTE:DWIHTE),L"    系统文件\n",);
		  wprintfc(((m_Characteristics&IMAGE_FILE_UP_SYSTEM_ONLY)?HWIHTE:DWIHTE),L"    限制单处理器\n",);
	  }

	  enum PETYPE m_petype;
	  WORD m_Machine;
	  WORD m_NumberOfSections;
	  DWORD m_TimeDateStamp;
	  DWORD m_PointerToSymbolTable;
	  DWORD m_NumberOfSymbols;
	  WORD m_SizeOfOptionalHeader;
	  WORD m_Characteristics;//todo 解析位域

};