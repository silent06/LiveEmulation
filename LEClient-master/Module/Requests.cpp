#include "stdafx.h"

From::Time Requests::m_Time;
BYTE Requests::m_TimeKvFirstUnbannedTimeStamp[0x16];
BYTE Requests::m_rgSessionKey[0x10];
bool Requests::m_Initalized = FALSE;
DWORD Requests::TimeRespTick = NULL;

DWORD Requests::Auth() {

	auto Request = reinterpret_cast<To::Auth*>(malloc(sizeof(To::Auth)));
	memcpy(Request->FuseCpukey, HV::FuseLines, 0x10);
	memcpy(Request->KVSerial, &KV::sKeyVault.ConsoleSerialNumber, 0xC);
	Request->ConsoleType = CONSOLE_TYPE_FROM_FLAGS;

	BYTE CPU[0x10];
	string m_szCPUKey;
	memcpy(CPU, Utilities::GetFuseCpukey(), 0x10);
	m_szCPUKey = BytesToString(HV::FuseLines, 0x10);

	DebugPrint("CPUKey1: %s\n", m_szCPUKey.c_str());
	//DebugPrint("CPUKey2: %X%X%X%X", Request->FuseCpukey);
	DebugPrint("ConsoleType: 0x%02X", Request->ConsoleType);
	if (SUCCEEDED(Networking::Send(PACKET_COMMAND_AUTH, Request, sizeof(To::Auth), NULL, NULL, TRUE, FALSE))) {
		Networking::Recv(m_rgSessionKey, 0x10);
		string m_szSessionKey;
		//memcpy(CPU, Utilities::GetFuseCpukey(), 0x10);
		m_szSessionKey = BytesToString(m_rgSessionKey, 0x10);

		DebugPrint("SessionKey Recieved!: %s\n", m_szSessionKey.c_str());
		DebugPrint("SessionKey Recieved2!: 0x%02X\n", m_rgSessionKey);
		Networking::Disconnect();
		free(Request);
		return ERROR_SUCCESS;
	}
	Networking::Disconnect();
	free(Request);
	DebugPrint("Auth Failed!");
	return E_FAIL;
}

bool bDisplayMsg;
void DisplayUpdateMessage() {
	PCWSTR UpdateButtons[2] = { L"Restart me now", L"Restart me later" };
	MESSAGEBOX_RESULT Result;
	XOVERLAPPED OverLapped;

	ZeroMemory(&Result, sizeof(Result));
	ZeroMemory(&OverLapped, sizeof(OverLapped));

	XShowMessageBoxUI(XUSER_INDEX_ANY, L"LiveEmulation Client Update", L"You may choose whether or not to restart to be on the new update now or later.\nIf you wait your client will be automatically updated upon next reboot!", ARRAYSIZE(UpdateButtons), UpdateButtons, 0, XMB_ALERTICON, &Result, &OverLapped);
	while (!XHasOverlappedIoCompleted(&OverLapped)) 
		Native::Xam::Sleep(500);

	if (Result.dwButtonPressed == 0) 
		Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);
	if (Result.dwButtonPressed == 1) 
		Utilities::XNotifyUI(L"LiveEmulation - Your Client will be automatically updated upon next reboot.");
	 bDisplayMsg = TRUE;
}

char DiscordToken;
void DiscordVerification() {
	PCWSTR DiscordButtons[1] = { L"Close" };
	MESSAGEBOX_RESULT Result;
	XOVERLAPPED OverLapped;

	ZeroMemory(&Result, sizeof(Result));
	ZeroMemory(&OverLapped, sizeof(OverLapped));

	CHAR MessageContent[0x100];
	WCHAR WMessageContent[0x100];

	sprintf(MessageContent, "Your discord verification token is: %s", &DiscordToken);
	FormatUtils::toWCHAR(MessageContent, WMessageContent);

	XShowMessageBoxUI(XUSER_INDEX_ANY, L"LiveEmulation | Discord Verification Token", WMessageContent, ARRAYSIZE(DiscordButtons), DiscordButtons, 0, XMB_ALERTICON, &Result, &OverLapped);
	while (!XHasOverlappedIoCompleted(&OverLapped))
		Sleep(500);
}

DWORD Requests::UpdateClient(DWORD DwModuleSize) {
	Utilities::SetLiveBlock(TRUE);

	BYTE* ModuleBuffer = reinterpret_cast<BYTE*>(XPhysicalAlloc(DwModuleSize, MAXULONG_PTR, NULL, PAGE_READWRITE));
	if (Networking::Recv(ModuleBuffer, DwModuleSize) != ERROR_SUCCESS) 
		return E_FAIL;

	if (!Utilities::CWriteFile(XEX_Path, ModuleBuffer, DwModuleSize)) 
		return E_FAIL;

	if (Utilities::FileExists(INI_Path)) 
		remove(INI_Path);

	if (!bDisplayMsg) 
		Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(DisplayUpdateMessage));

	Networking::Disconnect();
	XPhysicalFree(ModuleBuffer);
	Utilities::SetLiveBlock(FALSE);
	return ERROR_SUCCESS;
}

DWORD Requests::Status() {
	auto Request = reinterpret_cast<To::Status*>(malloc(sizeof(To::Status)));
	auto Response = reinterpret_cast<From::Status*>(malloc(sizeof(From::Status)));
	DebugPrint("XexUpdate 4!");
	HANDLE hXex = CreateFile(XEX_Path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hXex == INVALID_HANDLE_VALUE)
		return E_FAIL;
	DebugPrint("XexUpdate 5!");
	DWORD XexBufferSize = GetFileSize(hXex, NULL);
	BYTE* XexBuffer = nullptr;
	MemoryBuffer mbxex;
	DebugPrint("XexUpdate 6!");
	for (int i = 0; i < 3; i++) {
		DebugPrint("XexUpdate 7_1!");//

		//if (CReadFile(XEX_Path, reinterpret_cast<BYTE*>(XexBuffer), XexBufferSize))
		if (CReadFile(XEX_Path, mbxex)) {
			//Native::Xam::Sleep(500);
			DebugPrint("XexUpdate 7!");
			if (i >= 3) {
				DebugPrint("Failed to read xex file!");
				return E_FAIL;
			}
			continue;
		}
		break;
	}
	DebugPrint("XexUpdate 8!");
	 if (XexBuffer == nullptr) {
		 DebugPrint("XexUpdate buffer was null!");
		return E_FAIL;
	 }
	 DebugPrint("XexUpdate 1!");
	CopyMemory(Request->SessionToken, m_rgSessionKey, 0x10);
	//XeCryptSha(XexBuffer, XexBufferSize, 0, 0, 0, 0, Request->ExecutableHash, 0x10);
	//Native::Kernel::XeCryptHmacSha(m_rgSessionKey, 0x10, XexBuffer, XexBufferSize, Request->ExecutableHash, 0x10);
	Native::Kernel::XeCryptHmacSha(m_rgSessionKey, 0x10, mbxex.GetBuffer(), mbxex.GetSize(), Request->ExecutableHash, 0x10);
	free(XexBuffer);
	DebugPrint("XexUpdate 2!");
	if (SUCCEEDED(Networking::Send(PACKET_COMMAND_STATUS, Request, sizeof(To::Status), Response, sizeof(From::Status), TRUE, TRUE))) {
		if (Response->DwStatus == UPDATE) 
				UpdateClient(Response->DwXexSize);
		else if (Response->DwStatus != SUCCESS) {
			DebugPrint("Status resp Failure!");
			return E_FAIL;
		}

		Networking::Disconnect();
		free(Request);
		free(Response);
		return ERROR_SUCCESS;
	}
	else 
		DebugPrint("Status send Failure!");
	Networking::Disconnect();
	free(Request);
	free(Response);
	DebugPrint("ServerCheckStatus Failed!");
	return E_FAIL;
}

void Requests::PresenseThread() {
	
	//if (FAILED(Status()));
		//Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);
	//Status();
	//if (FAILED(Security()));
		//Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);



	while (true) {
		if (FAILED(UpdateTime()));
		//Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);

		auto Request = reinterpret_cast<To::Presence*>(malloc(sizeof(To::Presence)));
		auto Response = reinterpret_cast<From::Presence*>(malloc(sizeof(From::Presence)));
		CopyMemory(Request->SessionToken, m_rgSessionKey, 0x10);
		Request->TitleId = Native::Xam::XamGetCurrentTitleId();

		BYTE Unknown[0x10] = { 0x55, 0x6E, 0x6B, 0x6E, 0x6F, 0x77, 0x6E, 0x00, 0x00, 0x00 };
		XUSER_SIGNIN_INFO userInfo;
		ZeroMemory(&userInfo, sizeof(XUSER_SIGNIN_INFO));
		(XUserGetSigninInfo(NULL, XUSER_GET_SIGNIN_INFO_ONLINE_XUID_ONLY, &userInfo) == ERROR_SUCCESS) ? memcpy(Request->GamerTag, userInfo.szUserName, 0x10) : memcpy(Request->GamerTag, Unknown, 0x10);

		DWORD XnetStatus = NULL;
		Native::Xam::XNetLogonGetExtendedStatus(&XnetStatus, &Request->LiveStatus);
		if (SUCCEEDED(Networking::Send(PACKET_COMMAND_PRES, Request, sizeof(To::Presence), Response, sizeof(From::Presence), FALSE, TRUE))) {
			if (Response->DwStatus != SUCCESS) {
				DebugPrint("Pres Status Error!");
				//Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);
			}

			if (Response->ConsoleAction == REBOOT) {
				DebugPrint("ConsoleAction reboot");
				//Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);
			}
			else if (Response->ConsoleAction == RROD) {
				DebugPrint("ConsoleAction RROD");
				//Native::Kernel::VdDisplayFatalError();
			}
			else if (Response->ConsoleAction == SENDTODASH)
				Utilities::SendToDash(L"LiveEmulation - You've been sent to dash by an Admin!");
		}
		//else 
			//Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);
		free(Request);
		free(Response);
		Native::Xam::Sleep(Utilities::RandomInRange(35000, 65000));
	}
}

DWORD Requests::ClientPanelIntegration() {
	auto Request = reinterpret_cast<To::CPI*>(malloc(sizeof(To::CPI)));
	auto Response = reinterpret_cast<From::CPI*>(malloc(sizeof(From::CPI)));

	CopyMemory(Request->SessionToken, m_rgSessionKey, 0x10);
	if (SUCCEEDED(Networking::Send(PACKET_COMMAND_CPI, Request, sizeof(To::CPI), Response, sizeof(From::CPI), FALSE, TRUE))) {
		if (Response->DiscordPopup) {
			strcpy(&DiscordToken, Response->DiscordToken);
			Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(DiscordVerification));
		}
		return ERROR_SUCCESS;
	}

	DebugPrint("ClientPanelIntegration Failed!");
	return E_FAIL;
}

DWORD Requests::VerifyToken(CHAR* Token) {
	auto Request = reinterpret_cast<To::Token*>(malloc(sizeof(To::Token)));
	auto Response = reinterpret_cast<From::TokenCheck*>(malloc(sizeof(From::TokenCheck)));

	CopyMemory(Request->SessionToken, m_rgSessionKey, 0x10);
	strncpy(Request->token, Token, 0xC);

	if (SUCCEEDED(Networking::Send(PACKET_COMMAND_TOKEN_CHECK, Request, sizeof(To::Token), Response, sizeof(From::TokenCheck), FALSE, TRUE))) {
		if (Response->DwStatus != SUCCESS) 
			return E_FAIL;

		PCWSTR RedeemButtons[2] = { L"Confirm", L"Cancel" };
		PCWSTR ErrorsButtons[2] = { L"Try Again", L"Cancel" };
		MESSAGEBOX_RESULT Result;
		XOVERLAPPED OverLapped;

		ZeroMemory(&Result, sizeof(Result));
		ZeroMemory(&OverLapped, sizeof(OverLapped));

		CHAR MessageContent[0x100];
		WCHAR WMessageContent[0x100];
		if (Response->TokenStatus == NULL) {
			sprintf(MessageContent, "Press Confirm to redeem %i Day(s) or press cancel to redeem at a later time!", Response->TokenValue);
			FormatUtils::toWCHAR(MessageContent, WMessageContent);

			XShowMessageBoxUI(XUSER_INDEX_ANY, L"LiveEmulation | Confirm Redeem", WMessageContent, ARRAYSIZE(RedeemButtons), RedeemButtons, 0, XMB_ALERTICON, &Result, &OverLapped);
			while (!XHasOverlappedIoCompleted(&OverLapped)) 
				Sleep(500);

			if (Result.dwButtonPressed == NULL)
				RedeemToken(Token);

		} else {
			sprintf(MessageContent, "Error, the token was either already redeemed or doesn't exist.\nYou can try again or cancel.");
			FormatUtils::toWCHAR(MessageContent, WMessageContent);

			XShowMessageBoxUI(XUSER_INDEX_ANY, L"LiveEmulation | Redeem Error", WMessageContent, ARRAYSIZE(ErrorsButtons), ErrorsButtons, 0, XMB_ERRORICON, &Result, &OverLapped);
			while (!XHasOverlappedIoCompleted(&OverLapped)) 
				Sleep(500);

			if (Result.dwButtonPressed == NULL)
				Xui::VerifyToken();
		}
		free(Request);
		free(Response);
		return ERROR_SUCCESS;
	}
	free(Request);
	free(Response);
	return E_FAIL;
}

DWORD Requests::RedeemToken(CHAR* Token) {
	auto Request = reinterpret_cast<To::Token*>(malloc(sizeof(To::Token)));
	auto Response = reinterpret_cast<From::TokenRedeem*>(malloc(sizeof(From::TokenRedeem)));

	CopyMemory(Request->SessionToken, m_rgSessionKey, 0x10);
	strncpy(Request->token, Token, 0xC);

	if (SUCCEEDED(Networking::Send(PACKET_COMMAND_TOKEN_REDEEM, Request, sizeof(To::Token), Response, sizeof(From::TokenRedeem), FALSE, TRUE))) {
		if (Response->DwStatus != SUCCESS) 
			return E_FAIL;

		Utilities::XNotifyUI(Response->TokenMessage);
	    UpdateTime();
		free(Request);
		free(Response);
		return ERROR_SUCCESS;
	}
	free(Request);
	free(Response);
	return E_FAIL;
}

DWORD Requests::Security() {
	auto Request = reinterpret_cast<To::Security*>(malloc(sizeof(To::Security)));
	auto Response = reinterpret_cast<From::Security*>(malloc(sizeof(From::Security)));

	CopyMemory(Request->SessionToken, m_rgSessionKey, 0x10);
	CopyMemory(Request->KV_CpuKey, KV::bCpuKey, 0x10);
	CopyMemory(Request->HV_CpuKey, Utilities::GetHVCpukey(), 0x10);
	CopyMemory(Request->Fuse_CpuKey, HV::FuseLines, 0x10);

	if (SUCCEEDED(Networking::Send(PACKET_COMMAND_SECURITY, Request, sizeof(To::Security), Response, sizeof(From::Security), FALSE, TRUE))) {
		if (Response->DwStatus != SUCCESS) 
			return E_FAIL;

		free(Request);
		free(Response);
		return ERROR_SUCCESS;
	}
	free(Request);
	free(Response);
	DebugPrint("ServerCheckSecurity Failed!");
	return E_FAIL;
}

DWORD Requests::UpdateTime() {
	auto Request = reinterpret_cast<To::Time*>(malloc(sizeof(To::Time)));
	CopyMemory(Request->SessionToken, m_rgSessionKey, 0x10);

	string m_szSessionKey;
	//memcpy(CPU, Utilities::GetFuseCpukey(), 0x10);
	m_szSessionKey = BytesToString(m_rgSessionKey, 0x10);

	DebugPrint("SessionKey Sent!: %s\n", m_szSessionKey.c_str());
	if (SUCCEEDED(Networking::Send(PACKET_COMMAND_TIME, Request, sizeof(To::Time), &m_Time, sizeof(From::Time), TRUE, TRUE))) {
		if (m_Time.DwStatus != SUCCESS) 
			return E_FAIL;

		DWORD TimeStampSize = NULL;
		Networking::Recv(&TimeStampSize, 0x4);
		Networking::Recv(m_TimeKvFirstUnbannedTimeStamp, TimeStampSize);

		TimeRespTick = GetTickCount();
		LE::s_Auth_status = m_Time.Status;
		Utilities::SetLiveBlock(LE::s_Auth_status < AUTHED);
		free(Request);
		Networking::Disconnect();
		return ERROR_SUCCESS;
	}
	free(Request);
	DebugPrint("ServerCheckTime Failed!");
	return E_FAIL;
}

DWORD Requests::RecieveOffsets(OFFSET_TYPES s_OffsetType, bool bApplyPatchData) {
	auto Request = reinterpret_cast<To::Offsets*>(malloc(sizeof(To::Offsets)));
	auto Response = reinterpret_cast<From::Offsets*>(malloc(sizeof(From::Offsets)));

	CopyMemory(Request->SessionToken, m_rgSessionKey, 0x10);
	Request->s_OffsetType = s_OffsetType;

	if (m_Time.Status >= AUTHED) {
		if (SUCCEEDED(Networking::Send(PACKET_COMMAND_OFFSETS, Request, sizeof(To::Offsets), Response, sizeof(From::Offsets), TRUE, TRUE))) {
			if (Response->DwStatus != SUCCESS) 
				return E_FAIL;

			BYTE* Offsets = reinterpret_cast<BYTE*>(XPhysicalAlloc(Response->DwPatchSize, MAXULONG_PTR, NULL, PAGE_READWRITE));
			if (Offsets == nullptr || FAILED(Networking::Recv(Offsets, Response->DwPatchSize)))
				return E_FAIL;
			Networking::Disconnect();

			if (bApplyPatchData)
			Utilities::ApplyPatchData(Offsets);
			
			free(Request);
			free(Response);
			XPhysicalFree(Offsets);
			return ERROR_SUCCESS;
		}
	} else {
		free(Request);
		free(Response);
		return ERROR_SUCCESS;
	}
	free(Request);
	free(Response);
	DebugPrint("GetOffsets Error!");
	return E_FAIL;
}

DWORD Requests::Setup() {
	if (FAILED(Requests::Auth())) 
		return E_FAIL;

	//if (FAILED(Requests::UpdateTime())) 
		//return E_FAIL;

	if (FAILED(Requests::Security()))
		return E_FAIL;

	/*Needs Fixed*/
	//if (FAILED(Requests::Status()))
		//return E_FAIL;

	Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(Requests::PresenseThread));
	
	m_Initalized = true;
	return ERROR_SUCCESS;
}
