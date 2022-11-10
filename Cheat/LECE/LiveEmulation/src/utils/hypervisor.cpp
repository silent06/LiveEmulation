#include "stdafx.h"

__declspec(noinline) bool Hypervisor::Initialize() {
	PVOID pPhysExp = XPhysicalAlloc(0x1000, MAXULONG_PTR, 0, PAGE_READWRITE);
	DWORD physExpAdd = (DWORD)MmGetPhysicalAddress(pPhysExp);

	ZeroMemory(pPhysExp, 0x1000);

	for (DWORD x = 0; x < sizeof(HvPeekPokeExp); x++) HvPeekPokeExp[x] ^= 0x95;
	memcpy(pPhysExp, HvPeekPokeExp, sizeof(HvPeekPokeExp));

	HRESULT result = (HRESULT)ExpansionInstall(physExpAdd, 0x1000);

	XPhysicalFree(pPhysExp);
	return SUCCEEDED(result) ? true : false;
}

UINT64 Hypervisor::PeekQWORD(UINT64 address) {
	return ExpansionCall(0x48565050, 3, address);
}

UINT64 Hypervisor::GetFuseLine(DWORD index) {
	return PeekQWORD(0x8000020000020000 + (index * 0x200));
}

DWORD Hypervisor::PeekBytes(UINT64 address, PVOID buffer, DWORD length) {
	PVOID pBuffer = XPhysicalAlloc(length, MAXULONG_PTR, NULL, PAGE_READWRITE);
	ZeroMemory(pBuffer, length);
	DWORD Result = (DWORD)ExpansionCall(0x48565050, 0x04, address, (UINT64)MmGetPhysicalAddress(pBuffer), length);
	if (Result == S_OK)
		memcpy(buffer, pBuffer, length);

	XPhysicalFree(pBuffer);
	return Result;
}

PBYTE Hypervisor::GetHypervisorCPUKey() {
	BYTE cpukey[0x10];

	Hypervisor::PeekBytes(0x20, cpukey, 0x10);
	return cpukey;
}

PBYTE Hypervisor::GetFuseCPUKey() {
	BYTE cpukey[0x10];

	UINT64 f1p1 = Hypervisor::GetFuseLine(3) | Hypervisor::GetFuseLine(4);
	UINT64 f1p2 = Hypervisor::GetFuseLine(5) | Hypervisor::GetFuseLine(6);
	memcpy(cpukey, &f1p1, 0x08);
	memcpy(cpukey + 0x08, &f1p2, 0x08);

	return cpukey;
}