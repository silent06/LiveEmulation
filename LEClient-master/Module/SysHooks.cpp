#include "stdafx.h"

XEX_EXECUTION_ID Hooks::SpoofedExecutionId;
VOID APCWorker(PVOID Arg1, PVOID Arg2, PVOID Arg3) {
	if (Arg2) 
		static_cast<LPOVERLAPPED_COMPLETION_ROUTINE>(Arg2)(reinterpret_cast<DWORD>(Arg3), NULL, static_cast<LPOVERLAPPED>(Arg1));
}

VOID XSecurityVerifyHook(DWORD dwMilliseconds, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {
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

DWORD Hooks::XexGetProcedureAddressHook(HANDLE hand, DWORD dwOrdinal, PVOID* pvAddress) {
	if (hand == LE::Xamhandle) {
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

DWORD Hooks::XamInputGetStateHook(QWORD r3, QWORD r4, QWORD r5) {
	return reinterpret_cast<pXamInputGetState>(Utilities::ResolveFunction(MODULE_XAM, 0x191))(r3, r4, r5);
}

PVOID Hooks::RtlImageXexHeaderFieldHook(PVOID headerBase, DWORD imageKey) {
	PVOID retVal = RtlImageXexHeaderField(headerBase, imageKey);
	if (imageKey == 0x40006 && retVal) {
		switch (static_cast<XEX_EXECUTION_ID*>(retVal)->TitleID) {
		case 0xFFFF0055:
		case 0xC0DE9999:
		case 0xFFFE07FF:
		case 0xF5D20000:
		case 0xFFFF011D:
			CopyMemory(retVal, &SpoofedExecutionId, sizeof(XEX_EXECUTION_ID));
			break;
		}
	} 
	else if (imageKey == 0x40006 && !retVal) retVal = &SpoofedExecutionId; return retVal;
}

NTSTATUS Hooks::XexLoadImageHooks(PCHAR szXexName, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion, PHANDLE pHandle) {
	HANDLE mHandle = nullptr;
	NTSTATUS Res = Native::Kernel::XexLoadImage(szXexName, dwModuleTypeFlags, dwMinimumVersion, &mHandle);
	
	if (pHandle != nullptr) 
		*pHandle = mHandle;

	if (NT_SUCCESS(Res))
		Games::TitlePatch::InitializeTitlePatching(static_cast<PLDR_DATA_TABLE_ENTRY>(mHandle));
	return Res;
}

NTSTATUS Hooks::XexLoadHooks(PCHAR szXexName, PHANDLE pHandle, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion) {
	
	HANDLE mHandle = nullptr;
	NTSTATUS Res = Native::Kernel::XexLoadExecutable(szXexName, &mHandle, dwModuleTypeFlags, dwMinimumVersion);
	
	if (pHandle != nullptr)
		*pHandle = mHandle;

	if (NT_SUCCESS(Res))
		Games::TitlePatch::InitializeTitlePatching(static_cast<PLDR_DATA_TABLE_ENTRY>(*XexExecutableModuleHandle));
	return Res;
}

DWORD Hooks::SetupSysHooks() {
	DWORD Version = ((XboxKrnlVersion->Major & 0xF) << 0x1C) | ((XboxKrnlVersion->Minor & 0xF) << 0x18) | (XboxKrnlVersion->Build << 0x8) | (XboxKrnlVersion->Qfe);
	ZeroMemory(&SpoofedExecutionId, sizeof(XEX_EXECUTION_ID));
	SpoofedExecutionId.Version = Version;
	SpoofedExecutionId.BaseVersion = Version;
	SpoofedExecutionId.TitleID = Games::DASHBOARD;

	if (!SUCCEEDED(Utilities::PatchModuleImport(MODULE_XAM, MODULE_KERNEL, 0x12B, reinterpret_cast<DWORD>(Hooks::RtlImageXexHeaderFieldHook)))) {
#ifdef DEBUG
		DebugPrint("RtlImageXexHeaderFieldHook failled!");
#endif
		return E_FAIL;
	}

	if (!SUCCEEDED(Utilities::PatchModuleImport(MODULE_XAM, MODULE_KERNEL, 0x198, reinterpret_cast<DWORD>(Hooks::XexLoadHooks)))) {
#ifdef DEBUG
		DebugPrint("XexLoadHooks failled!");
#endif
		return E_FAIL;
	}

	if (!SUCCEEDED(Utilities::PatchModuleImport(MODULE_XAM, MODULE_KERNEL, 0x199, reinterpret_cast<DWORD>(Hooks::XexLoadImageHooks)))) {
#ifdef DEBUG
		DebugPrint("XexLoadImageHooks failled!");
#endif
		return E_FAIL;
	} 
	
	if (!SUCCEEDED(Utilities::PatchModuleImport(MODULE_XAM, MODULE_KERNEL, 0x25F, reinterpret_cast<DWORD>(LiveTools::XKE)))) {
	#ifdef DEBUG
			DebugPrint("XKE failed!");
	#endif
			return E_FAIL;
	}
	Utilities::PatchInJump(reinterpret_cast<PDWORD>(KV::IsDevkit ? 0x0 : 0x81857DA0), reinterpret_cast<DWORD>(LiveTools::XSC_XnpLogonSetChallengeResponseHook), FALSE);
	return ERROR_SUCCESS;
}
