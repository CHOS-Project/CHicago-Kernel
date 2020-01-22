// File author is Ítalo Lima Marconato Matias
//
// Created on June 28 of 2018, at 18:48 BRT
// Last edited on January 21 of 2020, at 12:25 BRT

#ifndef __CHICAGO_MM_H__
#define __CHICAGO_MM_H__

#include <chicago/status.h>

#ifndef MM_PAGE_SIZE
#define MM_PAGE_SIZE 0x1000
#endif

#ifndef MM_USER_START
#define MM_USER_START 0
#endif

#ifndef MM_USER_END
#if __INTPTR_WIDTH__ == 64
#define MM_USER_END 0x800000000000
#else
#define MM_USER_END 0xC0000000
#endif
#endif

#define MM_MAP_USER 0x01
#define MM_MAP_KERNEL 0x02
#define MM_MAP_READ 0x04
#define MM_MAP_WRITE 0x08
#define MM_MAP_EXEC 0x10
#define MM_MAP_AOR 0x20
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
Status MmMap(UIntPtr virt, UIntPtr phys, UInt32 flags);
Status MmUnmap(UIntPtr virt);
Status MmPrepareMapFile(UIntPtr start, UIntPtr end);
Status MmPrepareUnmapFile(UIntPtr start, UIntPtr end);
UIntPtr MmCreateDirectory(Void);
Void MmFreeDirectory(UIntPtr dir);
UIntPtr MmGetCurrentDirectory(Void);
Void MmSwitchDirectory(UIntPtr dir);

#endif		// __CHICAGO_MM_H__
