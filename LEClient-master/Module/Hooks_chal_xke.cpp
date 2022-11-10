#include "StdAfx.h"

DWORD SysHooks::XKE(BYTE* Buffer, DWORD BufferSize, BYTE* Salt, void* KrnlBuild, void* arg3, void* arg4) {
	while (!Requests::m_Initalized)
		Native::Xam::Sleep(100);

	auto Request = reinterpret_cast<To::XKE*>(malloc(sizeof(To::XKE)));
	auto Response = reinterpret_cast<From::XKE*>(malloc(sizeof(From::XKE)));

	CopyMemory(Request->SessionToken, Requests::m_rgSessionKey, 0x10);
	CopyMemory(Request->KVcpukey, KV::bKvCpuKey, 0x10);
	CopyMemory(Request->HvSalt, Salt, 0x10);
	CopyMemory(Request->SMac, KV::bMacAddress, 0x6);
	memset(Request->CRL, (KV::bCRL) ? 0x1 : 0x0, 0x1);
	CopyMemory(Request->ConsoleCertificate, &KV::sKeyVault.ConsoleCertificate.ConsolePartNumber, 0xB);
	CopyMemory(Request->KeyvaultSignature, &KV::sKeyVault.KeyVaultSignature, 0x100);
	CopyMemory(Request->Oddfeatures, &KV::sKeyVault.OddFeatures, 0x2);
	memset(Request->FCRT, (KV::bFCRT) ? 0x1 : 0x0, 0x1);

	if (Requests::m_Time.Status >= AUTHED && SUCCEEDED(Networking::Send(PACKET_COMMAND_XKE, Request, sizeof(To::XKE), Response, sizeof(From::XKE), FALSE, TRUE))) {
		if (Response->DwStatus != SUCCESS)
			return E_FAIL;

		if (*reinterpret_cast<DWORD*>(Response->XKEBuffer + 0x28) != 0x4E4E4497)
			Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);

		CopyMemory(Buffer, Response->XKEBuffer, 0x100);
		if (!KV::bCRL) {
			Utilities::XNotifyUI(L"LiveEmulation - Fully Stealthed!");
			KV::bCRL = true;
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