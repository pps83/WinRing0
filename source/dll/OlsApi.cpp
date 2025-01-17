//-----------------------------------------------------------------------------
//     Author : hiyohiyo
//       Mail : hiyohiyo@crystalmark.info
//        Web : http://openlibsys.org/
//    License : The modified BSD license
//
//                     Copyright 2007-2009 OpenLibSys.org. All rights reserved.
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include <tchar.h>
#include <winioctl.h>
#include <stddef.h>
#include <malloc.h>
#include <intrin.h>
#include "OlsDll.h"
#include "OlsApi.h"
#include "OlsDef.h"
#include "Driver.h"
#include "OlsIoctl.h"

const BYTE gPciNumberOfDevice = 32;
const BYTE gPciNumberOfFunction = 8;

BYTE gPciMaxNumberOfBus = 255;

extern HANDLE gHandle;

extern BOOL gIsNT;
extern BOOL gIsCpuid;
extern BOOL gIsMsr;
extern BOOL gIsTsc;
extern BOOL gInitDll;

extern DWORD gDllStatus;
extern DWORD gDriverType;
extern TCHAR gDriverPath[MAX_PATH];

WINRING0_API DWORD GetDllStatus()
{
    if (gDllStatus == OLS_DLL_NO_ERROR && gDriverType >= OLS_DRIVER_TYPE_WIN_NT &&
        GetDriverVersion(NULL, NULL, NULL, NULL) == 0)
    {
        gDllStatus = OLS_DLL_DRIVER_UNLOADED;
    }

    return gDllStatus;
}

WINRING0_API DWORD GetDllVersion(PBYTE major, PBYTE minor, PBYTE revision, PBYTE release)
{
    if (major != NULL && minor != NULL && revision != NULL && release != NULL)
    {
        *major = OLS_MAJOR_VERSION;
        *minor = OLS_MINOR_VERSION;
        *revision = OLS_REVISION;
        *release = OLS_RELESE;
    }

    return OLS_VERSION;
}

WINRING0_API DWORD GetDriverVersion(PBYTE major, PBYTE minor, PBYTE revision, PBYTE release)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    DWORD value = 0;

    result =
        DeviceIoControl(gHandle, IOCTL_OLS_GET_DRIVER_VERSION, NULL, 0, &value, sizeof(value), &returnedLength, NULL);

    if (result)
    {
        if (major != NULL && minor != NULL && revision != NULL && release != NULL)
        {
            *major = (BYTE)((value >> 24) & 0xFF);
            *minor = (BYTE)((value >> 16) & 0xFF);
            *revision = (BYTE)((value >> 8) & 0xFF);
            *release = (BYTE)((value) & 0xFF);
        }
    }
    else
    {
        value = 0;
    }

    return value;
}

WINRING0_API DWORD GetDriverType()
{
    return gDriverType;
}

WINRING0_API BOOL IsCpuid()
{
    return gIsCpuid;
}

WINRING0_API BOOL IsMsr()
{
    return gIsMsr;
}

WINRING0_API BOOL IsTsc()
{
    return gIsTsc;
}

WINRING0_API BOOL Rdmsr(DWORD index, PDWORD eax, PDWORD edx)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    if (eax == NULL || edx == NULL || gIsMsr == FALSE)
    {
        return FALSE;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    BYTE outBuf[8] = {0};

    result = DeviceIoControl(
        gHandle, IOCTL_OLS_READ_MSR, &index, sizeof(index), &outBuf, sizeof(outBuf), &returnedLength, NULL);

    if (result)
    {
        memcpy(eax, outBuf, 4);
        memcpy(edx, outBuf + 4, 4);
    }

    if (result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

WINRING0_API BOOL RdmsrTx(DWORD index, PDWORD eax, PDWORD edx, DWORD_PTR threadAffinityMask)
{
    BOOL result = FALSE;
    DWORD_PTR mask = 0;
    HANDLE hThread = NULL;

    if (gIsNT)
    {
        hThread = GetCurrentThread();
        mask = SetThreadAffinityMask(hThread, threadAffinityMask);
        if (mask == 0)
        {
            return FALSE;
        }
    }

    result = Rdmsr(index, eax, edx);

    if (gIsNT)
    {
        SetThreadAffinityMask(hThread, mask);
    }

    return result;
}

WINRING0_API BOOL RdmsrPx(DWORD index, PDWORD eax, PDWORD edx, DWORD_PTR processAffinityMask)
{
    BOOL result = FALSE;
    DWORD_PTR processMask = 0;
    DWORD_PTR systemMask = 0;
    HANDLE hProcess = NULL;

    if (gIsNT)
    {
        hProcess = GetCurrentProcess();
        GetProcessAffinityMask(hProcess, &processMask, &systemMask);
        if (!SetProcessAffinityMask(hProcess, processAffinityMask))
        {
            return FALSE;
        }
    }

    result = Rdmsr(index, eax, edx);

    if (gIsNT)
    {
        SetProcessAffinityMask(hProcess, processMask);
    }

    return result;
}

WINRING0_API BOOL Wrmsr(DWORD index, DWORD eax, DWORD edx)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    if (gIsMsr == FALSE)
    {
        return FALSE;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    DWORD outBuf;
    OLS_WRITE_MSR_INPUT inBuf;

    inBuf.Register = index;
    inBuf.Value.HighPart = edx;
    inBuf.Value.LowPart = eax;

    result = DeviceIoControl(
        gHandle, IOCTL_OLS_WRITE_MSR, &inBuf, sizeof(inBuf), &outBuf, sizeof(outBuf), &returnedLength, NULL);

    if (result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

WINRING0_API BOOL WrmsrTx(DWORD index, DWORD eax, DWORD edx, DWORD_PTR threadAffinityMask)
{
    BOOL result = FALSE;
    DWORD_PTR mask = 0;
    HANDLE hThread = NULL;

    if (gIsNT)
    {
        hThread = GetCurrentThread();
        mask = SetThreadAffinityMask(hThread, threadAffinityMask);
        if (mask == 0)
        {
            return FALSE;
        }
    }

    result = Wrmsr(index, eax, edx);

    if (gIsNT)
    {
        SetThreadAffinityMask(hThread, mask);
    }

    return result;
}

WINRING0_API BOOL WrmsrPx(DWORD index, DWORD eax, DWORD edx, DWORD_PTR processAffinityMask)
{
    BOOL result = FALSE;
    DWORD_PTR processMask = 0;
    DWORD_PTR systemMask = 0;
    HANDLE hProcess = NULL;

    if (gIsNT)
    {
        hProcess = GetCurrentProcess();
        GetProcessAffinityMask(hProcess, &processMask, &systemMask);
        if (!SetProcessAffinityMask(hProcess, processAffinityMask))
        {
            return FALSE;
        }
    }

    result = Wrmsr(index, eax, edx);

    if (gIsNT)
    {
        SetProcessAffinityMask(hProcess, processMask);
    }

    return result;
}

WINRING0_API BOOL Rdpmc(DWORD index, PDWORD eax, PDWORD edx)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    if (eax == NULL || edx == NULL)
    {
        return FALSE;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    BYTE outBuf[8] = {0};

    result = DeviceIoControl(
        gHandle, IOCTL_OLS_READ_PMC, &index, sizeof(index), &outBuf, sizeof(outBuf), &returnedLength, NULL);

    if (result)
    {
        memcpy(eax, outBuf, 4);
        memcpy(edx, outBuf + 4, 4);
    }

    if (result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

WINRING0_API BOOL RdpmcTx(DWORD index, PDWORD eax, PDWORD edx, DWORD_PTR threadAffinityMask)
{
    BOOL result = FALSE;
    DWORD_PTR mask = 0;
    HANDLE hThread = NULL;

    if (gIsNT)
    {
        hThread = GetCurrentThread();
        mask = SetThreadAffinityMask(hThread, threadAffinityMask);
        if (mask == 0)
        {
            return FALSE;
        }
    }

    result = Rdpmc(index, eax, edx);

    if (gIsNT)
    {
        SetThreadAffinityMask(hThread, mask);
    }

    return result;
}

WINRING0_API BOOL RdpmcPx(DWORD index, PDWORD eax, PDWORD edx, DWORD_PTR processAffinityMask)
{
    BOOL result = FALSE;
    DWORD_PTR processMask = 0;
    DWORD_PTR systemMask = 0;
    HANDLE hProcess = NULL;

    if (gIsNT)
    {
        hProcess = GetCurrentProcess();
        GetProcessAffinityMask(hProcess, &processMask, &systemMask);
        if (!SetProcessAffinityMask(hProcess, processAffinityMask))
        {
            return FALSE;
        }
    }

    result = Rdpmc(index, eax, edx);

    if (gIsNT)
    {
        SetProcessAffinityMask(hProcess, processMask);
    }

    return result;
}

WINRING0_API BOOL Cpuid(DWORD index, DWORD* pEAX, DWORD* pEBX, DWORD* pECX, DWORD* pEDX)
{
    if (pEAX == NULL || pEBX == NULL || pECX == NULL || pEDX == NULL || gIsCpuid == FALSE)
    {
        return FALSE;
    }

    int info[4];
    __cpuidex(info, index, 0);
    *pEAX = info[0];
    *pEBX = info[1];
    *pECX = info[2];
    *pEDX = info[3];

    return TRUE;
}

WINRING0_API BOOL CpuidTx(DWORD index, PDWORD eax, PDWORD ebx, PDWORD ecx, PDWORD edx, DWORD_PTR threadAffinityMask)
{
    BOOL result = FALSE;
    DWORD_PTR mask = 0;
    HANDLE hThread = NULL;

    if (gIsNT)
    {
        hThread = GetCurrentThread();
        mask = SetThreadAffinityMask(hThread, threadAffinityMask);
        if (mask == 0)
        {
            return FALSE;
        }
    }

    result = Cpuid(index, eax, ebx, ecx, edx);

    if (gIsNT)
    {
        SetThreadAffinityMask(hThread, mask);
    }

    return result;
}

WINRING0_API BOOL CpuidPx(DWORD index, PDWORD eax, PDWORD ebx, PDWORD ecx, PDWORD edx, DWORD_PTR processAffinityMask)
{
    BOOL result = FALSE;
    DWORD_PTR processMask = 0;
    DWORD_PTR systemMask = 0;
    HANDLE hProcess = NULL;

    if (gIsNT)
    {
        hProcess = GetCurrentProcess();
        GetProcessAffinityMask(hProcess, &processMask, &systemMask);
        if (!SetProcessAffinityMask(hProcess, processAffinityMask))
        {
            return FALSE;
        }
    }

    result = Cpuid(index, eax, ebx, ecx, edx);

    if (gIsNT)
    {
        SetProcessAffinityMask(hProcess, processMask);
    }

    return result;
}

WINRING0_API BOOL Rdtsc(PDWORD eax, PDWORD edx)
{
    if (eax == NULL || edx == NULL || gIsTsc == FALSE)
    {
        return FALSE;
    }

    ULONGLONG value = 0;

    value = __rdtsc();
    *eax = (DWORD)((value >> 0) & 0xFFFFFFFF);
    *edx = (DWORD)((value >> 32) & 0xFFFFFFFF);

    return TRUE;
}

WINRING0_API BOOL RdtscTx(PDWORD eax, PDWORD edx, DWORD_PTR threadAffinityMask)
{
    BOOL result = FALSE;
    DWORD_PTR mask = 0;
    HANDLE hThread = NULL;

    if (gIsNT)
    {
        hThread = GetCurrentThread();
        mask = SetThreadAffinityMask(hThread, threadAffinityMask);
        if (mask == 0)
        {
            return FALSE;
        }
    }

    result = Rdtsc(eax, edx);

    if (gIsNT)
    {
        SetThreadAffinityMask(hThread, mask);
    }

    return result;
}

WINRING0_API BOOL RdtscPx(PDWORD eax, PDWORD edx, DWORD_PTR processAffinityMask)
{
    BOOL result = FALSE;
    DWORD_PTR processMask = 0;
    DWORD_PTR systemMask = 0;
    HANDLE hProcess = NULL;

    if (gIsNT)
    {
        hProcess = GetCurrentProcess();
        GetProcessAffinityMask(hProcess, &processMask, &systemMask);
        if (!SetProcessAffinityMask(hProcess, processAffinityMask))
        {
            return FALSE;
        }
    }

    result = Rdtsc(eax, edx);

    if (gIsNT)
    {
        SetProcessAffinityMask(hProcess, processMask);
    }

    return result;
}

WINRING0_API BOOL Hlt()
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;

    result = DeviceIoControl(gHandle, IOCTL_OLS_HALT, NULL, 0, NULL, 0, &returnedLength, NULL);

    if (result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

WINRING0_API BOOL HltTx(DWORD_PTR threadAffinityMask)
{
    BOOL result = FALSE;
    DWORD_PTR mask = 0;
    HANDLE hThread = NULL;

    if (gIsNT)
    {
        hThread = GetCurrentThread();
        mask = SetThreadAffinityMask(hThread, threadAffinityMask);
        if (mask == 0)
        {
            return FALSE;
        }
    }

    result = Hlt();

    if (gIsNT)
    {
        SetThreadAffinityMask(hThread, mask);
    }

    return result;
}

WINRING0_API BOOL HltPx(DWORD_PTR processAffinityMask)
{
    BOOL result = FALSE;
    DWORD_PTR processMask = 0;
    DWORD_PTR systemMask = 0;
    HANDLE hProcess = NULL;

    if (gIsNT)
    {
        hProcess = GetCurrentProcess();
        GetProcessAffinityMask(hProcess, &processMask, &systemMask);
        if (!SetProcessAffinityMask(hProcess, processAffinityMask))
        {
            return FALSE;
        }
    }

    result = Hlt();

    if (gIsNT)
    {
        SetProcessAffinityMask(hProcess, processMask);
    }

    return result;
}

WINRING0_API BYTE ReadIoPortByte(WORD port)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    WORD value = 0;

    result = DeviceIoControl(
        gHandle, IOCTL_OLS_READ_IO_PORT_BYTE, &port, sizeof(port), &value, sizeof(value), &returnedLength, NULL);

    return (BYTE)value;
}

WINRING0_API WORD ReadIoPortWord(WORD port)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    WORD value = 0;

    result = DeviceIoControl(
        gHandle, IOCTL_OLS_READ_IO_PORT_WORD, &port, sizeof(port), &value, sizeof(value), &returnedLength, NULL);

    return value;
}

WINRING0_API DWORD ReadIoPortDword(WORD port)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    DWORD port4 = port;
    DWORD value = 0;

    result = DeviceIoControl(gHandle, IOCTL_OLS_READ_IO_PORT_DWORD, &port4,
        sizeof(port4), // required 4 bytes
        &value, sizeof(value), &returnedLength, NULL);

    return value;
}

WINRING0_API BOOL ReadIoPortByteEx(WORD port, PBYTE value)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    WORD val = 0;

    result = DeviceIoControl(
        gHandle, IOCTL_OLS_READ_IO_PORT_BYTE, &port, sizeof(port), &val, sizeof(val), &returnedLength, NULL);

    if (result)
    {
        *value = (BYTE)val;
    }

    if (result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

WINRING0_API BOOL ReadIoPortWordEx(WORD port, PWORD value)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    WORD val = 0;

    result = DeviceIoControl(
        gHandle, IOCTL_OLS_READ_IO_PORT_WORD, &port, sizeof(port), &val, sizeof(val), &returnedLength, NULL);

    if (result)
    {
        *value = val;
    }

    if (result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

WINRING0_API BOOL ReadIoPortDwordEx(WORD port, PDWORD value)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    DWORD val = port;

    result = DeviceIoControl(gHandle, IOCTL_OLS_READ_IO_PORT_DWORD, &val,
        sizeof(val), // required 4 bytes
        &val, sizeof(val), &returnedLength, NULL);

    if (result)
    {
        *value = val;
    }

    if (result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

WINRING0_API VOID WriteIoPortByte(WORD port, BYTE value)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    DWORD length = 0;
    OLS_WRITE_IO_PORT_INPUT inBuf;

    inBuf.CharData = value;
    inBuf.PortNumber = port;
    length = offsetof(OLS_WRITE_IO_PORT_INPUT, CharData) + sizeof(inBuf.CharData);

    result = DeviceIoControl(gHandle, IOCTL_OLS_WRITE_IO_PORT_BYTE, &inBuf, length, NULL, 0, &returnedLength, NULL);
}

WINRING0_API VOID WriteIoPortWord(WORD port, WORD value)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    DWORD length = 0;
    OLS_WRITE_IO_PORT_INPUT inBuf;

    inBuf.ShortData = value;
    inBuf.PortNumber = port;
    length = offsetof(OLS_WRITE_IO_PORT_INPUT, CharData) + sizeof(inBuf.ShortData);

    result = DeviceIoControl(gHandle, IOCTL_OLS_WRITE_IO_PORT_WORD, &inBuf, length, NULL, 0, &returnedLength, NULL);
}

WINRING0_API VOID WriteIoPortDword(WORD port, DWORD value)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    DWORD length = 0;
    OLS_WRITE_IO_PORT_INPUT inBuf;

    inBuf.LongData = value;
    inBuf.PortNumber = port;
    length = offsetof(OLS_WRITE_IO_PORT_INPUT, CharData) + sizeof(inBuf.LongData);

    result = DeviceIoControl(gHandle, IOCTL_OLS_WRITE_IO_PORT_DWORD, &inBuf, length, NULL, 0, &returnedLength, NULL);
}

WINRING0_API BOOL WriteIoPortByteEx(WORD port, BYTE value)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    DWORD returnedLength = 0;
    DWORD length = 0;
    BOOL result = FALSE;
    OLS_WRITE_IO_PORT_INPUT inBuf;

    inBuf.CharData = value;
    inBuf.PortNumber = port;
    length = offsetof(OLS_WRITE_IO_PORT_INPUT, CharData) + sizeof(inBuf.CharData);

    result = DeviceIoControl(gHandle, IOCTL_OLS_WRITE_IO_PORT_BYTE, &inBuf, length, NULL, 0, &returnedLength, NULL);

    if (result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

WINRING0_API BOOL WriteIoPortWordEx(WORD port, WORD value)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    DWORD returnedLength = 0;
    DWORD length = 0;
    BOOL result = FALSE;
    OLS_WRITE_IO_PORT_INPUT inBuf;

    inBuf.ShortData = value;
    inBuf.PortNumber = port;
    length = offsetof(OLS_WRITE_IO_PORT_INPUT, CharData) + sizeof(inBuf.ShortData);

    result = DeviceIoControl(gHandle, IOCTL_OLS_WRITE_IO_PORT_WORD, &inBuf, length, NULL, 0, &returnedLength, NULL);

    if (result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

WINRING0_API BOOL WriteIoPortDwordEx(WORD port, DWORD value)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    DWORD length = 0;
    OLS_WRITE_IO_PORT_INPUT inBuf;

    inBuf.LongData = value;
    inBuf.PortNumber = port;
    length = offsetof(OLS_WRITE_IO_PORT_INPUT, CharData) + sizeof(inBuf.LongData);

    result = DeviceIoControl(gHandle, IOCTL_OLS_WRITE_IO_PORT_DWORD, &inBuf, length, NULL, 0, &returnedLength, NULL);
    if (result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

WINRING0_API BOOL pciConfigRead(DWORD pciAddress, DWORD regAddress, PBYTE value, DWORD size, PDWORD error)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    if (value == NULL)
    {
        return FALSE;
    }
    // alignment check
    if (size == 2 && (regAddress & 1) != 0)
    {
        return FALSE;
    }
    if (size == 4 && (regAddress & 3) != 0)
    {
        return FALSE;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    OLS_READ_PCI_CONFIG_INPUT inBuf;

    inBuf.PciAddress = pciAddress;
    inBuf.PciOffset = regAddress;

    result =
        DeviceIoControl(gHandle, IOCTL_OLS_READ_PCI_CONFIG, &inBuf, sizeof(inBuf), value, size, &returnedLength, NULL);

    if (error != NULL)
    {
        *error = GetLastError();
    }

    if (result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

WINRING0_API BYTE ReadPciConfigByte(DWORD pciAddress, BYTE regAddress)
{
    BYTE ret;
    if (pciConfigRead(pciAddress, regAddress, (PBYTE)&ret, sizeof(ret), NULL))
    {
        return ret;
    }
    else
    {
        return 0xFF;
    }
}

WINRING0_API WORD ReadPciConfigWord(DWORD pciAddress, BYTE regAddress)
{
    WORD ret;
    if (pciConfigRead(pciAddress, regAddress, (PBYTE)&ret, sizeof(ret), NULL))
    {
        return ret;
    }
    else
    {
        return 0xFFFF;
    }
}

WINRING0_API DWORD ReadPciConfigDword(DWORD pciAddress, BYTE regAddress)
{
    DWORD ret;
    if (pciConfigRead(pciAddress, regAddress, (PBYTE)&ret, sizeof(ret), NULL))
    {
        return ret;
    }
    else
    {
        return 0xFFFFFFFF;
    }
}

WINRING0_API BOOL ReadPciConfigByteEx(DWORD pciAddress, DWORD regAddress, PBYTE value)
{
    return pciConfigRead(pciAddress, regAddress, (PBYTE)value, sizeof(BYTE), NULL);
}

WINRING0_API BOOL ReadPciConfigWordEx(DWORD pciAddress, DWORD regAddress, PWORD value)
{
    return pciConfigRead(pciAddress, regAddress, (PBYTE)value, sizeof(WORD), NULL);
}

WINRING0_API BOOL ReadPciConfigDwordEx(DWORD pciAddress, DWORD regAddress, PDWORD value)
{
    return pciConfigRead(pciAddress, regAddress, (PBYTE)value, sizeof(DWORD), NULL);
}

WINRING0_API BOOL pciConfigWrite(DWORD pciAddress, DWORD regAddress, PBYTE value, DWORD size)
{
    DWORD returnedLength = 0;
    BOOL result = FALSE;
    int inputSize = 0;
    OLS_WRITE_PCI_CONFIG_INPUT* inBuf;

    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    if (value == NULL)
    {
        return FALSE;
    }
    // alignment check
    if (size == 2 && (regAddress & 1) != 0)
    {
        return FALSE;
    }
    if (size == 4 && (regAddress & 3) != 0)
    {
        return FALSE;
    }

    inputSize = offsetof(OLS_WRITE_PCI_CONFIG_INPUT, Data) + size;
    inBuf = (OLS_WRITE_PCI_CONFIG_INPUT*)malloc(inputSize);
    if (inBuf == NULL)
    {
        return FALSE;
    }
    memcpy(inBuf->Data, value, size);
    inBuf->PciAddress = pciAddress;
    inBuf->PciOffset = regAddress;
    result = DeviceIoControl(gHandle, IOCTL_OLS_WRITE_PCI_CONFIG, inBuf, inputSize, NULL, 0, &returnedLength, NULL);
    free(inBuf);

    if (result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

WINRING0_API VOID WritePciConfigByte(DWORD pciAddress, BYTE regAddress, BYTE value)
{
    pciConfigWrite(pciAddress, regAddress, (PBYTE)&value, sizeof(value));
}

WINRING0_API VOID WritePciConfigWord(DWORD pciAddress, BYTE regAddress, WORD value)
{
    pciConfigWrite(pciAddress, regAddress, (PBYTE)&value, sizeof(value));
}

WINRING0_API VOID WritePciConfigDword(DWORD pciAddress, BYTE regAddress, DWORD value)
{
    pciConfigWrite(pciAddress, regAddress, (PBYTE)&value, sizeof(value));
}

WINRING0_API BOOL WritePciConfigByteEx(DWORD pciAddress, DWORD regAddress, BYTE value)
{
    return pciConfigWrite(pciAddress, regAddress, (PBYTE)&value, sizeof(value));
}

WINRING0_API BOOL WritePciConfigWordEx(DWORD pciAddress, DWORD regAddress, WORD value)
{
    return pciConfigWrite(pciAddress, regAddress, (PBYTE)&value, sizeof(value));
}

WINRING0_API BOOL WritePciConfigDwordEx(DWORD pciAddress, DWORD regAddress, DWORD value)
{
    return pciConfigWrite(pciAddress, regAddress, (PBYTE)&value, sizeof(value));
}

WINRING0_API VOID SetPciMaxBusIndex(BYTE max)
{
    gPciMaxNumberOfBus = max;
}

WINRING0_API DWORD FindPciDeviceById(WORD vendorId, WORD deviceId, BYTE index)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return 0xFFFFFFFF;
    }

    DWORD bus = 0, dev = 0, func = 0;
    DWORD count = 0;
    DWORD pciAddress = 0xFFFFFFFF;
    DWORD id = 0;
    DWORD error = 0;
    BOOL multiFuncFlag = FALSE;
    BYTE type = 0;
    count = 0;

    if (vendorId == 0xFFFF)
    {
        return 0xFFFFFFFF;
    }

    for (bus = 0; bus <= gPciMaxNumberOfBus; bus++)
    {
        for (dev = 0; dev < gPciNumberOfDevice; dev++)
        {
            multiFuncFlag = FALSE;
            for (func = 0; func < gPciNumberOfFunction; func++)
            {
                if (multiFuncFlag == 0 && func > 0)
                {
                    break;
                }

                pciAddress = PciBusDevFunc(bus, dev, func);
                if (pciConfigRead(pciAddress, 0, (PBYTE)&id, sizeof(id), &error))
                {
                    if (func == 0) // Is Multi Function Device
                    {
                        if (pciConfigRead(pciAddress, 0x0E, (PBYTE)&type, sizeof(type), NULL))
                        {
                            if (type & 0x80)
                            {
                                multiFuncFlag = TRUE;
                            }
                        }
                    }

                    if (id == (vendorId | ((DWORD)deviceId << 16)))
                    {
                        if (count == index)
                        {
                            return pciAddress;
                        }
                        count++;
                        continue;
                    }
                }
                /*
                else if(error == OLS_ERROR_PCI_BUS_NOT_EXIST)
                {
                    break;
                }
                else if(error == OLS_ERROR_PCI_NO_DEVICE && func == 0)
                {
                    break;
                }
                */
            }
        }
    }
    return 0xFFFFFFFF;
}

WINRING0_API DWORD FindPciDeviceByClass(BYTE baseClass, BYTE subClass, BYTE programIf, BYTE index)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return 0xFFFFFFFF;
    }

    DWORD bus = 0, dev = 0, func = 0;
    DWORD count = 0;
    DWORD pciAddress = 0xFFFFFFFF;
    DWORD conf[3] = {0};
    DWORD error = 0;
    BOOL multiFuncFlag = FALSE;
    BYTE type = 0;
    count = 0;

    for (bus = 0; bus <= gPciMaxNumberOfBus; bus++)
    {
        for (dev = 0; dev < gPciNumberOfDevice; dev++)
        {
            multiFuncFlag = FALSE;
            for (func = 0; func < gPciNumberOfFunction; func++)
            {
                if (multiFuncFlag == FALSE && func > 0)
                {
                    break;
                }
                pciAddress = PciBusDevFunc(bus, dev, func);
                if (pciConfigRead(pciAddress, 0, (BYTE*)conf, sizeof(conf), &error))
                {
                    if (func == 0) // Is Multi Function Device
                    {
                        if (pciConfigRead(pciAddress, 0x0E, (BYTE*)&type, sizeof(type), NULL))
                        {
                            if (type & 0x80)
                            {
                                multiFuncFlag = TRUE;
                            }
                        }
                    }
                    if ((conf[2] & 0xFFFFFF00) ==
                        (((DWORD)baseClass << 24) | ((DWORD)subClass << 16) | ((DWORD)programIf << 8)))
                    {
                        if (count == index)
                        {
                            return pciAddress;
                        }
                        count++;
                        continue;
                    }
                }
                /*
                else if(error == OLS_ERROR_PCI_BUS_NOT_EXIST)
                {
                    break;
                }
                else if(error == OLS_ERROR_PCI_NO_DEVICE && func == 0)
                {
                    break;
                }
                */
            }
        }
    }
    return 0xFFFFFFFF;
}

#ifdef _PHYSICAL_MEMORY_SUPPORT
WINRING0_API DWORD ReadDmiMemory(PBYTE buffer, DWORD count, DWORD unitSize)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    if (buffer == NULL)
    {
        return 0;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    DWORD size = 0;
    OLS_READ_MEMORY_INPUT inBuf;

    if (sizeof(DWORD_PTR) == 4)
    {
        inBuf.Address.HighPart = 0;
        inBuf.Address.LowPart = (DWORD)0x000F0000;
    }
    else
    {
        inBuf.Address.QuadPart = 0x000F0000;
    }

    inBuf.UnitSize = unitSize;
    inBuf.Count = count;
    size = inBuf.UnitSize * inBuf.Count;

    if (size > 65536)
    {
        return 0;
    }

    result = DeviceIoControl(
        gHandle, IOCTL_OLS_READ_MEMORY, &inBuf, sizeof(OLS_READ_MEMORY_INPUT), buffer, size, &returnedLength, NULL);

    if (result && returnedLength == size)
    {
        return count * unitSize;
    }
    else
    {
        return 0;
    }
}

WINRING0_API DWORD ReadPhysicalMemory(DWORD_PTR address, PBYTE buffer, DWORD count, DWORD unitSize)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    if (buffer == NULL)
    {
        return 0;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    DWORD size = 0;
    OLS_READ_MEMORY_INPUT inBuf;

    if (sizeof(DWORD_PTR) == 4)
    {
        inBuf.Address.HighPart = 0;
        inBuf.Address.LowPart = (DWORD)address;
    }
    else
    {
        inBuf.Address.QuadPart = address;
    }

    inBuf.UnitSize = unitSize;
    inBuf.Count = count;
    size = inBuf.UnitSize * inBuf.Count;

    result = DeviceIoControl(
        gHandle, IOCTL_OLS_READ_MEMORY, &inBuf, sizeof(OLS_READ_MEMORY_INPUT), buffer, size, &returnedLength, NULL);

    if (result && returnedLength == size)
    {
        return count * unitSize;
    }
    else
    {
        return 0;
    }
}

WINRING0_API DWORD WritePhysicalMemory(DWORD_PTR address, PBYTE buffer, DWORD count, DWORD unitSize)
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    if (buffer == NULL)
    {
        return 0;
    }

    DWORD returnedLength = 0;
    BOOL result = FALSE;
    DWORD size = 0;
    DWORD rCode = 0;
    OLS_WRITE_MEMORY_INPUT* inBuf;

    size = offsetof(OLS_WRITE_MEMORY_INPUT, Data) + count * unitSize;
    inBuf = (OLS_WRITE_MEMORY_INPUT*)malloc(size);
    if (sizeof(DWORD_PTR) == 4)
    {
        inBuf->Address.HighPart = 0;
        inBuf->Address.LowPart = (DWORD)address;
    }
    else
    {
        inBuf->Address.QuadPart = address;
    }
    inBuf->UnitSize = unitSize;
    inBuf->Count = count;
    memcpy(&inBuf->Data, buffer, count * unitSize);

    result = DeviceIoControl(gHandle, IOCTL_OLS_WRITE_MEMORY, inBuf, size, NULL, 0, &returnedLength, NULL);
    if (result)
    {
        rCode = count * unitSize;
    }
    else
    {
        rCode = 0;
    }

    free(inBuf);

    return rCode;
}
#endif
