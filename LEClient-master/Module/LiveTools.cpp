#include "StdAfx.h"

DWORD GetStorageDeviceSize(PCHAR Path) {
	PINT OutSize = nullptr;
	HANDLE File = nullptr;
	NTSTATUS Status = STATUS_SUCCESS;

	ANSI_STRING DeviceName = { 0 };
	OBJECT_ATTRIBUTES objAttr = { nullptr };
	IO_STATUS_BLOCK statusBlock = { 0 };
	FILE_FS_SIZE_INFORMATION sizeInfo = { 0 };

	RtlInitAnsiString(&DeviceName, Path);
	InitializeObjectAttributes(&objAttr, &DeviceName, OBJ_CASE_INSENSITIVE, NULL);

	if (NT_SUCCESS(NtOpenFile(&File, (SYNCHRONIZE | 1), &objAttr, &statusBlock, FILE_SHARE_READ, FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT | 0x800000))) {
		if (NT_SUCCESS(Status = NtQueryVolumeInformationFile(File, &statusBlock, &sizeInfo, 0x18, FileFsSizeInformation)))
			*OutSize = sizeInfo.TotalAllocationUnits.LowPart;
	}
	NtClose(File);
	return Status;
}

DWORD LiveTools::XKE(PBYTE Buffer, DWORD BufferSize, PBYTE Salt, PVOID KrnlBuild, PVOID arg3, PVOID arg4) {
	while (!Requests::m_Initalized) 
		Native::Xam::Sleep(100);

	auto Request = static_cast<To::XKE*>(malloc(sizeof(To::XKE)));
	auto Response = static_cast<From::XKE*>(malloc(sizeof(From::XKE)));

	CopyMemory(Request->SessionToken, Requests::m_rgSessionKey, 0x10);
	CopyMemory(Request->KVcpukey, KV::KvCpuKey, 0x10);
	CopyMemory(Request->HvSalt, Salt, 0x10);
	CopyMemory(Request->SMac, KV::sMacAddress, 0x6);
	memset(Request->CRL, (KV::CRL) ? 0x1 : 0x0, 0x1);
	CopyMemory(Request->ConsoleCertificate, &KV::KeyVault.ConsoleCertificate.ConsolePartNumber, 0xB);
	CopyMemory(Request->KeyvaultSignature, &KV::KeyVault.KeyVaultSignature, 0x100);
	CopyMemory(Request->Oddfeatures, &KV::KeyVault.OddFeatures, 0x2);
	memset(Request->FCRT, (KV::FCRT) ? 0x1 : 0x0, 0x1);

	if (Requests::m_Time.Status >= AUTHED && SUCCEEDED(Networking::Send(PACKET_COMMAND_XKE, Request, sizeof(To::XKE), Response, sizeof(From::XKE), FALSE, TRUE))) {
		if (Response->DwStatus != SUCCESS) 
			return E_FAIL;

		if (*reinterpret_cast<PDWORD>(Response->XKEBuffer + 0x28) != 0x4E4E4497)
			Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);

		CopyMemory(Buffer, Response->XKEBuffer, 0x100);
		if (!KV::CRL) {
			Utilities::XNotifyUI(L"LiveEmulation - Fully Stealthed !");
			KV::CRL = TRUE;
		}
#ifdef DUMPS
		Utilities::CWriteFile(XKEDUMP_Path, Buffer, 0x100);
#endif
		free(Response);
		return ERROR_SUCCESS;
	}
#ifdef DEBUG
	DebugPrint("Server Get XKE Failed!");
#endif
	free(Request);
	free(Response);
	return E_FAIL;
}

DWORD LiveTools::XSC_XnpLogonSetChallengeResponseHook(DWORD Task, PBYTE Buffer, DWORD BufferSize) {
	while (!Networking::m_Initalized) 
		Native::Xam::Sleep(100);

	auto Request = static_cast<To::XSC*>(malloc(sizeof(To::XSC)));
	auto Response = static_cast<From::XSC*>(malloc(sizeof(From::XSC)));

	auto XoscAddr = reinterpret_cast<DWORD>(Utilities::ResolveFunction(MODULE_XOSC, 0x1));
	auto HashAddr = 0x90010000 | *reinterpret_cast<PWORD>(XoscAddr - 0x62);
	auto HashLength = *reinterpret_cast<PWORD>(XoscAddr - 0x66);

	CopyMemory(Request->SessionToken, Requests::m_rgSessionKey, 0x10);
	CopyMemory(Request->KVcpukey, KV::KvCpuKey, 0x10);
	CopyMemory(Request->SMac, KV::sMacAddress, 0x6);
	CopyMemory(Request->KeyvaultSignature, &KV::KeyVault.KeyVaultSignature, 0x100);
	CopyMemory(Request->DrivePhaseLevel, &KV::KeyVault.XeikaCertificate.Data.OddData.PhaseLevel, 0x4);
	CopyMemory(Request->DriveOsig, &KV::KeyVault.XeikaCertificate.Data.OddData.InquiryData, 0x24);
	CopyMemory(Request->KvSerial, &KV::KeyVault.ConsoleSerialNumber, 0xC);
	CopyMemory(Request->XamRegion, &KV::KeyVault.GameRegion, 0x2);
	CopyMemory(Request->XamOdd, &KV::KeyVault.OddFeatures, 0x2);
	CopyMemory(Request->Policyflashsize, &KV::KeyVault.PolicyFlashSize, 0x4);
	CopyMemory(Request->Oddfeatures, &KV::KeyVault.OddFeatures, 0x2);
	memset(Request->FCRT, (KV::FCRT) ? 0x1 : 0x0, 0x1);
	CopyMemory(Request->ConsoleID, &KV::KeyVault.ConsoleCertificate.ConsoleId, 0x5);
	CopyMemory(Request->ConsoleCertificate, &KV::KeyVault.ConsoleCertificate.ConsolePartNumber, 0xB);
	CopyMemory(Request->KvHMACDigest, KV::HmacKVHash, 0x10);

	if (SUCCEEDED(Requests::m_Time.Status >= AUTHED && Networking::Send(PACKET_COMMAND_XSC, Request, sizeof(To::XSC), Response, sizeof(From::XSC), FALSE, TRUE))) {
		if (Response->DwStatus != SUCCESS) {

#ifdef DEBUG
			DebugPrint("Server Recv XSC Failed!");
#endif
			return E_FAIL;
		}
		// XSCIntegrity of the module itself must be left in client this is because of the different
		// locations associated with xosc different location = different name ex sofia
		// reads location from math above and size then hashes it with previous hash
		Native::Kernel::XeCryptSha(reinterpret_cast<PBYTE>(HashAddr), HashLength, (Response->XscBuff + 0x60), 0x14, nullptr, 0, (Response->XscBuff + 0x60), 0x14);
		(Response->XscBuff + 0x60)[0] = 0x07;
		CopyMemory(Buffer, Response->XscBuff, 0x2E0);

#ifdef DUMPS
		Utilities::CWriteFile(XSCDUMP_Path, Buffer, 0x2E0);
#endif 
		free(Request);
		free(Response);
		return ERROR_SUCCESS;
	}
	free(Request);
	free(Response);
#ifdef DEBUG
	DebugPrint("Server Get XSC Failed!");
#endif
	return E_FAIL;
}