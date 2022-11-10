#include "stdafx.h"

XEX_EXECUTION_ID SysHooks::SpoofedExecutionId;
void APCWorker(void* Arg1, void* Arg2, void* Arg3) {
	if (Arg2) 
		reinterpret_cast<LPOVERLAPPED_COMPLETION_ROUTINE>(Arg2)(reinterpret_cast<DWORD>(Arg3), NULL, reinterpret_cast<LPOVERLAPPED>(Arg1));
}

void XSecurityVerifyHook(DWORD dwMilliseconds, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {
	if (lpCompletionRoutine) 
		NtQueueApcThread(reinterpret_cast<HANDLE>(-2), reinterpret_cast<PIO_APC_ROUTINE>(APCWorker), lpOverlapped, (PIO_STATUS_BLOCK)lpCompletionRoutine, 0);
}

DWORD XSecurityGetFailureInfoHook(PXSECURITY_FAILURE_INFORMATION pFailureInformation) {
	if (pFailureInformation->dwSize != 0x18) 
		return ERROR_NOT_ENOUGH_MEMORY;
	pFailureInformation->dwBlocksChecked = NULL;
	pFailureInformation->dwFailedReads = NULL;
	pFailureInformation->dwFailedHashes = NULL;
	pFailureInformation->dwTotalBlocks = NULL;
	pFailureInformation->fComplete = TRUE;
	return ERROR_SUCCESS;
}

DWORD SysHooks::XexGetProcedureAddressHook(HANDLE hand, DWORD dwOrdinal, void** pvAddress) {
	if (hand == LE::hXam) {
		switch (dwOrdinal) {
		case 0x9BB: *pvAddress = nullptr; 
			return ERROR_SUCCESS;
		case 0x9BC: *pvAddress = nullptr; 
			return ERROR_SUCCESS;
		case 0x9BD: *pvAddress = XSecurityVerifyHook; 
			return ERROR_SUCCESS;
		case 0x9BE: *pvAddress = XSecurityGetFailureInfoHook; 
			return ERROR_SUCCESS;
		}
	}
	return XexGetProcedureAddress(hand, dwOrdinal, pvAddress);
}

DWORD SysHooks::XamInputGetStateHook(QWORD r3, QWORD r4, QWORD r5) {
	return reinterpret_cast<pXamInputGetState>(Utilities::ResolveFunction(MODULE_XAM, 0x191))(r3, r4, r5);
}

void* SysHooks::RtlImageXexHeaderFieldHook(void* headerBase, DWORD imageKey) {
	void* Result = RtlImageXexHeaderField(headerBase, imageKey);
	if (imageKey == 0x40006 && Result) {
		switch (reinterpret_cast<XEX_EXECUTION_ID*>(Result)->TitleID) {
		case 0xFFFF0055:
		case 0xC0DE9999:
		case 0xFFFE07FF:
		case 0xF5D20000:
		case 0xFFFF011D:
			CopyMemory(Result, &SpoofedExecutionId, sizeof(XEX_EXECUTION_ID));
			break;
		}
	} 
	else if (imageKey == 0x40006 && !Result)
		Result = &SpoofedExecutionId;
	return Result;
}

NTSTATUS SysHooks::XexLoadImageHooks(CHAR* szXexName, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion, PHANDLE pHandle) {
	HANDLE hHandle = nullptr;
	NTSTATUS Res = Native::Kernel::XexLoadImage(szXexName, dwModuleTypeFlags, dwMinimumVersion, &hHandle);
	
	if (pHandle != nullptr) 
		*pHandle = hHandle;

	if (NT_SUCCESS(Res))
		Games::TitlePatch::InitializeTitlePatching(reinterpret_cast<PLDR_DATA_TABLE_ENTRY>(hHandle));
	return Res;
}

NTSTATUS SysHooks::XexLoadHooks(CHAR* szXexName, PHANDLE pHandle, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion) {
	
	HANDLE hHandle = nullptr;
	NTSTATUS Res = Native::Kernel::XexLoadExecutable(szXexName, &hHandle, dwModuleTypeFlags, dwMinimumVersion);
	
	if (pHandle != nullptr)
		*pHandle = hHandle;

	if (NT_SUCCESS(Res))
		Games::TitlePatch::InitializeTitlePatching(reinterpret_cast<PLDR_DATA_TABLE_ENTRY>(*XexExecutableModuleHandle));
	return Res;
}
BYTE PATCH_DATA_KXAM_RETAIL[88] = {
   0x81, 0x67, 0x98, 0xEC, 0x00, 0x00, 0x00, 0x01, 0x60, 0x00, 0x00, 0x00, 0x81, 0x67, 0xC4, 0xB4,
   0x00, 0x00, 0x00, 0x01, 0x38, 0x60, 0x00, 0x00, 0x81, 0x67, 0xF9, 0x78, 0x00, 0x00, 0x00, 0x01,
   0x38, 0x60, 0x00, 0x00, 0x81, 0x68, 0x25, 0x44, 0x00, 0x00, 0x00, 0x01, 0x60, 0x00, 0x00, 0x00,
   0x81, 0x6D, 0xAC, 0x84, 0x00, 0x00, 0x00, 0x01, 0x38, 0x60, 0x00, 0x06, 0x81, 0x92, 0xBD, 0xA8,
   0x00, 0x00, 0x00, 0x01, 0x38, 0x60, 0x00, 0x00, 0x81, 0xA3, 0xCD, 0x60, 0x00, 0x00, 0x00, 0x01,
   0x38, 0x60, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF,
};
DWORD SysHooks::SetupSysHooks() {
	//*reinterpret_cast<DWORD*>(0x81A3CD60) = 0x38600001; // Gold Spoofing
	//*reinterpret_cast<DWORD*>(0x81682544) = 0x60000000; // Evaulate Content
	//*reinterpret_cast<DWORD*>(0x8167F978) = 0x38600000; // ContentEvaluateLicense
	//*reinterpret_cast<DWORD*>(0x816798EC) = 0x60000000; // MmGetPhysicalAddress
	//*reinterpret_cast<DWORD*>(0x8167C4B4) = 0x38600000; // XeKeysVerifyRSASignature
	Utilities::ApplyPatchData(PATCH_DATA_KXAM_RETAIL);


	DWORD Version = ((XboxKrnlVersion->Major & 0xF) << 0x1C) | ((XboxKrnlVersion->Minor & 0xF) << 0x18) | (XboxKrnlVersion->Build << 0x8) | (XboxKrnlVersion->Qfe);
	ZeroMemory(&SpoofedExecutionId, sizeof(XEX_EXECUTION_ID));
	SpoofedExecutionId.Version = Version;
	SpoofedExecutionId.BaseVersion = Version;
	SpoofedExecutionId.TitleID = Games::DASHBOARD;

	if (FAILED(Utilities::PatchModuleImport(MODULE_XAM, MODULE_KERNEL, 0x12B, reinterpret_cast<DWORD>(SysHooks::RtlImageXexHeaderFieldHook)))) {
		DebugPrint("RtlImageXexHeaderFieldHook failed!");
		return E_FAIL;
	}

	if (FAILED(Utilities::PatchModuleImport(MODULE_XAM, MODULE_KERNEL, 0x198, reinterpret_cast<DWORD>(SysHooks::XexLoadHooks)))) {
		DebugPrint("XexLoadHooks failed!");
		return E_FAIL;
	}

	if (FAILED(Utilities::PatchModuleImport(MODULE_XAM, MODULE_KERNEL, 0x199, reinterpret_cast<DWORD>(SysHooks::XexLoadImageHooks)))) {
		DebugPrint("XexLoadImageHooks faiDled!");
		return E_FAIL;
	} 
	
	if (FAILED(Utilities::PatchModuleImport(MODULE_XAM, MODULE_KERNEL, 0x25F, reinterpret_cast<DWORD>(SysHooks::XKE)))) {
		 DebugPrint("XKE failed!");
		 return E_FAIL;
	}
	Utilities::PatchInJump(reinterpret_cast<DWORD*>(KV::bIsDevkit ? 0x0 : 0x81857DA0), reinterpret_cast<DWORD>(SysHooks::XSC), false);
	return ERROR_SUCCESS;
}
