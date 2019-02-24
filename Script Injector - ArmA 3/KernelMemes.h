#include <tchar.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <array>
#include <string>
#include <sstream>
#include <chrono>
#include <cstdio>
#include <vector>
#include <fstream>
#include <TlHelp32.h>
#include <iostream>
#include <stdlib.h>
#include <tlhelp32.h>
#include <direct.h>
#include <stdio.h>
#include <cstring>
#include <Psapi.h>
#include <string_view>
#include <dxgi.h>
#include <d3d9.h>
#include <Softpub.h>
#include <wincrypt.h>
#include <wintrust.h>
#include <thread>
#include "Structs.h"

#include <cstdlib>   // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <io.h>
#include <cstdlib>

#include <tchar.h>
#pragma comment(lib, "urlmon.lib")

#pragma comment (lib, "wintrust")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "psapi.lib")


using namespace std;

static HANDLE drvhandle;

#define MOUNTMGRCONTROLTYPE                         0x0000006D
#define IOCTL_CHECK_ALIVE                CTL_CODE(MOUNTMGRCONTROLTYPE, 0x950 , METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_UNLOAD                     CTL_CODE(MOUNTMGRCONTROLTYPE, 0x955 , METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_GET_MODULE_HANDLE          CTL_CODE(MOUNTMGRCONTROLTYPE, 0x970 , METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_READ_WRITE_MEMORY          CTL_CODE(MOUNTMGRCONTROLTYPE, 0x975 , METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS) //its fine it works


class kernelmemes
{
public:

	kernelmemes(LPCSTR regpath)
	{
		drvhandle = CreateFileA(regpath, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	}

	bool rpm_wpm(int PID, uint64_t lpAddress, PVOID buffer,  uint64_t size, bool write) {

		READ_WRITE pReadWrite;
		DRIVER_RETURN out;
		DWORD bytes;

		pReadWrite.SourcePID = (HANDLE)GetCurrentProcessId();
		pReadWrite.TargetPID = (HANDLE)PID;
		pReadWrite.BaseAddress = lpAddress;
		pReadWrite.Buffer = (ULONGLONG)buffer;
		pReadWrite.Size = size;
		pReadWrite.write = write;

		bool ioctl = DeviceIoControl(drvhandle, IOCTL_READ_WRITE_MEMORY, &pReadWrite, sizeof(pReadWrite), &out, sizeof(out), &bytes, NULL);

		if (out.Return == 0x1337)
			return true;

		return false;
	}

	ULONGLONG BaseAddy(
		std::string ModuleName, 
		int PID) {

		DWORD Bytes;
		GET_MODULE_HANDLE Request;
		DRIVER_RETURN ret{};

		const wchar_t* modname = std::wstring(ModuleName.begin(), ModuleName.end()).c_str();
		if (wcslen(modname) < (sizeof(Request.MoudleName) / sizeof(wchar_t)))
		{
			wcscpy(Request.MoudleName, modname);
		}
		else
			return 0;

		Request.pid = (HANDLE*)PID;

		if (DeviceIoControl(drvhandle, IOCTL_GET_MODULE_HANDLE, &Request, sizeof(Request), &ret, sizeof(ret), &Bytes, NULL)) {
			return ret.Value;
		}
	}
};

