#pragma once
#include "stdafx.h"

class SysHooks {
public:

	static XEX_EXECUTION_ID SpoofedExecutionId;
	typedef HRESULT(*pXamInputGetState)(QWORD r3, QWORD r4, QWORD r5);

	static DWORD XamInputGetStateHook(QWORD r3, QWORD r4, QWORD r5);

	static DWORD XexGetProcedureAddressHook(HANDLE hand, DWORD dwOrdinal, void** pvAddress);
	static DWORD SetupSysHooks();
	static void* RtlImageXexHeaderFieldHook(void* headerBase, DWORD imageKey);

	static NTSTATUS XexLoadImageHooks(CHAR* szXexName, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion, PHANDLE pHandle);
	static NTSTATUS XexLoadHooks(CHAR* szXexName, PHANDLE pHandle, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion);

	static DWORD XKE(BYTE* Buffer, DWORD BufferSize, BYTE* Salt, void* KrnlBuild, void* arg3, void* arg4);
	static DWORD XSC(DWORD Task, BYTE* Buffer, DWORD BufferSize);
};