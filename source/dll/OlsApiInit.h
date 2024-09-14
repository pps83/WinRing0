//-----------------------------------------------------------------------------
//     Author : hiyohiyo
//       Mail : hiyohiyo@crystalmark.info
//        Web : http://openlibsys.org/
//    License : The modified BSD license
//
//                     Copyright 2007-2009 OpenLibSys.org. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

#include "OlsDef.h"
#include "OlsApiInitDef.h"

int InitOpenLibSys(HMODULE* hModule);
BOOL DeinitOpenLibSys(HMODULE* hModule);

_GetDllStatus GetDllStatus = NULL;
_GetDllVersion GetDllVersion = NULL;
_GetDriverVersion GetDriverVersion = NULL;
_GetDriverType GetDriverType = NULL;
_GetRefCount GetRefCount = NULL;

_InitializeOls InitializeOls = NULL;
_DeinitializeOls DeinitializeOls = NULL;

_IsCpuid IsCpuid = NULL;
_IsMsr IsMsr = NULL;
_IsTsc IsTsc = NULL;

_Hlt Hlt = NULL;
_Rdmsr Rdmsr = NULL;
_Wrmsr Wrmsr = NULL;
_Rdpmc Rdpmc = NULL;
_Cpuid Cpuid = NULL;
_Rdtsc Rdtsc = NULL;

_HltTx HltTx = NULL;
_RdmsrTx RdmsrTx = NULL;
_WrmsrTx WrmsrTx = NULL;
_RdpmcTx RdpmcTx = NULL;
_CpuidTx CpuidTx = NULL;
_RdtscTx RdtscTx = NULL;

_HltPx HltPx = NULL;
_RdmsrPx RdmsrPx = NULL;
_WrmsrPx WrmsrPx = NULL;
_RdpmcPx RdpmcPx = NULL;
_CpuidPx CpuidPx = NULL;
_RdtscPx RdtscPx = NULL;

_ReadIoPortByte ReadIoPortByte = NULL;
_ReadIoPortWord ReadIoPortWord = NULL;
_ReadIoPortDword ReadIoPortDword = NULL;

_ReadIoPortByteEx ReadIoPortByteEx = NULL;
_ReadIoPortWordEx ReadIoPortWordEx = NULL;
_ReadIoPortDwordEx ReadIoPortDwordEx = NULL;

_WriteIoPortByte WriteIoPortByte = NULL;
_WriteIoPortWord WriteIoPortWord = NULL;
_WriteIoPortDword WriteIoPortDword = NULL;

_WriteIoPortByteEx WriteIoPortByteEx = NULL;
_WriteIoPortWordEx WriteIoPortWordEx = NULL;
_WriteIoPortDwordEx WriteIoPortDwordEx = NULL;

_SetPciMaxBusIndex SetPciMaxBusIndex = NULL;

_ReadPciConfigByte ReadPciConfigByte = NULL;
_ReadPciConfigWord ReadPciConfigWord = NULL;
_ReadPciConfigDword ReadPciConfigDword = NULL;

_ReadPciConfigByteEx ReadPciConfigByteEx = NULL;
_ReadPciConfigWordEx ReadPciConfigWordEx = NULL;
_ReadPciConfigDwordEx ReadPciConfigDwordEx = NULL;

_WritePciConfigByte WritePciConfigByte = NULL;
_WritePciConfigWord WritePciConfigWord = NULL;
_WritePciConfigDword WritePciConfigDword = NULL;

_WritePciConfigByteEx WritePciConfigByteEx = NULL;
_WritePciConfigWordEx WritePciConfigWordEx = NULL;
_WritePciConfigDwordEx WritePciConfigDwordEx = NULL;

_FindPciDeviceById FindPciDeviceById = NULL;
_FindPciDeviceByClass FindPciDeviceByClass = NULL;

#ifdef _PHYSICAL_MEMORY_SUPPORT
_ReadDmiMemory ReadDmiMemory = NULL;
_ReadPhysicalMemory ReadPhysicalMemory = NULL;
_WritePhysicalMemory WritePhysicalMemory = NULL;
#endif

int InitOpenLibSys(HMODULE* hModule)
{
    HMODULE hmod = NULL;
#if defined(_M_X64)
#ifdef _DEBUG
    hmod = LoadLibrary(_T("WinRing0x64Debug.dll"));
#endif
    if (hmod == NULL)
        hmod = LoadLibrary(_T("WinRing0x64.dll"));
#elif defined(_M_IX86)
#ifdef _DEBUG
    hmod = LoadLibrary(_T("WinRing0Debug.dll"));
#endif
    if (hmod == NULL)
        hmod = LoadLibrary(_T("WinRing0.dll"));
#else
#error unsupported build target
#endif

    *hModule = hmod;
    if (hmod == NULL)
    {
        return 0;
    }

    GetDllStatus = (_GetDllStatus)GetProcAddress(hmod, "GetDllStatus");
    GetDllVersion = (_GetDllVersion)GetProcAddress(hmod, "GetDllVersion");
    GetDriverVersion = (_GetDriverVersion)GetProcAddress(hmod, "GetDriverVersion");
    GetDriverType = (_GetDriverType)GetProcAddress(hmod, "GetDriverType");
    GetRefCount = (_GetRefCount)GetProcAddress(hmod, "GetRefCount");

    InitializeOls = (_InitializeOls)GetProcAddress(hmod, "InitializeOls");
    DeinitializeOls = (_DeinitializeOls)GetProcAddress(hmod, "DeinitializeOls");

    IsCpuid = (_IsCpuid)GetProcAddress(hmod, "IsCpuid");
    IsMsr = (_IsMsr)GetProcAddress(hmod, "IsMsr");
    IsTsc = (_IsTsc)GetProcAddress(hmod, "IsTsc");
    Hlt = (_Hlt)GetProcAddress(hmod, "Hlt");
    Rdmsr = (_Rdmsr)GetProcAddress(hmod, "Rdmsr");
    Wrmsr = (_Wrmsr)GetProcAddress(hmod, "Wrmsr");
    Rdpmc = (_Rdpmc)GetProcAddress(hmod, "Rdpmc");
    Cpuid = (_Cpuid)GetProcAddress(hmod, "Cpuid");
    Rdtsc = (_Rdtsc)GetProcAddress(hmod, "Rdtsc");
    HltTx = (_HltTx)GetProcAddress(hmod, "HltTx");
    RdmsrTx = (_RdmsrTx)GetProcAddress(hmod, "RdmsrTx");
    WrmsrTx = (_WrmsrTx)GetProcAddress(hmod, "WrmsrTx");
    RdpmcTx = (_RdpmcTx)GetProcAddress(hmod, "RdpmcTx");
    CpuidTx = (_CpuidTx)GetProcAddress(hmod, "CpuidTx");
    RdtscTx = (_RdtscTx)GetProcAddress(hmod, "RdtscTx");
    HltPx = (_HltPx)GetProcAddress(hmod, "HltPx");
    RdmsrPx = (_RdmsrPx)GetProcAddress(hmod, "RdmsrPx");
    WrmsrPx = (_WrmsrPx)GetProcAddress(hmod, "WrmsrPx");
    RdpmcPx = (_RdpmcPx)GetProcAddress(hmod, "RdpmcPx");
    CpuidPx = (_CpuidPx)GetProcAddress(hmod, "CpuidPx");
    RdtscPx = (_RdtscPx)GetProcAddress(hmod, "RdtscPx");

    ReadIoPortByte = (_ReadIoPortByte)GetProcAddress(hmod, "ReadIoPortByte");
    ReadIoPortWord = (_ReadIoPortWord)GetProcAddress(hmod, "ReadIoPortWord");
    ReadIoPortDword = (_ReadIoPortDword)GetProcAddress(hmod, "ReadIoPortDword");

    ReadIoPortByteEx = (_ReadIoPortByteEx)GetProcAddress(hmod, "ReadIoPortByteEx");
    ReadIoPortWordEx = (_ReadIoPortWordEx)GetProcAddress(hmod, "ReadIoPortWordEx");
    ReadIoPortDwordEx = (_ReadIoPortDwordEx)GetProcAddress(hmod, "ReadIoPortDwordEx");

    WriteIoPortByte = (_WriteIoPortByte)GetProcAddress(hmod, "WriteIoPortByte");
    WriteIoPortWord = (_WriteIoPortWord)GetProcAddress(hmod, "WriteIoPortWord");
    WriteIoPortDword = (_WriteIoPortDword)GetProcAddress(hmod, "WriteIoPortDword");

    WriteIoPortByteEx = (_WriteIoPortByteEx)GetProcAddress(hmod, "WriteIoPortByteEx");
    WriteIoPortWordEx = (_WriteIoPortWordEx)GetProcAddress(hmod, "WriteIoPortWordEx");
    WriteIoPortDwordEx = (_WriteIoPortDwordEx)GetProcAddress(hmod, "WriteIoPortDwordEx");

    SetPciMaxBusIndex = (_SetPciMaxBusIndex)GetProcAddress(hmod, "SetPciMaxBusIndex");

    ReadPciConfigByte = (_ReadPciConfigByte)GetProcAddress(hmod, "ReadPciConfigByte");
    ReadPciConfigWord = (_ReadPciConfigWord)GetProcAddress(hmod, "ReadPciConfigWord");
    ReadPciConfigDword = (_ReadPciConfigDword)GetProcAddress(hmod, "ReadPciConfigDword");

    ReadPciConfigByteEx = (_ReadPciConfigByteEx)GetProcAddress(hmod, "ReadPciConfigByteEx");
    ReadPciConfigWordEx = (_ReadPciConfigWordEx)GetProcAddress(hmod, "ReadPciConfigWordEx");
    ReadPciConfigDwordEx = (_ReadPciConfigDwordEx)GetProcAddress(hmod, "ReadPciConfigDwordEx");

    WritePciConfigByte = (_WritePciConfigByte)GetProcAddress(hmod, "WritePciConfigByte");
    WritePciConfigWord = (_WritePciConfigWord)GetProcAddress(hmod, "WritePciConfigWord");
    WritePciConfigDword = (_WritePciConfigDword)GetProcAddress(hmod, "WritePciConfigDword");

    WritePciConfigByteEx = (_WritePciConfigByteEx)GetProcAddress(hmod, "WritePciConfigByteEx");
    WritePciConfigWordEx = (_WritePciConfigWordEx)GetProcAddress(hmod, "WritePciConfigWordEx");
    WritePciConfigDwordEx = (_WritePciConfigDwordEx)GetProcAddress(hmod, "WritePciConfigDwordEx");

    FindPciDeviceById = (_FindPciDeviceById)GetProcAddress(hmod, "FindPciDeviceById");
    FindPciDeviceByClass = (_FindPciDeviceByClass)GetProcAddress(hmod, "FindPciDeviceByClass");

#ifdef _PHYSICAL_MEMORY_SUPPORT
    ReadDmiMemory = (_ReadDmiMemory)GetProcAddress(hmod, "ReadDmiMemory");
    ReadPhysicalMemory = (_ReadPhysicalMemory)GetProcAddress(hmod, "ReadPhysicalMemory");
    WritePhysicalMemory = (_WritePhysicalMemory)GetProcAddress(hmod, "WritePhysicalMemory");
#endif

    if (!(GetDllStatus && GetDllVersion && GetDriverVersion && GetDriverType && GetRefCount && InitializeOls &&
            DeinitializeOls && IsCpuid && IsMsr && IsTsc && Hlt && HltTx && HltPx && Rdmsr && RdmsrTx && RdmsrPx &&
            Wrmsr && WrmsrTx && WrmsrPx && Rdpmc && RdpmcTx && RdpmcPx && Cpuid && CpuidTx && CpuidPx && Rdtsc &&
            RdtscTx && RdtscPx && ReadIoPortByte && ReadIoPortWord && ReadIoPortDword && ReadIoPortByteEx &&
            ReadIoPortWordEx && ReadIoPortDwordEx && WriteIoPortByte && WriteIoPortWord && WriteIoPortDword &&
            WriteIoPortByteEx && WriteIoPortWordEx && WriteIoPortDwordEx && SetPciMaxBusIndex && ReadPciConfigByte &&
            ReadPciConfigWord && ReadPciConfigDword && ReadPciConfigByteEx && ReadPciConfigWordEx &&
            ReadPciConfigDwordEx && WritePciConfigByte && WritePciConfigWord && WritePciConfigDword &&
            WritePciConfigByteEx && WritePciConfigWordEx && WritePciConfigDwordEx && FindPciDeviceById &&
            FindPciDeviceByClass
#ifdef _PHYSICAL_MEMORY_SUPPORT
            && ReadDmiMemory && ReadPhysicalMemory && WritePhysicalMemory
#endif
            ))
    {
        return 0;
    }

    return InitializeOls();
}

BOOL DeinitOpenLibSys(HMODULE* hModule)
{
    BOOL result = FALSE;

    if (*hModule == NULL)
    {
        return TRUE;
    }
    else
    {
        DeinitializeOls();
        result = FreeLibrary(*hModule);
        *hModule = NULL;

        return result;
    }
}
