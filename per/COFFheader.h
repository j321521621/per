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
    if(code==0x1c2) return L"ARM or Thumb (��interworking��)";
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

    DWORD Init(PCHAR base,DWORD size)
    {
        assert(size>=20,L"�ļ����Ȳ���(>=20)�޷���ʼ��COFFHeader",FALSE);
        memcpy_s(&m_Machine,20,base,20);
        if(GetPetype()==PETYPE_EXCUTE)
        {
            assert(m_NumberOfSections<=96,L"NumberOfSections�Ƿ�(<=96)�޷���ʼ��COFFHeader",FALSE);
        }
        assert(GetPetype()!=PETYPE_EXCUTE || m_PointerToSymbolTable==0,L"m_PointerToSymbolTable�Ƿ�(=0)�޷���ʼ��COFFHeader",FALSE);
        assert(GetPetype()!=PETYPE_EXCUTE || m_NumberOfSymbols==0,L"m_NumberOfSymbols�Ƿ�(=0)�޷���ʼ��COFFHeader",FALSE);
        assert(GetPetype()!=PETYPE_OBJECT || m_SizeOfOptionalHeader==0,L"m_SizeOfOptionalHeader�Ƿ�(=0)�޷���ʼ��COFFHeader",FALSE);

        return 20;
    }

    void Print()
    {
        wprintf(L"---------------COFFͷ--------------\n");
        wprintf(L"����      : [%8x] %s\n",m_Machine,GetMachineDesc(m_Machine));
        wprintf(L"����      : [%8x]\n",m_NumberOfSections);
        printf("ʱ���    : %s",ctime((time_t*)&m_TimeDateStamp));
        wprintf(L"���ű����: [%8x]\n",m_PointerToSymbolTable);
        wprintf(L"������    : [%8x]\n",m_NumberOfSymbols);
        wprintf(L"��ѡͷ��С: [%8x]\n",m_SizeOfOptionalHeader);
        wprintf(L"��־λ    : [%8x]\n",m_Characteristics);
        wprintfc(((m_Characteristics&IMAGE_FILE_RELOCS_STRIPPED)?HWIHTE:DWIHTE),L"        �����ض�λ��Ϣ\n",);
        wprintfc(((m_Characteristics&IMAGE_FILE_EXECUTABLE_IMAGE)?HWIHTE:DWIHTE),L"        ������\n",);
        wprintfc(((m_Characteristics&IMAGE_FILE_LINE_NUMS_STRIPPED)?HWIHTE:DWIHTE),L"        ����Դ����Ϣ\n",);
        wprintfc(((m_Characteristics&IMAGE_FILE_LOCAL_SYMS_STRIPPED)?HWIHTE:DWIHTE),L"        ����������Ϣ\n",);
        wprintfc(((m_Characteristics&IMAGE_FILE_LARGE_ADDRESS_AWARE)?HWIHTE:DWIHTE),L"        ���ڴ��֪\n",);
        wprintfc(((m_Characteristics&IMAGE_FILE_BYTES_REVERSED_LO)?HWIHTE:DWIHTE),L"        С�˻���\n",);
        wprintfc(((m_Characteristics&IMAGE_FILE_32BIT_MACHINE)?HWIHTE:DWIHTE),L"        32λ����\n",);
        wprintfc(((m_Characteristics&IMAGE_FILE_DEBUG_STRIPPED)?HWIHTE:DWIHTE),L"        ����������Ϣ\n",);
        wprintfc(((m_Characteristics&IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP)?HWIHTE:DWIHTE),L"        ��ʱ���ʻ���\n",);
        wprintfc(((m_Characteristics&IMAGE_FILE_NET_RUN_FROM_SWAP)?HWIHTE:DWIHTE),L"        ������ʻ���\n",);
        wprintfc(((m_Characteristics&IMAGE_FILE_SYSTEM)?HWIHTE:DWIHTE),L"        ϵͳ�ļ�\n",);
        wprintfc(((m_Characteristics&IMAGE_FILE_DLL)?HWIHTE:DWIHTE),L"        DLL�ļ�\n",);
        wprintfc(((m_Characteristics&IMAGE_FILE_UP_SYSTEM_ONLY)?HWIHTE:DWIHTE),L"        ���Ƶ�������\n",);
        wprintfc(((m_Characteristics&IMAGE_FILE_BYTES_REVERSED_LO)?HWIHTE:DWIHTE),L"        ��˻���\n",);
    }

    PETYPE GetPetype()
    {
        return m_Characteristics&IMAGE_FILE_EXECUTABLE_IMAGE?PETYPE_EXCUTE:PETYPE_OBJECT;
    }

    WORD m_Machine;
    WORD m_NumberOfSections;
    DWORD m_TimeDateStamp;
    DWORD m_PointerToSymbolTable;
    DWORD m_NumberOfSymbols;
    WORD m_SizeOfOptionalHeader;
    WORD m_Characteristics;

};