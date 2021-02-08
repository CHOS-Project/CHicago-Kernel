/* File author is Ítalo Lima Marconato Matias
 *
 * Created on February 07 of 2021, at 14:01 BRT
 * Last edited on February 08 of 2021 at 00:08 BRT */

#pragma once

#include <status.hxx>

class String {
public:
    /* For now we only support initializing ourselves in a way that doesn't require any allocations (this is going to
     * change/go back to the normal String class after the memory allocator is readded).
     * Also, at least for now, the whole string is read-only. */

    String(Void);
    String(const Char*);
    String(const String&);

    String &operator =(const Char*);
    String &operator =(const String&);

    static String FromInt(Char*, Int64, UIntPtr, UInt8);
    static String FromUInt(Char*, UInt64, UIntPtr, UInt8);

    Boolean Compare(const String&) const;

    const Char *GetValue(Void) const { return Value; }
    UIntPtr GetLength(Void) const { return Length; }

    /* Operators for ranges-for and for accessing the string as a normal character array. */

    const Char *begin(Void) { return &Value[0]; }
    const Char *begin(Void) const { return &Value[0]; }
    const Char *end(Void) { return &Value[Length]; }
    const Char *end(Void) const { return &Value[Length]; }

    Char operator [](UIntPtr Index) const { return Index >= Length ? 0 : Value[Index]; }
private:
    static Void FromUInt(Char*, UInt64, UInt8, IntPtr&, IntPtr);
    Void CalculateLength(Void);

    const Char *Value;
    UIntPtr Length;
};

UIntPtr VariadicFormat(const String&, VariadicList&, Boolean (*)(Char, Void*), Void*, Char*, UIntPtr, UIntPtr);

Void CopyMemory(Void*, const Void*, UIntPtr);
Void CopyMemory32(Void*, const Void*, UIntPtr);
Void SetMemory(Void*, UInt8, UIntPtr);
Void SetMemory32(Void*, UInt32, UIntPtr);
Void MoveMemory(Void*, const Void*, UIntPtr);
Boolean CompareMemory(const Void *const, const Void *const, UIntPtr);