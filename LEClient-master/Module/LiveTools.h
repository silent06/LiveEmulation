#include "stdafx.h"

class LiveTools {
public:
	static DWORD XKE(PBYTE Buffer, DWORD BufferSize, PBYTE Salt, PVOID KrnlBuild, PVOID arg3, PVOID arg4);
	static DWORD XSC_XnpLogonSetChallengeResponseHook(DWORD Task, PBYTE Buffer, DWORD BufferSize);

private:
};

