//-----------------------------------------------------------------------------
//     Author : hiyohiyo
//       Mail : hiyohiyo@crystalmark.info
//        Web : http://openlibsys.org/
//    License : The modified BSD license
//
//                          Copyright 2007 OpenLibSys.org. All rights reserved.
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include <tchar.h>
#include <winioctl.h>
#include <intrin.h>

#include "OlsApi.h"
#include "OlsIoctl.h"
#include "OlsDll.h"
#include "OlsDef.h"
#include "Driver.h"

HANDLE gHandle = INVALID_HANDLE_VALUE;

BOOL gIsNT = FALSE;
BOOL gIsCpuid = FALSE;
BOOL gIsMsr = FALSE;
BOOL gIsTsc = FALSE;
BOOL gInitDll = FALSE;

TCHAR gDriverFileName[MAX_PATH];
TCHAR gDriverPath[MAX_PATH];
DWORD gDllStatus = OLS_DLL_UNKNOWN_ERROR;
DWORD gDriverType = OLS_DRIVER_TYPE_UNKNOWN;

static BOOL IsNT();
static BOOL IsCpuid_impl();
static BOOL IsMsr_impl();
static BOOL IsTsc_impl();
static BOOL IsWow64();
static BOOL IsX64();
static BOOL IsFileExist(LPCTSTR fileName);
static BOOL IsOnNetworkDrive(LPCTSTR fileName);

WINRING0_API BOOL InitializeOls()
{
    if (gInitDll == FALSE)
    {
        gIsNT = IsNT();
        gIsCpuid = IsCpuid_impl();
        if (gIsCpuid)
        {
            gIsMsr = IsMsr_impl();
            gIsTsc = IsTsc_impl();
        }
        gDllStatus = InitDriverInfo();

        if (gDllStatus == OLS_DLL_NO_ERROR)
        {
            // Retry, Max 1000ms
            for (int i = 0; i < 4; i++)
            {
                gDllStatus = Initialize();
                if (gDllStatus == OLS_DLL_NO_ERROR)
                {
                    break;
                }
                Sleep(100 * i);
            }
        }
        gInitDll = TRUE;
    }
    return (BOOL)(gDllStatus == OLS_DLL_NO_ERROR);
}

WINRING0_API VOID DeinitializeOls()
{
    if (gInitDll == TRUE)
    {
        if (gIsNT && GetRefCount() == 1)
        {
            CloseHandle(gHandle);
            gHandle = INVALID_HANDLE_VALUE;
            ManageDriver(OLS_DRIVER_ID, gDriverPath, OLS_DRIVER_REMOVE);
        }

        if (gHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(gHandle);
            gHandle = INVALID_HANDLE_VALUE;
        }
        gInitDll = FALSE;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        DeinitializeOls();
        break;
    }
    return TRUE;
}

DWORD Initialize()
{
    TCHAR dir[MAX_PATH];
    TCHAR* ptr;

    GetModuleFileName(NULL, dir, MAX_PATH);
    if ((ptr = _tcsrchr(dir, '\\')) != NULL)
    {
        *ptr = '\0';
    }
    wsprintf(gDriverPath, _T("%s\\%s"), dir, gDriverFileName);

    if (IsFileExist(gDriverPath) == FALSE)
    {
        return OLS_DLL_DRIVER_NOT_FOUND;
    }

    if (IsOnNetworkDrive(gDriverPath) == TRUE)
    {
        return OLS_DLL_DRIVER_NOT_LOADED_ON_NETWORK;
    }

    if (gIsNT)
    {
        if (OpenDriver())
        {
            return OLS_DLL_NO_ERROR;
        }

        ManageDriver(OLS_DRIVER_ID, gDriverPath, OLS_DRIVER_REMOVE);
        if (!ManageDriver(OLS_DRIVER_ID, gDriverPath, OLS_DRIVER_INSTALL))
        {
            ManageDriver(OLS_DRIVER_ID, gDriverPath, OLS_DRIVER_REMOVE);
            return OLS_DLL_DRIVER_NOT_LOADED;
        }

        if (OpenDriver())
        {
            return OLS_DLL_NO_ERROR;
        }
        return OLS_DLL_DRIVER_NOT_LOADED;
    }
    else
    {
        gHandle = INVALID_HANDLE_VALUE;

        if (gHandle == INVALID_HANDLE_VALUE)
        {
            return OLS_DLL_DRIVER_NOT_LOADED;
        }
        return OLS_DLL_NO_ERROR;
    }
}

BOOL OpenDriver()
{
    gHandle = CreateFile(
        _T("\\\\.\\") OLS_DRIVER_ID, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    return TRUE;
}

DWORD GetRefCount()
{
    if (gHandle == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    DWORD refCount;
    DWORD length, result;

    refCount = 0;

    result = DeviceIoControl(gHandle, IOCTL_OLS_GET_REFCOUNT, NULL, 0, &refCount, sizeof(refCount), &length, NULL);
    if (!result)
    {
        refCount = 0;
    }

    return refCount;
}

DWORD InitDriverInfo()
{
    OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);

    if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
#if defined(_M_X64)
        _tcscpy_s(gDriverFileName, MAX_PATH, OLS_DRIVER_FILE_NAME_WIN_NT_X64);
        gDriverType = OLS_DRIVER_TYPE_WIN_NT_X64;
        return OLS_DLL_NO_ERROR;
#elif defined(_M_IX86)
        if (IsWow64() && IsX64())
        {
            _tcscpy_s(gDriverFileName, MAX_PATH, OLS_DRIVER_FILE_NAME_WIN_NT_X64);
            gDriverType = OLS_DRIVER_TYPE_WIN_NT_X64;
        }
        else
        {
            _tcscpy_s(gDriverFileName, MAX_PATH, OLS_DRIVER_FILE_NAME_WIN_NT);
            gDriverType = OLS_DRIVER_TYPE_WIN_NT;
        }
        return OLS_DLL_NO_ERROR;
#else
#error unsupported build target
#endif
    }
    gDriverType = OLS_DRIVER_TYPE_UNKNOWN;
    return OLS_DLL_UNKNOWN_ERROR;
}

static BOOL IsCpuid_impl()
{
    __try
    {
        int info[4];
        __cpuid(info, 0x0);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return FALSE;
    }

    return TRUE;
}

static BOOL IsMsr_impl()
{
    // MSR : Standard Feature Flag EDX, Bit 5
    int info[4];
    __cpuid(info, 0x1);

    return ((info[3] >> 5) & 1);
}

static BOOL IsTsc_impl()
{
    // TSC : Standard Feature Flag EDX, Bit 4
    int info[4];
    __cpuid(info, 0x1);

    return ((info[3] >> 4) & 1);
}

static BOOL IsNT()
{
    OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);

    return (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT);
}

typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS)(HANDLE hProcess, PBOOL Wow64Process);

static BOOL IsWow64()
{
    BOOL isWow64 = FALSE;
    LPFN_ISWOW64PROCESS fnIsWow64Process =
        (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process");

    if (fnIsWow64Process != NULL)
    {
        if (!fnIsWow64Process(GetCurrentProcess(), &isWow64))
        {
            // handle error
            isWow64 = FALSE;
        }
    }
    return isWow64;
}

typedef void(WINAPI* LPFN_GETNATIVESYSTEMINFO)(LPSYSTEM_INFO lpSystemInfo);

static BOOL IsX64()
{
    SYSTEM_INFO systemInfo;
    BOOL isX64 = FALSE;
    LPFN_GETNATIVESYSTEMINFO fnGetNativeSystemInfo =
        (LPFN_GETNATIVESYSTEMINFO)GetProcAddress(GetModuleHandle(_T("kernel32")), "GetNativeSystemInfo");

    if (fnGetNativeSystemInfo != NULL)
    {
        fnGetNativeSystemInfo(&systemInfo);
        isX64 = (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64);
    }
    return isX64;
}

static BOOL IsFileExist(LPCTSTR fileName)
{
    WIN32_FIND_DATA findData;

    HANDLE hFile = FindFirstFile(fileName, &findData);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        FindClose(hFile);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL IsOnNetworkDrive(LPCTSTR fileName)
{
    TCHAR root[4];
    root[0] = fileName[0];
    root[1] = ':';
    root[2] = '\\';
    root[3] = '\0';

    if (root[0] == '\\' || GetDriveType((LPCTSTR)root) == DRIVE_REMOTE)
    {
        return TRUE;
    }

    return FALSE;
}
