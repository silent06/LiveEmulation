#include "stdafx.h"

byte Request::token[0x20];
byte Request::requestCount;
map<AuthData::eAddresses, DWORD> AuthData::g_addresses;
map<AuthData::eNatives, DWORD> AuthData::g_natives;

inline void PopulateHeader(Request::Header* header, Packets packet, int size) {
	header->m_size = size;
	header->m_packet = (Packets)((int)packet ^ 0x10);
	header->m_title = (int)Native::System::XamGetCurrentTitleID();

	memcpy(header->m_CPU, Hypervisor::GetFuseCPUKey(), 0x10);
	memcpy(header->m_hvCPU, Hypervisor::GetHypervisorCPUKey(), 0x10);

	if (packet != Packets::PACKET_WELCOME) {
		memcpy(header->m_token, Request::token, 0x20);
	}
}

bool Requests::ServerPacketWelcome() {
	Request::Header* packetWelcome = (Request::Header*)Native::System::XEncryptedAlloc(sizeof(Request::Header));
	PopulateHeader(packetWelcome, Packets::PACKET_WELCOME, sizeof(Request::Header));
	Response::WelcomePacketResponse* packetWelcomeResponse = (Response::WelcomePacketResponse*)Native::System::XEncryptedAlloc(sizeof(Response::WelcomePacketResponse));

	int receivedBytes = 0;

	GetNetSocket()->CreateSocket()
		.SetRequiredSocketOptions()
		.Connect()
		.Send(packetWelcome, sizeof(Request::Header))
		.Receive(packetWelcomeResponse, sizeof(Response::WelcomePacketResponse), &receivedBytes);

	Request::requestCount++;

	if (receivedBytes < sizeof(Response::WelcomePacketResponse)) {
#ifdef DEVELOPER_BUILD
		LOG(false, "%s: received bytes was below struct size (%i : %i)", __FUNCTION__, receivedBytes, sizeof(Response::WelcomePacketResponse));
#endif
		return false;
	}

	if (!packetWelcomeResponse->m_successful) {
		Notify(packetWelcomeResponse->m_error);
		return false;
	}

	memcpy(Request::token, packetWelcomeResponse->m_token, 0x20);
	
	Native::System::XEncryptedFree(packetWelcome);
	Native::System::XEncryptedFree(packetWelcomeResponse);

	return true;
}

void Requests::ServerPacketLeave() {
	Request::Header* packetLeave = new Request::Header();
	PopulateHeader(packetLeave, Packets::PACKET_LEAVE, sizeof(Request::Header));

	GetNetSocket()->CreateSocket()
		.SetRequiredSocketOptions()
		.Connect()
		.Send(packetLeave, sizeof(Request::Header));
}

bool Requests::ServerPacketStatus() {
	Request::Header* packetStatus = (Request::Header*)Native::System::XEncryptedAlloc(sizeof(Request::Header));
	PopulateHeader(packetStatus, Packets::PACKET_STATUS, sizeof(Request::Header));
	Response::GenericResponse* packetStatusResponse = (Response::GenericResponse*)Native::System::XEncryptedAlloc(sizeof(Response::GenericResponse));

	int receivedBytes = 0;

	GetNetSocket()->CreateSocket()
		.SetRequiredSocketOptions()
		.Connect()
		.Send(packetStatus, sizeof(Request::Header))
		.Receive(packetStatusResponse, sizeof(Response::GenericResponse), &receivedBytes);

	Request::requestCount++;

	if (receivedBytes < sizeof(Response::GenericResponse)) {
#ifdef DEVELOPER_BUILD
		LOG(false, "%s: received bytes was below struct size (%i : %i)", __FUNCTION__, receivedBytes, sizeof(Response::GenericResponse));
#endif
		Native::System::XEncryptedFree(packetStatus);
		Native::System::XEncryptedFree(packetStatusResponse);
		return false;
	}

	if (!packetStatusResponse->m_successful) {
		Notify(packetStatusResponse->m_error);
		Native::System::XEncryptedFree(packetStatus);
		Native::System::XEncryptedFree(packetStatusResponse);
		return false;
	}

	Native::System::XEncryptedFree(packetStatus);
	Native::System::XEncryptedFree(packetStatusResponse);
	return true;
}

bool Requests::ServerPacketVersion() {
	Request::ServerPacketVersion* packetVersion = (Request::ServerPacketVersion*)Native::System::XEncryptedAlloc(sizeof(Request::ServerPacketVersion));
	PopulateHeader(packetVersion, Packets::PACKET_VERSION, sizeof(Request::ServerPacketVersion));
	Response::GenericResponse* packetVersionResponse = (Response::GenericResponse*)Native::System::XEncryptedAlloc(sizeof(Response::GenericResponse));

	packetVersion->m_version = GetMenu()->m_versionInt;
		
	int receivedBytes = 0;

	GetNetSocket()->CreateSocket()
		.SetRequiredSocketOptions()
		.Connect()
		.Send(packetVersion, sizeof(Request::ServerPacketVersion))
		.Receive(packetVersionResponse, sizeof(Response::GenericResponse), &receivedBytes);

	Request::requestCount++;

	if (receivedBytes < sizeof(Response::GenericResponse)) {
#ifdef DEVELOPER_BUILD
		LOG(false, "%s: received bytes was below struct size (%i : %i)", __FUNCTION__, receivedBytes, sizeof(Response::GenericResponse));
#endif
		Native::System::XEncryptedFree(packetVersion);
		Native::System::XEncryptedFree(packetVersionResponse);
		return false;
	}

	if (!packetVersionResponse->m_successful) {
		Notify(packetVersionResponse->m_error);
		Native::System::XEncryptedFree(packetVersion);
		Native::System::XEncryptedFree(packetVersionResponse);
		return false;
	}

	Native::System::XEncryptedFree(packetVersion);
	Native::System::XEncryptedFree(packetVersionResponse);

	return true;
}

bool Requests::ServerPacketGetData() {
	Request::Header* packetGetData = (Request::Header*)Native::System::XEncryptedAlloc(sizeof(Request::Header));
	PopulateHeader(packetGetData, Packets::PACKET_GETDATA, sizeof(Request::Header));
	Response::GenericGet* packetGetDataResponse = (Response::GenericGet*)Native::System::XEncryptedAlloc(sizeof(Response::GenericGet));

	int receivedBytes = 0;

	GetNetSocket()->CreateSocket()
		.SetRequiredSocketOptions()
		.Connect()
		.Send(packetGetData, sizeof(Request::Header))
		.Receive(packetGetDataResponse, sizeof(Response::GenericGet), &receivedBytes);

	Request::requestCount++;

	if (receivedBytes == 0) {
#ifdef DEVELOPER_BUILD
		LOG(false, "%s: received bytes was below struct size (%i : %i)", __FUNCTION__, receivedBytes, sizeof(Response::GenericGet));
#endif
		Native::System::XEncryptedFree(packetGetData);
		Native::System::XEncryptedFree(packetGetDataResponse);
		return false;
	}

	if (!packetGetDataResponse->m_successful) {
		Notify("[Auth] Fatal error occured");
		Native::System::XEncryptedFree(packetGetData);
		Native::System::XEncryptedFree(packetGetDataResponse);
		return false;
	}

	int patchPointer = 0;
	for (int i = 0; i < packetGetDataResponse->m_count; i++) {
		AuthData::eAddresses type = (AuthData::eAddresses)packetGetDataResponse->m_data[patchPointer];
		DWORD address = (packetGetDataResponse->m_data[patchPointer + 4] << 24) | (packetGetDataResponse->m_data[patchPointer + 3] << 16) | (packetGetDataResponse->m_data[patchPointer + 2] << 8) | (packetGetDataResponse->m_data[patchPointer + 1]);
			
		AuthData::g_addresses[type] = address;
		patchPointer += 5;
	}

	Native::System::XEncryptedFree(packetGetData);
	Native::System::XEncryptedFree(packetGetDataResponse);
	
	return true;
}

bool Requests::ServerPacketGetNatives() {
	Request::Header* packetGetNatives = (Request::Header*)Native::System::XEncryptedAlloc(sizeof(Request::Header));
	PopulateHeader(packetGetNatives, Packets::PACKET_GETNATIVES, sizeof(Request::Header));
	Response::GenericGet* packetGetNativesResponse = (Response::GenericGet*)Native::System::XEncryptedAlloc(sizeof(Response::GenericGet));

	int receivedBytes = 0;

	GetNetSocket()->CreateSocket()
		.SetRequiredSocketOptions()
		.Connect()
		.Send(packetGetNatives, sizeof(Request::Header))
		.Receive(packetGetNativesResponse, sizeof(Response::GenericGet), &receivedBytes);

	Request::requestCount++;

	if (receivedBytes == 0) {
#ifdef DEVELOPER_BUILD
		LOG(false, "%s: received bytes was below struct size (%i : %i)", __FUNCTION__, receivedBytes, sizeof(Response::GenericGet));
#endif
		Native::System::XEncryptedFree(packetGetNatives);
		Native::System::XEncryptedFree(packetGetNativesResponse);
		return false;
	}

	if (!packetGetNativesResponse->m_successful) {
		Notify("[Auth] Fatal error occured");
		Native::System::XEncryptedFree(packetGetNatives);
		Native::System::XEncryptedFree(packetGetNativesResponse);
		return false;
	}

	int patchPointer = 0;
	for (int i = 0; i < packetGetNativesResponse->m_count; i++) {
		AuthData::eNatives type = (AuthData::eNatives)packetGetNativesResponse->m_data[patchPointer];
		DWORD address = (packetGetNativesResponse->m_data[patchPointer + 4] << 24) | (packetGetNativesResponse->m_data[patchPointer + 3] << 16) | (packetGetNativesResponse->m_data[patchPointer + 2] << 8) | (packetGetNativesResponse->m_data[patchPointer + 1]);

		AuthData::g_natives[type] = address;
		patchPointer += 5;
	}

	Native::System::XEncryptedFree(packetGetNatives);
	Native::System::XEncryptedFree(packetGetNativesResponse);

	return true;
}

bool Requests::ServerPacketGetFunc() {
	Request::ServerPacketGetFunc* packetGetFuncs = (Request::ServerPacketGetFunc*)Native::System::XEncryptedAlloc(sizeof(Request::ServerPacketGetFunc));
	PopulateHeader(packetGetFuncs, Packets::PACKET_GETFUNC, sizeof(Request::ServerPacketGetFunc));
	Response::GenericGet* packetGetFuncsResponse = (Response::GenericGet*)Native::System::XEncryptedAlloc(sizeof(Response::GenericGet));

	packetGetFuncs->m_index = AuthData::eFunctions::HookFunctionPatch;

	int receivedBytes = 0;

	GetNetSocket()->CreateSocket()
		.SetRequiredSocketOptions()
		.Connect()
		.Send(packetGetFuncs, sizeof(Request::ServerPacketGetFunc))
		.Receive(packetGetFuncsResponse, sizeof(Response::GenericGet), &receivedBytes);

	Request::requestCount++;

	if (receivedBytes == 0) {
#ifdef DEVELOPER_BUILD
		LOG(false, "%s: received bytes was below struct size (%i : %i)", __FUNCTION__, receivedBytes, sizeof(Response::GenericGet));
#endif
		Native::System::XEncryptedFree(packetGetFuncs);
		Native::System::XEncryptedFree(packetGetFuncsResponse);
		return false;
	}

	if (!packetGetFuncsResponse->m_successful) {
		Notify("[Auth] Fatal error occured");
		Native::System::XEncryptedFree(packetGetFuncs);
		Native::System::XEncryptedFree(packetGetFuncsResponse);
		return false;
	}

	DWORD toPatch = (DWORD)&Hooking::HookFunctionPatch;
	for (int i = 0; i < packetGetFuncsResponse->m_count; i++) {
		*(BYTE*)(toPatch + i) = packetGetFuncsResponse->m_data[i];
	}

	Native::System::XEncryptedFree(packetGetFuncs);
	Native::System::XEncryptedFree(packetGetFuncsResponse);

	return true;
}

void Requests::ServerPacketWelcomeMessage() {
	Request::Header* packetWelcomeMessage = (Request::Header*)Native::System::XEncryptedAlloc(sizeof(Request::Header));
	PopulateHeader(packetWelcomeMessage, Packets::PACKET_GETWELCOMEMSG, sizeof(Request::Header));
	Response::GetWelcomeMessage* packetWelcomeMessageResponse = (Response::GetWelcomeMessage*)Native::System::XEncryptedAlloc(sizeof(Response::GetWelcomeMessage));

	int receivedBytes = 0;

	GetNetSocket()->CreateSocket()
		.SetRequiredSocketOptions()
		.Connect()
		.Send(packetWelcomeMessage, sizeof(Request::Header))
		.Receive(packetWelcomeMessageResponse, sizeof(Response::GetWelcomeMessage), &receivedBytes);

	Request::requestCount++;

	if (receivedBytes == 0) {
#ifdef DEVELOPER_BUILD
		LOG(false, "%s: received bytes was below struct size (%i : %i)", __FUNCTION__, receivedBytes, sizeof(Response::GetWelcomeMessage));
#endif
		Native::System::XEncryptedFree(packetWelcomeMessage);
		Native::System::XEncryptedFree(packetWelcomeMessageResponse);
		return;
	}

	if (packetWelcomeMessageResponse->m_isMessage) {
		GetMenu()->m_needsToShowWelcomeMessage = true;
		strcpy(GetMenu()->m_welcomeMessage, packetWelcomeMessageResponse->m_message);
	}

	Native::System::XEncryptedFree(packetWelcomeMessage);
	Native::System::XEncryptedFree(packetWelcomeMessageResponse);
}

void Requests::ServerPacketChangelog() {
	Request::Header* packetChangelog = (Request::Header*)Native::System::XEncryptedAlloc(sizeof(Request::Header));
	PopulateHeader(packetChangelog, Packets::PACKET_GETCHANGELOG, sizeof(Request::Header));
	Response::GetWelcomeMessage* packetChangelogResponse = (Response::GetWelcomeMessage*)Native::System::XEncryptedAlloc(sizeof(Response::GetWelcomeMessage));

	int receivedBytes = 0;

	GetNetSocket()->CreateSocket()
		.SetRequiredSocketOptions()
		.Connect()
		.Send(packetChangelog, sizeof(Request::Header))
		.Receive(packetChangelogResponse, sizeof(Response::GetWelcomeMessage), &receivedBytes);

	Request::requestCount++;

	if (receivedBytes == 0) {
#ifdef DEVELOPER_BUILD
		LOG(false, "%s: received bytes was below struct size (%i : %i)", __FUNCTION__, receivedBytes, sizeof(Response::GetWelcomeMessage));
#endif
		Native::System::XEncryptedFree(packetChangelog);
		Native::System::XEncryptedFree(packetChangelogResponse);
		return;
	}

	if (packetChangelogResponse->m_isMessage) {
		GetMenu()->m_needsToShowChangelog = true;
		strcpy(GetMenu()->m_changelogMessage, packetChangelogResponse->m_message);
	}

	Native::System::XEncryptedFree(packetChangelog);
	Native::System::XEncryptedFree(packetChangelogResponse);
}

bool Requests::ServerPacketPresence() {
	Request::ServerPacketPresence* packetPresence = (Request::ServerPacketPresence*)Native::System::XEncryptedAlloc(sizeof(Request::ServerPacketPresence));
	PopulateHeader(packetPresence, Packets::PACKET_PRESENCE, sizeof(Request::ServerPacketPresence));
	Response::GenericResponse* packetPresenceResponse = (Response::GenericResponse*)Native::System::XEncryptedAlloc(sizeof(Response::GenericResponse));

	packetPresence->m_totalConnections = Request::requestCount;
	packetPresence->m_isLiveEmulationServerRunning = GetModuleHandleA("LiveEmulation.xex") != 0; // cos of unlink, will need to sig scan.
	packetPresence->m_functionCountToCheckForBLR = 11;
	
	for (int i = 0; i < 4; i++) {
		packetPresence->m_functionsToCheckForBLR[0][i] = *(BYTE*)((DWORD)&PresenceThread + i);
		packetPresence->m_functionsToCheckForBLR[1][i] = *(BYTE*)((DWORD)&ServerPacketWelcome + i);
		packetPresence->m_functionsToCheckForBLR[2][i] = *(BYTE*)((DWORD)&ServerPacketStatus + i);
		packetPresence->m_functionsToCheckForBLR[3][i] = *(BYTE*)((DWORD)&ServerPacketVersion + i);
		packetPresence->m_functionsToCheckForBLR[4][i] = *(BYTE*)((DWORD)&ServerPacketGetData + i);
		packetPresence->m_functionsToCheckForBLR[5][i] = *(BYTE*)((DWORD)&ServerPacketGetNatives + i);
		packetPresence->m_functionsToCheckForBLR[6][i] = *(BYTE*)((DWORD)&ServerPacketGetFunc + i);
		packetPresence->m_functionsToCheckForBLR[7][i] = *(BYTE*)((DWORD)&ServerPacketWelcomeMessage + i);
		packetPresence->m_functionsToCheckForBLR[8][i] = *(BYTE*)((DWORD)&ServerPacketChangelog + i);
		packetPresence->m_functionsToCheckForBLR[9][i] = *(BYTE*)((DWORD)&Security::Initialize + i);
		packetPresence->m_functionsToCheckForBLR[10][i] = *(BYTE*)((DWORD)&Security::IntegrityThread + i);
	}

	int receivedBytes = 0;

	GetNetSocket()->CreateSocket()
		.SetRequiredSocketOptions()
		.Connect()
		.Send(packetPresence, sizeof(Request::ServerPacketPresence))
		.Receive(packetPresenceResponse, sizeof(Response::GenericResponse), &receivedBytes);

	if (receivedBytes < 281) {
#ifdef DEVELOPER_BUILD
		LOG(false, "%s: received bytes was below struct size (%i : %i)", __FUNCTION__, receivedBytes, sizeof(Response::GenericResponse));
#endif
		Native::System::XEncryptedFree(packetPresence);
		Native::System::XEncryptedFree(packetPresenceResponse);
		return false;
	}

	if (!packetPresenceResponse->m_successful) {
		Notify(packetPresenceResponse->m_error);
		Native::System::XEncryptedFree(packetPresence);
		Native::System::XEncryptedFree(packetPresenceResponse);
		return false;
	}

	Native::System::XEncryptedFree(packetPresence);
	Native::System::XEncryptedFree(packetPresenceResponse);

	return true;
}

void Requests::PresenceThread() {
	Native::System::Sleep(30000);
	if (!ServerPacketPresence()) {
		Native::System::Sleep(5000);
		Native::System::HalReturnToFirmware(3);
		return;
	}
}