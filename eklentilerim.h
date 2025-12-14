#pragma once 
#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>
#include <minwindef.h>
#include <intrin.h>
#include "apiler.h"

NTSTATUS ReadPhysicalAddress(uint64_t TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesRead)
{
    if (!TargetAddress || !Size)
        return STATUS_UNSUCCESSFUL;
    MM_COPY_ADDRESS AddrToRead = { 0 };
    AddrToRead.PhysicalAddress.QuadPart = TargetAddress;
    return MmCopyMemory(lpBuffer, AddrToRead, Size, MM_COPY_MEMORY_PHYSICAL, BytesRead);
}


typedef union _virt_addr_t
{
    void* value;
    struct
    {
        uintptr_t offset : 12;
        uintptr_t pt_index : 9;
        uintptr_t pd_index : 9;
        uintptr_t pdpt_index : 9;
        uintptr_t pml4_index : 9;
        uintptr_t reserved : 16;
    };
} virt_addr_t, * pvirt_addr_t;

namespace pml4
{
    PVOID split_memory(PVOID SearchBase, SIZE_T SearchSize, const void* Pattern, SIZE_T PatternSize)
    {

        const UCHAR* searchBase = static_cast<const UCHAR*>(SearchBase);
        const UCHAR* pattern = static_cast<const UCHAR*>(Pattern);

        for (SIZE_T i = 0; i <= SearchSize - PatternSize; ++i) {
            SIZE_T j = 0;
            for (; j < PatternSize; ++j) {
                if (searchBase[i + j] != pattern[j])
                    break;
            }

            if (j == PatternSize)
                return const_cast<UCHAR*>(&searchBase[i]);
        }
        return nullptr;
    }
    void* g_mmonp_MmPfnDatabase;

    static NTSTATUS InitializeMmPfnDatabase()
    {
        struct MmPfnDatabaseSearchPattern
        {
            const UCHAR* bytes;
            SIZE_T bytes_size;
            bool hard_coded;
        };

        MmPfnDatabaseSearchPattern patterns;

        static const UCHAR kPatternWin10x64[] = {
            0x48, 0x8B, 0xC1,
            0x48, 0xC1, 0xE8, 0x0C,
            0x48, 0x8D, 0x14, 0x40,
            0x48, 0x03, 0xD2,
            0x48, 0xB8,
        };

        patterns.bytes = kPatternWin10x64;
        patterns.bytes_size = sizeof(kPatternWin10x64);
        patterns.hard_coded = true;

        const auto p_MmGetVirtualForPhysical = reinterpret_cast<UCHAR*>(MmGetVirtualForPhysical);
        if (!p_MmGetVirtualForPhysical) {
            return STATUS_PROCEDURE_NOT_FOUND;
        }

        auto found = reinterpret_cast<UCHAR*>(split_memory(p_MmGetVirtualForPhysical, 0x20, patterns.bytes, patterns.bytes_size));
        if (!found) {
            return STATUS_UNSUCCESSFUL;
        }

        found += patterns.bytes_size;
        if (patterns.hard_coded) {
            g_mmonp_MmPfnDatabase = *reinterpret_cast<void**>(found);
        }
        else {
            const auto mmpfn_address = *reinterpret_cast<ULONG_PTR*>(found);
            g_mmonp_MmPfnDatabase = *reinterpret_cast<void**>(mmpfn_address);
        }

        g_mmonp_MmPfnDatabase = PAGE_ALIGN(g_mmonp_MmPfnDatabase);

        return STATUS_SUCCESS;
    }

    uintptr_t dirbase_from_base_address(void* base)
    {
        if (!NT_SUCCESS(InitializeMmPfnDatabase()))
            return 0;
        virt_addr_t virt_base{}; virt_base.value = base;
        size_t read{};
        auto ranges = MmGetPhysicalMemoryRanges();
        for (int i = 0;; i++) {

            auto elem = &ranges[i];

            if (!elem->BaseAddress.QuadPart || !elem->NumberOfBytes.QuadPart)
                break;

            uintptr_t current_phys_address = elem->BaseAddress.QuadPart;
            for (int j = 0; j < (elem->NumberOfBytes.QuadPart / 0x1000); j++, current_phys_address += 0x1000) {

                _MMPFN* pnfinfo = (_MMPFN*)((uintptr_t)g_mmonp_MmPfnDatabase + (current_phys_address >> 12) * sizeof(_MMPFN));

                if (pnfinfo->u4.PteFrame == (current_phys_address >> 12)) {
                    MMPTE pml4e{};
                    if (!NT_SUCCESS(ReadPhysicalAddress(current_phys_address + 8 * virt_base.pml4_index, &pml4e, 8, &read)))
                        continue;

                    if (!pml4e.u.Hard.Valid)
                        continue;

                    MMPTE pdpte{};
                    if (!NT_SUCCESS(ReadPhysicalAddress((pml4e.u.Hard.PageFrameNumber << 12) + 8 * virt_base.pdpt_index, &pdpte, 8, &read)))
                        continue;

                    if (!pdpte.u.Hard.Valid)
                        continue;

                    MMPTE pde{};
                    if (!NT_SUCCESS(ReadPhysicalAddress((pdpte.u.Hard.PageFrameNumber << 12) + 8 * virt_base.pd_index, &pde, 8, &read)))
                        continue;

                    if (!pde.u.Hard.Valid)
                        continue;

                    MMPTE pte{};
                    if (!NT_SUCCESS(ReadPhysicalAddress((pde.u.Hard.PageFrameNumber << 12) + 8 * virt_base.pt_index, &pte, 8, &read)))
                        continue;

                    if (!pte.u.Hard.Valid)
                        continue;

                    return current_phys_address;
                }
            }
        }
        return 0;
    }
}

NTSTATUS WritePhysicalAddress(uint64_t TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesWritten)
{
    if (!TargetAddress || !Size)
        return STATUS_UNSUCCESSFUL;
    PHYSICAL_ADDRESS AddrToWrite = { 0 };
    AddrToWrite.QuadPart = TargetAddress;
    PVOID pmapped_mem = MmMapIoSpaceEx(AddrToWrite, Size, PAGE_READWRITE);
    if (!pmapped_mem) return STATUS_UNSUCCESSFUL;
    memcpy(pmapped_mem, lpBuffer, Size);
    *BytesWritten = Size;
    MmUnmapIoSpace(pmapped_mem, Size);
    return STATUS_SUCCESS;
}

#define PAGE_OFFSET_SIZE 12
static const uint64_t PMASK = (~0xfull << 8) & 0xfffffffffull;
//struct cache {
//    uintptr_t Address;
//    uintptr_t Value;
//};
//static cache cached_pml4e[512];
//
//ULONGLONG read_cached(ULONGLONG address, cache* entry, SIZE_T* readsize) {
//    if (entry->Address == address)
//        return entry->Value;
//
//    if (!NT_SUCCESS(ReadPhysicalAddress(address, &entry->Value, sizeof(entry->Value), readsize))) {
//        entry->Address = 0;
//        return 0;
//    }
//    entry->Address = address;
//    return entry->Value;
//}
//
//uintptr_t translate_linear(uintptr_t directoryTableBase, uintptr_t virtualAddress) {
//    directoryTableBase &= ~0xf;
//
//    uintptr_t pageOffset = virtualAddress & ((1ULL << PAGE_OFFSET_SIZE) - 1);
//    uintptr_t pte = (virtualAddress >> 12) & 0x1ff;
//    uintptr_t pt = (virtualAddress >> 21) & 0x1ff;
//    uintptr_t pd = (virtualAddress >> 30) & 0x1ff;
//    uintptr_t pdp = (virtualAddress >> 39) & 0x1ff;
//
//    SIZE_T readsize = 0;
//    uintptr_t pdpe = 0;
//
//    pdpe = read_cached(directoryTableBase + 8 * pdp, &cached_pml4e[pdp], &readsize);
//    if ((pdpe & 1) == 0)
//        return 0;
//
//    uintptr_t pde = 0;
//
//    ReadPhysicalAddress(((pdpe & PMASK) + 8 * pd), &pde, sizeof(pde), &readsize);
//    if ((pde & 1) == 0)
//        return 0;
//
//    if (pde & 0x80) {
//        return (pde & PMASK) + (virtualAddress & ((1ULL << 30) - 1));
//    }
//
//    uintptr_t pteAddr = 0;
//
//    ReadPhysicalAddress(((pde & PMASK) + 8 * pt), &pteAddr, sizeof(pteAddr), &readsize);
//    if ((pteAddr & 1) == 0)
//        return 0;
//
//    if (pteAddr & 0x80) {
//        return (pteAddr & PMASK) + (virtualAddress & ((1ULL << 21) - 1));
//    }
//
//    uintptr_t finalAddr = 0;
//
//    ReadPhysicalAddress(((pteAddr & PMASK) + 8 * pte), &finalAddr, sizeof(finalAddr), &readsize);
//    finalAddr &= PMASK;
//
//    if (finalAddr == 0)
//        return 0;
//
//    return finalAddr + pageOffset;
//}
uint64_t translate_linear(uint64_t directoryTableBase, uint64_t virtualAddress) {
    directoryTableBase &= ~0xf;

    uint64_t pageOffset = virtualAddress & ~(~0ul << PAGE_OFFSET_SIZE);
    uint64_t pte = ((virtualAddress >> 12) & (0x1ffll));
    uint64_t pt = ((virtualAddress >> 21) & (0x1ffll));
    uint64_t pd = ((virtualAddress >> 30) & (0x1ffll));
    uint64_t pdp = ((virtualAddress >> 39) & (0x1ffll));

    SIZE_T readsize = 0;
    uint64_t pdpe = 0;
    ReadPhysicalAddress(directoryTableBase + 8 * pdp, &pdpe, sizeof(pdpe), &readsize);
    if (~pdpe & 1)
        return 0;

    uint64_t pde = 0;
    ReadPhysicalAddress((pdpe & PMASK) + 8 * pd, &pde, sizeof(pde), &readsize);
    if (~pde & 1)
        return 0;

    if (pde & 0x80)
        return (pde & (~0ull << 42 >> 12)) + (virtualAddress & ~(~0ull << 30));

    uint64_t pteAddr = 0;
    ReadPhysicalAddress((pde & PMASK) + 8 * pt, &pteAddr, sizeof(pteAddr), &readsize);
    if (~pteAddr & 1)
        return 0;

    /* 2MB large page */
    if (pteAddr & 0x80)
        return (pteAddr & PMASK) + (virtualAddress & ~(~0ull << 21));

    virtualAddress = 0;
    ReadPhysicalAddress((pteAddr & PMASK) + 8 * pte, &virtualAddress, sizeof(virtualAddress), &readsize);
    virtualAddress &= PMASK;

    if (!virtualAddress)
        return 0;

    return virtualAddress + pageOffset;
}

uintptr_t handle_get_base_address(HANDLE processpid)
{
    PEPROCESS process = nullptr;
    NTSTATUS  status = PsLookupProcessByProcessId(processpid, &process);

    if (!NT_SUCCESS(status))
        return 0;

    const auto base_address = uintptr_t(PsGetProcessSectionBaseAddress(process));
    ObfDereferenceObject(process);

    return base_address;
}

ULONG64 find_min(INT32 g, SIZE_T f) {
    INT32 h = (INT32)f;
    ULONG64 result = 0;

    result = (((g) < (h)) ? (g) : (h));

    return result;
}

NTSTATUS memory_kernel_memory(HANDLE pid, uint64_t Address, PVOID AllocatedBuffer, SIZE_T size, bool memo = false) {
    if (pid == 0) return STATUS_UNSUCCESSFUL;

    NTSTATUS NtRet = STATUS_UNSUCCESSFUL;
    if (!cr3_check && !pid_save) {
        savedptr = pml4::dirbase_from_base_address((void*)handle_get_base_address(pid));
        pid_save = pid;
        cr3_check = true;
    }
    else if (cr3_check && pid_save != pid) {
        savedptr = pml4::dirbase_from_base_address((void*)handle_get_base_address(pid));
        pid_save = pid;
    }

    if (!savedptr)
        return STATUS_UNSUCCESSFUL;

    SIZE_T CurOffset = 0;
    while (size > 0) {
        uint64_t CurPhysAddr = translate_linear(savedptr, Address + CurOffset);
        if (!CurPhysAddr)
            return STATUS_UNSUCCESSFUL;

        SIZE_T ReadSize = find_min(PAGE_SIZE - (CurPhysAddr & 0xFFF), size);
        if (ReadSize == 0)
            return STATUS_UNSUCCESSFUL;

        SIZE_T BytesRead = 0;
        NTSTATUS NtRet = (memo)
            ? WritePhysicalAddress(CurPhysAddr, (PVOID)((ULONG64)AllocatedBuffer + CurOffset), ReadSize, &BytesRead)
            : ReadPhysicalAddress(CurPhysAddr, (PVOID)((ULONG64)AllocatedBuffer + CurOffset), ReadSize, &BytesRead);

        if (NtRet != STATUS_SUCCESS || BytesRead == 0)
            return STATUS_UNSUCCESSFUL;

        size -= BytesRead;
        CurOffset += BytesRead;
    }

    return STATUS_SUCCESS;
}
