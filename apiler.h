#pragma once
#define _STDINT

// Integer types
typedef signed char        int8_t;
typedef short             int16_t;
typedef int               int32_t;
typedef long long         int64_t;
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      uint32_t;
typedef unsigned long long uint64_t;

// Least integer types
typedef signed char        int_least8_t;
typedef short             int_least16_t;
typedef int               int_least32_t;
typedef long long         int_least64_t;
typedef unsigned char     uint_least8_t;
typedef unsigned short    uint_least16_t;
typedef unsigned int      uint_least32_t;
typedef unsigned long long uint_least64_t;

// Fast integer types
typedef signed char        int_fast8_t;
typedef int               int_fast16_t;
typedef int               int_fast32_t;
typedef long long         int_fast64_t;
typedef unsigned char     uint_fast8_t;
typedef unsigned int      uint_fast16_t;
typedef unsigned int      uint_fast32_t;
typedef unsigned long long uint_fast64_t;

// Max integer types
typedef long long          intmax_t;
typedef unsigned long long uintmax_t;

// Limits
#define INT8_MIN   (-127i8 - 1)
#define INT16_MIN  (-32767i16 - 1)
#define INT32_MIN  (-2147483647i32 - 1)
#define INT64_MIN  (-9223372036854775807i64 - 1)
#define INT8_MAX   127i8
#define INT16_MAX  32767i16
#define INT32_MAX  2147483647i32
#define INT64_MAX  9223372036854775807i64
#define UINT8_MAX  0xffui8
#define UINT16_MAX 0xffffui16
#define UINT32_MAX 0xffffffffui32
#define UINT64_MAX 0xffffffffffffffffui64

#define INT_LEAST8_MIN   INT8_MIN
#define INT_LEAST16_MIN  INT16_MIN
#define INT_LEAST32_MIN  INT32_MIN
#define INT_LEAST64_MIN  INT64_MIN
#define INT_LEAST8_MAX   INT8_MAX
#define INT_LEAST16_MAX  INT16_MAX
#define INT_LEAST32_MAX  INT32_MAX
#define INT_LEAST64_MAX  INT64_MAX
#define UINT_LEAST8_MAX  UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#define INT_FAST8_MIN    INT8_MIN
#define INT_FAST16_MIN   INT32_MIN
#define INT_FAST32_MIN   INT32_MIN
#define INT_FAST64_MIN   INT64_MIN
#define INT_FAST8_MAX    INT8_MAX
#define INT_FAST16_MAX   INT32_MAX
#define INT_FAST32_MAX   INT32_MAX
#define INT_FAST64_MAX   INT64_MAX
#define UINT_FAST8_MAX   UINT8_MAX
#define UINT_FAST16_MAX  UINT32_MAX
#define UINT_FAST32_MAX  UINT32_MAX
#define UINT_FAST64_MAX  UINT64_MAX

#ifdef _WIN64
#define INTPTR_MIN   INT64_MIN
#define INTPTR_MAX   INT64_MAX
#define UINTPTR_MAX  UINT64_MAX
#else
#define INTPTR_MIN   INT32_MIN
#define INTPTR_MAX   INT32_MAX
#define UINTPTR_MAX  UINT32_MAX
#endif

#define INTMAX_MIN       INT64_MIN
#define INTMAX_MAX       INT64_MAX
#define UINTMAX_MAX      UINT64_MAX

#define PTRDIFF_MIN      INTPTR_MIN
#define PTRDIFF_MAX      INTPTR_MAX

#ifndef SIZE_MAX
#define SIZE_MAX     UINTPTR_MAX
#endif

#define SIG_ATOMIC_MIN   INT32_MIN
#define SIG_ATOMIC_MAX   INT32_MAX

#define WCHAR_MIN        0x0000
#define WCHAR_MAX        0xffff

#define WINT_MIN         0x0000
#define WINT_MAX         0xffff

#define INT8_C(x)    (x)
#define INT16_C(x)   (x)
#define INT32_C(x)   (x)
#define INT64_C(x)   (x ## LL)

#define UINT8_C(x)   (x)
#define UINT16_C(x)  (x)
#define UINT32_C(x)  (x ## U)
#define UINT64_C(x)  (x ## ULL)

#define INTMAX_C(x)  INT64_C(x)
#define UINTMAX_C(x) UINT64_C(x)

typedef struct _PEB_LDR_DATA {
    ULONG Length;
    BOOLEAN Initialized;
    PVOID SsHandle;
    LIST_ENTRY ModuleListLoadOrder;
    LIST_ENTRY ModuleListMemoryOrder;
    LIST_ENTRY ModuleListInitOrder;
} PEB_LDR_DATA, * PPEB_LDR_DATA;
typedef struct _RTL_USER_PROCESS_PARAMETERS {
    BYTE Reserved1[16];
    PVOID Reserved2[10];
    UNICODE_STRING ImagePathName;
    UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;
typedef void(__stdcall* PPS_POST_PROCESS_INIT_ROUTINE)(void); // not exported
typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    PVOID Reserved3[2];
    PPEB_LDR_DATA Ldr;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    PVOID Reserved4[3];
    PVOID AtlThunkSListPtr;
    PVOID Reserved5;
    ULONG Reserved6;
    PVOID Reserved7;
    ULONG Reserved8;
    ULONG AtlThunkSListPtr32;
    PVOID Reserved9[45];
    BYTE Reserved10[96];
    PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
    BYTE Reserved11[128];
    PVOID Reserved12[1];
    ULONG SessionId;
} PEB, * PPEB;
typedef struct _PEB_LDR_DATA1
{
    ULONG Length;                                                           //0x0
    UCHAR Initialized;                                                      //0x4
    VOID* SsHandle;                                                         //0x8
    struct _LIST_ENTRY InLoadOrderModuleList;                               //0x10
    struct _LIST_ENTRY InMemoryOrderModuleList;                             //0x20
    struct _LIST_ENTRY InInitializationOrderModuleList;                     //0x30
    VOID* EntryInProgress;                                                  //0x40
    UCHAR ShutdownInProgress;                                               //0x48
    VOID* ShutdownThreadId;                                                 //0x50
} PEB_LDR_DATA1, * PPEB_LDR_DATA1;

typedef struct _LDR_DATA_TABLE_ENTRY1 {
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    // ...
} LDR_DATA_TABLE_ENTRY1, * PLDR_DATA_TABLE_ENTRY1;


typedef struct _MI_ACTIVE_PFN
{
    union
    {
        struct
        {
            struct /* bitfield */
            {
                /* 0x0000 */ unsigned __int64 Tradable : 1; /* bit position: 0 */
                /* 0x0000 */ unsigned __int64 NonPagedBuddy : 43; /* bit position: 1 */
            }; /* bitfield */
        } /* size: 0x0008 */ Leaf;
        struct
        {
            struct /* bitfield */
            {
                /* 0x0000 */ unsigned __int64 Tradable : 1; /* bit position: 0 */
                /* 0x0000 */ unsigned __int64 WsleAge : 3; /* bit position: 1 */
                /* 0x0000 */ unsigned __int64 OldestWsleLeafEntries : 10; /* bit position: 4 */
                /* 0x0000 */ unsigned __int64 OldestWsleLeafAge : 3; /* bit position: 14 */
                /* 0x0000 */ unsigned __int64 NonPagedBuddy : 43; /* bit position: 17 */
            }; /* bitfield */
        } /* size: 0x0008 */ PageTable;
        /* 0x0000 */ unsigned __int64 EntireActiveField;
    }; /* size: 0x0008 */
} MI_ACTIVE_PFN, * PMI_ACTIVE_PFN; /* size: 0x0008 */

typedef struct _MMPTE_HARDWARE
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned __int64 Valid : 1; /* bit position: 0 */
        /* 0x0000 */ unsigned __int64 Dirty1 : 1; /* bit position: 1 */
        /* 0x0000 */ unsigned __int64 Owner : 1; /* bit position: 2 */
        /* 0x0000 */ unsigned __int64 WriteThrough : 1; /* bit position: 3 */
        /* 0x0000 */ unsigned __int64 CacheDisable : 1; /* bit position: 4 */
        /* 0x0000 */ unsigned __int64 Accessed : 1; /* bit position: 5 */
        /* 0x0000 */ unsigned __int64 Dirty : 1; /* bit position: 6 */
        /* 0x0000 */ unsigned __int64 LargePage : 1; /* bit position: 7 */
        /* 0x0000 */ unsigned __int64 Global : 1; /* bit position: 8 */
        /* 0x0000 */ unsigned __int64 CopyOnWrite : 1; /* bit position: 9 */
        /* 0x0000 */ unsigned __int64 Unused : 1; /* bit position: 10 */
        /* 0x0000 */ unsigned __int64 Write : 1; /* bit position: 11 */
        /* 0x0000 */ unsigned __int64 PageFrameNumber : 40; /* bit position: 12 */
        /* 0x0000 */ unsigned __int64 ReservedForSoftware : 4; /* bit position: 52 */
        /* 0x0000 */ unsigned __int64 WsleAge : 4; /* bit position: 56 */
        /* 0x0000 */ unsigned __int64 WsleProtection : 3; /* bit position: 60 */
        /* 0x0000 */ unsigned __int64 NoExecute : 1; /* bit position: 63 */
    }; /* bitfield */
} MMPTE_HARDWARE, * PMMPTE_HARDWARE; /* size: 0x0008 */

typedef struct _MMPTE_PROTOTYPE
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned __int64 Valid : 1; /* bit position: 0 */
        /* 0x0000 */ unsigned __int64 DemandFillProto : 1; /* bit position: 1 */
        /* 0x0000 */ unsigned __int64 HiberVerifyConverted : 1; /* bit position: 2 */
        /* 0x0000 */ unsigned __int64 ReadOnly : 1; /* bit position: 3 */
        /* 0x0000 */ unsigned __int64 SwizzleBit : 1; /* bit position: 4 */
        /* 0x0000 */ unsigned __int64 Protection : 5; /* bit position: 5 */
        /* 0x0000 */ unsigned __int64 Prototype : 1; /* bit position: 10 */
        /* 0x0000 */ unsigned __int64 Combined : 1; /* bit position: 11 */
        /* 0x0000 */ unsigned __int64 Unused1 : 4; /* bit position: 12 */
        /* 0x0000 */ __int64 ProtoAddress : 48; /* bit position: 16 */
    }; /* bitfield */
} MMPTE_PROTOTYPE, * PMMPTE_PROTOTYPE; /* size: 0x0008 */

typedef struct _MMPTE_SOFTWARE
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned __int64 Valid : 1; /* bit position: 0 */
        /* 0x0000 */ unsigned __int64 PageFileReserved : 1; /* bit position: 1 */
        /* 0x0000 */ unsigned __int64 PageFileAllocated : 1; /* bit position: 2 */
        /* 0x0000 */ unsigned __int64 ColdPage : 1; /* bit position: 3 */
        /* 0x0000 */ unsigned __int64 SwizzleBit : 1; /* bit position: 4 */
        /* 0x0000 */ unsigned __int64 Protection : 5; /* bit position: 5 */
        /* 0x0000 */ unsigned __int64 Prototype : 1; /* bit position: 10 */
        /* 0x0000 */ unsigned __int64 Transition : 1; /* bit position: 11 */
        /* 0x0000 */ unsigned __int64 PageFileLow : 4; /* bit position: 12 */
        /* 0x0000 */ unsigned __int64 UsedPageTableEntries : 10; /* bit position: 16 */
        /* 0x0000 */ unsigned __int64 ShadowStack : 1; /* bit position: 26 */
        /* 0x0000 */ unsigned __int64 Unused : 5; /* bit position: 27 */
        /* 0x0000 */ unsigned __int64 PageFileHigh : 32; /* bit position: 32 */
    }; /* bitfield */
} MMPTE_SOFTWARE, * PMMPTE_SOFTWARE; /* size: 0x0008 */

typedef struct _MMPTE_TIMESTAMP
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned __int64 MustBeZero : 1; /* bit position: 0 */
        /* 0x0000 */ unsigned __int64 Unused : 3; /* bit position: 1 */
        /* 0x0000 */ unsigned __int64 SwizzleBit : 1; /* bit position: 4 */
        /* 0x0000 */ unsigned __int64 Protection : 5; /* bit position: 5 */
        /* 0x0000 */ unsigned __int64 Prototype : 1; /* bit position: 10 */
        /* 0x0000 */ unsigned __int64 Transition : 1; /* bit position: 11 */
        /* 0x0000 */ unsigned __int64 PageFileLow : 4; /* bit position: 12 */
        /* 0x0000 */ unsigned __int64 Reserved : 16; /* bit position: 16 */
        /* 0x0000 */ unsigned __int64 GlobalTimeStamp : 32; /* bit position: 32 */
    }; /* bitfield */
} MMPTE_TIMESTAMP, * PMMPTE_TIMESTAMP; /* size: 0x0008 */

typedef struct _MMPTE_TRANSITION
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned __int64 Valid : 1; /* bit position: 0 */
        /* 0x0000 */ unsigned __int64 Write : 1; /* bit position: 1 */
        /* 0x0000 */ unsigned __int64 Spare : 1; /* bit position: 2 */
        /* 0x0000 */ unsigned __int64 IoTracker : 1; /* bit position: 3 */
        /* 0x0000 */ unsigned __int64 SwizzleBit : 1; /* bit position: 4 */
        /* 0x0000 */ unsigned __int64 Protection : 5; /* bit position: 5 */
        /* 0x0000 */ unsigned __int64 Prototype : 1; /* bit position: 10 */
        /* 0x0000 */ unsigned __int64 Transition : 1; /* bit position: 11 */
        /* 0x0000 */ unsigned __int64 PageFrameNumber : 40; /* bit position: 12 */
        /* 0x0000 */ unsigned __int64 Unused : 12; /* bit position: 52 */
    }; /* bitfield */
} MMPTE_TRANSITION, * PMMPTE_TRANSITION; /* size: 0x0008 */

typedef struct _MMPTE_SUBSECTION
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned __int64 Valid : 1; /* bit position: 0 */
        /* 0x0000 */ unsigned __int64 Unused0 : 3; /* bit position: 1 */
        /* 0x0000 */ unsigned __int64 SwizzleBit : 1; /* bit position: 4 */
        /* 0x0000 */ unsigned __int64 Protection : 5; /* bit position: 5 */
        /* 0x0000 */ unsigned __int64 Prototype : 1; /* bit position: 10 */
        /* 0x0000 */ unsigned __int64 ColdPage : 1; /* bit position: 11 */
        /* 0x0000 */ unsigned __int64 Unused1 : 3; /* bit position: 12 */
        /* 0x0000 */ unsigned __int64 ExecutePrivilege : 1; /* bit position: 15 */
        /* 0x0000 */ __int64 SubsectionAddress : 48; /* bit position: 16 */
    }; /* bitfield */
} MMPTE_SUBSECTION, * PMMPTE_SUBSECTION; /* size: 0x0008 */

typedef struct _MMPTE_LIST
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned __int64 Valid : 1; /* bit position: 0 */
        /* 0x0000 */ unsigned __int64 OneEntry : 1; /* bit position: 1 */
        /* 0x0000 */ unsigned __int64 filler0 : 2; /* bit position: 2 */
        /* 0x0000 */ unsigned __int64 SwizzleBit : 1; /* bit position: 4 */
        /* 0x0000 */ unsigned __int64 Protection : 5; /* bit position: 5 */
        /* 0x0000 */ unsigned __int64 Prototype : 1; /* bit position: 10 */
        /* 0x0000 */ unsigned __int64 Transition : 1; /* bit position: 11 */
        /* 0x0000 */ unsigned __int64 filler1 : 16; /* bit position: 12 */
        /* 0x0000 */ unsigned __int64 NextEntry : 36; /* bit position: 28 */
    }; /* bitfield */
} MMPTE_LIST, * PMMPTE_LIST; /* size: 0x0008 */

typedef struct _MMPTE
{
    union
    {
        union
        {
            /* 0x0000 */ unsigned __int64 Long;
            /* 0x0000 */ volatile unsigned __int64 VolatileLong;
            /* 0x0000 */ struct _MMPTE_HARDWARE Hard;
            /* 0x0000 */ struct _MMPTE_PROTOTYPE Proto;
            /* 0x0000 */ struct _MMPTE_SOFTWARE Soft;
            /* 0x0000 */ struct _MMPTE_TIMESTAMP TimeStamp;
            /* 0x0000 */ struct _MMPTE_TRANSITION Trans;
            /* 0x0000 */ struct _MMPTE_SUBSECTION Subsect;
            /* 0x0000 */ struct _MMPTE_LIST List;
        }; /* size: 0x0008 */
    } /* size: 0x0008 */ u;
} MMPTE, * PMMPTE; /* size: 0x0008 */

typedef struct _MIPFNBLINK
{
    union
    {
        struct /* bitfield */
        {
            /* 0x0000 */ unsigned __int64 Blink : 40; /* bit position: 0 */
            /* 0x0000 */ unsigned __int64 NodeBlinkLow : 19; /* bit position: 40 */
            /* 0x0000 */ unsigned __int64 TbFlushStamp : 3; /* bit position: 59 */
            /* 0x0000 */ unsigned __int64 PageBlinkDeleteBit : 1; /* bit position: 62 */
            /* 0x0000 */ unsigned __int64 PageBlinkLockBit : 1; /* bit position: 63 */
        }; /* bitfield */
        struct /* bitfield */
        {
            /* 0x0000 */ unsigned __int64 ShareCount : 62; /* bit position: 0 */
            /* 0x0000 */ unsigned __int64 PageShareCountDeleteBit : 1; /* bit position: 62 */
            /* 0x0000 */ unsigned __int64 PageShareCountLockBit : 1; /* bit position: 63 */
        }; /* bitfield */
        /* 0x0000 */ unsigned __int64 EntireField;
        /* 0x0000 */ volatile __int64 Lock;
        struct /* bitfield */
        {
            /* 0x0000 */ unsigned __int64 LockNotUsed : 62; /* bit position: 0 */
            /* 0x0000 */ unsigned __int64 DeleteBit : 1; /* bit position: 62 */
            /* 0x0000 */ unsigned __int64 LockBit : 1; /* bit position: 63 */
        }; /* bitfield */
    }; /* size: 0x0008 */
} MIPFNBLINK, * PMIPFNBLINK; /* size: 0x0008 */

typedef struct _MMPFNENTRY1
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned char PageLocation : 3; /* bit position: 0 */
        /* 0x0000 */ unsigned char WriteInProgress : 1; /* bit position: 3 */
        /* 0x0000 */ unsigned char Modified : 1; /* bit position: 4 */
        /* 0x0000 */ unsigned char ReadInProgress : 1; /* bit position: 5 */
        /* 0x0000 */ unsigned char CacheAttribute : 2; /* bit position: 6 */
    }; /* bitfield */
} MMPFNENTRY1, * PMMPFNENTRY1; /* size: 0x0001 */

typedef struct _MMPFNENTRY3
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned char Priority : 3; /* bit position: 0 */
        /* 0x0000 */ unsigned char OnProtectedStandby : 1; /* bit position: 3 */
        /* 0x0000 */ unsigned char InPageError : 1; /* bit position: 4 */
        /* 0x0000 */ unsigned char SystemChargedPage : 1; /* bit position: 5 */
        /* 0x0000 */ unsigned char RemovalRequested : 1; /* bit position: 6 */
        /* 0x0000 */ unsigned char ParityError : 1; /* bit position: 7 */
    }; /* bitfield */
} MMPFNENTRY3, * PMMPFNENTRY3; /* size: 0x0001 */

typedef struct _MI_PFN_ULONG5
{
    union
    {
        /* 0x0000 */ unsigned long EntireField;
        struct
        {
            struct /* bitfield */
            {
                /* 0x0000 */ unsigned long NodeBlinkHigh : 21; /* bit position: 0 */
                /* 0x0000 */ unsigned long NodeFlinkMiddle : 11; /* bit position: 21 */
            }; /* bitfield */
        } /* size: 0x0004 */ StandbyList;
        struct
        {
            /* 0x0000 */ unsigned char ModifiedListBucketIndex : 4; /* bit position: 0 */
        } /* size: 0x0001 */ MappedPageList;
        struct
        {
            struct /* bitfield */
            {
                /* 0x0000 */ unsigned char AnchorLargePageSize : 2; /* bit position: 0 */
                /* 0x0000 */ unsigned char Spare1 : 6; /* bit position: 2 */
            }; /* bitfield */
            /* 0x0001 */ unsigned char ViewCount;
            /* 0x0002 */ unsigned short Spare2;
        } /* size: 0x0004 */ Active;
    }; /* size: 0x0004 */
} MI_PFN_ULONG5, * PMI_PFN_ULONG5; /* size: 0x0004 */

typedef struct _MMPFN
{
    union
    {
        /* 0x0000 */ struct _LIST_ENTRY ListEntry;
        /* 0x0000 */ struct _RTL_BALANCED_NODE TreeNode;
        struct
        {
            union
            {
                union
                {
                    /* 0x0000 */ struct _SINGLE_LIST_ENTRY NextSlistPfn;
                    /* 0x0000 */ void* Next;
                    struct /* bitfield */
                    {
                        /* 0x0000 */ unsigned __int64 Flink : 40; /* bit position: 0 */
                        /* 0x0000 */ unsigned __int64 NodeFlinkLow : 24; /* bit position: 40 */
                    }; /* bitfield */
                    /* 0x0000 */ struct _MI_ACTIVE_PFN Active;
                }; /* size: 0x0008 */
            } /* size: 0x0008 */ u1;
            union
            {
                /* 0x0008 */ struct _MMPTE* PteAddress;
                /* 0x0008 */ unsigned __int64 PteLong;
            }; /* size: 0x0008 */
            /* 0x0010 */ struct _MMPTE OriginalPte;
        }; /* size: 0x0018 */
    }; /* size: 0x0018 */
    /* 0x0018 */ struct _MIPFNBLINK u2;
    union
    {
        union
        {
            struct
            {
                /* 0x0020 */ unsigned short ReferenceCount;
                /* 0x0022 */ struct _MMPFNENTRY1 e1;
                /* 0x0023 */ struct _MMPFNENTRY3 e3;
            }; /* size: 0x0004 */
            struct
            {
                /* 0x0020 */ unsigned short ReferenceCount;
            } /* size: 0x0002 */ e2;
            struct
            {
                /* 0x0020 */ unsigned long EntireField;
            } /* size: 0x0004 */ e4;
        }; /* size: 0x0004 */
    } /* size: 0x0004 */ u3;
    /* 0x0024 */ struct _MI_PFN_ULONG5 u5;
    union
    {
        union
        {
            struct /* bitfield */
            {
                /* 0x0028 */ unsigned __int64 PteFrame : 40; /* bit position: 0 */
                /* 0x0028 */ unsigned __int64 ResidentPage : 1; /* bit position: 40 */
                /* 0x0028 */ unsigned __int64 Unused1 : 1; /* bit position: 41 */
                /* 0x0028 */ unsigned __int64 Unused2 : 1; /* bit position: 42 */
                /* 0x0028 */ unsigned __int64 Partition : 10; /* bit position: 43 */
                /* 0x0028 */ unsigned __int64 FileOnly : 1; /* bit position: 53 */
                /* 0x0028 */ unsigned __int64 PfnExists : 1; /* bit position: 54 */
                /* 0x0028 */ unsigned __int64 NodeFlinkHigh : 5; /* bit position: 55 */
                /* 0x0028 */ unsigned __int64 PageIdentity : 3; /* bit position: 60 */
                /* 0x0028 */ unsigned __int64 PrototypePte : 1; /* bit position: 63 */
            }; /* bitfield */
            /* 0x0028 */ unsigned __int64 EntireField;
        }; /* size: 0x0008 */
    } /* size: 0x0008 */ u4;
} MMPFN, * PMMPFN; /* size: 0x0030 */

uint64_t savedbaseadd;
uint64_t savedptr;
bool cr3_check = false;
HANDLE pid_save;

NTKERNELAPI
extern "C" NTSTATUS
PsLookupProcessByProcessId(
    IN HANDLE ProcessId,
    OUT PEPROCESS * Process
);

extern "C" NTKERNELAPI PVOID PsGetProcessSectionBaseAddress(
    IN PEPROCESS		Process
);

NTKERNELAPI
extern "C" NTSTATUS
IoCreateDriver(
    IN PUNICODE_STRING DriverName, OPTIONAL
    IN PDRIVER_INITIALIZE InitializationFunction
);