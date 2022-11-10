#include "StdAfx.h"

DWORD GetStorageDeviceSize(CHAR* Path) {
	int* OutSize = nullptr;
	HANDLE File = nullptr;
	DWORD Status = STATUS_SUCCESS;

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

BYTE bXoscsupportedhash[0x14] = {0};
DWORD SysHooks::XSC(DWORD Task, BYTE* Buffer, DWORD BufferSize) {
	while (!Networking::m_Initalized)
		Native::Xam::Sleep(100);

	auto Request = reinterpret_cast<To::XSC*>(malloc(sizeof(To::XSC)));
	auto Response = reinterpret_cast<From::XSC*>(malloc(sizeof(From::XSC)));

	auto XoscAddr = reinterpret_cast<DWORD>(Utilities::ResolveFunction("xosc9v2.xex", 0x1));
	auto HashAddr = 0x90010000 | *reinterpret_cast<PWORD>(XoscAddr - 0x62);
	auto HashLength = *reinterpret_cast<PWORD>(XoscAddr - 0x66);
    
	BYTE bCurXoscHash[0x14];
	Native::Kernel::XeCryptSha(reinterpret_cast<BYTE*>(HashAddr), HashLength, NULL, NULL, NULL, NULL, bXoscsupportedhash, 0x14);
	DebugPrint("XoscSupportedHash: 0x%X0x%X0x%X0x%X0x%X", *(DWORD*)bXoscsupportedhash, *(DWORD*)bXoscsupportedhash + 4, *(DWORD*)bXoscsupportedhash + 8, *(DWORD*)bXoscsupportedhash + 12, *(DWORD*)bXoscsupportedhash + 16, *(DWORD*)bXoscsupportedhash + 20);

	/*if (memcmp(bCurXoscHash, bXoscsupportedhash, 0x14) != NULL) {
	    Native::Xam::Sleep(4000);
		Utilities::XNotifyUI(L"LiveEmulation - A change to xosc has been detected. \nPlease report to LE Support!");
		return E_FAIL;
	}*/

	CopyMemory(Request->SessionToken, Requests::m_rgSessionKey, 0x10);
	CopyMemory(Request->KVcpukey, KV::bKvCpuKey, 0x10);
	Request->Title = Native::Xam::XamGetCurrentTitleId();
	CopyMemory(Request->SMac, KV::bMacAddress, 0x6);
	CopyMemory(Request->KeyvaultSignature, &KV::sKeyVault.KeyVaultSignature, 0x100);
	CopyMemory(Request->DrivePhaseLevel, &KV::sKeyVault.XeikaCertificate.Data.OddData.PhaseLevel, 0x4);
	CopyMemory(Request->DriveOsig, &KV::sKeyVault.XeikaCertificate.Data.OddData.InquiryData, 0x24);
	CopyMemory(Request->KvSerial, &KV::sKeyVault.ConsoleSerialNumber, 0xC);
	CopyMemory(Request->XamRegion, &KV::sKeyVault.GameRegion, 0x2);
	CopyMemory(Request->XamOdd, &KV::sKeyVault.OddFeatures, 0x2);
	CopyMemory(Request->Policyflashsize, &KV::sKeyVault.PolicyFlashSize, 0x4);
	CopyMemory(Request->Oddfeatures, &KV::sKeyVault.OddFeatures, 0x2);
	memset(Request->FCRT, (KV::bFCRT) ? 0x1 : 0x0, 0x1);
	CopyMemory(Request->ConsoleID, &KV::sKeyVault.ConsoleCertificate.ConsoleId, 0x5);
	CopyMemory(Request->ConsoleCertificate, &KV::sKeyVault.ConsoleCertificate.ConsolePartNumber, 0xB);
	CopyMemory(Request->KvHMACDigest, KV::bHmacKVHash, 0x10);

	if (SUCCEEDED(Requests::m_Time.Status >= AUTHED && Networking::Send(PACKET_COMMAND_XSC, Request, sizeof(To::XSC), Response, sizeof(From::XSC), FALSE, TRUE))) {
		if (Response->DwStatus != SUCCESS) {
			DebugPrint("Server Recv XSC Failed!");
			return E_FAIL;
		}
		// XSCIntegrity of the module itself must be left in client this is because of the different
		// locations associated with xosc different location = different name ex sofia
		// reads location from math above and size then hashes it with previous hash
		Native::Kernel::XeCryptSha(reinterpret_cast<BYTE*>(HashAddr), HashLength, (Response->XscBuff + 0x60), 0x14, nullptr, 0, (Response->XscBuff + 0x60), 0x10);
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
	DebugPrint("Server Get XSC Failed!");
	return E_FAIL;
}