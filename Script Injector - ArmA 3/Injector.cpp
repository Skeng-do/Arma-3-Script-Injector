#include "KernelMemes.h"
#include "Offsets.h"
#include "MetaString.h"

kernelmemes kem(META_STRING("\\\\.\\MountPointManager"));

int ProccessID;
UINT_PTR BaseAddress;
bool checkpidkey = false;
string processname = META_STRING("arma3_x64.exe");
namespace Scripts
{
	string hint = RandomString(7);

	string randomvar1 = RandomString(8);
	string randomvar2 = RandomString(8);
	string var1 = randomvar1 + "_" + randomvar2 + "_" + RandomString(8);
	string var2 = randomvar1 + "_" + randomvar2;
	string Executor = "hint \"hello world\";";
};

DWORD_PTR FindProcessId2222(const std::string& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}


namespace Kernel {

	class KernelMemory {
	private:
		DWORD pID = NULL;

	public:

		KernelMemory(DWORD pid)
		{
			this->pID = pid; 
		}

		__forceinline BOOLEAN KeRtlCopyMemory(const DWORD64 Address, const PVOID Buffer, const DWORD_PTR Size, const BOOLEAN Write) {

			if (!Address || !Buffer || !Size)
				return false;

			if (!kem.rpm_wpm(this->pID, Address, Buffer, Size, Write))
				return false;

			return true;
		};

		template <typename Type>
		Type read(const DWORD64 address)
		{
			if (!address)
				return Type();

			Type buffer;
			return KeRtlCopyMemory(address, &buffer, sizeof(Type), FALSE) ? buffer : Type();
		};

		template <class Type>
		bool write(const DWORD64 address, Type data)
		{
			if (!address)
				return false;

			return KeRtlCopyMemory(address, &data, sizeof(data), TRUE) ? true : false; // need to debug
		};

		std::string readString(const DWORD64 Address, size_t Size) {

			std::vector<char> buffer(Size, char{ 0 });

			if (!Address)
				return std::string("");

			char string[] = "";
			return KeRtlCopyMemory(Address, buffer.data(), buffer.size(), FALSE) ? std::string(buffer.data()) : std::string("");
		};


		std::string readString(const DWORD64 Address) {
			if (!Address)
				return std::string("");

			char string[256] = "";
			return KeRtlCopyMemory(Address, string, 256, FALSE) ? std::string(string) : std::string("");
		};

		bool writeString(const DWORD64 Address, char* String, size_t Size) { // cant be const
			if (!Address)
				return false;

			return KeRtlCopyMemory(Address, String, Size, TRUE) ? true : false;
		};

		bool writeString(const DWORD64 Address, char* String) {
			if (!Address)
				return false;

			return KeRtlCopyMemory(Address, String, 128, TRUE) ? true : false;
		};
	};
};

Kernel::KernelMemory* pMem;

#define _mCast(Address) reinterpret_cast<DWORD64>(Address)
#define Read(Type, Address) pMem->read<Type>(Address) 

void startup()
{
	while (1)
	{
		if (GetAsyncKeyState(VK_F9))
		{
			if (!checkpidkey)
			{
				while (!pidR6S)
				{
					ProccessID = FindProcessId2222(processname);
					BaseAddress = (UINT_PTR)kem.BaseAddy(META_STRING("arma3_x64.exe"), ProccessID);
					Sleep(10);
				}
				checkpidkey = true;
				break;
			}
		}
		Sleep(7);
	};

	if (ProccessID)
		pMem = new Kernel::KernelMemory(ProccessID);
}

void CheckDriver()
{
	if (drvhandle == INVALID_HANDLE_VALUE)
	{
		cout << META_STRING("Error Counldn't Connect To Driver Try Again.") << endl;
		Sleep(8500);
		exit(0);
	}
	else
	{
		cout << META_STRING("Loaded Press F9 On ArmA 3.") << endl;
	}
}

std::string ReadArmaString(DWORD64 Address)
{
	int len = pMem->read<int>(Address + 0x8);
	string read2 = pMem->readString(Address + 0x10, len);

	return std::string(read2);
}

uint64_t GetWorld()
{
	return Read(uint64_t, BaseAddress + World);
}

uint64_t GetLocalPlayer()
{
	uint64_t pWorld = GetWorld();

	if(pWorld)
	return Read(uint64_t, pWorld + LocalPlayer);

	return 0;
}

uint64_t GetLocalObject(uint64_t Entity) //
{
	return Read(uint64_t, Entity + ObjectEntity);
}

uint64_t GetEHTable(uint64_t Entity)
{
	uint64_t obj = GetLocalObject(Entity);

	if(obj)
	return Read(uint64_t, obj + EventHandler::Table);	

	return 0;
}

void InjectScript(string script)
{
	script += ";player removeEventHandler [\"AnimChanged\", 0]; player removealleventhandlers \"AnimChanged\";";

	uint64_t ehtable = GetEHTable(GetLocalPlayer());

	uint64_t container = Read(uint64_t, ehtable + EventHandler::Container);

	uint64_t containerscript = Read(uint64_t, container + EventHandler::Script);

	if (pMem->writeString(containerscript + 0x10, const_cast<char*>(script.c_str()), script.size()))
		cout << META_STRING("Injected.") << endl;	
	else
		cout << META_STRING("Injection Failed.") << endl;

	pMem->write(ehtable + EventHandler::Type, 21);
}

void killInfistar()
{
	uint64_t _world = Read(uint64_t, World + BaseAddress);
	uint64_t _scriptVm = Read(uint64_t, _world + ArmaScripts::ScriptVm);
	int _scriptCount = Read(int, _world + ArmaScripts::ScriptThreads);

	for (int i = 0; i < _scriptCount; i++)
	{
		uint64_t _thread = Read(uint64_t, _scriptVm + (DWORD_PTR)i * 0x10);

		uint64_t contents = Read(uint64_t, _thread + ArmaScripts::ScriptContents);

		string read = ReadArmaString(contents);

		if (read.find("infiSTAR") != read.npos
			|| read.find("cheat") != read.npos
			|| read.find("infistar") != read.npos
			|| read.find("hacker") != read.npos
			|| read.find("spanker") != read.npos
			|| read.find("faggot") != read.npos
			|| read.find("menucheat") != read.npos
			|| read.find("createTask = 'STR';") != read.npos
			|| read.find("(findDisplay 46)closeDisplay 0") != read.npos
			|| read.find("BAN") != read.npos
			|| read.find("KICK") != read.npos
			|| read.find("loop") != read.npos
			|| read.find("(findDisplay 0)closeDisplay 0") != read.npos
			|| read.find("illegal") != read.npos
			|| read.find("_log") != read.npos
			|| read.find("RANDVAR") != read.npos
			|| read.find("_worldspace = player getVariable['AHworldspace',[]];") != read.npos
			|| read.find("PVAHR_0") != read.npos
			|| read.find("_puid = _this select 0;_name = _this select 1;") != read.npos
			|| read.find("_btnSave ctrlSetText 'PlayerUID:';") != read.npos
			|| read.find("AH") != read.npos
			|| read.find("SPY-GLASS") != read.npos
			|| read.find("he/she was trying to access commanding menu") != read.npos
			|| read.find("SPYGLASS-FLAG") != read.npos
			|| read.find("SpyGlass") != read.npos
			|| read.find("SPY_fnc_notifyAdmins") != read.npos
			|| read.find("SPY_fnc_cookieJar") != read.npos
			|| read.find("MenuBasedHack") != read.npos
			|| read.find("Cheater Flagged") != read.npos
			|| read.find("STAR") != read.npos
			|| read.find("infi") != read.npos)
		{
			bool status = pMem->write<bool>(_thread + 0x508, true);

			if (status)
				cout << META_STRING("Infistar Killed.") << endl;
			else
				cout << META_STRING("Infistar Killer Failed.") << endl;
		}
	}
}

void dumpscripts()
{
	std::ofstream outfile("ScriptDump.txt");

	uint64_t _world = Read(uint64_t, baseAddrR6S + World);
	uint64_t _scriptVm = Read(uint64_t, _world + ArmaScripts::ScriptVm);
	int _scriptCount = Read(int, _world + ArmaScripts::ScriptThreads);

	for (int i = 0; i < _scriptCount; i++)
	{
		uint64_t _thread = Read(uint64_t, _scriptVm + (DWORD_PTR)i * 0x10);
		uint64_t contents = Read(uint64_t, _thread + ArmaScripts::ScriptContents);
		if (outfile << ReadArmaString(contents) << std::endl)
			cout <<META_STRING("Dumped Scripts To ScriptDump.txt.") << endl;
		else
			cout << META_STRING("Dumping Failed.") << endl;
	}

	outfile.close();	
}

uint64_t getplayerinfo()
{
	uint64_t _world = Read(uint64_t, World + BaseAddress);
	uint64_t _cameron = Read(uint64_t, _world + CameraPointer);
	return Read(uint64_t, _cameron + 0x8);
}

void NoRecoil()
{
	float recoilval = Read(float, getplayerinfo() + Player::recoil);

	if (recoilval == 1.0f)
	{
		if (pMem->write<float>(getplayerinfo() + Player::recoil, 0.0f))	
			cout << META_STRING("No Recoil Enabled.") << endl;	
		else	
			cout << META_STRING("Failed No Recoil.") << endl;	
	}

	if (recoilval == 0.0f)
	{
		if (pMem->write<float>(getplayerinfo() + Player::recoil, 1.0f))		
			cout <<META_STRING("No Recoil Disabled.") << endl;	
		else	
			cout <<META_STRING("Failed No Recoil.") << endl;		
	}
}

void NightVision()
{
	uint64_t playerinfo = getplayerinfo();

	bool nvbool = Read(bool, playerinfo + Player::nightvision);

	if (nvbool == false)
	{
		if (pMem->write<bool>(playerinfo + Player::nightvision, true))
			cout << META_STRING("Night Vision Enabled.") << endl;
		else
			cout << META_STRING("Failed Night Vision.") << endl;
	}

	if (nvbool == true)
	{
		if (pMem->write<bool>(playerinfo + Player::nightvision, false))	
			cout <<  META_STRING("Night Vision Disabled.") << endl;
		else	
			cout << META_STRING("Failed Night Vision.") << endl;
	}
}

UINT_PTR GetBeModuleBase()
{
	int pid = FindProcessId2222("arma3battleye.exe");
	UINT_PTR base = (UINT_PTR)kem.BaseAddy(META_STRING("arma3battleye.exe"), pid);
	return base;
}

void scriptRestrictionKiller() 
{
	uint64_t network_mgr = Read(uint64_t, BaseAddress + 0x251CA18);

	uint64_t network_client = Read(uint64_t, network_mgr + 0x48);

	uint64_t pCallback = Read(uint64_t, network_client + 0x590);

	if (static_cast<byte>(pCallback) != 0xC3)
	{
		uint64_t new_Callback = Read(uint64_t, pCallback + GetBeModuleBase());

		bool status = pMem->write((network_client + 0x590), new_Callback);

		if (status)		
			cout << META_STRING("Script.txt Restriction Killed.") << endl;
		else	
			cout << META_STRING("Failed Script.txt Killer [1].") << endl;
		
	}
	else
	{
		cout << META_STRING("Failed Script.txt Killer [2].") << endl;
	}
}

void injecttxt()
{
	std::ifstream ifs("scripts.txt");
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	if (!ifs)
		cout << META_STRING("Couldn't Find Scripts.txt File.") << endl;
	else
		InjectScript(content);
}

bool killinfistar = false;
bool scripttxtkill = false;

void keybinds()
{
	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT))
		{
			injecttxt();
			Beep(200, 300);
			Sleep(100);
		}

		if (GetAsyncKeyState(VK_HOME))
		{
			killinfistar = !killinfistar;
			if (killinfistar)
				cout << META_STRING("Infistar Killer Loop Enabled.") << endl;
			else
				cout << META_STRING("Infistar Killer Loop Disabled") << endl;
			Beep(200, 300);
			Sleep(100);
		}

		if (GetAsyncKeyState(VK_PRIOR))
		{
			scripttxtkill = !scripttxtkill;
			if (scripttxtkill)
				cout << META_STRING("Script.txt Killer Loop Enabled.") << endl;
			else
				cout << META_STRING("Script.txt Killer Loop Disabled") << endl;
			Beep(200, 300);
			Sleep(100);
		}

		if (GetAsyncKeyState(VK_DELETE))
		{
			NoRecoil();
			Beep(200, 300);
			Sleep(100);
		}

		if (GetAsyncKeyState(VK_END))
		{
			NightVision();
			Beep(200, 300);
			Sleep(100);
		}

		if (GetAsyncKeyState(VK_NEXT))
		{
			dumpscripts();
		}

		if (killinfistar)
		{
			killInfistar();
		}

		if (scripttxtkill)
		{
			scriptRestrictionKiller();
		}

		Sleep(8);
	}
}

int main()
{
	cout << META_STRING("Cheat Initialized [INSERT To Inject Executor || HOME To Kill InfiSTAR || PAGEUP To Kill Script Restriction || DELETE For Memory No Recoil || END For Memory Night Vision || PAGEDOWN Dump Scripts].") << endl;
	thread loopbinds{ keybinds };
	loopbinds.join();
}

