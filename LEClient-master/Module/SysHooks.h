#pragma once
#include "stdafx.h"

class Hooks {
public:

	static XEX_EXECUTION_ID SpoofedExecutionId;
	typedef HRESULT(*pXamInputGetState)(QWORD r3, QWORD r4, QWORD r5);

	static DWORD XamInputGetStateHook(QWORD r3, QWORD r4, QWORD r5);

	static DWORD XexGetProcedureAddressHook(HANDLE hand, DWORD dwOrdinal, PVOID* pvAddress);
	static DWORD SetupSysHooks();
	static PVOID RtlImageXexHeaderFieldHook(PVOID headerBase, DWORD imageKey);

	static NTSTATUS XexLoadImageHooks(PCHAR szXexName, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion, PHANDLE pHandle);
	static NTSTATUS XexLoadHooks(PCHAR szXexName, PHANDLE pHandle, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion);
};