#include "stdafx.h"

bool KV::bIsDevkit = false;
bool KV::bCRL = false;
bool KV::bFCRT = false;
KEY_VAULT KV::sKeyVault;

BYTE KV::bStockMacAddress[0x6];
BYTE KV::bMacAddress[0x6];
BYTE KV::bCpuKey[0x10];
BYTE KV::bKvCpuKey[0x10];
BYTE KV::bHmacKVHash[0x14];

BYTE bDevKitRoamableObfuscationKey[0x10] = { 0xDA, 0xB6, 0x9A, 0xD9, 0x8E, 0x28, 0x76, 0x4F, 0x97, 0x7E, 0xE2, 0x48, 0x7E, 0x4F, 0x3F, 0x68 };
BYTE bRetailRoamableObfuscationKey[0x10] = { 0xE1, 0xBC, 0x15, 0x9C, 0x73, 0xB1, 0xEA, 0xE9, 0xAB, 0x31, 0x70, 0xF3, 0xAD, 0x47, 0xEB, 0xF3 };

#define RetailKvAddress 0x00000002000163C0
#define DevKitKvAddress 0x00000002000162E0

DWORD KV::SetupKeyvault() {
	// Sec purposes
	HV::HvPeekBytes(0x20, bCpuKey, 0x10);
	memcpy(HV::FuseLines, Utilities::GetFuseCpukey(), 0x10);

	// cpukey read or generate for digesting
	if (Utilities::FileExists(CPUKEY_PathTxt)) {
		if (Utilities::CReadFile(CPUKEY_PathTxt, reinterpret_cast<BYTE*>(bKvCpuKey), 0x10)) {
			auto CPUKeyString = reinterpret_cast<const char*>(bKvCpuKey);
			FormatUtils::HexStringToByteArray(bKvCpuKey, const_cast<char*>(CPUKeyString), 0x10);
			memcpy(bKvCpuKey, CPUKeyString, 0x10);
			Utilities::CWriteFile(CPUKEY_PathBin, bKvCpuKey, 0x10);
			remove(CPUKEY_PathTxt);
		}
	} else {
		if (Utilities::CReadFile(CPUKEY_PathBin, reinterpret_cast<BYTE*>(bKvCpuKey), 0x10)) {
			for (auto i = 0; i < 0x10; i++) {
				if (static_cast<BYTE>(bKvCpuKey[i]) < 0x00 || static_cast<BYTE>(bKvCpuKey[i]) > 0xFF) {
					Native::Kernel::XeCryptRandom(reinterpret_cast<BYTE*>(bKvCpuKey[i]), 0x01);
					Utilities::CWriteFile(CPUKEY_PathBin, bKvCpuKey, 0x10);
					break;
				}
			}
		} else {
			Native::Kernel::XeCryptRandom(bKvCpuKey, 0x10);
			Utilities::CWriteFile(CPUKEY_PathBin, bKvCpuKey, 0x10);
		}
	}

	QWORD kvAddress = HV::HvPeekQWORD(bIsDevkit ? DevKitKvAddress : RetailKvAddress);
	// read kv from hdd or nand section
	if (!Utilities::CReadFile(KV_Path, reinterpret_cast<BYTE*>(&sKeyVault), 0x4000)) {
		BYTE* NandKv = reinterpret_cast<BYTE*>(malloc(0x4000));
		HV::HvPeekBytes(kvAddress, NandKv, 0x4000);
		CopyMemory(&sKeyVault, NandKv, 0x4000);
		free(NandKv);
	}

	// update kv sections
	HV::HvPeekBytes(kvAddress + 0xD0, &sKeyVault.ConsoleObfuscationKey, 0x40);
	CopyMemory(sKeyVault.RoamableObfuscationKey, bRetailRoamableObfuscationKey, 0x10);
	HV::HvPokeBytes(kvAddress, &sKeyVault, 0x4000);

	CopyMemory(reinterpret_cast<void*>(0x8E03A000), &sKeyVault.ConsoleCertificate, 0x1A8);
	CopyMemory(reinterpret_cast<void*>(0x8E038020), &sKeyVault.ConsoleCertificate.ConsoleId.abData, 0x5);
	Native::Kernel::XeCryptSha(reinterpret_cast<BYTE*>(0x8E038014), 0x3EC, nullptr, NULL, nullptr, NULL, reinterpret_cast<BYTE*>(0x8E038000), 0x14);

	XECRYPT_HMACSHA_STATE HMACState;
	Native::Kernel::XeCryptHmacShaInit(&HMACState, bKvCpuKey, 0x10);
	Native::Kernel::XeCryptHmacShaUpdate(&HMACState, reinterpret_cast<BYTE*>(&sKeyVault.OddFeatures), 0xD4);
	Native::Kernel::XeCryptHmacShaUpdate(&HMACState, reinterpret_cast<BYTE*>(&sKeyVault.DvdKey), 0x1CF8);
	Native::Kernel::XeCryptHmacShaUpdate(&HMACState, reinterpret_cast<BYTE*>(&sKeyVault.CardeaCertificate), 0x2108);
	Native::Kernel::XeCryptHmacShaFinal(&HMACState, bHmacKVHash, 0x14);

	bFCRT = (sKeyVault.OddFeatures & ODD_POLICY_FLAG_CHECK_FIRMWARE) != 0 ? true : false;

	bMacAddress[0] = 0x00;
	bMacAddress[1] = 0x1D;
	bMacAddress[2] = 0xD8;
	bMacAddress[3] = sKeyVault.ConsoleCertificate.ConsoleId.asBits.MacIndex3;
	bMacAddress[4] = sKeyVault.ConsoleCertificate.ConsoleId.asBits.MacIndex4;
	bMacAddress[5] = sKeyVault.ConsoleCertificate.ConsoleId.asBits.MacIndex5;

	Native::Kernel::ExGetXConfigSetting(XCONFIG_SECURED_CATEGORY, XCONFIG_SECURED_MAC_ADDRESS, bStockMacAddress, 0x6, NULL);
	if (memcmp(bStockMacAddress, bMacAddress, 0x6) != NULL) {
		Native::Kernel::ExSetXConfigSetting(XCONFIG_SECURED_CATEGORY, XCONFIG_SECURED_MAC_ADDRESS, bMacAddress, 0x6);
		Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);
	}
	Native::Xam::XamCacheReset(XAM_CACHE_ALL);
	return ERROR_SUCCESS;
}

