/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef _WINTERNL_
#define _WINTERNL_

#include <windef.h>

#ifndef NT_SUCCESS
#define NT_SUCCESS(status)	((NTSTATUS) (status) >= 0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __UNICODE_STRING_DEFINED
#define __UNICODE_STRING_DEFINED
  typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
  } UNICODE_STRING;
#endif

  typedef struct _PEB_LDR_DATA {
    BYTE Reserved1[8];
    PVOID Reserved2[3];
    LIST_ENTRY InMemoryOrderModuleList;
  } PEB_LDR_DATA,*PPEB_LDR_DATA;
 
  typedef struct _LDR_DATA_TABLE_ENTRY {
    PVOID Reserved1[2];
    LIST_ENTRY InMemoryOrderLinks;
    PVOID Reserved2[2];
    PVOID DllBase;
    PVOID Reserved3[2];
    UNICODE_STRING FullDllName;
    BYTE Reserved4[8];
    PVOID Reserved5[3];
    __C89_NAMELESS union {
      ULONG CheckSum;
      PVOID Reserved6;
    };
    ULONG TimeDateStamp;
  } LDR_DATA_TABLE_ENTRY,*PLDR_DATA_TABLE_ENTRY;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
    BYTE Reserved1[16];
    PVOID Reserved2[10];
    UNICODE_STRING ImagePathName;
    UNICODE_STRING CommandLine;
  } RTL_USER_PROCESS_PARAMETERS,*PRTL_USER_PROCESS_PARAMETERS;
  
  /* This function pointer is undocumented and just valid for windows 2000.
     Therefore I guess.  */
  typedef VOID (WINAPI *PPS_POST_PROCESS_INIT_ROUTINE)(VOID);
  
  typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    PVOID Reserved3[2];
    PPEB_LDR_DATA Ldr;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    BYTE Reserved4[104];
    PVOID Reserved5[52];
    PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
    BYTE Reserved6[128];
    PVOID Reserved7[1];
    ULONG SessionId;
  } PEB,*PPEB;

  typedef struct _TEB {
    BYTE Reserved1[1952];
    PVOID Reserved2[412];
    PVOID TlsSlots[64];
    BYTE Reserved3[8];
    PVOID Reserved4[26];
    PVOID ReservedForOle;
    PVOID Reserved5[4];
    PVOID TlsExpansionSlots;
  } TEB;

  typedef TEB *PTEB;
  typedef LONG NTSTATUS;
  typedef CONST char *PCSZ;

#ifndef __STRING_DEFINED
#define __STRING_DEFINED
  typedef struct _STRING {
    USHORT Length;
    USHORT MaximumLength;
    PCHAR Buffer;
  } STRING;
#endif

  typedef STRING *PSTRING;
  typedef STRING ANSI_STRING;
  typedef PSTRING PANSI_STRING;
  typedef PSTRING PCANSI_STRING;
  typedef STRING OEM_STRING;
  typedef PSTRING POEM_STRING;
  typedef CONST STRING *PCOEM_STRING;

  typedef UNICODE_STRING *PUNICODE_STRING;
  typedef const UNICODE_STRING *PCUNICODE_STRING;

#ifndef __OBJECT_ATTRIBUTES_DEFINED
#define __OBJECT_ATTRIBUTES_DEFINED
  typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
#ifdef _WIN64
    ULONG pad1;
#endif
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
#ifdef _WIN64
    ULONG pad2;
#endif
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
  } OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;
#endif

/* Values for the Attributes member */
 #define OBJ_INHERIT             0x00000002
 #define OBJ_PERMANENT           0x00000010
 #define OBJ_EXCLUSIVE           0x00000020
 #define OBJ_CASE_INSENSITIVE    0x00000040
 #define OBJ_OPENIF              0x00000080
 #define OBJ_OPENLINK            0x00000100
 #define OBJ_KERNEL_HANDLE       0x00000200
 #define OBJ_FORCE_ACCESS_CHECK  0x00000400
 #define OBJ_VALID_ATTRIBUTES    0x000007F2

 /* Helper Macro */
 #define InitializeObjectAttributes(p,n,a,r,s) { \
   (p)->Length = sizeof(OBJECT_ATTRIBUTES); \
   (p)->RootDirectory = (r); \
   (p)->Attributes = (a); \
   (p)->ObjectName = (n); \
   (p)->SecurityDescriptor = (s); \
   (p)->SecurityQualityOfService = NULL; \
 }

  typedef struct _OBJECT_DATA_INFORMATION {
    BOOLEAN InheritHandle;
    BOOLEAN ProtectFromClose;
  } OBJECT_DATA_INFORMATION, *POBJECT_DATA_INFORMATION;

  typedef struct _OBJECT_BASIC_INFORMATION {
    ULONG  Attributes;
    ACCESS_MASK  GrantedAccess;
    ULONG  HandleCount;
    ULONG  PointerCount;
    ULONG  PagedPoolUsage;
    ULONG  NonPagedPoolUsage;
    ULONG  Reserved[3];
    ULONG  NameInformationLength;
    ULONG  TypeInformationLength;
    ULONG  SecurityDescriptorLength;
    LARGE_INTEGER  CreateTime;
  } OBJECT_BASIC_INFORMATION, *POBJECT_BASIC_INFORMATION;

  typedef struct _OBJECT_NAME_INFORMATION {
    UNICODE_STRING Name;
  } OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;

  typedef struct _OBJECT_TYPE_INFORMATION {
    UNICODE_STRING TypeName;
    ULONG TotalNumberOfObjects;
    ULONG TotalNumberOfHandles;
    ULONG TotalPagedPoolUsage;
    ULONG TotalNonPagedPoolUsage;
    ULONG TotalNamePoolUsage;
    ULONG TotalHandleTableUsage;
    ULONG HighWaterNumberOfObjects;
    ULONG HighWaterNumberOfHandles;
    ULONG HighWaterPagedPoolUsage;
    ULONG HighWaterNonPagedPoolUsage;
    ULONG HighWaterNamePoolUsage;
    ULONG HighWaterHandleTableUsage;
    ULONG InvalidAttributes;
    GENERIC_MAPPING GenericMapping;
    ULONG ValidAccessMask;
    BOOLEAN SecurityRequired;
    BOOLEAN MaintainHandleCount;
    ULONG PoolType;
    ULONG DefaultPagedPoolCharge;
    ULONG DefaultNonPagedPoolCharge;
  } OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;

  typedef struct _OBJECT_ALL_INFORMATION { 
    ULONG NumberOfObjects; 
    OBJECT_TYPE_INFORMATION ObjectTypeInformation[1]; 
  }OBJECT_ALL_INFORMATION, *POBJECT_ALL_INFORMATION;

  typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation = 1,
    FileFullDirectoryInformation,
    FileBothDirectoryInformation,
    FileBasicInformation,
    FileStandardInformation,
    FileInternalInformation,
    FileEaInformation,
    FileAccessInformation,
    FileNameInformation,
    FileRenameInformation,
    FileLinkInformation,
    FileNamesInformation,
    FileDispositionInformation,
    FilePositionInformation,
    FileFullEaInformation,
    FileModeInformation,
    FileAlignmentInformation,
    FileAllInformation,
    FileAllocationInformation,
    FileEndOfFileInformation,
    FileAlternateNameInformation,
    FileStreamInformation,
    FilePipeInformation,
    FilePipeLocalInformation,
    FilePipeRemoteInformation,
    FileMailslotQueryInformation,
    FileMailslotSetInformation,
    FileCompressionInformation,
    FileObjectIdInformation,
    FileCompletionInformation,
    FileMoveClusterInformation,
    FileQuotaInformation,
    FileReparsePointInformation,
    FileNetworkOpenInformation,
    FileAttributeTagInformation,
    FileTrackingInformation,
    FileIdBothDirectoryInformation,
    FileIdFullDirectoryInformation,
    FileValidDataLengthInformation,
    FileShortNameInformation = 40,
    FileSfioReserveInformation = 44,
    FileSfioVolumeInformation = 45,
    FileHardLinkInformation = 46,
    FileNormalizedNameInformation = 48,
    FileIdGlobalTxDirectoryInformation = 50,
    FileStandardLinkInformation = 54,
    FileMaximumInformation
  } FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

  typedef struct _FILE_DIRECTORY_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    WCHAR FileName[ANYSIZE_ARRAY];
  } FILE_DIRECTORY_INFORMATION, *PFILE_DIRECTORY_INFORMATION;

  typedef struct _FILE_FULL_DIRECTORY_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    WCHAR FileName[ANYSIZE_ARRAY];
  } FILE_FULL_DIRECTORY_INFORMATION, *PFILE_FULL_DIRECTORY_INFORMATION,FILE_FULL_DIR_INFORMATION, *PFILE_FULL_DIR_INFORMATION;

  typedef struct _FILE_ID_FULL_DIRECTORY_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    LARGE_INTEGER FileId;
    WCHAR FileName[ANYSIZE_ARRAY];
  } FILE_ID_FULL_DIRECTORY_INFORMATION, *PFILE_ID_FULL_DIRECTORY_INFORMATION;

  typedef struct _FILE_BOTH_DIRECTORY_INFORMATION {
    ULONG NextEntryOffset;
	 ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    CHAR ShortNameLength;
    WCHAR ShortName[12];
    WCHAR FileName[ANYSIZE_ARRAY];
  } FILE_BOTH_DIRECTORY_INFORMATION, *PFILE_BOTH_DIRECTORY_INFORMATION,FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;

  typedef struct _FILE_ID_BOTH_DIRECTORY_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    CHAR ShortNameLength;
    WCHAR ShortName[12];
    LARGE_INTEGER FileId;
    WCHAR FileName[ANYSIZE_ARRAY];
  } FILE_ID_BOTH_DIRECTORY_INFORMATION, *PFILE_ID_BOTH_DIRECTORY_INFORMATION;

  typedef struct _FILE_BASIC_INFORMATION {
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    ULONG FileAttributes;
  } FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;

  typedef struct _FILE_STANDARD_INFORMATION {
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG NumberOfLinks;
    BOOLEAN DeletePending;
    BOOLEAN Directory;
  } FILE_STANDARD_INFORMATION, *PFILE_STANDARD_INFORMATION;

  typedef struct _FILE_INTERNAL_INFORMATION {
    LARGE_INTEGER IndexNumber;
  } FILE_INTERNAL_INFORMATION, *PFILE_INTERNAL_INFORMATION;

  typedef struct _FILE_EA_INFORMATION {
    ULONG EaSize;
  } FILE_EA_INFORMATION, *PFILE_EA_INFORMATION;

  typedef struct _FILE_ACCESS_INFORMATION {
    ACCESS_MASK AccessFlags;
  } FILE_ACCESS_INFORMATION, *PFILE_ACCESS_INFORMATION;

  typedef struct _FILE_LINK_INFORMATION {
    BOOLEAN ReplaceIfExists;
    HANDLE RootDirectory;
    ULONG FileNameLength;
    WCHAR FileName[1];
  } FILE_LINK_INFORMATION, *PFILE_LINK_INFORMATION;

  typedef struct _FILE_NAME_INFORMATION {
    ULONG FileNameLength;
    WCHAR FileName[1];
  } FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;

  typedef struct _FILE_RENAME_INFORMATION {
    BOOLEAN ReplaceIfExists;
    HANDLE RootDirectory;
    ULONG FileNameLength;
    WCHAR FileName[1];
  } FILE_RENAME_INFORMATION, *PFILE_RENAME_INFORMATION;

  typedef struct _FILE_NAMES_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    ULONG FileNameLength;
    WCHAR FileName[1];
  } FILE_NAMES_INFORMATION, *PFILE_NAMES_INFORMATION;

  typedef struct _FILE_DISPOSITION_INFORMATION {
    BOOLEAN DoDeleteFile;
  } FILE_DISPOSITION_INFORMATION, *PFILE_DISPOSITION_INFORMATION;

  typedef struct _FILE_POSITION_INFORMATION {
    LARGE_INTEGER CurrentByteOffset;
  } FILE_POSITION_INFORMATION, *PFILE_POSITION_INFORMATION;

  typedef struct _FILE_ALIGNMENT_INFORMATION {
    ULONG AlignmentRequirement;
  } FILE_ALIGNMENT_INFORMATION, *PFILE_ALIGNMENT_INFORMATION;

  typedef struct _FILE_ALLOCATION_INFORMATION {
    LARGE_INTEGER AllocationSize;
  } FILE_ALLOCATION_INFORMATION, *PFILE_ALLOCATION_INFORMATION;

  typedef struct _FILE_END_OF_FILE_INFORMATION {
    LARGE_INTEGER EndOfFile;
  } FILE_END_OF_FILE_INFORMATION, *PFILE_END_OF_FILE_INFORMATION;

  typedef struct _FILE_NETWORK_OPEN_INFORMATION {
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG FileAttributes;
  } FILE_NETWORK_OPEN_INFORMATION, *PFILE_NETWORK_OPEN_INFORMATION;

  typedef struct _FILE_FULL_EA_INFORMATION {
    ULONG NextEntryOffset;
    UCHAR Flags;
    UCHAR EaNameLength;
    USHORT EaValueLength;
    CHAR EaName[1];
  } FILE_FULL_EA_INFORMATION, *PFILE_FULL_EA_INFORMATION;

  typedef struct _FILE_MODE_INFORMATION {
    ULONG Mode;
  } FILE_MODE_INFORMATION, *PFILE_MODE_INFORMATION;

  typedef struct _FILE_STREAM_INFORMATION {
    ULONG NextEntryOffset;
    ULONG StreamNameLength;
    LARGE_INTEGER StreamSize;
    LARGE_INTEGER StreamAllocationSize;
    WCHAR StreamName[1];
  } FILE_STREAM_INFORMATION, *PFILE_STREAM_INFORMATION;

  typedef struct _FILE_ATTRIBUTE_TAG_INFORMATION {
    ULONG FileAttributes;
    ULONG ReparseTag;
  } FILE_ATTRIBUTE_TAG_INFORMATION, *PFILE_ATTRIBUTE_TAG_INFORMATION;

  typedef struct _FILE_MAILSLOT_QUERY_INFORMATION {
    ULONG MaximumMessageSize;
    ULONG MailslotQuota;
    ULONG NextMessageSize;
    ULONG MessagesAvailable;
    LARGE_INTEGER ReadTimeout;
  } FILE_MAILSLOT_QUERY_INFORMATION, *PFILE_MAILSLOT_QUERY_INFORMATION;

  typedef struct _FILE_MAILSLOT_SET_INFORMATION {
    LARGE_INTEGER ReadTimeout;
  } FILE_MAILSLOT_SET_INFORMATION, *PFILE_MAILSLOT_SET_INFORMATION;

  typedef struct _FILE_PIPE_LOCAL_INFORMATION {
    ULONG NamedPipeType;
    ULONG NamedPipeConfiguration;
    ULONG MaximumInstances;
    ULONG CurrentInstances;
    ULONG InboundQuota;
    ULONG ReadDataAvailable;
    ULONG OutboundQuota;
    ULONG WriteQuotaAvailable;
    ULONG NamedPipeState;
    ULONG NamedPipeEnd;
  } FILE_PIPE_LOCAL_INFORMATION, *PFILE_PIPE_LOCAL_INFORMATION;

  typedef struct _FILE_ALL_INFORMATION {
    FILE_BASIC_INFORMATION     BasicInformation;
    FILE_STANDARD_INFORMATION  StandardInformation;
    FILE_INTERNAL_INFORMATION  InternalInformation;
    FILE_EA_INFORMATION        EaInformation;
    FILE_ACCESS_INFORMATION    AccessInformation;
    FILE_POSITION_INFORMATION  PositionInformation;
    FILE_MODE_INFORMATION      ModeInformation;
    FILE_ALIGNMENT_INFORMATION AlignmentInformation;
    FILE_NAME_INFORMATION      NameInformation;
  } FILE_ALL_INFORMATION, *PFILE_ALL_INFORMATION;
 
  typedef struct _IO_STATUS_BLOCK {
    __C89_NAMELESS union {
      NTSTATUS Status;
      PVOID Pointer;
    };
    ULONG_PTR Information;
  } IO_STATUS_BLOCK,*PIO_STATUS_BLOCK;

  typedef VOID (NTAPI *PIO_APC_ROUTINE)(PVOID ApcContext,PIO_STATUS_BLOCK IoStatusBlock,ULONG Reserved);

#ifdef __ia64__
  typedef struct _FRAME_POINTERS {
    ULONGLONG MemoryStackFp;
    ULONGLONG BackingStoreFp;
  } FRAME_POINTERS,*PFRAME_POINTERS;

#define UNWIND_HISTORY_TABLE_SIZE 12

  typedef struct _RUNTIME_FUNCTION {
    ULONG BeginAddress;
    ULONG EndAddress;
    ULONG UnwindInfoAddress;
  } RUNTIME_FUNCTION,*PRUNTIME_FUNCTION;

  typedef struct _UNWIND_HISTORY_TABLE_ENTRY {
    ULONG64 ImageBase;
    ULONG64 Gp;
    PRUNTIME_FUNCTION FunctionEntry;
  } UNWIND_HISTORY_TABLE_ENTRY,*PUNWIND_HISTORY_TABLE_ENTRY;

  typedef struct _UNWIND_HISTORY_TABLE {
    ULONG Count;
    UCHAR Search;
    ULONG64 LowAddress;
    ULONG64 HighAddress;
    UNWIND_HISTORY_TABLE_ENTRY Entry[UNWIND_HISTORY_TABLE_SIZE];
  } UNWIND_HISTORY_TABLE,*PUNWIND_HISTORY_TABLE;
#endif

  typedef struct _VM_COUNTERS {
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
  } VM_COUNTERS, *PVM_COUNTERS;

  typedef enum _THREAD_STATE {
    StateInitialized = 0,
    StateReady, StateRunning, StateStandby, StateTerminated,
    StateWait, StateTransition,
    StateUnknown
  } THREAD_STATE;

  typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
  } CLIENT_ID, *PCLIENT_ID;

  typedef LONG KPRIORITY;

  typedef enum _KWAIT_REASON {
    Executive = 0,
    FreePage, PageIn, PoolAllocation, DelayExecution,
    Suspended, UserRequest, WrExecutive, WrFreePage, WrPageIn,
    WrPoolAllocation, WrDelayExecution, WrSuspended,
    WrUserRequest, WrEventPair, WrQueue, WrLpcReceive,
    WrLpcReply, WrVirtualMemory, WrPageOut, WrRendezvous,
    Spare2, Spare3, Spare4, Spare5, Spare6, WrKernel,
    MaximumWaitReason
  } KWAIT_REASON;

  typedef struct _SYSTEM_THREADS
  {
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER CreateTime;
    ULONG WaitTime;
    PVOID StartAddress;
    CLIENT_ID ClientId;
    KPRIORITY Priority;
    KPRIORITY BasePriority;
    ULONG ContextSwitchCount;
    THREAD_STATE State;
    KWAIT_REASON WaitReason;
  } SYSTEM_THREADS, *PSYSTEM_THREADS;

  typedef struct _PROCESS_BASIC_INFORMATION {
    NTSTATUS ExitStatus;
    PPEB PebBaseAddress;
    KAFFINITY AffinityMask;
    KPRIORITY BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR InheritedFromUniqueProcessId;
  } PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;

  typedef struct _KERNEL_USER_TIMES {
    FILETIME CreateTime;
    FILETIME ExitTime;
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
  } KERNEL_USER_TIMES, *PKERNEL_USER_TIMES;

  typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION {
    LARGE_INTEGER IdleTime;
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER Reserved1[2];
    ULONG Reserved2;
  } SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION,*PSYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

  typedef struct _SYSTEM_PROCESS_INFORMATION {
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    LARGE_INTEGER Reserved[3];
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ImageName;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
    ULONG HandleCount;
    ULONG SessionId;
    ULONG PageDirectoryBase;
    VM_COUNTERS VirtualMemoryCounters;
    SIZE_T PrivatePageCount;
    IO_COUNTERS IoCounters;
  } SYSTEM_PROCESS_INFORMATION,*PSYSTEM_PROCESS_INFORMATION;

  typedef struct _SYSTEM_REGISTRY_QUOTA_INFORMATION {
    ULONG RegistryQuotaAllowed;
    ULONG RegistryQuotaUsed;
    PVOID Reserved1;
  } SYSTEM_REGISTRY_QUOTA_INFORMATION,*PSYSTEM_REGISTRY_QUOTA_INFORMATION;

  typedef struct _SYSTEM_BASIC_INFORMATION {
    BYTE Reserved1[4];
    ULONG MaximumIncrement;
    ULONG PhysicalPageSize;
    ULONG NumberOfPhysicalPages;
    ULONG LowestPhysicalPage;
    ULONG HighestPhysicalPage;
    ULONG AllocationGranularity;
    ULONG LowestUserAddress;
    ULONG HighestUserAddress;
    ULONG ActiveProcessors;
    CCHAR NumberOfProcessors;
  } SYSTEM_BASIC_INFORMATION,*PSYSTEM_BASIC_INFORMATION;

  typedef struct _SYSTEM_PROCESSOR_INFORMATION {
    USHORT ProcessorArchitecture;
    USHORT ProcessorLevel;
    USHORT ProcessorRevision;
    USHORT Unknown;
    ULONG FeatureBits;
  } SYSTEM_PROCESSOR_INFORMATION, *PSYSTEM_PROCESSOR_INFORMATION;

  typedef struct _SYSTEM_TIMEOFDAY_INFORMATION {
    LARGE_INTEGER BootTime;
    LARGE_INTEGER CurrentTime;
    LARGE_INTEGER TimeZoneBias;
    ULONG CurrentTimeZoneId;
    BYTE Reserved1[20];
  } SYSTEM_TIMEOFDAY_INFORMATION,*PSYSTEM_TIMEOFDAY_INFORMATION;

  typedef struct _SYSTEM_PERFORMANCE_INFORMATION {
    LARGE_INTEGER IdleTime;
    LARGE_INTEGER ReadTransferCount;
    LARGE_INTEGER WriteTransferCount;
    LARGE_INTEGER OtherTransferCount;
    ULONG ReadOperationCount;
    ULONG WriteOperationCount;
    ULONG OtherOperationCount;
    ULONG AvailablePages;
    ULONG TotalCommittedPages;
    ULONG TotalCommitLimit;
    ULONG PeakCommitment;
    ULONG PageFaults;
    ULONG WriteCopyFaults;
    ULONG TransitionFaults;
    ULONG CacheTransitionFaults;
    ULONG DemandZeroFaults;
    ULONG PagesRead;
    ULONG PageReadIos;
    ULONG CacheReads;
    ULONG CacheIos;
    ULONG PagefilePagesWritten;
    ULONG PagefilePageWriteIos;
    ULONG MappedFilePagesWritten;
    ULONG MappedFilePageWriteIos;
    ULONG PagedPoolUsage;
    ULONG NonPagedPoolUsage;
    ULONG PagedPoolAllocs;
    ULONG PagedPoolFrees;
    ULONG NonPagedPoolAllocs;
    ULONG NonPagedPoolFrees;
    ULONG TotalFreeSystemPtes;
    ULONG SystemCodePage;
    ULONG TotalSystemDriverPages;
    ULONG TotalSystemCodePages;
    ULONG SmallNonPagedLookasideListAllocateHits;
    ULONG SmallPagedLookasideListAllocateHits;
    ULONG Reserved3;
    ULONG MmSystemCachePage;
    ULONG PagedPoolPage;
    ULONG SystemDriverPage;
    ULONG FastReadNoWait;
    ULONG FastReadWait;
    ULONG FastReadResourceMiss;
    ULONG FastReadNotPossible;
    ULONG FastMdlReadNoWait;
    ULONG FastMdlReadWait;
    ULONG FastMdlReadResourceMiss;
    ULONG FastMdlReadNotPossible;
    ULONG MapDataNoWait;
    ULONG MapDataWait;
    ULONG MapDataNoWaitMiss;
    ULONG MapDataWaitMiss;
    ULONG PinMappedDataCount;
    ULONG PinReadNoWait;
    ULONG PinReadWait;
    ULONG PinReadNoWaitMiss;
    ULONG PinReadWaitMiss;
    ULONG CopyReadNoWait;
    ULONG CopyReadWait;
    ULONG CopyReadNoWaitMiss;
    ULONG CopyReadWaitMiss;
    ULONG MdlReadNoWait;
    ULONG MdlReadWait;
    ULONG MdlReadNoWaitMiss;
    ULONG MdlReadWaitMiss;
    ULONG ReadAheadIos;
    ULONG LazyWriteIos;
    ULONG LazyWritePages;
    ULONG DataFlushes;
    ULONG DataPages;
    ULONG ContextSwitches;
    ULONG FirstLevelTbFills;
    ULONG SecondLevelTbFills;
    ULONG SystemCalls;
  } SYSTEM_PERFORMANCE_INFORMATION, *PSYSTEM_PERFORMANCE_INFORMATION;

  typedef struct _SYSTEM_EXCEPTION_INFORMATION {
    BYTE Reserved1[16];
  } SYSTEM_EXCEPTION_INFORMATION,*PSYSTEM_EXCEPTION_INFORMATION;

  typedef struct _SYSTEM_LOOKASIDE_INFORMATION {
    BYTE Reserved1[32];
  } SYSTEM_LOOKASIDE_INFORMATION,*PSYSTEM_LOOKASIDE_INFORMATION;

  typedef struct _SYSTEM_INTERRUPT_INFORMATION {
    BYTE Reserved1[24];
  } SYSTEM_INTERRUPT_INFORMATION,*PSYSTEM_INTERRUPT_INFORMATION;

  typedef struct _SYSTEM_HANDLE_ENTRY {
    ULONG OwnerPid;
    BYTE ObjectType;
    BYTE HandleFlags;
    USHORT HandleValue;
    PVOID ObjectPointer;
    ULONG AccessMask;
  } SYSTEM_HANDLE_ENTRY, *PSYSTEM_HANDLE_ENTRY;

  typedef struct _SYSTEM_HANDLE_INFORMATION {
    ULONG Count;
    SYSTEM_HANDLE_ENTRY Handle[1];
  } SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

  typedef enum _PROCESSINFOCLASS {
    ProcessBasicInformation,
    ProcessQuotaLimits,
    ProcessIoCounters,
    ProcessVmCounters,
    ProcessTimes,
    ProcessBasePriority,
    ProcessRaisePriority,
    ProcessDebugPort,
    ProcessExceptionPort,
    ProcessAccessToken,
    ProcessLdtInformation,
    ProcessLdtSize,
    ProcessDefaultHardErrorMode,
    ProcessIoPortHandlers,
    ProcessPooledUsageAndLimits,
    ProcessWorkingSetWatch,
    ProcessUserModeIOPL,
    ProcessEnableAlignmentFaultFixup,
    ProcessPriorityClass,
    ProcessWx86Information,
    ProcessHandleCount,
    ProcessAffinityMask,
    ProcessPriorityBoost,
    ProcessDeviceMap,
    ProcessSessionInformation,
    ProcessForegroundInformation,
    ProcessWow64Information,
    ProcessImageFileName,
    ProcessLUIDDeviceMapsEnabled,
    ProcessBreakOnTermination,
    ProcessDebugObjectHandle,
    ProcessDebugFlags,
    ProcessHandleTracing,
    ProcessIoPriority,
    ProcessExecuteFlags,
    ProcessTlsInformation,
    ProcessCookie,
    ProcessImageInformation,
    ProcessCycleTime,
    ProcessPagePriority,
    ProcessInstrumentationCallback,
    ProcessThreadStackAllocation,
    ProcessWorkingSetWatchEx,
    ProcessImageFileNameWin32,
    ProcessImageFileMapping,
    ProcessAffinityUpdateMode,
    ProcessMemoryAllocationMode,
    ProcessGroupInformation,
    ProcessTokenVirtualizationEnabled,
    ProcessConsoleHostProcess,
    ProcessWindowInformation,
    MaxProcessInfoClass
  } PROCESSINFOCLASS;

  typedef enum _THREADINFOCLASS {
     ThreadBasicInformation,
     ThreadTimes,
     ThreadPriority,
     ThreadBasePriority,
     ThreadAffinityMask,
     ThreadImpersonationToken,
     ThreadDescriptorTableEntry,
     ThreadEnableAlignmentFaultFixup,
     ThreadEventPair,
     ThreadQuerySetWin32StartAddress,
     ThreadZeroTlsCell,
     ThreadPerformanceCount,
     ThreadAmILastThread,
     ThreadIdealProcessor,
     ThreadPriorityBoost,
     ThreadSetTlsArrayAddress,
     ThreadIsIoPending,
     ThreadHideFromDebugger
  } THREADINFOCLASS;
  typedef THREADINFOCLASS THREAD_INFORMATION_CLASS, *PTHREAD_INFORMATION_CLASS;

  typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemBasicInformation = 0,SystemProcessorInformation = 1,SystemPerformanceInformation = 2,SystemTimeOfDayInformation = 3,SystemProcessInformation = 5,
    SystemProcessorPerformanceInformation = 8,SystemHandleInformation = 16,SystemInterruptInformation = 23,SystemExceptionInformation = 33,SystemRegistryQuotaInformation = 37,
    SystemLookasideInformation = 45
  } SYSTEM_INFORMATION_CLASS;

  typedef enum _OBJECT_INFORMATION_CLASS {
    ObjectBasicInformation,
    ObjectNameInformation,
    ObjectTypeInformation,
    ObjectAllInformation,
    ObjectDataInformation
 } OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;

#define INTERNAL_TS_ACTIVE_CONSOLE_ID (*((volatile ULONG*)(0x7ffe02d8)))

#define RtlMoveMemory(Destination,Source,Length) memmove((Destination),(Source),(Length))
#define RtlFillMemory(Destination,Length,Fill) memset((Destination),(Fill),(Length))
#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))

  NTSTATUS WINAPI NtClose(HANDLE Handle);
  NTSTATUS WINAPI NtCreateFile(PHANDLE FileHandle,ACCESS_MASK DesiredAccess,POBJECT_ATTRIBUTES ObjectAttributes,PIO_STATUS_BLOCK IoStatusBlock,PLARGE_INTEGER AllocationSize,ULONG FileAttributes,ULONG ShareAccess,ULONG CreateDisposition,ULONG CreateOptions,PVOID EaBuffer,ULONG EaLength);
  NTSTATUS WINAPI NtOpenFile(PHANDLE FileHandle,ACCESS_MASK DesiredAccess,POBJECT_ATTRIBUTES ObjectAttributes,PIO_STATUS_BLOCK IoStatusBlock,ULONG ShareAccess,ULONG OpenOptions);
  NTSTATUS WINAPI NtDeviceIoControlFile(HANDLE FileHandle,HANDLE Event,PIO_APC_ROUTINE ApcRoutine,PVOID ApcContext,PIO_STATUS_BLOCK IoStatusBlock,ULONG IoControlCode,PVOID InputBuffer,ULONG InputBufferLength,PVOID OutputBuffer,ULONG OutputBufferLength);
  NTSTATUS WINAPI NtWaitForSingleObject(HANDLE Handle,BOOLEAN Alertable,PLARGE_INTEGER Timeout);
  BOOLEAN WINAPI RtlIsNameLegalDOS8Dot3(PUNICODE_STRING Name,POEM_STRING OemName,PBOOLEAN NameContainsSpaces);
  ULONG WINAPI RtlNtStatusToDosError (NTSTATUS Status);
  NTSTATUS WINAPI NtQueryInformationProcess(HANDLE ProcessHandle,PROCESSINFOCLASS ProcessInformationClass,PVOID ProcessInformation,ULONG ProcessInformationLength,PULONG ReturnLength);
  NTSTATUS WINAPI NtQueryInformationThread(HANDLE ThreadHandle,THREADINFOCLASS ThreadInformationClass,PVOID ThreadInformation,ULONG ThreadInformationLength,PULONG ReturnLength);
  NTSTATUS WINAPI NtQueryInformationFile(HANDLE hFile,PIO_STATUS_BLOCK io,PVOID ptr,ULONG len,FILE_INFORMATION_CLASS FileInformationClass);
  NTSTATUS WINAPI NtQueryObject(HANDLE Handle,OBJECT_INFORMATION_CLASS ObjectInformationClass,PVOID ObjectInformation,ULONG ObjectInformationLength,PULONG ReturnLength);
  NTSTATUS WINAPI NtQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass,PVOID SystemInformation,ULONG SystemInformationLength,PULONG ReturnLength);
  NTSTATUS WINAPI NtQuerySystemTime(PLARGE_INTEGER SystemTime);
  NTSTATUS NTAPI NtSetInformationProcess(HANDLE ProcessHandle, PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength);
  NTSTATUS WINAPI RtlLocalTimeToSystemTime(PLARGE_INTEGER LocalTime,PLARGE_INTEGER SystemTime);
  BOOLEAN WINAPI RtlTimeToSecondsSince1970(PLARGE_INTEGER Time,PULONG ElapsedSeconds);
  VOID WINAPI RtlFreeAnsiString(PANSI_STRING AnsiString);
  VOID WINAPI RtlFreeUnicodeString(PUNICODE_STRING UnicodeString);
  VOID WINAPI RtlFreeOemString(POEM_STRING OemString);
  VOID WINAPI RtlInitString (PSTRING DestinationString,PCSZ SourceString);
  VOID WINAPI RtlInitAnsiString(PANSI_STRING DestinationString,PCSZ SourceString);
  VOID WINAPI RtlInitUnicodeString(PUNICODE_STRING DestinationString,PCWSTR SourceString);
  NTSTATUS WINAPI RtlAnsiStringToUnicodeString(PUNICODE_STRING DestinationString,PCANSI_STRING SourceString,BOOLEAN AllocateDestinationString);
  NTSTATUS WINAPI RtlUnicodeStringToAnsiString(PANSI_STRING DestinationString,PCUNICODE_STRING SourceString,BOOLEAN AllocateDestinationString);
  NTSTATUS WINAPI RtlUnicodeStringToOemString(POEM_STRING DestinationString,PCUNICODE_STRING SourceString,BOOLEAN AllocateDestinationString);
  NTSTATUS WINAPI RtlUnicodeToMultiByteSize(PULONG BytesInMultiByteString,PWCH UnicodeString,ULONG BytesInUnicodeString);
  NTSTATUS WINAPI RtlCharToInteger (PCSZ String,ULONG Base,PULONG Value);
  NTSTATUS WINAPI RtlConvertSidToUnicodeString(PUNICODE_STRING UnicodeString,PSID Sid,BOOLEAN AllocateDestinationString);
  ULONG WINAPI RtlUniform(PULONG Seed);
  VOID WINAPI RtlUnwind (PVOID TargetFrame,PVOID TargetIp,PEXCEPTION_RECORD ExceptionRecord,PVOID ReturnValue);

#ifdef __ia64__
  VOID RtlUnwind2(FRAME_POINTERS TargetFrame,PVOID TargetIp,PEXCEPTION_RECORD ExceptionRecord,PVOID ReturnValue,PCONTEXT ContextRecord);
  VOID RtlUnwindEx(FRAME_POINTERS TargetFrame,PVOID TargetIp,PEXCEPTION_RECORD ExceptionRecord,PVOID ReturnValue,PCONTEXT ContextRecord,PUNWIND_HISTORY_TABLE HistoryTable);
#endif

#define LOGONID_CURRENT ((ULONG)-1)
#define SERVERNAME_CURRENT ((HANDLE)NULL)

  typedef enum _WINSTATIONINFOCLASS {
    WinStationInformation = 8
  } WINSTATIONINFOCLASS;

  typedef struct _WINSTATIONINFORMATIONW {
    BYTE Reserved2[70];
    ULONG LogonId;
    BYTE Reserved3[1140];
  } WINSTATIONINFORMATIONW,*PWINSTATIONINFORMATIONW;

  typedef BOOLEAN (WINAPI *PWINSTATIONQUERYINFORMATIONW)(HANDLE,ULONG,WINSTATIONINFOCLASS,PVOID,ULONG,PULONG);

#ifdef __cplusplus
}
#endif

#endif

