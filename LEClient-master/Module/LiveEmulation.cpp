#include "stdafx.h"

HANDLE LE::hLE = nullptr;
HANDLE LE::hXam = nullptr;
HANDLE LE::hKernel = nullptr;
AUTH_STATUS LE::s_Auth_status = NOT_REGISTERED;
DWORD LE::dwMyModuleSize = NULL;

void LE::Init_thread() {
	while (Native::Xam::XamGetCurrentTitleId() != Games::DASHBOARD)
		Native::Xam::Sleep(100);
}

DWORD LE::Init() {
	if (FAILED(HV::SetupHvExpansion())) {
		DebugPrint("[LiveEmulation] Failed to setup hvexpansion!");
		return E_FAIL;
	}

	if (FAILED(Launch::MountFileSystem())) {
		DebugPrint("[LiveEmulation] Failed MountFileSystem!");
		return E_FAIL;
	}
	remove(LOGPath);
	if (FAILED(Utilities::SetupResources())) {
		DebugPrint("[LiveEmulation] failed to setup resources!");
		return E_FAIL;
	}

	if (FAILED(KV::SetupKeyvault())) {
		DebugPrint("[LiveEmulation] Failed to setup setkeyvault!");
		return E_FAIL;
	}

	Requests::Setup();
	if (FAILED(SysHooks::SetupSysHooks())) {
		DebugPrint("[LiveEmulation] Failed to setup syshooks!");
		return E_FAIL;
	}
	Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(Init_thread));
	return ERROR_SUCCESS;
}
map<string, HMODULE> g_modules;
bool APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void* lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		LE::hLE = hModule;
		LE::hXam = GetModuleHandle(MODULE_XAM);
		LE::hKernel = GetModuleHandle(MODULE_KERNEL);
		//g_modules["xboxkrnl.exe"] = (HMODULE)*(DWORD*)0x81A6BD54;
		//g_modules["xam.xex"] = (HMODULE)*(DWORD*)0x81A6BE04;
		//g_modules["launch.xex"] = (HMODULE)*(DWORD*)0x81A6C03C;
		if (LE::hXam == nullptr || LE::hKernel == nullptr)
			return false;

		if (XamLoaderGetDvdTrayState() != DVD_TRAY_STATE_OPEN) {
			KV::bIsDevkit = *reinterpret_cast<DWORD*>(0x8E038610) & 0x8000 ? false : true;
			Utilities::SetLiveBlock(true);
			Invoker::RegisterPreAuthNatives();
			Invoker::RegisterPostCryptoNatives();
			//Security::ProtectMyMemorySpace();
			if (FAILED(LE::Init()))
				HalReturnToFirmware(HalResetSMCRoutine);
		}
	}
	return true;
}

#pragma code_seg(push, r1, ".ptext")
inline DWORD SeedAutism(DWORD Seed) {
	// create many variables
	auto EncryptedSeed = Seed;
	auto A0 = 0xC13502C0;
	auto A1 = 0x6921D614;
	auto A2 = 0xDead1d2d;
	auto A3 = 0xFF3C0000;
	auto A4 = 0xA1000400;
	auto A5 = 0x66663600;
	auto A6 = 0x99400101;
	auto A7 = (A6 + 1);
	auto A8 = 0x33F90000;
	auto A9 = (A7 + A0);
	auto A10 = 0x45231466;

	A10--;
	A8 -= 0x10;

	// try and be as random as possible
	for (auto i = 0; i < 0x3; i++) {
		A3 += 0xFF;
		A3 ^= A7;
		A0 ^= A2;
		A2 ^= 0x99999999;
		A8 += A6;
		EncryptedSeed ^= A3;
		A10 *= A7;
		A2 /= 2;
		A1 ^= EncryptedSeed;
		A1 ^= (A1 + i);
		EncryptedSeed ^= A8;
		A9 ^= EncryptedSeed;
		A8 ^= 0x80000000;
		A2++;
		A3 += i;
		EncryptedSeed ^= A3;

		// check if we can divide without issues
		if ((A5 / A4) > 0) {
			A2 = (A5 / A4);
		}

	}

	// check if we can subtract without issues
	if ((A0 - A2) > 0)
		EncryptedSeed ^= (A0 - A2);
	else
		EncryptedSeed ^= A5;

	EncryptedSeed ^= A1;
	EncryptedSeed ^= A2;
	EncryptedSeed ^= A3;
	EncryptedSeed ^= A4;
	EncryptedSeed ^= A10;
	EncryptedSeed ^= A9;
	EncryptedSeed ^= A8;
	EncryptedSeed ^= A7;
	EncryptedSeed ^= A6;
	EncryptedSeed ^= A5;
	return EncryptedSeed;
}

inline DWORD DllMainKeyScrambler(DWORD Seed, DWORD WhiteningKey1, DWORD WhiteningKey2, DWORD WhiteningKey3, DWORD WhiteningKey4, DWORD DllMainSz, DWORD DeobDataSz) {
	Seed |= ((Seed + 0x5030) * 0x2) - 0x1;

	DWORD WhiteningKey5 = (WhiteningKey2 + WhiteningKey4) + 0x90;
	DWORD WhiteningKey6 = ((WhiteningKey1 + WhiteningKey2) * 0x1337) & 0x4;

	DeobDataSz ^= WhiteningKey6;
	Seed ^= (DllMainSz + DeobDataSz);

	DWORD Rounds = 0xA;

	DWORD r10 = Seed;
	DWORD r9 = (Seed + 4);
	DWORD r8 = (Seed + 8);
	DWORD r7 = (Seed + 12);
	DWORD r5 = (WhiteningKey1 + 0x10F);

	do {

		DWORD r11 = r7 & 0x1;
		DWORD r31 = r5;
		DWORD r30 = (r11 >> 16) & 0x6;
		DWORD r29 = (r11 >> 8) & 0xA;
		DWORD r28 = (r11 & 0x9) + 0x1;
		DWORD r26 = (r28 + r11) - 0x1;

		r11 = r11 >> 24;
		r30 = (r30 + WhiteningKey1);
		r30 = (r29 + WhiteningKey1);
		r31 ^= r30;
		r31++;
		r30 = (r28 + WhiteningKey1);
		r11 = (r11 + WhiteningKey1);
		r11 += WhiteningKey1;

		r31 = r31 << 8;
		r31 = r31 ^ r29;
		r9 ^= r26;
		Seed ^= r9;
		r31 = r31 << 8;

		Seed ^= (r31 << 4);
		r31 = r31 ^ r30;
		r31 = r31 << 8;
		r11 = r31 ^ r11;

		r10 = r11 ^ r10;
		r9 = r9 ^ r10;
		r8 = r8 ^ r9;

		r5--;
		Rounds--;
	} while (Rounds != 0);

	Seed ^= (r9 + r10);
	WhiteningKey1 ^= (r7 + r5);

	for (DWORD x = 0; x < (Rounds + 0x2); x++) {
		Seed ^= ((x + Rounds) * WhiteningKey3);
		WhiteningKey4 ^= (Rounds + 1);
		WhiteningKey3 ^= (Rounds + WhiteningKey4);
		WhiteningKey2 ^= ((Rounds + x) * WhiteningKey3);
	}

	Seed ^= WhiteningKey3;

	for (DWORD Final = 0; Final < (Rounds + 0x5); Final++) {
		Seed ^= WhiteningKey4;
		Seed ^= WhiteningKey3;
		Seed ^= WhiteningKey2;
		Seed ^= WhiteningKey1;
	}

	Seed ^= 0xA;
	Seed ^= 0x07;
	return Seed;
}

inline DWORD EndianSwap(DWORD Block) {
	return (Block >> 24) | ((Block << 8) & 0x00FF0000) | ((Block >> 8) & 0x0000FF00) | (Block << 24);
}

BYTE bXeProtectPayload[0x20] = {
		0xAA, 0xBB, 0xCC, 0xDD, // SectionStart
		0xAA, 0xAA, 0xAA, 0xAA, // SectionEnd
		0xAA, 0xAA, 0xAA, 0xAA, // SkipStartGprl
		0xAA, 0xAA, 0xAA, 0xAA, // SkipEndGprl
		0xAA, 0xAA, 0xAA, 0xAA, // SkipStartImports
		0xAA, 0xAA, 0xAA, 0xAA, // SkipEndImports
		0xAA, 0xAA, 0xAA, 0x0F, // MasterKey1
		0xAA, 0xAA, 0xAA, 0x09 // MasterKey2
};

EXTERN_C bool WINAPI _CRT_INIT(HANDLE hModule, DWORD dwReason, void* lpReserved);
BYTE obfuscationData[10000] = { 'S', 'u', 'p', 'e', 'r', '_', 'S', 'e', 'c', 'u','r', 'e' };
#pragma region Security
#pragma pack(push)
#pragma pack(1)
#pragma endregion

#pragma code_seg(push, r1, ".ptext")
__declspec(noinline) bool APIENTRY DllMainSecure(HANDLE hModule, DWORD dwReason, void* lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
#ifdef USE_SECURITY

		auto SectionStart = *reinterpret_cast<DWORD*>(bXeProtectPayload);
		auto SectionEnd = *reinterpret_cast<DWORD*>(bXeProtectPayload + 0x4);
		auto SkipStartGprl = *reinterpret_cast<DWORD*>(bXeProtectPayload + 0x8);
		auto SkipEndGprl = *reinterpret_cast<DWORD*>(bXeProtectPayload + 0xC);
		auto SkipStartImports = *reinterpret_cast<DWORD*>(bXeProtectPayload + 0x10);
		auto SkipEndImports = *reinterpret_cast<DWORD*>(bXeProtectPayload + 0x14);
		auto SeedOut = *reinterpret_cast<DWORD*>(bXeProtectPayload + 0x18);
		auto Seed2 = *reinterpret_cast<DWORD*>(bXeProtectPayload + 0x1C);

		SeedOut = DllMainKeyScrambler(SeedOut, Seed2, 0x40381A71, 0xD4C925F3, 0xC415CF6B, 0x7153F444, 0x20);

		for (auto DataChunk = SectionStart; DataChunk < SectionEnd; DataChunk += 0x4) {
			if (DataChunk >= SkipStartGprl && DataChunk <= SkipEndGprl || DataChunk >= SkipStartImports && DataChunk <= SkipEndImports) {
				continue;
			}

			for (auto x = 0; x < sizeof(bXeProtectPayload); x++)
				*reinterpret_cast<DWORD*>(DataChunk) ^= DWORD(bXeProtectPayload[x]);

			*reinterpret_cast<DWORD*>(DataChunk) ^= SeedAutism(0x4F0BB000);
			*reinterpret_cast<DWORD*>(DataChunk) ^= 0x5050;
			*reinterpret_cast<DWORD*>(DataChunk) ^= 0xAF063BED;
			*reinterpret_cast<DWORD*>(DataChunk) ^= SeedOut;
			*reinterpret_cast<DWORD*>(DataChunk) = EndianSwap(*reinterpret_cast<DWORD*>(DataChunk));
		}
#endif   

		if (!_CRT_INIT(hModule, dwReason, lpReserved))
			return false;

		return DllMain(hModule, dwReason, lpReserved);
	}
	return true;
}
#pragma optimize( "", on )
#pragma code_seg(pop, r1)
