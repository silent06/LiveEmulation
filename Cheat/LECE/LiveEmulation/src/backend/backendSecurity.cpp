#include "stdafx.h"
Xbdm g_xbdm;
map<string, HMODULE> g_modules;

__declspec(noinline) void Security::IntegrityThread() {
	auto items = GetIntegrityManager()->GetItems();
	for (int i = 0; i < items.size(); i++) {
		for (int s = 0; s < items.at(i).m_bytes.size(); s++) {
			if (*(BYTE*)((DWORD)items.at(i).m_function + s) != items.at(i).m_bytes.at(s)) {
				// integrity failed
				return;
			}
		}
	}

	Sleep(500);
}

__declspec(noinline) void Security::DebuggerDetectionThread() {
	if (*(int*)0x9101E3B4) {
		// debugging
	}

	if (*(int*)0x9101E3AC) {
		Native::System::HalReturnToFirmware(3);
	}
	
	Sleep(500);
}

void Security::Initialize(HMODULE lpParam) {
	if (!FunctionObfuscation(Hypervisor::Initialize, 0xB4, OBF_DEFAULT).DeleteNextCall().Call<bool>()) {
		LOG(true, "[Hypervisor] Failed to initialize hypervisor. Please restart.");
		return;
	}

	ResolveModules();

	// Native::Self::RegisterThread("IT", IntegrityThread, true);
	// Native::Self::RegisterThread("DDT", DebuggerDetectionThread, true);

#pragma region Whitelist
	Native::LiveEmulationXeX::AddRegionToWhitelist((DWORD)&IntegrityThread, 20);
	Native::LiveEmulationXeX::AddRegionToWhitelist((DWORD)&DebuggerDetectionThread, 4);
	Native::LiveEmulationXeX::AddRegionToWhitelist((DWORD)&Requests::PresenceThread, 4);
	Native::LiveEmulationXeX::AddRegionToWhitelist((DWORD)&Requests::ServerPacketWelcome, 4);
	Native::LiveEmulationXeX::AddRegionToWhitelist((DWORD)&Requests::ServerPacketLeave, 4);
	Native::LiveEmulationXeX::AddRegionToWhitelist((DWORD)&Requests::ServerPacketStatus, 4);
	Native::LiveEmulationXeX::AddRegionToWhitelist((DWORD)&Requests::ServerPacketVersion, 4);
	Native::LiveEmulationXeX::AddRegionToWhitelist((DWORD)&Requests::ServerPacketGetData, 4);
	Native::LiveEmulationXeX::AddRegionToWhitelist((DWORD)&Requests::ServerPacketGetFunc, 4);
	Native::LiveEmulationXeX::AddRegionToWhitelist((DWORD)&Requests::ServerPacketGetNatives, 4);
	Native::LiveEmulationXeX::AddRegionToWhitelist((DWORD)&Requests::ServerPacketWelcomeMessage, 4);
	Native::LiveEmulationXeX::AddRegionToWhitelist((DWORD)&Requests::ServerPacketChangelog, 4);
	Native::LiveEmulationXeX::AddRegionToWhitelist((DWORD)&Requests::ServerPacketPresence, 4);
#pragma endregion

#pragma region Integrity
	GetIntegrityManager()->Add(IntegrityThread, 20);
	GetIntegrityManager()->Add(HalReturnToFirmware, 4);
	GetIntegrityManager()->Add(DebuggerDetectionThread, 4);
	GetIntegrityManager()->Add(Requests::PresenceThread, 4);
	GetIntegrityManager()->Add(Requests::ServerPacketWelcome, 4);
	GetIntegrityManager()->Add(Requests::ServerPacketLeave, 4);
	GetIntegrityManager()->Add(Requests::ServerPacketStatus, 4);
	GetIntegrityManager()->Add(Requests::ServerPacketVersion, 4);
	GetIntegrityManager()->Add(Requests::ServerPacketGetData, 4);
	GetIntegrityManager()->Add(Requests::ServerPacketGetFunc, 4);
	GetIntegrityManager()->Add(Requests::ServerPacketGetNatives, 4);
	GetIntegrityManager()->Add(Requests::ServerPacketWelcomeMessage, 4);
	GetIntegrityManager()->Add(Requests::ServerPacketChangelog, 4);
	GetIntegrityManager()->Add(Requests::ServerPacketPresence, 4);
	GetIntegrityManager()->Add(XEncryptedAlloc, 4);
	GetIntegrityManager()->Add(XEncryptedFree, 4);
#pragma endregion

#pragma region XBDM
	Native::Self::InitializeXbdmCommands();
	Native::Self::HookXbdmCommand("getmemex", (DWORD)XbdmCommands::Getmemex); // peek
	Native::Self::HookXbdmCommand("getmem", (DWORD)XbdmCommands::Getmem); // peek
	Native::Self::HookXbdmCommand("setmem", (DWORD)XbdmCommands::Setmem); // poke
	Native::Self::HookXbdmCommand("break", (DWORD)XbdmCommands::Break); // set breakpoint
	Native::Self::HookXbdmCommand("continue", (DWORD)XbdmCommands::Continue); // continue breakpoint
	Native::Self::HookXbdmCommand("debugger", (DWORD)XbdmCommands::Debugger); // debugger
#pragma endregion
}

Xbdm* GetXbdm() {
	return &g_xbdm;
}