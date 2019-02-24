#include <Windows.h>

typedef struct MMAP_RET
{
	DWORD64 base_address;
	DWORD64 actual_size;
} MMapRet, *PMMapRet;

typedef struct _HOOK_DATA
{
	uintptr_t function_address;
	uintptr_t hook_function;
	char jmp[50];
	UINT64 asm_instr_length;
	char original_bytes[50];
	uintptr_t trampoline_function;
	uintptr_t jmp_address;
} HOOK_DATA, *PHOOK_DATA;

typedef struct _DRIVER_RETURN
{
	BOOL Return;
	ULONGLONG Value;
} DRIVER_RETURN, *PDRIVER_RETURN;

typedef struct _GET_MODULE_HANDLE
{
	WCHAR MoudleName[256];
	HANDLE     pid;
}GET_MODULE_HANDLE, *PGET_MODULE_HANDLE;

typedef struct _READ_WRITE
{
	ULONGLONG BaseAddress;
	ULONGLONG Buffer;
	SIZE_T Size;
	HANDLE TargetPID;
	HANDLE SourcePID;
	BOOL write;
	BOOL unsafe;
}READ_WRITE, *PREAD_WRITE;

typedef struct _PEB_LDR_DATA {
	BYTE Reserved1[8];
	PVOID Reserved2[3];
	LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _KERNEL_GET_MODULE_REQUEST
{
	ULONG ProcessId;
	UINT_PTR BaseAddress;

} KERNEL_GET_MODULE_REQUEST, *PKERNEL_GET_MODULE_REQUEST;
