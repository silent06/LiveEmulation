#include "stdafx.h"

Games::AW::AWBypassData_s AwBypassData = { 0 };
DWORD XNetXnAddrToMachineIdHook(XNCALLER_TYPE xnc, XNADDR * pxnaddr, PQWORD pqwMachineId) {
	*pqwMachineId = 0xFA00000000000000 | (0x2000000 | rand() % 0x7FFFFF);
	return ERROR_SUCCESS;
}

typedef bool(*tAWLiveAntiCheatProcessResp)(INT Controller, SHORT FunctionID, PINT Ptr, __int64* Result);
tAWLiveAntiCheatProcessResp AWLiveAntiCheatProccessRespStub;
bool AWLiveAntiCheatProcessResp(INT Controller, SHORT FunctionID, PINT Ptr, __int64* Result) {
	bool Ret = AWLiveAntiCheatProccessRespStub(Controller, FunctionID, Ptr, Result);

	__int64 Seed;
	switch (FunctionID) {
	case 0xC8:
		Seed = *(__int64 *)*(PINT)(Ptr[0] + 0x04);
		if (Seed == 0xA5BFE) {
			Result[0] = 0x04333393;
			return Ret;
		}
		else 
			goto Dash;
		break;
	default:
	Dash:
		Utilities::SendToDash(L"LiveEmulation - AW Bypass failed!");
		break;
	}
	return Ret;
}

DWORD WINAPI Games::AW::AwBypass() {
	if (INI::AwBypass) {
		AWLiveAntiCheatProccessRespStub = (tAWLiveAntiCheatProcessResp)Utilities::HookFunctionStub(0x822CC740, AWLiveAntiCheatProcessResp);
		//Native::Xam::Sleep(2500);
		Utilities::XNotifyUI(L"LiveEmulation - AW success!");
	}
	return ERROR_SUCCESS;
}

DWORD WINAPI Games::BO2::BO2Bypass() {
	if (INI::BO2Bypass) {
		if (SUCCEEDED(Requests::RecieveOffsets(B02, true))) {
			//Native::Xam::Sleep(2500);
			Utilities::XNotifyUI(L"LiveEmulation - BO2 success!");
			return ERROR_SUCCESS;
		}
		else 
			Utilities::SendToDash(L"LiveEmulation - BO2 Bypass failed!");
	}
	return E_FAIL;
}

DWORD WINAPI  Games::Ghosts::GhostBypass() {
	if (INI::GhostBypass) {
		if (SUCCEEDED(Requests::RecieveOffsets(GHOSTS, true))) {
			Native::Xam::Sleep(2500);
			//Utilities::XNotifyUI(L"LiveEmulation - Ghosts success!");
			return ERROR_SUCCESS;
		}
		else 
			Utilities::SendToDash(L"LiveEmulation - Ghosts Bypass failed!");
	}
	return E_FAIL;
}
