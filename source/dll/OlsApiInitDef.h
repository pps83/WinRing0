//-----------------------------------------------------------------------------
//     Author : hiyohiyo
//       Mail : hiyohiyo@crystalmark.info
//        Web : http://openlibsys.org/
//    License : The modified BSD license
//
//                     Copyright 2007-2009 OpenLibSys.org. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

typedef DWORD (*_GetDllStatus)();
typedef DWORD (*_GetDllVersion)(PBYTE major, PBYTE minor, PBYTE revision, PBYTE release);
typedef DWORD (*_GetDriverVersion)(PBYTE major, PBYTE minor, PBYTE revision, PBYTE release);
typedef DWORD (*_GetDriverType)();
typedef DWORD (*_GetRefCount)();

typedef int (*_InitializeOls)();
typedef VOID (*_DeinitializeOls)();

typedef BOOL (*_IsCpuid)();
typedef BOOL (*_IsMsr)();
typedef BOOL (*_IsTsc)();

typedef BOOL (*_Hlt)();
typedef DWORD (*_Rdmsr)(DWORD index, PDWORD eax, PDWORD edx);
typedef DWORD (*_Wrmsr)(DWORD index, DWORD eax, DWORD edx);
typedef DWORD (*_Rdpmc)(DWORD index, PDWORD eax, PDWORD edx);
typedef DWORD (*_Cpuid)(DWORD index, PDWORD eax, PDWORD ebx, PDWORD ecx, PDWORD edx);
typedef DWORD (*_Rdtsc)(PDWORD eax, PDWORD edx);

typedef BOOL (*_HltTx)(DWORD_PTR threadAffinityMask);
typedef DWORD (*_RdmsrTx)(DWORD index, PDWORD eax, PDWORD edx, DWORD_PTR threadAffinityMask);
typedef DWORD (*_WrmsrTx)(DWORD index, DWORD eax, DWORD edx, DWORD_PTR threadAffinityMask);
typedef DWORD (*_RdpmcTx)(DWORD index, PDWORD eax, PDWORD edx, DWORD_PTR threadAffinityMask);
typedef DWORD (*_CpuidTx)(DWORD index, PDWORD eax, PDWORD ebx, PDWORD ecx, PDWORD edx, DWORD_PTR threadAffinityMask);
typedef DWORD (*_RdtscTx)(PDWORD eax, PDWORD edx, DWORD_PTR threadAffinityMask);

typedef BOOL (*_HltPx)(DWORD_PTR processAffinityMask);
typedef DWORD (*_RdmsrPx)(DWORD index, PDWORD eax, PDWORD edx, DWORD_PTR processAffinityMask);
typedef DWORD (*_WrmsrPx)(DWORD index, DWORD eax, DWORD edx, DWORD_PTR processAffinityMask);
typedef DWORD (*_RdpmcPx)(DWORD index, PDWORD eax, PDWORD edx, DWORD_PTR processAffinityMask);
typedef DWORD (*_CpuidPx)(DWORD index, PDWORD eax, PDWORD ebx, PDWORD ecx, PDWORD edx, DWORD_PTR processAffinityMask);
typedef DWORD (*_RdtscPx)(PDWORD eax, PDWORD edx, DWORD_PTR processAffinityMask);

typedef BYTE (*_ReadIoPortByte)(WORD address);
typedef WORD (*_ReadIoPortWord)(WORD address);
typedef DWORD (*_ReadIoPortDword)(WORD address);

typedef BOOL (*_ReadIoPortByteEx)(WORD address, PBYTE value);
typedef BOOL (*_ReadIoPortWordEx)(WORD address, PWORD value);
typedef BOOL (*_ReadIoPortDwordEx)(WORD address, PDWORD value);

typedef VOID (*_WriteIoPortByte)(WORD address, BYTE value);
typedef VOID (*_WriteIoPortWord)(WORD address, WORD value);
typedef VOID (*_WriteIoPortDword)(WORD address, DWORD value);

typedef BOOL (*_WriteIoPortByteEx)(WORD address, BYTE value);
typedef BOOL (*_WriteIoPortWordEx)(WORD address, WORD value);
typedef BOOL (*_WriteIoPortDwordEx)(WORD address, DWORD value);

typedef VOID (*_SetPciMaxBusIndex)(BYTE max);

typedef BYTE (*_ReadPciConfigByte)(DWORD pciAddress, BYTE regAddress);
typedef WORD (*_ReadPciConfigWord)(DWORD pciAddress, BYTE regAddress);
typedef DWORD (*_ReadPciConfigDword)(DWORD pciAddress, BYTE regAddress);

typedef BOOL (*_ReadPciConfigByteEx)(DWORD pciAddress, DWORD regAddress, PBYTE value);
typedef BOOL (*_ReadPciConfigWordEx)(DWORD pciAddress, DWORD regAddress, PWORD value);
typedef BOOL (*_ReadPciConfigDwordEx)(DWORD pciAddress, DWORD regAddress, PDWORD value);

typedef VOID (*_WritePciConfigByte)(DWORD pciAddress, BYTE regAddress, BYTE value);
typedef VOID (*_WritePciConfigWord)(DWORD pciAddress, BYTE regAddress, WORD value);
typedef VOID (*_WritePciConfigDword)(DWORD pciAddress, BYTE regAddress, DWORD value);

typedef BOOL (*_WritePciConfigByteEx)(DWORD pciAddress, DWORD regAddress, BYTE value);
typedef BOOL (*_WritePciConfigWordEx)(DWORD pciAddress, DWORD regAddress, WORD value);
typedef BOOL (*_WritePciConfigDwordEx)(DWORD pciAddress, DWORD regAddress, DWORD value);

typedef DWORD (*_FindPciDeviceById)(WORD vendorId, WORD deviceId, BYTE index);
typedef DWORD (*_FindPciDeviceByClass)(BYTE baseClass, BYTE subClass, BYTE programIf, BYTE index);

#ifdef _PHYSICAL_MEMORY_SUPPORT
typedef DWORD (*_ReadDmiMemory)(PBYTE buffer, DWORD count, DWORD unitSize);
typedef DWORD (*_ReadPhysicalMemory)(DWORD_PTR address, PBYTE buffer, DWORD count, DWORD unitSize);
typedef DWORD (*_WritePhysicalMemory)(DWORD_PTR address, PBYTE buffer, DWORD count, DWORD unitSize);
#endif
