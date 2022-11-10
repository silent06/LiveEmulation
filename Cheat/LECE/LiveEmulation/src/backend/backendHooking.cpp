#include "stdafx.h"

vector<Hook> Hooking::m_hooks;
unsigned char Hooking::m_hookSection[0x500] = { 0 };
unsigned int Hooking::m_hookCount = 0;

namespace HookingUtils {
	void __declspec(naked) GLPR(void) {
		__asm {
			std     r14, -0x98(sp)
			std     r15, -0x90(sp)
			std     r16, -0x88(sp)
			std     r17, -0x80(sp)
			std     r18, -0x78(sp)
			std     r19, -0x70(sp)
			std     r20, -0x68(sp)
			std     r21, -0x60(sp)
			std     r22, -0x58(sp)
			std     r23, -0x50(sp)
			std     r24, -0x48(sp)
			std     r25, -0x40(sp)
			std     r26, -0x38(sp)
			std     r27, -0x30(sp)
			std     r28, -0x28(sp)
			std     r29, -0x20(sp)
			std     r30, -0x18(sp)
			std     r31, -0x10(sp)
			stw     r12, -0x8(sp)
			blr
		}
	}

	DWORD RelinkGPLR(DWORD SFSOffset, PDWORD SaveStubAddress, PDWORD OriginalAddress) {
		DWORD Instruction = 0, Replacing;
		PDWORD Saver = (PDWORD)GLPR;
		if (SFSOffset & 0x2000000) {
			SFSOffset = SFSOffset | 0xFC000000;
		}
		Replacing = OriginalAddress[SFSOffset / 4];
		for (int i = 0; i < 20; i++) {
			if (Replacing == Saver[i]) {
				DWORD NewOffset = (DWORD)&Saver[i] - (DWORD)SaveStubAddress;
				Instruction = 0x48000001 | (NewOffset & 0x3FFFFFC);
			}
		}
		return Instruction;
	}
}

vector<Hook> Hooking::GetHooks() {
	return m_hooks;
}

__declspec(naked) void Hooking::HookFunctionPatch(DWORD Address, void* Function, BOOL Linked) {
	__asm {
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		lwz r3, 0x1337(r1)
		blr
	}
}

DWORD Hooking::HookFunctionStub(AuthData::eAddresses Address, void* Function, const char* name) {
	DWORD ret = HookFunctionStub(AuthData::g_addresses[Address], Function, name);
	return ret;
}

DWORD Hooking::HookFunctionStub(DWORD _Address, void* Function, const char* name) {
	DWORD* Address = (DWORD*)_Address;

	m_hooks.push_back(Hook());
	int hookIndex = m_hooks.size() - 1;

	m_hooks[hookIndex].m_address = _Address;
	m_hooks[hookIndex].m_hook = Function;
	m_hooks[hookIndex].m_name = name;
	memcpy(m_hooks[hookIndex].m_originalAssembly, (void*)_Address, 0x10);

	DWORD *startStub = (DWORD*)&m_hookSection[m_hookCount * 32];
	m_hookCount++;

	for (int i = 0; i < 7; i++)
		startStub[i] = 0x60000000;
	startStub[7] = 0x4E800020;

	HookFunctionStart(Address, startStub, (DWORD)Function);

#ifdef DEVELOPER_BUILD
	LOG(false, "[Hooking] Hooked %s @ 0x%X with stub @ 0x%X", name, _Address, startStub);
#endif

	return (DWORD)startStub;
}

void Hooking::HookFunctionStart(PDWORD Address, PDWORD SaveStub, DWORD Destination, DWORD expected) {
	if ((SaveStub != NULL) && (Address != NULL)) {
		DWORD AddressRelocation = (DWORD)(&Address[4]);
		if (AddressRelocation & 0x8000) {
			SaveStub[0] = 0x3D600000 + (((AddressRelocation >> 16) & 0xFFFF) + 1);
		} else {
			SaveStub[0] = 0x3D600000 + ((AddressRelocation >> 16) & 0xFFFF);
		}

		SaveStub[1] = 0x396B0000 + (AddressRelocation & 0xFFFF);
		SaveStub[2] = 0x7D6903A6;

		for (int i = 0; i < 4; i++) {
			if ((Address[i] & 0x48000003) == 0x48000001) {
				SaveStub[i + 3] = HookingUtils::RelinkGPLR((Address[i] & ~0x48000003), &SaveStub[i + 3], &Address[i]);
			} else {
				SaveStub[i + 3] = Address[i];
			}
		}

		SaveStub[7] = 0x4E800420;
		__dcbst(0, SaveStub);
		__sync();
		__isync();
		HookFunctionPatch((DWORD)Address, (void*)Destination);
	}
}