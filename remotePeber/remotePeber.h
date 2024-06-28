#pragma once

#include <locale.h>
#include <stdio.h>

#include <windows.h>
#include <winternl.h>
#include <tlhelp32.h>

// https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntqueryinformationprocess
typedef NTSTATUS(NTAPI* NtQueryInformationProcessPtr)(
	HANDLE           ProcessHandle,
	PROCESSINFOCLASS ProcessInformationClass,
	PVOID            ProcessInformation,
	ULONG            ProcessInformationLength,
	PULONG           ReturnLength
);

// https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-rtlntstatustodoserror
typedef ULONG(NTAPI* RtlNtStatusToDosErrorPtr)(
	NTSTATUS Status
);

// Generated with https://github.com/ajkhoury/Windbg2Struct
typedef struct _MY_PEB
{
	UCHAR InheritedAddressSpace; // 0x0
	UCHAR ReadImageFileExecOptions; // 0x1
	UCHAR BeingDebugged; // 0x2
	union
	{
		UCHAR BitField; // 0x3
		struct
		{
			UCHAR ImageUsesLargePages : 1; // 0x3
			UCHAR IsProtectedProcess : 1; // 0x3
			UCHAR IsImageDynamicallyRelocated : 1; // 0x3
			UCHAR SkipPatchingUser32Forwarders : 1; // 0x3
			UCHAR IsPackagedProcess : 1; // 0x3
			UCHAR IsAppContainer : 1; // 0x3
			UCHAR IsProtectedProcessLight : 1; // 0x3
			UCHAR IsLongPathAwareProcess : 1; // 0x3
		};
	};
	UCHAR Padding0[4]; // 0x4
	PVOID Mutant; // 0x8
	PVOID ImageBaseAddress; // 0x10
	struct _PEB_LDR_DATA* Ldr; // 0x18
	struct _RTL_USER_PROCESS_PARAMETERS* ProcessParameters; // 0x20
	PVOID SubSystemData; // 0x28
	PVOID ProcessHeap; // 0x30
	struct _RTL_CRITICAL_SECTION* FastPebLock; // 0x38
	LONGLONG AtlThunkSListPtr; // 0x40
	PVOID IFEOKey; // 0x48
	union
	{
		ULONG CrossProcessFlags; // 0x50
		struct
		{
			ULONG ProcessInJob : 1; // 0x50
			ULONG ProcessInitializing : 1; // 0x50
			ULONG ProcessUsingVEH : 1; // 0x50
			ULONG ProcessUsingVCH : 1; // 0x50
			ULONG ProcessUsingFTH : 1; // 0x50
			ULONG ProcessPreviouslyThrottled : 1; // 0x50
			ULONG ProcessCurrentlyThrottled : 1; // 0x50
			ULONG ProcessImagesHotPatched : 1; // 0x50
			ULONG ReservedBits0 : 24; // 0x50
		};
	};
	UCHAR Padding1[4]; // 0x54
	PVOID UserSharedInfoPtr; // 0x58
	ULONG SystemReserved; // 0x60
	ULONG AtlThunkSListPtr32; // 0x64
	PVOID ApiSetMap; // 0x68
	ULONG TlsExpansionCounter; // 0x70
	UCHAR Padding2[4]; // 0x74
	struct _RTL_BITMAP* TlsBitmap; // 0x78
	ULONG TlsBitmapBits[2]; // 0x80
	PVOID ReadOnlySharedMemoryBase; // 0x88
	PVOID SharedData; // 0x90
	PVOID* ReadOnlyStaticServerData; // 0x98
	PVOID AnsiCodePageData; // 0xA0
	PVOID OemCodePageData; // 0xA8
	PVOID UnicodeCaseTableData; // 0xB0
	ULONG NumberOfProcessors; // 0xB8
	ULONG NtGlobalFlag; // 0xBC
	LONGLONG CriticalSectionTimeout; // 0xC0
	ULONG64 HeapSegmentReserve; // 0xC8
	ULONG64 HeapSegmentCommit; // 0xD0
	ULONG64 HeapDeCommitTotalFreeThreshold; // 0xD8
	ULONG64 HeapDeCommitFreeBlockThreshold; // 0xE0
	ULONG NumberOfHeaps; // 0xE8
	ULONG MaximumNumberOfHeaps; // 0xEC
	PVOID* ProcessHeaps; // 0xF0
	PVOID GdiSharedHandleTable; // 0xF8
	PVOID ProcessStarterHelper; // 0x100
	ULONG GdiDCAttributeList; // 0x108
	UCHAR Padding3[4]; // 0x10C
	struct _RTL_CRITICAL_SECTION* LoaderLock; // 0x110
	ULONG OSMajorVersion; // 0x118
	ULONG OSMinorVersion; // 0x11C
	USHORT OSBuildNumber; // 0x120
	USHORT OSCSDVersion; // 0x122
	ULONG OSPlatformId; // 0x124
	ULONG ImageSubsystem; // 0x128
	ULONG ImageSubsystemMajorVersion; // 0x12C
	ULONG ImageSubsystemMinorVersion; // 0x130
	UCHAR Padding4[4]; // 0x134
	ULONG64 ActiveProcessAffinityMask; // 0x138
	ULONG GdiHandleBuffer[60]; // 0x140
	void* PostProcessInitRoutine; // 0x230
	struct _RTL_BITMAP* TlsExpansionBitmap; // 0x238
	ULONG TlsExpansionBitmapBits[32]; // 0x240
	ULONG SessionId; // 0x2C0
	UCHAR Padding5[4]; // 0x2C4
	LONGLONG AppCompatFlags; // 0x2C8
	LONGLONG AppCompatFlagsUser; // 0x2D0
	PVOID pShimData; // 0x2D8
	PVOID AppCompatInfo; // 0x2E0
	struct _UNICODE_STRING CSDVersion; // 0x2E8
	struct _ACTIVATION_CONTEXT_DATA* ActivationContextData; // 0x2F8
	struct _ASSEMBLY_STORAGE_MAP* ProcessAssemblyStorageMap; // 0x300
	struct _ACTIVATION_CONTEXT_DATA* SystemDefaultActivationContextData; // 0x308
	struct _ASSEMBLY_STORAGE_MAP* SystemAssemblyStorageMap; // 0x310
	ULONG64 MinimumStackCommit; // 0x318
	PVOID SparePointers[2]; // 0x320
	PVOID PatchLoaderData; // 0x330
	struct _CHPEV2_PROCESS_INFO* ChpeV2ProcessInfo; // 0x338
	ULONG AppModelFeatureState; // 0x340
	ULONG SpareUlongs[2]; // 0x344
	USHORT ActiveCodePage; // 0x34C
	USHORT OemCodePage; // 0x34E
	USHORT UseCaseMapping; // 0x350
	USHORT UnusedNlsField; // 0x352
	PVOID WerRegistrationData; // 0x358
	PVOID WerShipAssertPtr; // 0x360
	PVOID EcCodeBitMap; // 0x368
	PVOID pImageHeaderHash; // 0x370
	union
	{
		ULONG TracingFlags; // 0x378
		struct
		{
			ULONG HeapTracingEnabled : 1; // 0x378
			ULONG CritSecTracingEnabled : 1; // 0x378
			ULONG LibLoaderTracingEnabled : 1; // 0x378
			ULONG SpareTracingBits : 29; // 0x378
		};
	};
	UCHAR Padding6[4]; // 0x37C
	ULONG64 CsrServerReadOnlySharedMemoryBase; // 0x380
	ULONG64 TppWorkerpListLock; // 0x388
	struct _LIST_ENTRY TppWorkerpList; // 0x390
	PVOID WaitOnAddressHashTable[128]; // 0x3A0
	PVOID TelemetryCoverageHeader; // 0x7A0
	ULONG CloudFileFlags; // 0x7A8
	ULONG CloudFileDiagFlags; // 0x7AC
	CHAR PlaceholderCompatibilityMode; // 0x7B0
	CHAR PlaceholderCompatibilityModeReserved[7]; // 0x7B1
	struct _LEAP_SECOND_DATA* LeapSecondData; // 0x7B8
	union
	{
		ULONG LeapSecondFlags; // 0x7C0
		struct
		{
			ULONG SixtySecondEnabled : 1; // 0x7C0
			ULONG Reserved : 31; // 0x7C0
		};
	};
	ULONG NtGlobalFlag2; // 0x7C4
	ULONG64 ExtendedFeatureDisableMask; // 0x7C8
} MY_PEB, * MY_PPEB;