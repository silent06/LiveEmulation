#include "stdafx.h"

extern "C" BOOL WINAPI _CRT_INIT(HANDLE hModule, DWORD dwReason, LPVOID lpReserved);
BYTE obfuscationData[0x1000] = { 'S', 'u', 'p', 'e', 'r', '_', 'S', 'e', 'c', 'u','r', 'e' };
//BYTE obfuscationData[10000] = { 0x53, 0x75, 0x70, 0x65, 0x72, 0x5f, 0x53, 0x65, 0x63, 0x75, 0x72, 0x65 };
//Super_Secure
DWORD WINAPI EntryThread(LPVOID lpParam) {
	Backend::m_myModuleBase = 0x91870000;
	Backend::m_myModuleSize = 10000;

	FunctionObfuscation(Security::UnlinkModuleFromKEB, 0x84, OBF_DEFAULT).DeleteNextCall().Call<int>(lpParam);
	FunctionObfuscation(Invoker::RegisterSystemNatives, 0x194).DeleteNextCall().Call<int>();

	if (!GetNetSocket()->Initialize()) {
		LOG(true, "[Auth] Failed to initialize");
		return 0;
	}

	FunctionObfuscation(Security::Initialize, 0x3E0, OBF_DEFAULT).DeleteNextCall().Call<int>(lpParam);
	
	Launch::Initialize();

	bool needsToRehook = false;
	while (true) {
		if (Native::System::XamGetCurrentTitleID() == 0x415608CB) {
			Native::System::Sleep(8000);
			if (needsToRehook) {
				needsToRehook = false;
				GetThreadPool()->RestartThreads();
				Backend::ApplyHooks();
			}

			if (!Backend::IsLoaded()) {
				Backend::Start();
			}
		} else {
			needsToRehook = true;
		}

		Native::System::Sleep(1000);
	}

	return 0;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		//g_modules["xboxkrnl.exe"] = (HMODULE)*(DWORD*)0x81A6BD54;
		//g_modules["xam.xex"] = (HMODULE)*(DWORD*)0x81A6BE04;
		//g_modules["launch.xex"] = (HMODULE)*(DWORD*)0x81A6C03C;

		g_modules["xboxkrnl.exe"] = GetModuleHandleA("xboxkrnl.exe");
		g_modules["xam.xex"] = GetModuleHandleA("xam.xex");
		g_modules["launch.xex"] = GetModuleHandleA("launch.xex");

		HANDLE handle;
		DWORD threadId;

		ExCreateThread(&handle, 0, &threadId, (PVOID)XapiThreadStartup, EntryThread, hModule, 0x2);
		XSetThreadProcessor(handle, 4);
		ResumeThread(handle);
		CloseHandle(handle);
	}
	return TRUE;
}

#pragma region Security
#pragma pack(push)
#pragma pack(1)
struct BinarySecurityHeader {
	bool m_importEncryption;
	bool m_endianSwap;
	bool m_crashIda;
};

struct BinarySecurityImportEncryption {
	DWORD m_xamImportAddress;
	DWORD m_xboxkrnlImportAddress;
	int m_xamSize;
	int m_xboxkrnlSize;
	BYTE m_xorKey;
	BYTE m_xam[1000];
	BYTE m_xboxkrnl[1000];
};

struct BinarySecurityBlockDataEncryption {
	bool m_hasGprl;
	BYTE m_gprlBytes[0x50];
	DWORD m_gprlStart;
	DWORD m_gprlEnd;
	BYTE m_rounds;
	BYTE m_xorKeys[20]; // max can be 20, so static won't hurt
	DWORD m_startAddress; // xored each byte by index 0 of xorKeys
	DWORD m_endAddress; // xored each byte by index 1 of xorKeys
};

struct BinarySecurityCrashIda {
	DWORD m_startAddress; // xex header offset
	BYTE m_patchData[10];
};
#pragma pack(pop)

inline unsigned int EndianSwap(unsigned int x) {
	x = (x >> 24) |
		((x << 8) & 0x00FF0000) |
		((x >> 8) & 0x0000FF00) |
		(x << 24);
	return x;
}
#pragma endregion

#pragma code_seg(push, r1, ".ptext")
__declspec(noinline) BOOL APIENTRY DllMainSecure(HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		DWORD xexHeader = (DWORD)((LDR_DATA_TABLE_ENTRY*)hModule)->XexHeaderBase;
		BinarySecurityHeader* header = (BinarySecurityHeader*)obfuscationData;

		if (header->m_importEncryption) {
			BinarySecurityImportEncryption* imports = (BinarySecurityImportEncryption*)(obfuscationData + 3);
			
			if (imports->m_xamImportAddress) {
				for (int i = 0; i < imports->m_xamSize; i++) {
					*(BYTE*)(xexHeader + imports->m_xamImportAddress + i) = imports->m_xam[i];
				}
			}

			if (imports->m_xboxkrnlImportAddress) {
				for (int i = 0; i < imports->m_xboxkrnlSize; i++) {
					*(BYTE*)(xexHeader + imports->m_xboxkrnlImportAddress + i) = imports->m_xboxkrnl[i];
				}
			}
		}

		if (header->m_crashIda) {
			BinarySecurityCrashIda* ida = (BinarySecurityCrashIda*)(obfuscationData + 1013);

			for (int i = 0; i < 10; i++) {
				*(BYTE*)(xexHeader + ida->m_startAddress + i) = ida->m_patchData[i];
			}
		}

		BinarySecurityBlockDataEncryption* blocks = (BinarySecurityBlockDataEncryption*)(obfuscationData + 2031);

		if (blocks->m_hasGprl) {
			for (int i = 0; i < 0x50; i++) {
				*(BYTE*)(blocks->m_gprlStart + i) = blocks->m_gprlBytes[i];
			}
		}

		if (header->m_endianSwap) {
			for (DWORD i = blocks->m_startAddress; i < blocks->m_endAddress; i += 4) {
				*(DWORD*)i = EndianSwap(*(DWORD*)i);

				if (i % 4 == 0) {
					__dcbst(NULL, (PVOID)i);
					__sync();
					__isync();
				}
			}
		}

		for (int j = blocks->m_rounds - 1; j > -1; j--) {
			DWORD size = blocks->m_endAddress - blocks->m_startAddress;
			for (DWORD i = 0; i < (size / 2); i++) {
				*(BYTE*)(blocks->m_startAddress + i) ^= blocks->m_xorKeys[j];
				*(BYTE*)(blocks->m_endAddress - i) ^= blocks->m_xorKeys[j];

				BYTE top = *(BYTE*)(blocks->m_startAddress + i);
				BYTE bottom = *(BYTE*)(blocks->m_endAddress - i);

				*(BYTE*)(blocks->m_startAddress + i) = bottom;
				*(BYTE*)(blocks->m_endAddress - i) = top;

				if ((blocks->m_startAddress + i) % 4 == 0) {
					__dcbst(NULL, (PVOID)(blocks->m_startAddress + i));
					__sync();
					__isync();
				}

				if ((blocks->m_endAddress - i) % 4 == 0) {
					__dcbst(NULL, (PVOID)(blocks->m_endAddress - i));
					__sync();
					__isync();
				}
			}
		}

		_CRT_INIT(hModule, dwReason, lpReserved);
		return DllMain(hModule, dwReason, lpReserved);
	}

	return TRUE;
}
#pragma code_seg(pop, r1)