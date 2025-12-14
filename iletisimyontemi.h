#pragma once
#include "eklentilerim.h"
//#define phy_memory CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9573265, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
ULONG phy_memory;

typedef struct _k_rw_memoryrequest {
    ULONG pid;
    UINT_PTR address;
    ULONGLONG size;
    void* output;
    bool memo;
} k_rw_memoryrequest, * pk_rw_memoryrequest;
