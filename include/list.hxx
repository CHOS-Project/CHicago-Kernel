/* File author is Ítalo Lima Marconato Matias
 *
 * Created on February 28 of 2021, at 11:51 BRT
 * Last edited on March 02 of 2021 at 11:51 BRT */

#pragma once

#include <mm.hxx>
#include <utility.hxx>

#define DO_ADD(x) \
    Status status; \
    \
    if (Index + (Index > Length) >= Capacity && \
        (status = Reserve(!Capacity ? 2 : Capacity * 2)) != Status::Success) { \
        return status; \
    } else if (Index < Length) { \
        MoveMemory(&Elements[Index + 1], &Elements[Index], sizeof(T) * (Length - Index)); \
    } \
    \
    Elements[Index] = T(x); \
    Length = (Index > Length ? Index : Length) + 1; \
    \
    return Status::Success

namespace CHicago {

Void CopyMemory(Void*, const Void*, UIntPtr);
Void SetMemory(Void*, UInt8, UIntPtr);
Void MoveMemory(Void*, const Void*, UIntPtr);

template<class T> class List {
public:
    List() : Elements(Null), Length(0), Capacity(0) { }
    List(UIntPtr Size) : Elements(Null), Length(0), Capacity(0) { Reserve(Size); }
    List(const List &Source) : Elements(Null), Length(0), Capacity(0) { Reserve(Source.Length); Add(Source); }

    List(List &&Source) : Elements(Source.Elements), Length(Source.Length), Capacity(Source.Capacity) {
        Source.Elements = Null;
        Source.Length = Source.Capacity = 0;
    }

    List(const initializer_list<T> &Source) : Elements(Null), Length(0), Capacity(0) {
        /* Let's already try to reserve the space that we need (if it fails, the Add() calls will also probably
         * fail). */

        Reserve(Source.GetSize());

        for (const T &data : Source) {
            Add(data);
        }
    }

    ~List() { if (Elements != Null) { Clear(); Fit(); } }

    List &operator =(List &&Source) {
        /* We need to overwrite the move operator, which is just the copy operator, but we have to clear/set to length=0
         * the source list. */

        if (this != &Source) {
            Elements = Source.Elements;
            Length = Source.Length;
            Capacity = Source.Capacity;
            Source.Elements = Null;
            Source.Length = Source.Capacity = 0;
        }

        return *this;
    }

    List &operator =(const List &Source) {
        /* Basic rule: If you are overwriting the copy constructor and the destructor, you also need to overwrite the
         * copy operator. Thankfully for us, we just need to call Clear() and Add(). */

        if (this != &Source) {
            Clear();
            Reserve(Source.Length);
            Add(Source);
        }

        return *this;
    }

    Status Reserve(UIntPtr Size) {
        T *buf = Null;

        /* Allocating with new[] would call the destructor for all the items everytime we delete[] it, we don't want
         * that, we want to be able to manually call the destructors using Remove(), and later just deallocate the
         * memory without calling the destructors again (that is, without causing UB), so let's use the Heap::Allocate
         * function (which is our malloc function). */

        if (Size <= Capacity) {
            return Status::InvalidArg;
        } else if ((buf = (T*)Heap::Allocate(sizeof(T) * Size)) == Null) {
            return Status::OutOfMemory;
        }

        /* Don't do the same mistake I did when I first wrote this function. Remember to check if this isn't the first
         * allocation we're doing, if that's the case, we don't need to copy the old elements nor deallocate them. */

        if (Elements != Null) {
            CopyMemory(buf, Elements, Length * sizeof(T));
            Heap::Deallocate(Elements);
        }

        return Elements = buf, Capacity = Size, Status::Success;
    }

    Status Fit() {
        T *buf = Null;

        /* While the Reserve function allocates a buffer that can contain at least all the items that the user
         * specified, this function deallocates any extra space, and fits the element buffer to make the
         * capacity=length. If the length is 0 (for example, we were called on the destructor), we just need to free the
         * buffer. */

        if (Elements == Null || Capacity == Length) {
            return Status::Success;
        } else if (!Length) {
            Heap::Deallocate(Elements);
            return Elements = Null, Capacity = 0, Status::Success;
        } else if ((buf = (T*)Heap::Allocate(sizeof(T) * Length)) == Null) {
            return Status::OutOfMemory;
        }

        CopyMemory(buf, Elements, Length * sizeof(T));
        Heap::Deallocate(Elements);

        return Elements = buf, Capacity = Length, Status::Success;
    }

    inline Void Clear() { while (Length) { Elements[--Length].~T(); } }

    inline Status Add(const List &Source) { return Add(Source, Length); }
    inline Status Add(List &&Source) { return Add(Source, Length); }
    inline Status Add(const T &Data) { return Add(Data, Length); }
    inline Status Add(T &&Data) { return Add(Data, Length); }

    Status Add(const List &Source, UIntPtr Index) {
        Status status;

        for (const T &data : Source) {
            if ((status = Add(data, Index++)) != Status::Success) {
                return status;
            }
        }

        return Status::Success;
    }

    Status Add(List &&Source, UIntPtr Index) {
        /* When moving the list, we should add everything using the T&& List::Add, and later clear the other list
         * manually. */

        Status status;

        for (const T &data : Source) {
            if ((status = Add(Move(data), Index++)) != Status::Success) {
                return status;
            }
        }

        Source.Length = 0;
        Source.Fit();

        return Status::Success;
    }

    Status Add(const T &Data, UIntPtr Index) {
        /* As the elements are stored in one big array, setting the value (or even adding a new one and relocating
         * the others) isn't hard. First, we need to check if we have enough space for adding the new item and
         * relocating the others that are on the position/after the position forward (of course, this last part isn't
         * required when adding beyond the current length), if there isn't enough space, we can call Reserve(), if this
         * is the first entry, allocate * 2 items than the last allocation, else, the default allocation size is 2 (we
         * can change this later). Before actually copying the data into the array, we need to move everything on the
         * index forward (if it's not beyond the current length), we need to use MoveMemory for this, as the memory
         * locations ARE going to overlap. */

        DO_ADD(Data);
    }

    Status Add(T &&Data, UIntPtr Index) {
        /* This is like the other add function, but we're going to move the value (instead of copying it). */

        DO_ADD(static_cast<T&&>(Data));
    }

    Status Remove(UIntPtr Index) {
        /* No need to dealloc any memory here (the destructor and the Fit() functions are responsible for deallocating
         * the memory), but we DO need to call T's destructor, and move the items like we do on Add, but we need to move
         * the items backwards one slot this time, instead of forward one slot. */

        if (Index >= Length) {
            return Status::InvalidArg;
        }

        Elements[Index].~T();

        if (Index < --Length) {
            MoveMemory(&Elements[Index], &Elements[Index + 1], sizeof(T) * (Length - Index));
            SetMemory(&Elements[Length], 0, sizeof(T));
        } else {
            SetMemory(&Elements[Index], 0, sizeof(T));
        }

        return Status::Success;
    }

    Void Sort(Boolean (*Compare)(const T&, const T&)) {
        MergeSort(Elements, Length, Compare);
    }

    inline UIntPtr GetLength() const { return Length; }
    inline UIntPtr GetCapacity() const { return Capacity; }

    inline ReverseIterator<T> Reverse(Void) { return { Elements, &Elements[Length] }; }
    inline ConstReverseIterator<T> Reverse(Void) const { return { Elements, &Elements[Length] }; }

    /* begin() and end() are required for using ranged-for loops (equivalent to C# foreach loops, but the format is
     * for (val : list)' instead of 'foreach (val in list)'). */

    inline T *begin() { return Elements; }
    inline const T *begin() const { return Elements; }
    inline T *end() { return Elements + Length; }
    inline const T *end() const { return Elements + Length; }

    inline T &operator [](UIntPtr Index) {
        /* For the non-const index operator, we can auto increase the length if we're accessing a region inside the
         * 0->Capacity-1 range (accessing things outside this range is UB, and you're probably just going to page fault
         * the kernel). */

        if (Index < Capacity && Index >= Length) {
            Length = Index + 1;
        }

        return Elements[Index];
    }

    inline const T &operator [](UIntPtr Index) const { return Elements[Index]; }
private:
    inline static Void MergeSort(T *Array, UIntPtr Length, Boolean (*Compare)(const T&, const T&)) {
        /* Merge sort is a divide and conquer sorting algorithm, we subdivide the array until we the length is <= 1, and
         * we sort the subdivided arrays, best/worst/avg cases are O(nlogn), but the space complexity is O(n), we're
         * allocating space on the stack for the temp array, so we may need to increase the kernel stack size in the
         * future. Other choice would be implementing something like quick sort, or trying to implement an in-place
         * merge sort algorithm. */

        if (Length <= 1) {
            return;
        }

        UIntPtr i = 0, j = 0, k = 0, llen = Length / 2, rlen = Length - llen;
        T left[llen], right[rlen];

        CopyMemory(left, Array, sizeof(T) * llen);
        CopyMemory(right, Array + llen, sizeof(T) * rlen);
        MergeSort(left, llen, Compare);
        MergeSort(right, rlen, Compare);

        for (; j < llen && k < rlen; i++) {
            T *src;

            if (Compare(left[j], right[k])) {
                src = &left[j++];
            } else {
                src = &right[k++];
            }

            CopyMemory(&Array[i], src, sizeof(T));
        }

        for (; j < llen; i++, j++) {
            CopyMemory(&Array[i], &left[j], sizeof(T));
        }

        for (; k < rlen; i++, k++) {
            CopyMemory(&Array[i], &right[k], sizeof(T));
        }
    }

    T *Elements;
    UIntPtr Length, Capacity;
};

}
