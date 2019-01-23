// File author is Ítalo Lima Marconato Matias
//
// Created on June 28 of 2018, at 18:48 BRT
// Last edited on January 22 of 2019, at 19:35 BRT

#ifndef __CHICAGO_MM_H__
#define __CHICAGO_MM_H__

#include <chicago/types.h>

#ifndef MM_PAGE_SIZE
#define MM_PAGE_SIZE 0x1000
#endif

#ifndef MM_USER_START
#define MM_USER_START 0x00000000
#endif

#ifndef MM_USER_END
#ifdef ARCH_64
#define MM_USER_END 0xFFFF800000000000
#else
#define MM_USER_END 0xC0000000
#endif
#endif

#define MM_MAP_USER 0x01
#define MM_MAP_KERNEL 0x02
#define MM_MAP_READ 0x04
#define MM_MAP_WRITE 0x08
#define MM_MAP_EXEC 0x10
#define MM_MAP_KDEF (MM_MAP_KERNEL | MM_MAP_READ | MM_MAP_WRITE)
#define MM_MAP_UDEF (MM_MAP_USER | MM_MAP_READ | MM_MAP_WRITE)

#define MmKernelDirectory (MmGetPhys(((UIntPtr)(&MmKernelDirectoryInt))))

extern UIntPtr MmKernelDirectoryInt;

UIntPtr MmBootAlloc(UIntPtr size, Boolean align);

UIntPtr MmAllocPage(Void);
UIntPtr MmReferencePage(UIntPtr addr);
Void MmFreePage(UIntPtr addr);
Void MmDereferencePage(UIntPtr addr);
UIntPtr MmGetReferences(UIntPtr addr);
UIntPtr MmGetSize(Void);
UIntPtr MmGetUsage(Void);
UIntPtr MmGetFree(Void);

UIntPtr MmGetPhys(UIntPtr virt);
UInt32 MmQuery(UIntPtr virt);
UIntPtr MmFindFreeVirt(UIntPtr start, UIntPtr end, UIntPtr count);
UIntPtr MmFindHighestFreeVirt(UIntPtr start, UIntPtr end, UIntPtr count);
UIntPtr MmMapTemp(UIntPtr phys, UInt32 flags);
Boolean MmMap(UIntPtr virt, UIntPtr phys, UInt32 flags);
Boolean MmUnmap(UIntPtr virt);
UIntPtr MmCreateDirectory(Void);
Void MmFreeDirectory(UIntPtr dir);
UIntPtr MmGetCurrentDirectory(Void);
Void MmSwitchDirectory(UIntPtr dir);

UIntPtr MmAllocUserMemory(UIntPtr size);
Void MmFreeUserMemory(UIntPtr block);
UIntPtr MmReallocUserMemory(UIntPtr block, UIntPtr size);

#endif		// __CHICAGO_MM_H__
