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
	DWORD Init(PCHAR base,DWORD size,PETYPE petype)
	{
        assert(petype==PETYPE_EXCUTE || petype==PETYPE_OBJECT);
		m_petype=petype;
		m_Magic=*(PWORD(base));
		if(m_Magic==PE32)
		{
			assert(size>=96,L"�ļ����Ȳ���(>=96),�޷���ʼ����ѡͷ",FALSE);
			memcpy_s(&m_Magic,96,base,96);
		}
		else if(m_Magic==PE32A)
		{
			assert(size>=112,L"�ļ����Ȳ���(>=112),�޷���ʼ����ѡͷ",FALSE);
			memcpy_s(&m_Magic,24,base,24);
			memcpy_s(&m_ImageBase_PE32A,8,base+24,8);
			memcpy_s(&m_SectionAlignment,40,base+32,40);
			memcpy_s(&m_SizeOfStackReserve_PE32A,32,base+72,32);
			memcpy_s(&m_LoaderFlags,8,base+104,8);
		}
		else
		{
			assert(FALSE,L"Magic�Ƿ�,��ѡͷ��ʼ��ʧ��",FALSE);
		}
		assert(!m_petype==PETYPE_EXCUTE || m_NumberOfRvaAndSizes==16,L"���ݱ����쳣,�޷���ʼ����ѡͷ",FALSE);
		if(m_petype==PETYPE_EXCUTE)
		{
			if(m_Magic==PE32)
			{
				assert(size>=224,L"�ļ����Ȳ���(>=224),�޷���ʼ����ѡͷ",FALSE);
				memcpy_s(&m_Export,128,base+96,128);
			}
			else if(m_Magic==PE32A)
			{
				assert(size>=240,L"�ļ����Ȳ���(>=240),�޷���ʼ����ѡͷ",FALSE);
				memcpy_s(&m_Export,128,base+112,128);
			}
		}
        if(m_Magic==PE32)
        {
            return 224;
        }
        else if(m_Magic==PE32A)
        {
            return 240;
        }
	}

	void Print()
	{
		wprintf(L"---------------��ѡͷ--------------\n");
		wprintf(L"����      : %s\n",(m_Magic==PE32)?L"PE32":L"PE32+");
		wprintf(L"�������汾: %d.%d\n",m_MajorLinkerVersion,m_MinorLinkerVersion);
		wprintf(L"����δ�С: [%8x]\n",m_SizeOfCode);
		wprintf(L"��ʼ����С: [%8x]\n",m_SizeOfInitializedData);
		wprintf(L"δ��ʼ��С: [%8x]\n",m_SizeOfUninitializedData);
		wprintf(L"�����    : [%8x]\n",m_AddressOfEntryPoint);
		wprintf(L"����λ�ַ: [%8x]\n",m_BaseOfCode);

		if(m_Magic==PE32)
		{
			wprintf(L"���ݶλ�ַ: [%8x]\n",m_BaseOfData_PE32);
			wprintf(L"32ƫ�û�ַ: [%8x]\n",m_ImageBase_PE32);
		}
		else if(m_Magic==PE32A)
		{
			wprintf(L"64ƫ�û�ַ: [%16llx]\n",m_ImageBase_PE32A);
		}
		wprintf(L"�ڶ���    : [%8x]\n",m_SectionAlignment);
		wprintf(L"�ļ�����  : [%8x]\n",m_FileAlignment);
		wprintf(L"OS�汾    : %d.%d\n",m_MajorOperatingSystemVersion,m_MinorOperatingSystemVersion);
		wprintf(L"ӳ��汾  : %d.%d\n",m_MajorImageVersion,m_MinorImageVersion);
		wprintf(L"��ϵͳ�汾: %d.%d\n",m_MajorSubsystemVersion,m_MinorSubsystemVersion);
		wprintf(L"ӳ���С  : [%8x]\n",m_SizeOfImage);
		wprintf(L"ͷ��С    : [%8x]\n",m_SizeOfHeaders);
		wprintf(L"У���    : [%8x]\n",m_CheckSum);
		wprintf(L"��ϵͳ    : %s (%d)\n",GetSubsystemDesc(m_Subsystem),m_Subsystem);//to do
		wprintf(L"DLL��־λ : [%8x]\n",m_DllCharacteristics);
		wprintfc(((m_DllCharacteristics&IMAGE_DLL_CHARACTERISTICS_DYNAMIC_BASE)?HWIHTE:DWIHTE),L"    ���ض�λ\n",);
		wprintfc(((m_DllCharacteristics&IMAGE_DLL_CHARACTERISTICS_FORCE_INTEGRITY)?HWIHTE:DWIHTE),L"    ǿ��ǩ�����\n",);
		wprintfc(((m_DllCharacteristics&IMAGE_DLL_CHARACTERISTICS_NX_COMPAT)?HWIHTE:DWIHTE),L"    NX����\n",);
		wprintfc(((m_DllCharacteristics&IMAGE_DLLCHARACTERISTICS_NO_ISOLATION)?HWIHTE:DWIHTE),L"    ���ɸ���\n",);
		wprintfc(((m_DllCharacteristics&IMAGE_DLLCHARACTERISTICS_NO_SEH)?HWIHTE:DWIHTE),L"    ��ʹ��SEH\n",);
		wprintfc(((m_DllCharacteristics&IMAGE_DLLCHARACTERISTICS_WDM_DRIVER)?HWIHTE:DWIHTE),L"    WDM����\n",);
		wprintfc(((m_DllCharacteristics&IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE)?HWIHTE:DWIHTE),L"    �ն˷����֪\n",);
		if(m_Magic==PE32)
		{
			wprintf(L"ջ������С(32): [%8x]\n",m_SizeOfStackReserve_PE32);
			wprintf(L"ջ�ύ��С(32): [%8x]\n",m_SizeOfStackCommit_PE32);
			wprintf(L"�ѱ�����С(32): [%8x]\n",m_SizeOfHeapReserve_PE32);
			wprintf(L"���ύ��С(32): [%8x]\n",m_SizeOfHeapCommit_PE32);

		}
		else if(m_Magic==PE32A)
		{

			wprintf(L"ջ������С(64): 0x%16llx\n",m_SizeOfStackReserve_PE32A);
			wprintf(L"ջ�ύ��С(64): 0x%16llx\n",m_SizeOfStackCommit_PE32A);
			wprintf(L"�ѱ�����С(64): 0x%16llx\n",m_SizeOfHeapReserve_PE32A);
			wprintf(L"���ύ��С(64): 0x%16llx\n",m_SizeOfHeapCommit_PE32A);
		}
		wprintf(L"���ݱ���: %d\n",m_NumberOfRvaAndSizes);
		if(m_petype==PETYPE_EXCUTE)
		{
			wprintfc(((m_Export.Size>0)?HWIHTE:DWIHTE),L"    00. RVA: [%8x], Size [%8x] %s\n",m_Export.VirtualAddress,m_Export.Size,L"��������");
			wprintfc(((m_Import.Size>0)?HWIHTE:DWIHTE),L"    01. RVA: [%8x], Size [%8x] %s\n",m_Import.VirtualAddress,m_Import.Size,L"�������");
			wprintfc(((m_Resource.Size>0)?HWIHTE:DWIHTE),L"    02. RVA: [%8x], Size [%8x] %s\n",m_Resource.VirtualAddress,m_Resource.Size,L"��Դ");
			wprintfc(((m_Exception.Size>0)?HWIHTE:DWIHTE),L"    03. RVA: [%8x], Size [%8x] %s\n",m_Exception.VirtualAddress,m_Exception.Size,L"�쳣");
			wprintfc(((m_Certificate.Size>0)?HWIHTE:DWIHTE),L"    04. RVA: [%8x], Size [%8x] %s\n",m_Certificate.VirtualAddress,m_Certificate.Size,L"֤��");
			wprintfc(((m_BaseRelocation.Size>0)?HWIHTE:DWIHTE),L"    05. RVA: [%8x], Size [%8x] %s\n",m_BaseRelocation.VirtualAddress,m_BaseRelocation.Size,L"�ض�λ��");
			wprintfc(((m_Debug.Size>0)?HWIHTE:DWIHTE),L"    06. RVA: [%8x], Size [%8x] %s\n",m_Debug.VirtualAddress,m_Debug.Size,L"������Ϣ");
			wprintfc(((m_GlobalPtr.VirtualAddress>0)?HWIHTE:DWIHTE),L"    08. RVA: [%8x], Size [%8x] %s\n",m_GlobalPtr.VirtualAddress,m_GlobalPtr.Size,L"ָ��Ĵ���");
			wprintfc(((m_TLS.Size>0)?HWIHTE:DWIHTE),L"    09. RVA: [%8x], Size [%8x] %s\n",m_TLS.VirtualAddress,m_TLS.Size,L"�ֲ߳̾��洢");
			wprintfc(((m_LoadConfig.Size>0)?HWIHTE:DWIHTE),L"    10. RVA: [%8x], Size [%8x] %s\n",m_LoadConfig.VirtualAddress,m_LoadConfig.Size,L"������Ϣ");
			wprintfc(((m_BoundImport.Size>0)?HWIHTE:DWIHTE),L"    11. RVA: [%8x], Size [%8x] %s\n",m_BoundImport.VirtualAddress,m_BoundImport.Size,L"�߽絼�����");
			wprintfc(((m_IAT.Size>0)?HWIHTE:DWIHTE),L"    12. RVA: [%8x], Size [%8x] %s\n",m_IAT.VirtualAddress,m_IAT.Size,L"�����ַ����");
			wprintfc(((m_DelayImport.Size>0)?HWIHTE:DWIHTE),L"    13. RVA: [%8x], Size [%8x] %s\n",m_DelayImport.VirtualAddress,m_DelayImport.Size,L"�ӳټ���");
			wprintfc(((m_CLRRuntime.Size>0)?HWIHTE:DWIHTE),L"    14. RVA: [%8x], Size [%8x] %s\n",m_CLRRuntime.VirtualAddress,m_CLRRuntime.Size,L"CLR����ʱ");
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
