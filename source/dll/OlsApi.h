//-----------------------------------------------------------------------------
//     Author : hiyohiyo
//       Mail : hiyohiyo@crystalmark.info
//        Web : http://openlibsys.org/
//    License : The modified BSD license
//
//                     Copyright 2007-2009 OpenLibSys.org. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

#ifdef __cplusplus
#define WINRING0_EXTEREN_C extern "C"
#else
#define WINRING0_EXTEREN_C
#endif

#if defined(WINRING0_STATIC)
#define WINRING0_API
#elif defined(WINRING0_BUILD)
#define WINRING0_API WINRING0_EXTEREN_C __declspec(dllexport)
#else
#define WINRING0_API WINRING0_EXTEREN_C __declspec(dllimport)
#endif

// DLL Status, defined OLS_DLL_****
WINRING0_API DWORD GetDllStatus();

// DLL Version, defined OLS_VERSION
WINRING0_API DWORD GetDllVersion(PBYTE major, // major version
    PBYTE minor,                              // minor version
    PBYTE revision,                           // revision
    PBYTE release                             // release/build
);

// // Device Driver Version, defined OLS_DRIVER_VERSION
WINRING0_API DWORD GetDriverVersion(PBYTE major, // major version
    PBYTE minor,                                 // minor version
    PBYTE revision,                              // revision
    PBYTE release                                // release/build
);

// Device Driver Type, defined OLS_DRIVER_TYPE_****
WINRING0_API DWORD GetDriverType();

// TRUE: success, FALSE: failure
WINRING0_API BOOL InitializeOls();

WINRING0_API VOID DeinitializeOls();

// TRUE: support CPUID instruction, FALSE: not support CPUID instruction
WINRING0_API BOOL IsCpuid();

// TRUE: support MSR(Model-Specific Register), FALSE: not support MSR
WINRING0_API BOOL IsMsr();

// TRUE: support TSC(Time Stamp Counter), FALSE: not support TSC
WINRING0_API BOOL IsTsc();

// TRUE: success, FALSE: failure
WINRING0_API BOOL Rdmsr(DWORD index, // MSR index
    PDWORD eax,                      // bit  0-31
    PDWORD edx                       // bit 32-63
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL RdmsrTx(DWORD index, // MSR index
    PDWORD eax,                        // bit  0-31
    PDWORD edx,                        // bit 32-63
    DWORD_PTR threadAffinityMask);

// TRUE: success, FALSE: failure
WINRING0_API BOOL RdmsrPx(DWORD index, // MSR index
    PDWORD eax,                        // bit  0-31
    PDWORD edx,                        // bit 32-63
    DWORD_PTR processAffinityMask);

// TRUE: success, FALSE: failure
WINRING0_API BOOL Wrmsr(DWORD index, // MSR index
    DWORD eax,                       // bit  0-31
    DWORD edx                        // bit 32-63
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL WrmsrTx(DWORD index, // MSR index
    DWORD eax,                         // bit  0-31
    DWORD edx,                         // bit 32-63
    DWORD_PTR threadAffinityMask);

// TRUE: success, FALSE: failure
WINRING0_API BOOL WrmsrPx(DWORD index, // MSR index
    DWORD eax,                         // bit  0-31
    DWORD edx,                         // bit 32-63
    DWORD_PTR processAffinityMask);

// TRUE: success, FALSE: failure
WINRING0_API BOOL Rdpmc(DWORD index, // PMC index
    PDWORD eax,                      // bit  0-31
    PDWORD edx                       // bit 32-63
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL RdpmcTx(DWORD index, // PMC index
    PDWORD eax,                        // bit  0-31
    PDWORD edx,                        // bit 32-63
    DWORD_PTR threadAffinityMask);

// TRUE: success, FALSE: failure
WINRING0_API BOOL RdpmcPx(DWORD index, // PMC index
    PDWORD eax,                        // bit  0-31
    PDWORD edx,                        // bit 32-63
    DWORD_PTR processAffinityMask);

// TRUE: success, FALSE: failure
WINRING0_API BOOL Cpuid(DWORD index, // CPUID index
    PDWORD eax, PDWORD ebx, PDWORD ecx, PDWORD edx);

// TRUE: success, FALSE: failure
WINRING0_API BOOL CpuidTx(DWORD index, // CPUID index
    PDWORD eax, PDWORD ebx, PDWORD ecx, PDWORD edx, DWORD_PTR threadAffinityMask);

// TRUE: success, FALSE: failure
WINRING0_API BOOL CpuidPx(DWORD index, // CPUID index
    PDWORD eax, PDWORD ebx, PDWORD ecx, PDWORD edx, DWORD_PTR processAffinityMask);

// TRUE: success, FALSE: failure
WINRING0_API BOOL Rdtsc(PDWORD eax, // bit  0-31
    PDWORD edx                      // bit 32-63
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL RdtscTx(PDWORD eax, // bit  0-31
    PDWORD edx,                       // bit 32-63
    DWORD_PTR threadAffinityMask);

// TRUE: success, FALSE: failure
WINRING0_API BOOL RdtscPx(PDWORD eax, // bit  0-31
    PDWORD edx,                       // bit 32-63
    DWORD_PTR processAffinityMask);

// TRUE: success, FALSE: failure
WINRING0_API BOOL Hlt();

// TRUE: success, FALSE: failure
WINRING0_API BOOL HltTx(DWORD_PTR threadAffinityMask);

// TRUE: success, FALSE: failure
WINRING0_API BOOL HltPx(DWORD_PTR processAffinityMask);

// Read Value
WINRING0_API BYTE ReadIoPortByte(WORD port // I/O port address
);

// Read Value
WINRING0_API WORD ReadIoPortWord(WORD port // I/O port address
);

// Read Value
WINRING0_API DWORD ReadIoPortDword(WORD port // I/O port address
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL ReadIoPortByteEx(WORD port, // I/O port address
    PBYTE value                               // Read Value
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL ReadIoPortWordEx(WORD port, // I/O port address
    PWORD value                               // Read Value
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL ReadIoPortDwordEx(WORD port, // I/O port address
    PDWORD value                               // Read Value
);

WINRING0_API VOID WriteIoPortByte(WORD port, // I/O port address
    BYTE value                               // Write Value
);

WINRING0_API VOID WriteIoPortDword(WORD port, // I/O port address
    DWORD value                               // Write Value
);

WINRING0_API VOID WriteIoPortWord(WORD port, // I/O port address
    WORD value                               // Write Value
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL WriteIoPortByteEx(WORD port, // I/O port address
    BYTE value                                 // Write Value
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL WriteIoPortWordEx(WORD port, // I/O port address
    WORD value                                 // Write Value
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL WriteIoPortDwordEx(WORD port, // I/O port address
    DWORD value                                 // Write Value
);

// pciAddress
//  0- 2: Function Number
//  3- 7: Device Number
//  8-15: PCI Bus Number
// 16-31: Reserved
// 0xFFFFFFFF : Error

WINRING0_API VOID SetPciMaxBusIndex(BYTE max // Max PCI Bus to Scan
);

// Read Value
WINRING0_API BYTE ReadPciConfigByte(DWORD pciAddress, // PCI Device Address
    BYTE regAddress                                   // Configuration Address 0-255
);

// Read Value
WINRING0_API WORD ReadPciConfigWord(DWORD pciAddress, // PCI Device Address
    BYTE regAddress                                   // Configuration Address 0-255
);

// Read Value
WINRING0_API DWORD ReadPciConfigDword(DWORD pciAddress, // PCI Device Address
    BYTE regAddress                                     // Configuration Address 0-255
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL ReadPciConfigByteEx(DWORD pciAddress, // PCI Device Address
    DWORD regAddress,                                   // Configuration Address 0-whatever
    PBYTE value                                         // Read Value
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL ReadPciConfigWordEx(DWORD pciAddress, // PCI Device Address
    DWORD regAddress,                                   // Configuration Address 0-whatever
    PWORD value                                         // Read Value
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL ReadPciConfigDwordEx(DWORD pciAddress, // PCI Device Address
    DWORD regAddress,                                    // Configuration Address 0-whatever
    PDWORD value                                         // Read Value
);

WINRING0_API VOID WritePciConfigByte(DWORD pciAddress, // PCI Device Address
    BYTE regAddress,                                   // Configuration Address 0-255
    BYTE value                                         // Write Value
);

WINRING0_API VOID WritePciConfigWord(DWORD pciAddress, // PCI Device Address
    BYTE regAddress,                                   // Configuration Address 0-255
    WORD value                                         // Write Value
);

WINRING0_API VOID WritePciConfigDword(DWORD pciAddress, // PCI Device Address
    BYTE regAddress,                                    // Configuration Address 0-255
    DWORD value                                         // Write Value
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL WritePciConfigByteEx(DWORD pciAddress, // PCI Device Address
    DWORD regAddress,                                    // Configuration Address 0-whatever
    BYTE value                                           // Write Value
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL WritePciConfigWordEx(DWORD pciAddress, // PCI Device Address
    DWORD regAddress,                                    // Configuration Address 0-whatever
    WORD value                                           // Write Value
);

// TRUE: success, FALSE: failure
WINRING0_API BOOL WritePciConfigDwordEx(DWORD pciAddress, // PCI Device Address
    DWORD regAddress,                                     // Configuration Address 0-whatever
    DWORD value                                           // Write Value
);

// pciAddress, 0xFFFFFFFF: failure
WINRING0_API DWORD FindPciDeviceById(WORD vendorId, // Vendor ID
    WORD deviceId,                                  // Device ID
    BYTE index                                      // Index
);

// pciAddress, 0xFFFFFFFF: failure
WINRING0_API DWORD FindPciDeviceByClass(BYTE baseClass, // Base Class
    BYTE subClass,                                      // Sub Class
    BYTE programIf,                                     // Program Interface
    BYTE index                                          // Index
);

#ifdef _PHYSICAL_MEMORY_SUPPORT
// Read size(byte), 0: failure
WINRING0_API DWORD ReadDmiMemory(PBYTE buffer, // Buffer
    DWORD count,                               // Count
    DWORD unitSize                             // Unit Size (BYTE, WORD, DWORD)
);

// ReadPhysicalMemory. Read size(byte), 0: failure
WINRING0_API DWORD ReadPhysicalMemory(DWORD_PTR address, // Physical Memory Address
    PBYTE buffer,                                        // Buffer
    DWORD count,                                         // Count
    DWORD unitSize                                       // Unit Size (BYTE, WORD, DWORD)
);

// Write size(byte), 0: failure
WINRING0_API DWORD WritePhysicalMemory(DWORD_PTR address, // Physical Memory Address
    PBYTE buffer,                                         // Buffer
    DWORD count,                                          // Count
    DWORD unitSize                                        // Unit Size (BYTE, WORD, DWORD)
);
#endif
