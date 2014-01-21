#pragma once
#include "constdef.h"

#define PE32 0x10b
#define PE32A 0x20b

#define IMAGE_DLL_CHARACTERISTICS_DYNAMIC_BASE	0x0040
#define IMAGE_DLL_CHARACTERISTICS_FORCE_INTEGRITY	0x0080
#define IMAGE_DLL_CHARACTERISTICS_NX_COMPAT	0x0100
#define IMAGE_DLLCHARACTERISTICS_NO_ISOLATION	0x0200
#define IMAGE_DLLCHARACTERISTICS_NO_SEH	0x0400
#define IMAGE_DLLCHARACTERISTICS_NO_BIND	0x0800
#define IMAGE_DLLCHARACTERISTICS_WDM_DRIVER	0x2000
#define IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE	0x8000


CString GetSubsystemDesc(WORD code)
{
	if(code==0) return L"An unknown subsystem";
	if(code==1) return L"Device drivers and native Windows processes";
	if(code==2) return L"The Windows graphical user interface (GUI) subsystem";
	if(code==3) return L"The Windows character subsystem";
	if(code==7) return L"The Posix character subsystem";
	if(code==9) return L"Windows CE";
	if(code==10) return L"An Extensible Firmware Interface (EFI) application";
	if(code==11) return L"An EFI driver with boot services";
	if(code==12) return L"An EFI driver with run-time services";
	if(code==13) return L"An EFI ROM image";
	if(code==14) return L"XBOX";
	return "Unknown SubsystemDesc";
}


class OptionalHeader
{
public:
	BOOL Init(PCHAR base,DWORD size,PETYPE petype)
	{
		m_petype=petype;
		m_Magic=*(PWORD(base));
		if(m_Magic==PE32)
		{
			assert(size>=96,L"文件长度不够(>=96),无法初始化可选头",FALSE);
			memcpy_s(&m_Magic,96,base,96);
		}
		else if(m_Magic==PE32A)
		{
			assert(size>=112,L"文件长度不够(>=112),无法初始化可选头",FALSE);
			memcpy_s(&m_Magic,24,base,24);
			memcpy_s(&m_ImageBase_PE32A,8,base+24,8);
			memcpy_s(&m_SectionAlignment,40,base+32,40);
			memcpy_s(&m_SizeOfStackReserve_PE32A,32,base+72,32);
			memcpy_s(&m_LoaderFlags,8,base+104,8);
		}
		else
		{
			assert(FALSE,L"Magic非法,可选头初始化失败",FALSE);
		}
		assert(!m_petype==PETYPE_IMAGE || m_NumberOfRvaAndSizes==16,L"数据表数异常,无法初始化可选头",FALSE);
		if(m_petype==PETYPE_IMAGE)
		{
			if(m_Magic==PE32)
			{
				assert(size>=224,L"文件长度不够(>=224),无法初始化可选头",FALSE);
				memcpy_s(&m_Export,128,base+96,128);
			}
			else if(m_Magic==PE32A)
			{
				assert(size>=240,L"文件长度不够(>=240),无法初始化可选头",FALSE);
				memcpy_s(&m_Export,128,base+112,128);
			}
		}
		return TRUE;
	}

	void Print()
	{
		wprintf(L"---------------可选头--------------\n");
		wprintf(L"类型: %s\n",(m_Magic==PE32)?L"PE32":L"PE32+");
		wprintf(L"连接器版本: %d.%d\n",m_MajorLinkerVersion,m_MinorLinkerVersion);
		wprintf(L"代码段大小: 0x%08x\n",m_SizeOfCode);
		wprintf(L"初始化数据大小: 0x%08x\n",m_SizeOfInitializedData);
		wprintf(L"未初始数据大小: 0x%08x\n",m_SizeOfUninitializedData);
		wprintf(L"进入点: 0x%08x\n",m_AddressOfEntryPoint);
		wprintf(L"代码段基址: 0x%08x\n",m_BaseOfCode);

		if(m_Magic==PE32)
		{
			wprintf(L"数据段基址: 0x08%x\n",m_BaseOfData_PE32);
			wprintf(L"映像偏好基址(32): 0x%08x\n",m_ImageBase_PE32);
		}
		else if(m_Magic==PE32A)
		{
			wprintf(L"映像偏好基址(64): 0x%16llx\n",m_ImageBase_PE32A);
		}
		wprintf(L"节对齐: 0x%08x\n",m_SectionAlignment);
		wprintf(L"文件对齐: 0x%08x\n",m_FileAlignment);
		wprintf(L"操作系统版本: %d.%d\n",m_MajorOperatingSystemVersion,m_MinorOperatingSystemVersion);
		wprintf(L"映像版本: %d.%d\n",m_MajorImageVersion,m_MinorImageVersion);
		wprintf(L"子系统版本: %d.%d\n",m_MajorSubsystemVersion,m_MinorSubsystemVersion);
		wprintf(L"映像大小: %d\n",m_SizeOfImage);
		wprintf(L"头大小: %d\n",m_SizeOfHeaders);
		wprintf(L"校验和: 0x%08x\n",m_CheckSum);
		wprintf(L"子系统: %s (%d)\n",GetSubsystemDesc(m_Subsystem),m_Subsystem);//to do
		wprintf(L"DLL标志位: 0x%08x\n",m_DllCharacteristics);
		wprintfc(((m_DllCharacteristics&IMAGE_DLL_CHARACTERISTICS_DYNAMIC_BASE)?HWIHTE:DWIHTE),L"    可重定位\n",);
		wprintfc(((m_DllCharacteristics&IMAGE_DLL_CHARACTERISTICS_FORCE_INTEGRITY)?HWIHTE:DWIHTE),L"    强制签名检查\n",);
		wprintfc(((m_DllCharacteristics&IMAGE_DLL_CHARACTERISTICS_NX_COMPAT)?HWIHTE:DWIHTE),L"    NX兼容\n",);
		wprintfc(((m_DllCharacteristics&IMAGE_DLLCHARACTERISTICS_NO_ISOLATION)?HWIHTE:DWIHTE),L"    不可隔离\n",);
		wprintfc(((m_DllCharacteristics&IMAGE_DLLCHARACTERISTICS_NO_SEH)?HWIHTE:DWIHTE),L"    不使用SEH\n",);
		wprintfc(((m_DllCharacteristics&IMAGE_DLLCHARACTERISTICS_WDM_DRIVER)?HWIHTE:DWIHTE),L"    WDM驱动\n",);
		wprintfc(((m_DllCharacteristics&IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE)?HWIHTE:DWIHTE),L"    终端服务感知\n",);
		if(m_Magic==PE32)
		{
			wprintf(L"栈保留大小(32): 0x%08x\n",m_SizeOfStackReserve_PE32);
			wprintf(L"栈提交大小(32): 0x%08x\n",m_SizeOfStackCommit_PE32);
			wprintf(L"堆保留大小(32): 0x%08x\n",m_SizeOfHeapReserve_PE32);
			wprintf(L"堆提交大小(32): 0x%08x\n",m_SizeOfHeapCommit_PE32);

		}
		else if(m_Magic==PE32A)
		{

			wprintf(L"栈保留大小(64): 0x%16llx\n",m_SizeOfStackReserve_PE32A);
			wprintf(L"栈提交大小(64): 0x%16llx\n",m_SizeOfStackCommit_PE32A);
			wprintf(L"堆保留大小(64): 0x%16llx\n",m_SizeOfHeapReserve_PE32A);
			wprintf(L"堆提交大小(64): 0x%16llx\n",m_SizeOfHeapCommit_PE32A);
		}
		wprintf(L"数据表数: %d\n",m_NumberOfRvaAndSizes);
		if(m_petype==PETYPE_IMAGE)
		{
			wprintfc(((m_Export.Size>0)?HWIHTE:DWIHTE),L"    00. RVA: 0x%08x, Size 0x%08x %s\n",m_Export.VirtualAddress,m_Export.Size,L"导出符号");
			wprintfc(((m_Import.Size>0)?HWIHTE:DWIHTE),L"    01. RVA: 0x%08x, Size 0x%08x %s\n",m_Import.VirtualAddress,m_Import.Size,L"导入符号");
			wprintfc(((m_Resource.Size>0)?HWIHTE:DWIHTE),L"    02. RVA: 0x%08x, Size 0x%08x %s\n",m_Resource.VirtualAddress,m_Resource.Size,L"资源");
			wprintfc(((m_Exception.Size>0)?HWIHTE:DWIHTE),L"    03. RVA: 0x%08x, Size 0x%08x %s\n",m_Exception.VirtualAddress,m_Exception.Size,L"异常");
			wprintfc(((m_Certificate.Size>0)?HWIHTE:DWIHTE),L"    04. RVA: 0x%08x, Size 0x%08x %s\n",m_Certificate.VirtualAddress,m_Certificate.Size,L"证书");
			wprintfc(((m_BaseRelocation.Size>0)?HWIHTE:DWIHTE),L"    05. RVA: 0x%08x, Size 0x%08x %s\n",m_BaseRelocation.VirtualAddress,m_BaseRelocation.Size,L"重定位表");
			wprintfc(((m_Debug.Size>0)?HWIHTE:DWIHTE),L"    06. RVA: 0x%08x, Size 0x%08x %s\n",m_Debug.VirtualAddress,m_Debug.Size,L"调试信息");
			wprintfc(((m_GlobalPtr.VirtualAddress>0)?HWIHTE:DWIHTE),L"    08. RVA: 0x%08x, Size 0x%08x %s\n",m_GlobalPtr.VirtualAddress,m_GlobalPtr.Size,L"指针寄存器");
			wprintfc(((m_TLS.Size>0)?HWIHTE:DWIHTE),L"    09. RVA: 0x%08x, Size 0x%08x %s\n",m_TLS.VirtualAddress,m_TLS.Size,L"线程局部存储");
			wprintfc(((m_LoadConfig.Size>0)?HWIHTE:DWIHTE),L"    10. RVA: 0x%08x, Size 0x%08x %s\n",m_LoadConfig.VirtualAddress,m_LoadConfig.Size,L"加载信息");
			wprintfc(((m_BoundImport.Size>0)?HWIHTE:DWIHTE),L"    11. RVA: 0x%08x, Size 0x%08x %s\n",m_BoundImport.VirtualAddress,m_BoundImport.Size,L"边界导入符号");
			wprintfc(((m_IAT.Size>0)?HWIHTE:DWIHTE),L"    12. RVA: 0x%08x, Size 0x%08x %s\n",m_IAT.VirtualAddress,m_IAT.Size,L"导入地址符号");
			wprintfc(((m_DelayImport.Size>0)?HWIHTE:DWIHTE),L"    13. RVA: 0x%08x, Size 0x%08x %s\n",m_DelayImport.VirtualAddress,m_DelayImport.Size,L"延迟加载");
			wprintfc(((m_CLRRuntime.Size>0)?HWIHTE:DWIHTE),L"    14. RVA: 0x%08x, Size 0x%08x %s\n",m_CLRRuntime.VirtualAddress,m_CLRRuntime.Size,L"CLR运行时");
		}
	}

public:

	enum PETYPE m_petype;

	WORD m_Magic;
	UCHAR m_MajorLinkerVersion;
	UCHAR m_MinorLinkerVersion;
	DWORD m_SizeOfCode;
	DWORD m_SizeOfInitializedData;
	DWORD m_SizeOfUninitializedData;
	DWORD m_AddressOfEntryPoint;
	DWORD m_BaseOfCode;
	DWORD m_BaseOfData_PE32;

	DWORD m_ImageBase_PE32;
	DWORD m_SectionAlignment;
	DWORD m_FileAlignment;
	WORD m_MajorOperatingSystemVersion;
	WORD m_MinorOperatingSystemVersion;
	WORD m_MajorImageVersion;
	WORD m_MinorImageVersion;
	WORD m_MajorSubsystemVersion;
	WORD m_MinorSubsystemVersion;
	DWORD m_Win32VersionValue;
	DWORD m_SizeOfImage;
	DWORD m_SizeOfHeaders;
	DWORD m_CheckSum;
	WORD m_Subsystem;
	WORD m_DllCharacteristics;
	DWORD m_SizeOfStackReserve_PE32;
	DWORD m_SizeOfStackCommit_PE32;
	DWORD m_SizeOfHeapReserve_PE32;
	DWORD m_SizeOfHeapCommit_PE32;
	DWORD m_LoaderFlags;
	DWORD m_NumberOfRvaAndSizes;

	ULONGLONG m_ImageBase_PE32A;
	ULONGLONG m_SizeOfStackReserve_PE32A;
	ULONGLONG m_SizeOfStackCommit_PE32A;
	ULONGLONG m_SizeOfHeapReserve_PE32A;
	ULONGLONG m_SizeOfHeapCommit_PE32A;

	IMAGE_DATA_DIRECTORY m_Export ;
	IMAGE_DATA_DIRECTORY m_Import ;
	IMAGE_DATA_DIRECTORY m_Resource ;
	IMAGE_DATA_DIRECTORY m_Exception ;
	IMAGE_DATA_DIRECTORY m_Certificate ;
	IMAGE_DATA_DIRECTORY m_BaseRelocation ;
	IMAGE_DATA_DIRECTORY m_Debug;
	IMAGE_DATA_DIRECTORY m_Architecture;
	IMAGE_DATA_DIRECTORY m_GlobalPtr;
	IMAGE_DATA_DIRECTORY m_TLS ;
	IMAGE_DATA_DIRECTORY m_LoadConfig;
	IMAGE_DATA_DIRECTORY m_BoundImport;
	IMAGE_DATA_DIRECTORY m_IAT;
	IMAGE_DATA_DIRECTORY m_DelayImport;
	IMAGE_DATA_DIRECTORY m_CLRRuntime;
	IMAGE_DATA_DIRECTORY m_Reserved;

};
