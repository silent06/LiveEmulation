#include "stdafx.h"

BOOL KV::IsDevkit = FALSE;
BOOL KV::CRL = FALSE;
BOOL KV::FCRT = FALSE;
KEY_VAULT KV::KeyVault;

BYTE KV::StockMacAddress[0x6];
BYTE KV::sMacAddress[0x6];
BYTE KV::CpuKey[0x10];
BYTE KV::KvCpuKey[0x10];
BYTE KV::HmacKVHash[0x14];

BYTE DevKitRoamableObfuscationKey[0x10] = { 0xDA, 0xB6, 0x9A, 0xD9, 0x8E, 0x28, 0x76, 0x4F, 0x97, 0x7E, 0xE2, 0x48, 0x7E, 0x4F, 0x3F, 0x68 };
BYTE RetailRoamableObfuscationKey[0x10] = { 0xE1, 0xBC, 0x15, 0x9C, 0x73, 0xB1, 0xEA, 0xE9, 0xAB, 0x31, 0x70, 0xF3, 0xAD, 0x47, 0xEB, 0xF3 };

#define RetailKvAddress 0x00000002000163C0
#define DevKitKvAddress 0x00000002000162E0

DWORD KV::SetupKeyvault() {
	// Sec purposes
	HV::HvPeekBytes(0x20, CpuKey, 0x10);
	CopyMemory(HV::FuseLines, Utilities::GetFuseCpukey(), 0x10);

	QWORD kvAddress = NULL;
	kvAddress = HV::HvPeekQWORD(RetailKvAddress);

	PBYTE KvCpukeyTxt = nullptr;
	DWORD KVCpukeyTextLen = NULL;
	// cpukey read or generate for digesting
	if (Utilities::FileExists(CPUKEY_PathTxt)) {
		if (SUCCEEDED(Utilities::CReadFile(CPUKEY_PathTxt, &KvCpukeyTxt, &KVCpukeyTextLen))) {
			auto CPUKeyString = reinterpret_cast<LPCSTR>(KvCpukeyTxt);
			FormatUtils::HexStringToByteArray(KvCpukeyTxt, const_cast<PCHAR>(CPUKeyString), 0x10);
			CopyMemory(KvCpuKey, CPUKeyString, 0x10);
			Utilities::CWriteFile(CPUKEY_PathBin, KvCpuKey, 0x10);
			remove(CPUKEY_PathTxt);
		}
	} else {
		PBYTE pbCpukey = nullptr;
		DWORD KvCpuKeyLen = NULL;
		if (Utilities::CReadFile(CPUKEY_PathBin, &pbCpukey, &KvCpuKeyLen)) {
			memcpy(KvCpuKey, pbCpukey, 0x10);
			for (auto i = 0; i < 0x10; i++) {
				if (static_cast<BYTE>(KvCpuKey[i]) < 0x00 || static_cast<BYTE>(KvCpuKey[i]) > 0xFF) {
					Native::Kernel::XeCryptRandom(reinterpret_cast<PBYTE>(KvCpuKey[i]), 0x01);
					Utilities::CWriteFile(CPUKEY_PathBin, KvCpuKey, 0x10);
					break;
				}
			}
		} else {
			Native::Kernel::XeCryptRandom(KvCpuKey, 0x10);
			Utilities::CWriteFile(CPUKEY_PathBin, KvCpuKey, 0x10);
		}
	}

	// read kv from hdd or nand section
	PBYTE KvData = nullptr;
	DWORD KvDataLen = NULL;
	if (Utilities::CReadFile(KV_Path, &KvData, &KvDataLen))
		CopyMemory(&KeyVault, KvData, 0x4000);
	else {
		PBYTE NandKv = static_cast<PBYTE>(malloc(0x4000));
		HV::HvPeekBytes(kvAddress, NandKv, 0x4000);
		CopyMemory(&KeyVault, NandKv, 0x4000);
		free(NandKv);
	}

	// update kv sections
	HV::HvPeekBytes(kvAddress + 0xD0, &KeyVault.ConsoleObfuscationKey, 0x40);
	memcpy(KeyVault.RoamableObfuscationKey, RetailRoamableObfuscationKey, 0x10);
	HV::HvPokeBytes(kvAddress, &KeyVault, 0x4000);

	CopyMemory(reinterpret_cast<PVOID>(0x8E03A000), &KeyVault.ConsoleCertificate, 0x1A8);
	CopyMemory(reinterpret_cast<PVOID>(0x8E038020), &KeyVault.ConsoleCertificate.ConsoleId.abData, 0x5);
	Native::Kernel::XeCryptSha(reinterpret_cast<PBYTE>(0x8E038014), 0x3EC, nullptr, 0, nullptr, 0, reinterpret_cast<PBYTE>(0x8E038000), 0x14);

	XECRYPT_HMACSHA_STATE hmacSha;
	Native::Kernel::XeCryptHmacShaInit(&hmacSha, KvCpuKey, 0x10);
	Native::Kernel::XeCryptHmacShaUpdate(&hmacSha, reinterpret_cast<PBYTE>(&KeyVault.OddFeatures), 0xD4);
	Native::Kernel::XeCryptHmacShaUpdate(&hmacSha, reinterpret_cast<PBYTE>(&KeyVault.DvdKey), 0x1CF8);
	Native::Kernel::XeCryptHmacShaUpdate(&hmacSha, reinterpret_cast<PBYTE>(&KeyVault.CardeaCertificate), 0x2108);
	Native::Kernel::XeCryptHmacShaFinal(&hmacSha, HmacKVHash, 0x14);

	FCRT = (KeyVault.OddFeatures & ODD_POLICY_FLAG_CHECK_FIRMWARE) != 0 ? TRUE : FALSE;

	sMacAddress[0] = 0x00;
	sMacAddress[1] = 0x1D;
	sMacAddress[2] = 0xD8;
	sMacAddress[3] = KeyVault.ConsoleCertificate.ConsoleId.asBits.MacIndex3;
	sMacAddress[4] = KeyVault.ConsoleCertificate.ConsoleId.asBits.MacIndex4;
	sMacAddress[5] = KeyVault.ConsoleCertificate.ConsoleId.asBits.MacIndex5;

	Native::Kernel::ExGetXConfigSetting(XCONFIG_SECURED_CATEGORY, XCONFIG_SECURED_MAC_ADDRESS, StockMacAddress, 0x6, NULL);
	if (memcmp(StockMacAddress, sMacAddress, 0x6) != NULL) {
		Native::Kernel::ExSetXConfigSetting(XCONFIG_SECURED_CATEGORY, XCONFIG_SECURED_MAC_ADDRESS, sMacAddress, 0x6);
		Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);
	}
	Native::Xam::XamCacheReset(XAM_CACHE_ALL);
	return ERROR_SUCCESS;
}

