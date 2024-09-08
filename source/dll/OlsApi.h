//-----------------------------------------------------------------------------
//     Author : hiyohiyo
//       Mail : hiyohiyo@crystalmark.info
//        Web : http://openlibsys.org/
//    License : The modified BSD license
//
//                     Copyright 2007-2009 OpenLibSys.org. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

// DLL Status, defined OLS_DLL_****
DWORD WINAPI GetDllStatus();

// DLL Version, defined OLS_VERSION
DWORD WINAPI GetDllVersion(PBYTE major, // major version
    PBYTE minor,                        // minor version
    PBYTE revision,                     // revision
    PBYTE release                       // release/build
);

// // Device Driver Version, defined OLS_DRIVER_VERSION
DWORD WINAPI GetDriverVersion(PBYTE major, // major version
    PBYTE minor,                           // minor version
    PBYTE revision,                        // revision
    PBYTE release                          // release/build
);

// Device Driver Type, defined OLS_DRIVER_TYPE_****
DWORD WINAPI GetDriverType();

// TRUE: success, FALSE: failure
BOOL WINAPI InitializeOls();

VOID WINAPI DeinitializeOls();

// TRUE: support CPUID instruction, FALSE: not support CPUID instruction
BOOL WINAPI IsCpuid();

// TRUE: support MSR(Model-Specific Register), FALSE: not support MSR
BOOL WINAPI IsMsr();

// TRUE: support TSC(Time Stamp Counter), FALSE: not support TSC
BOOL WINAPI IsTsc();

// TRUE: success, FALSE: failure
BOOL WINAPI Rdmsr(DWORD index, // MSR index
    PDWORD eax,                // bit  0-31
    PDWORD edx                 // bit 32-63
);

// TRUE: success, FALSE: failure
BOOL WINAPI RdmsrTx(DWORD index, // MSR index
    PDWORD eax,                  // bit  0-31
    PDWORD edx,                  // bit 32-63
    DWORD_PTR threadAffinityMask);

// TRUE: success, FALSE: failure
BOOL WINAPI RdmsrPx(DWORD index, // MSR index
    PDWORD eax,                  // bit  0-31
    PDWORD edx,                  // bit 32-63
    DWORD_PTR processAffinityMask);

// TRUE: success, FALSE: failure
BOOL WINAPI Wrmsr(DWORD index, // MSR index
    DWORD eax,                 // bit  0-31
    DWORD edx                  // bit 32-63
);

// TRUE: success, FALSE: failure
BOOL WINAPI WrmsrTx(DWORD index, // MSR index
    DWORD eax,                   // bit  0-31
    DWORD edx,                   // bit 32-63
    DWORD_PTR threadAffinityMask);

// TRUE: success, FALSE: failure
BOOL WINAPI WrmsrPx(DWORD index, // MSR index
    DWORD eax,                   // bit  0-31
    DWORD edx,                   // bit 32-63
    DWORD_PTR processAffinityMask);

// TRUE: success, FALSE: failure
BOOL WINAPI Rdpmc(DWORD index, // PMC index
    PDWORD eax,                // bit  0-31
    PDWORD edx                 // bit 32-63
);

// TRUE: success, FALSE: failure
BOOL WINAPI RdpmcTx(DWORD index, // PMC index
    PDWORD eax,                  // bit  0-31
    PDWORD edx,                  // bit 32-63
    DWORD_PTR threadAffinityMask);

// TRUE: success, FALSE: failure
BOOL WINAPI RdpmcPx(DWORD index, // PMC index
    PDWORD eax,                  // bit  0-31
    PDWORD edx,                  // bit 32-63
    DWORD_PTR processAffinityMask);

// TRUE: success, FALSE: failure
BOOL WINAPI Cpuid(DWORD index, // CPUID index
    PDWORD eax, PDWORD ebx, PDWORD ecx, PDWORD edx);

// TRUE: success, FALSE: failure
BOOL WINAPI CpuidTx(DWORD index, // CPUID index
    PDWORD eax, PDWORD ebx, PDWORD ecx, PDWORD edx, DWORD_PTR threadAffinityMask);

// TRUE: success, FALSE: failure
BOOL WINAPI CpuidPx(DWORD index, // CPUID index
    PDWORD eax, PDWORD ebx, PDWORD ecx, PDWORD edx, DWORD_PTR processAffinityMask);

// TRUE: success, FALSE: failure
BOOL WINAPI Rdtsc(PDWORD eax, // bit  0-31
    PDWORD edx                // bit 32-63
);

// TRUE: success, FALSE: failure
BOOL WINAPI RdtscTx(PDWORD eax, // bit  0-31
    PDWORD edx,                 // bit 32-63
    DWORD_PTR threadAffinityMask);

// TRUE: success, FALSE: failure
BOOL WINAPI RdtscPx(PDWORD eax, // bit  0-31
    PDWORD edx,                 // bit 32-63
    DWORD_PTR processAffinityMask);

// TRUE: success, FALSE: failure
BOOL WINAPI Hlt();

// TRUE: success, FALSE: failure
BOOL WINAPI HltTx(DWORD_PTR threadAffinityMask);

// TRUE: success, FALSE: failure
BOOL WINAPI HltPx(DWORD_PTR processAffinityMask);

// Read Value
BYTE WINAPI ReadIoPortByte(WORD port // I/O port address
);

// Read Value
WORD WINAPI ReadIoPortWord(WORD port // I/O port address
);

// Read Value
DWORD WINAPI ReadIoPortDword(WORD port // I/O port address
);

// TRUE: success, FALSE: failure
BOOL WINAPI ReadIoPortByteEx(WORD port, // I/O port address
    PBYTE value                         // Read Value
);

// TRUE: success, FALSE: failure
BOOL WINAPI ReadIoPortWordEx(WORD port, // I/O port address
    PWORD value                         // Read Value
);

// TRUE: success, FALSE: failure
BOOL WINAPI ReadIoPortDwordEx(WORD port, // I/O port address
    PDWORD value                         // Read Value
);

VOID WINAPI WriteIoPortByte(WORD port, // I/O port address
    BYTE value                         // Write Value
);

VOID WINAPI WriteIoPortDword(WORD port, // I/O port address
    DWORD value                         // Write Value
);

VOID WINAPI WriteIoPortWord(WORD port, // I/O port address
    WORD value                         // Write Value
);

// TRUE: success, FALSE: failure
BOOL WINAPI WriteIoPortByteEx(WORD port, // I/O port address
    BYTE value                           // Write Value
);

// TRUE: success, FALSE: failure
BOOL WINAPI WriteIoPortWordEx(WORD port, // I/O port address
    WORD value                           // Write Value
);

// TRUE: success, FALSE: failure
BOOL WINAPI WriteIoPortDwordEx(WORD port, // I/O port address
    DWORD value                           // Write Value
);

// pciAddress
//  0- 2: Function Number
//  3- 7: Device Number
//  8-15: PCI Bus Number
// 16-31: Reserved
// 0xFFFFFFFF : Error

VOID WINAPI SetPciMaxBusIndex(BYTE max // Max PCI Bus to Scan
);

// Read Value
BYTE WINAPI ReadPciConfigByte(DWORD pciAddress, // PCI Device Address
    BYTE regAddress                             // Configuration Address 0-255
);

// Read Value
WORD WINAPI ReadPciConfigWord(DWORD pciAddress, // PCI Device Address
    BYTE regAddress                             // Configuration Address 0-255
);

// Read Value
DWORD WINAPI ReadPciConfigDword(DWORD pciAddress, // PCI Device Address
    BYTE regAddress                               // Configuration Address 0-255
);

// TRUE: success, FALSE: failure
BOOL WINAPI ReadPciConfigByteEx(DWORD pciAddress, // PCI Device Address
    DWORD regAddress,                             // Configuration Address 0-whatever
    PBYTE value                                   // Read Value
);

// TRUE: success, FALSE: failure
BOOL WINAPI ReadPciConfigWordEx(DWORD pciAddress, // PCI Device Address
    DWORD regAddress,                             // Configuration Address 0-whatever
    PWORD value                                   // Read Value
);

// TRUE: success, FALSE: failure
BOOL WINAPI ReadPciConfigDwordEx(DWORD pciAddress, // PCI Device Address
    DWORD regAddress,                              // Configuration Address 0-whatever
    PDWORD value                                   // Read Value
);

VOID WINAPI WritePciConfigByte(DWORD pciAddress, // PCI Device Address
    BYTE regAddress,                             // Configuration Address 0-255
    BYTE value                                   // Write Value
);

VOID WINAPI WritePciConfigWord(DWORD pciAddress, // PCI Device Address
    BYTE regAddress,                             // Configuration Address 0-255
    WORD value                                   // Write Value
);

VOID WINAPI WritePciConfigDword(DWORD pciAddress, // PCI Device Address
    BYTE regAddress,                              // Configuration Address 0-255
    DWORD value                                   // Write Value
);

// TRUE: success, FALSE: failure
BOOL WINAPI WritePciConfigByteEx(DWORD pciAddress, // PCI Device Address
    DWORD regAddress,                              // Configuration Address 0-whatever
    BYTE value                                     // Write Value
);

// TRUE: success, FALSE: failure
BOOL WINAPI WritePciConfigWordEx(DWORD pciAddress, // PCI Device Address
    DWORD regAddress,                              // Configuration Address 0-whatever
    WORD value                                     // Write Value
);

// TRUE: success, FALSE: failure
BOOL WINAPI WritePciConfigDwordEx(DWORD pciAddress, // PCI Device Address
    DWORD regAddress,                               // Configuration Address 0-whatever
    DWORD value                                     // Write Value
);

// pciAddress, 0xFFFFFFFF: failure
DWORD WINAPI FindPciDeviceById(WORD vendorId, // Vendor ID
    WORD deviceId,                            // Device ID
    BYTE index                                // Index
);

// pciAddress, 0xFFFFFFFF: failure
DWORD WINAPI FindPciDeviceByClass(BYTE baseClass, // Base Class
    BYTE subClass,                                // Sub Class
    BYTE programIf,                               // Program Interface
    BYTE index                                    // Index
);

#ifdef _PHYSICAL_MEMORY_SUPPORT
// Read size(byte), 0: failure
DWORD WINAPI ReadDmiMemory(PBYTE buffer, // Buffer
    DWORD count,                         // Count
    DWORD unitSize                       // Unit Size (BYTE, WORD, DWORD)
);

// ReadPhysicalMemory. Read size(byte), 0: failure
DWORD WINAPI ReadPhysicalMemory(DWORD_PTR address, // Physical Memory Address
    PBYTE buffer,                                  // Buffer
    DWORD count,                                   // Count
    DWORD unitSize                                 // Unit Size (BYTE, WORD, DWORD)
);

// Write size(byte), 0: failure
DWORD WINAPI WritePhysicalMemory(DWORD_PTR address, // Physical Memory Address
    PBYTE buffer,                                   // Buffer
    DWORD count,                                    // Count
    DWORD unitSize                                  // Unit Size (BYTE, WORD, DWORD)
);
#endif
