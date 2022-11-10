#include "stdafx.h"

DWORD Xbdm::m_tableAddress;
vector<XbdmCommandTable> Xbdm::m_commandTable;
vector<pair<DWORD, DWORD>> Security::m_AutomaticWriteBackList;
vector<pair<DWORD, DWORD>> Security::hLEReadProtection;
vector<pair<DWORD, DWORD>> Security::hLEReadProtectionWhitelist;

__declspec(noinline) void Security::IntegrityThread() {
	auto items = GetIntegrityManager()->GetItems();
	for (auto i = 0; i < items.size(); i++) {
		for (auto s = 0; s < items.at(i).m_bytes.size(); s++) {
			if (*reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(items.at(i).m_function) + s) != items.at(i).m_bytes.at(s)) {
#ifdef DEBUG
			DebugPrint("Function: 0x%x | OpCode: 0x%x", items.at(i).m_bytes.at(s), *(DWORD*)((DWORD)items.at(i).m_function + s));
#endif
				*reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(items.at(i).m_function) + s) = items.at(i).m_bytes.at(s);
				return;
			}
		}
	}
	// manual check mmreadcheck since other xex's / tools use this we have to check this lightweight
	// detour result if its not this result the detour or function has been modified
	// first opcode is detour second is a standard patch jump
	if (*reinterpret_cast<PSHORT>(0x800817C0) != 0x3D60 && *reinterpret_cast<PSHORT>(0x800817C0) != 0x3C00) {
#ifdef DEBUG
		DebugPrint("Function: 0x800817C0 | OpCode: 0x%x", *(PSHORT)0x800817C0);
#endif
	}
	Native::Xam::Sleep(500);
}

bool bDisplayedWarning = FALSE;
__declspec(noinline) void Security::DebuggerDetectionThread() {
// Hr: 9101E3AC = debugger || watson
// Mm: 9101E3B4 = debugger
/* if (*(PINT)0x9101E3B4 != 0) {
	ProcessMetric(metricDebuggerPresent);
} */

#ifdef USE_SECURITY
// detect watson
if (*(PINT)0x9101E3AC) {
	if (!bDisplayedWarning) {
		printf("[LiveEmulation] -> Basic Debugger Detected!\n");
		bDisplayedWarning = TRUE;
	}
}
#endif
   Native::Xam::Sleep(500);
}

bool Security::XBDMSanityCheck() {
	auto XbdmHand = GetModuleHandle(MODULE_XBDM);
	if (!XbdmHand)
		return FALSE;

	PLDR_DATA_TABLE_ENTRY LDR = reinterpret_cast<PLDR_DATA_TABLE_ENTRY>(XbdmHand);
	if (LDR->CheckSum != 0x1E0BB) 
		return FALSE;
	return TRUE;
}

void Security::UnlinkFromKEB() {
	PLDR_DATA_TABLE_ENTRY ldr = reinterpret_cast<PLDR_DATA_TABLE_ENTRY>(LE::hKernel);
	PLIST_ENTRY CurrentEntry = ldr->InLoadOrderLinks.Flink;
	PLDR_DATA_TABLE_ENTRY Current = nullptr;

	while (CurrentEntry != &ldr->InLoadOrderLinks && CurrentEntry != nullptr) {
		Current = CONTAINING_RECORD(CurrentEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

		if (Current->ImageBase == reinterpret_cast<void*>(BASEADDY)) {
			Current->InLoadOrderLinks.Flink->Blink = Current->InLoadOrderLinks.Blink;
			Current->InLoadOrderLinks.Blink->Flink = Current->InLoadOrderLinks.Flink;
			Current->InInitializationOrderLinks.Flink->Blink = Current->InInitializationOrderLinks.Blink;
			Current->InInitializationOrderLinks.Blink->Flink = Current->InInitializationOrderLinks.Flink;
			Current->InClosureOrderLinks.Flink->Blink = Current->InClosureOrderLinks.Blink;
			Current->InClosureOrderLinks.Blink->Flink = Current->InClosureOrderLinks.Flink;
			break;
		}
		CurrentEntry = CurrentEntry->Flink;
	}
}

void Security::AddRegionToWhitelist(DWORD start, DWORD length) {
	hLEReadProtectionWhitelist.push_back(make_pair(start, length));
}

typedef bool(*t_MmDbgReadCheckHook)(DWORD dwAddress);
t_MmDbgReadCheckHook MmDbgReadCheckHookStub;
bool Security::MmDbgReadCheckHook(DWORD dwAddress) {
	DWORD Ret = MmDbgReadCheckHookStub(dwAddress);
	if (hLEReadProtectionWhitelist.size() > 0) {
		for (auto i = 0; i < hLEReadProtectionWhitelist.size(); i++) {
			if (dwAddress >= hLEReadProtectionWhitelist.at(i).first && dwAddress <= (hLEReadProtectionWhitelist.at(i).first + hLEReadProtectionWhitelist.at(i).second))
				return MmDbgReadCheckHookStub(dwAddress);
		}
	}

	if (hLEReadProtection.size() > 0) {
		for (auto i = 0; i < hLEReadProtection.size(); i++) {
			if (dwAddress >= hLEReadProtection.at(i).first && dwAddress <= (hLEReadProtection.at(i).first + hLEReadProtection.at(i).second))
				return FALSE;
		}
	}
	return Ret;
}

void Security::ProtectMyMemorySpace() {
	PLDR_DATA_TABLE_ENTRY m_XexTableEntry = nullptr;
	XexPcToFileHeader(reinterpret_cast<void*>(BASEADDY), &m_XexTableEntry);
	LE::dwMyModuleSize = m_XexTableEntry->SizeOfFullImage;

	if (m_XexTableEntry != nullptr) {
		hLEReadProtection.emplace_back(make_pair(BASEADDY, m_XexTableEntry->SizeOfFullImage));
		MmDbgReadCheckHookStub = reinterpret_cast<t_MmDbgReadCheckHook>(Utilities::HookFunctionStub(MODULE_KERNEL, 0x1AB, MmDbgReadCheckHook));
		GetIntegrityManager()->Add(reinterpret_cast<void*>(MmDbgReadCheckHook), Utilities::GetFunctionSize(reinterpret_cast<DWORD*>(MmDbgReadCheckHook)));
	}
	else 
		Native::Kernel::VdDisplayFatalError();
} 

void Security::SetupIntegrity() {
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XexLoadImageFromMemory), 0x16C); // XexLoadImageFromMemory 0x8007CFA8 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(HalReturnToFirmware), 0x94); // HalReturnToFirmware 0x800682A0 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(RtlImageXexHeaderField), 0x4); // RtlImageXexHeaderField 0x80088C90 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(KeGetCurrentProcessType), 0x4); // KeGetCurrentProcessType 0x80071A68 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XexUnloadImage), 0x4); // XexUnloadImage 0x8007C838 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XexLoadExecutable), 0x4); // XexLoadExecutable 0x8007CF30 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XexLoadImage), 0x4); // XexLoadImage 0x8007CF10 | devkit ->
	//GetIntegrityManager()->Add(reinterpret_cast<void*>(XexGetModuleSection), 0x4); // XexGetModuleSection 0x8007C958 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XexPcToFileHeader), 0x4); // XexPcToFileHeader 0x8007ACA8 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(ExCreateThread), 0x4); // ExCreateThread 0x80065F40  | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(ObCreateSymbolicLink), 0x4); // ObCreateSymbolicLink 0x8008A5F0 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(RtlInitAnsiString), 0x4); // RtlInitAnsiString 0x80085860 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(ExSetXConfigSetting), 0x4); // ExSetXConfigSetting 0x80065688 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(ExGetXConfigSetting), 0x4); // ExGetXConfigSetting 0x80065700 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(MmGetPhysicalAddress), 0x4); // MmGetPhysicalAddress 0x8007F798 | devkit ->
	//GetIntegrityManager()->Add(reinterpret_cast<void*>(VdDisplayFatalError), 0x27C); // VdDisplayFatalError 0x800BD398 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XeCryptRandom), 0x4); // XeCryptRandom 0x80061440 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XeCryptHmacShaInit), 0x4); // XeCryptHmacShaInit 0x801144B0 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XeCryptHmacShaUpdate), 0x4); // XeCryptHmacShaUpdate 0x80114600 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XeCryptHmacShaFinal), 0x4); // XeCryptHmacShaFinal 0x80114608 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XeCryptDes3Ecb), 0x4); // XeCryptDes3Ecb 0x80112598 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XeCryptDes3Key), 0x4); // XeCryptDes3Key 0x80112950 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XeCryptAesEcb), 0x4); // XeCryptAesEcb 0x8010F640 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XeCryptAesCbc), 0x4); // XeCryptAesCbc 0x8010F660 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XeCryptAesKey), 0x4); // XeCryptAesKey 0x8010F630 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XeCryptHmacSha), 0x4); // XeCryptHmacSha 0x80114658 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XeCryptRotSumSha), 0x4); // XeCryptRotSumSha 0x80114930 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XeCryptMd5), 0x4); // XeCryptMd5 0x80113F48 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XeCryptSha), 0x4); // XeCryptSha 0x801143E8 | devkit ->
	GetIntegrityManager()->Add(reinterpret_cast<void*>(XeCryptRc4), 0x4); // XeCryptRc4 0x80114750 | devkit ->

	GetIntegrityManager()->Add(ProtectMyMemorySpace, 0x4);
	GetIntegrityManager()->Add(IntegrityThread, 0x1F0);
	GetIntegrityManager()->Add(DebuggerDetectionThread, 0x8C);
	GetIntegrityManager()->Add(UnlinkFromKEB, 0x4);

	GetIntegrityManager()->Add(Xbdm::InitializeCommands, 0x4);
	GetIntegrityManager()->Add(Xbdm::HookCommand, 0x4);
	GetIntegrityManager()->Add(XbdmCommands::Getmemex, 0x4);
	GetIntegrityManager()->Add(XbdmCommands::Getmem, 0x4);
	GetIntegrityManager()->Add(XbdmCommands::Setmem, 0x4);
	GetIntegrityManager()->Add(XbdmCommands::Break, 0x4);
	GetIntegrityManager()->Add(XbdmCommands::Continue, 0x4);
	GetIntegrityManager()->Add(XbdmCommands::Debugger, 0x4);

	Xbdm::InitializeCommands();
	Xbdm::HookCommand("getmemex", reinterpret_cast<DWORD>(XbdmCommands::Getmemex)); // peek
	Xbdm::HookCommand("getmem", reinterpret_cast<DWORD>(XbdmCommands::Getmem)); // peek
	Xbdm::HookCommand("setmem", reinterpret_cast<DWORD>(XbdmCommands::Setmem)); // poke
	Xbdm::HookCommand("break", reinterpret_cast<DWORD>(XbdmCommands::Break)); // set breakpoint
	Xbdm::HookCommand("continue", reinterpret_cast<DWORD>(XbdmCommands::Continue)); // continue breakpoint
	Xbdm::HookCommand("debugger", reinterpret_cast<DWORD>(XbdmCommands::Debugger)); // debugger 

	//GetThreadPool()->RegisterThread(Security::DebuggerDetectionThread, TRUE);
	GetThreadPool()->RegisterThread(IntegrityThread, TRUE);
	GetThreadPool()->RegisterThread(Requests::PresenseThread, TRUE);
	UnlinkFromKEB();
}
