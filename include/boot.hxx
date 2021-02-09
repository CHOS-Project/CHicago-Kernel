/* File author is Ítalo Lima Marconato Matias
 *
 * Created on February 05 of 2021, at 20:33 BRT
 * Last edited on February 09 of 2021, at 18:34 BRT */

#pragma once

#include <types.hxx>

#define BOOT_INFO_MAGIC 0xC4057D41

#define BOOT_INFO_MEM_KCODE 0x00
#define BOOT_INFO_MEM_KDATA 0x01
#define BOOT_INFO_MEM_KDATA_RO 0x02
#define BOOT_INFO_MEM_MMU 0x03
#define BOOT_INFO_MEM_DEV 0x04
#define BOOT_INFO_MEM_RES 0x05
#define BOOT_INFO_MEM_FREE 0x06

struct packed BootInfoSymbol {
    UIntPtr Start, End;
    Char *Name;
};

struct packed BootInfoMemMap {
    UIntPtr Base, Count;
    UInt8 Type;
};

struct packed BootInfo {
    UInt32 Magic;
    UIntPtr KernelStart, RegionsStart, KernelEnd, EfiTempAddress,
            MinPhysicalAddress, MaxPhysicalAddress, PhysicalMemorySize;
    Void *Directory;

    struct packed {
        UIntPtr Count;
        BootInfoSymbol *Start;
    } Symbols;

    struct packed {
        UIntPtr Count;
        BootInfoMemMap *Entries;
    } MemoryMap;

    struct packed {
        UIntPtr Size, Index;
        UInt8 *Data;
    } BootImage;

    struct packed {
        UIntPtr Width, Height, BackBuffer, FrontBuffer;
    } FrameBuffer;

    UInt8 KernelStack[0x2000];
};

#ifdef USE_INIT_ARRAY
extern "C" UIntPtr __init_array_start, __init_array_end;
#else
extern "C" Void _init(Void);
#endif
