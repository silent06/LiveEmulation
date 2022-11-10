#include "stdafx.h"

CRITICAL_SECTION CriticalSection;
bool Networking::m_Initalized;
bool Networking::Connected;
SOCKET Networking::sSocket;
BYTE Networking::rgSessionKey[0x10];
BYTE Networking::rgSessionKeySaltSeed[0xA] = { 0xC8, 0x29, 0xF6, 0x06, 0xA7, 0xD9, 0x6F, 0x6C, 0x2F, 0xF8 };
BYTE Networking::rgSessionKeySalt[0x10] = { 0xE6, 0x53, 0xE6, 0x3E, 0x53, 0x2F, 0x03, 0x8B, 0xD0, 0xFE, 0x57, 0x89,0x7B, 0x90, 0xA9, 0xA2 };
BYTE Networking::PrimaryDec[0x14] = { 0x6E, 0xD5, 0x0B, 0xC9, 0xE3, 0xBD, 0x7B, 0xC0, 0xB1, 0xED, 0xD7, 0x9C, 0xA7, 0x98, 0xD3, 0x36, 0xC0, 0x8D, 0x87, 0xD7 };
DWORD dwIndex = NULL;

DWORD Port[] = {
#if !defined(LOCAL) && !defined(DEVELOPERSERVER)
	0, //Alpha
	0, //Brava
	0, //Charile
#else
#ifdef LOCAL 
	5489, //Local 4168
#else
#ifdef DEVELOPERSERVER 
	4001, //Dev 4001
#endif
#endif
#endif

};
BYTE IP[][0x4] = {
#if !defined(LOCAL)
	{ 0x9A,0x34,0xD6,0x10  } // 9A 34 D6 10 207,45,82,102 CF 2D 52 66 
#else
#ifdef LOCAL 
{ 0xD4,0x7F,0xE7,0x4C} //0x1E, 0x1C, 0xA0, 0x72 }//Local , dev: 0xD4,0x7F,0xE7,0x4C 
#endif
#endif
};

INT Networking::XorMagic(INT Key) {
	INT c = 0x60, s = 0x10, x = 0x101 - 5;
	for (c = 0; c < 2; c++) {
		x = (s << 4) | (s >> 3);
		x ^= s;
		s ^= 0x6 >> 1;
		x *= 0x5;
	}
	x ^= Key;
	return x;
}

void Networking::XorMagic(BYTE* Key) {
	int c = 0x4, s = 0x50, x = 0x90;
	for (c = 0; c < 4; c++) {
		x = (s << 1) | (s >> 7);
		x ^= s;
		s ^= 0x50 >> 1;
	}
	x ^= 0x63;
	for (auto i = 0; i < 2; i++) {
		x = (s >> 1) | (s ^= 0x70);
		x ^= s;
	}
	for (auto i = 0; i < 0x14; i++)
		Key[i] ^= x;

	Utilities::Reverse(reinterpret_cast<BYTE*>(Key), 0x14);
}
void Networking::DecryptNetworking() {
	Native::Kernel::XeCryptRc4(rgSessionKeySaltSeed, 0xA, rgSessionKeySalt, 0x10);
	reverse(rgSessionKeySalt, rgSessionKeySalt);
	Native::Kernel::XeCryptRc4(rgSessionKeySaltSeed, 0xA, rgSessionKeySalt, 0x10);

	for (auto x = 0; x < 0x10; x++)
		rgSessionKeySalt[x] ^= (rgSessionKeySalt[x] + 0x2);

	Native::Kernel::XeCryptSha(rgSessionKeySalt, 0x10, nullptr, NULL, nullptr, NULL, rgSessionKey, 0x10);

	for (auto x = 0; x < 0x10; x++)
		rgSessionKeySalt[x] ^= reinterpret_cast<INT>("LiveEmulation");

	XorMagic(PrimaryDec);
#ifdef LOCAL
	Native::Kernel::XeCryptRc4(PrimaryDec, 0x14, IP[0], 0x4);
	//Port[0] ^= XorMagic(static_cast<int>(0x9C));
	XorMagic(static_cast<int>(Port));
#endif
	
#ifdef DEVELOPERSERVER
	//Port[0] ^= XorMagic(static_cast<int>(0x9C));
#endif

#ifndef LOCAL
	for (DWORD i = 0; i < MaxReconAttempts; i++) {
		Native::Kernel::XeCryptRc4(Networking::PrimaryDec, 0x14, IP[i], 0x4);
#if !defined(DEVELOPERSERVER) && !defined(LOCAL)
		Port[0] ^= XorMagic(reinterpret_cast<int>(0x9C));
#endif
	}
#endif
}

DWORD Networking::StartupServerCommunicator() {
	DecryptNetworking();
	InitializeCriticalSection(&CriticalSection);

	XNetStartupParams Xnsp;
	ZeroMemory(&Xnsp, sizeof(Xnsp));
	Xnsp.cfgSizeOfStruct = sizeof(Xnsp);
	Xnsp.cfgFlags = XNET_STARTUP_BYPASS_SECURITY;
	Xnsp.cfgSockDefaultRecvBufsizeInK = static_cast<BYTE>(SEND_RECV_SIZE);
	Xnsp.cfgSockDefaultSendBufsizeInK = static_cast<BYTE>(SEND_RECV_SIZE);
	Xnsp.cfgQosSrvMaxSimultaneousResponses = 0x10;

	if (Native::Xam::NetDll_XNetStartup(CALLER, &Xnsp) != NULL) {
#ifdef  DEBUG
		DebugPrint("Error 0x%02X \n", NetDll_WSAGetLastError());
#endif
		m_Initalized = FALSE;
		return E_FAIL;
	}

	WSADATA WsaData;
	if (Native::Xam::NetDll_WSAStartupEx(CALLER, MAKEWORD(2, 2), &WsaData, CUR_VER) != NULL) {
#ifdef  DEBUG
		DebugPrint("Error 0x%02X \n", NetDll_WSAGetLastError());
#endif
		m_Initalized = FALSE;
		return E_FAIL;
	}
	m_Initalized = TRUE;
	return ERROR_SUCCESS;
}

void Networking::Disconnect() {
	if (sSocket != INVALID_SOCKET && Connected) {
		Native::Xam::NetDll_closesocket(CALLER, sSocket);
		Connected = false;
	}
}
void GetHttpServerAddr(sockaddr_in* sHttpserveraddr) {
	sHttpserveraddr->sin_family = AF_INET;

#ifdef DEVELOPERSERVER
	sHttpserveraddr->sin_port = htons(4001);//Port[0]
#else
	sHttpserveraddr->sin_port = htons(Port[dwIndex]);
#endif

	/*sHttpserveraddr->sin_addr.s_net = IP[dwIndex][0];
	sHttpserveraddr->sin_addr.s_host = IP[dwIndex][1];
	sHttpserveraddr->sin_addr.s_lh = IP[dwIndex][2];
	sHttpserveraddr->sin_addr.s_impno = IP[dwIndex][3];*/
	//0xCF, 0x2D, 0x52, 0x66 = 207.45.82.102
	sHttpserveraddr->sin_addr.s_net = 0xCF;
	sHttpserveraddr->sin_addr.s_host = 0x2D;
	sHttpserveraddr->sin_addr.s_lh = 0x52;
	sHttpserveraddr->sin_addr.s_impno = 0x66;

#ifdef DEBUG
	 DebugPrint("index: %i |  Ip: 0x%02X, 0x%02X, 0x%02X, 0x%02X | Port: %i", dwIndex, IP[dwIndex][0], IP[dwIndex][1], IP[dwIndex][2], IP[dwIndex][3], Port[0]);
	DebugPrint("-------------------------------- IP INFO: -----------------------");
	DebugPrint("Ip: 0x%02X, 0x%02X, 0x%02X, 0x%02X", IP[0][0], IP[0][1], IP[0][2], IP[0][3]);
	DebugPrint("Ip: 0x%02X, 0x%02X, 0x%02X, 0x%02X", IP[1][0], IP[1][1], IP[1][2], IP[1][3]);
	DebugPrint("Ip: 0x%02X, 0x%02X, 0x%02X, 0x%02X", IP[2][0], IP[2][1], IP[2][2], IP[2][3]);
	DebugPrint("-----------------------------------------------------------"); 
#endif
}

DWORD Networking::Connect() {
	for (auto i = 0; i < MaxReconAttempts; i++) {
		if (Connected) 
			return ERROR_SUCCESS;

		if ((sSocket = NetDll_socket(CALLER, AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
			DebugPrint("Error 0x%02X \n", NetDll_WSAGetLastError());
			Native::Xam::Sleep(100);
			continue;
		}

		bool bSockOpt = TRUE;
		if (NetDll_setsockopt(CALLER, sSocket, SOL_SOCKET, 0x5801, reinterpret_cast<PCSTR>(&bSockOpt), sizeof(INT)) != NULL) {
			DebugPrint("Error 0x%02X \n", NetDll_WSAGetLastError());
			Native::Xam::Sleep(100);
			continue;
		}

		auto sendRecvSize = SEND_RECV_SIZE;
		if (NetDll_setsockopt(CALLER, sSocket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<PCSTR>(&sendRecvSize), sizeof(INT)) != NULL) {
			DebugPrint("Error 0x%02X \n", NetDll_WSAGetLastError());
			Native::Xam::Sleep(100);
			continue;
		}

		if (NetDll_setsockopt(CALLER, sSocket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<PCSTR>(&sendRecvSize), sizeof(INT)) != NULL) {
			DebugPrint("Error 0x%02X \n", NetDll_WSAGetLastError());
			Native::Xam::Sleep(100);
			continue;
		}
		dwIndex++;
		if (dwIndex >= MaxReconAttempts) 
			dwIndex = NULL;

#ifdef LOCAL
		dwIndex = NULL;
#endif

		sockaddr_in sHttpserveraddr;
		GetHttpServerAddr(&sHttpserveraddr);

		if (NetDll_connect(CALLER, sSocket, reinterpret_cast<sockaddr*>(&sHttpserveraddr), sizeof(sHttpserveraddr)) == SOCKET_ERROR) {
			DebugPrint("Error 0x%02X \n", WSAGetLastError());
			Native::Xam::Sleep(100);
			continue;
		}
		Connected = true;
		return ERROR_SUCCESS;
	}
	Connected = false;
	return E_FAIL;
}

void Networking::BuildNetworkingHeader(BYTE* Data, DWORD DataLen) {
	//Native::Kernel::XeCryptRc4(rgSessionKey, 0x10, reinterpret_cast<BYTE*>(Data), DataLen);
	Utilities::Reverse(reinterpret_cast<BYTE*>(Data), DataLen);

	//for (auto i = 0; i < DataLen; i++)
		//reinterpret_cast<BYTE*>(Data)[i] ^= 0x31;

	//BYTE Rc4Key[0x10]; 
	//XeCryptSha(reinterpret_cast<BYTE*>(0x815F0000), 0xE0, nullptr, NULL, nullptr, NULL, Rc4Key, 0x10);
	//Native::Kernel::XeCryptMd5(Rc4Key, 0x10, nullptr, NULL, nullptr, NULL, Rc4Key, 0x10);

	//BYTE RotSumKey[0x2];
	//HV::HvPeekBytes(0, RotSumKey, 0x2);
	//Native::Kernel::XeCryptRotSumSha(Rc4Key, 0x10, RotSumKey, 0x2, Rc4Key, 0x10);

	//for (auto i = 0; i < sizeof(Rc4Key); i++)
		//Rc4Key[i] ^= CUR_VER;

	//Native::Kernel::XeCryptRc4(Rc4Key, 0x10, reinterpret_cast<BYTE*>(Data), DataLen);
	Utilities::Reverse(reinterpret_cast<BYTE*>(Data), DataLen);
}

DWORD Networking::Send(PACKET_COMMAND CommandID, void* Data, DWORD DataLen) {
	if (!Connected) 
		if (Connect() != ERROR_SUCCESS)
			return E_FAIL;

	BYTE* PacketHeader = reinterpret_cast<BYTE*>(malloc(DataLen + 0x8));
	CopyMemory(PacketHeader, &CommandID, 0x4);
	CopyMemory(PacketHeader + 0x4, &DataLen, 0x4);
	BuildNetworkingHeader(reinterpret_cast<BYTE*>(Data), DataLen);
	CopyMemory(PacketHeader + 0x8, Data, DataLen);

	DWORD BytesLeft = (DataLen + 0x8);
	CHAR* CurPos = reinterpret_cast<CHAR*>(PacketHeader);
	while (BytesLeft > NULL) {
		DWORD sendSize = min(SEND_RECV_SIZE, BytesLeft);
		DWORD SendStatus = NetDll_send(CALLER, sSocket, CurPos, sendSize, NULL);

		if (SendStatus == SOCKET_ERROR) {
			DebugPrint("Send error %d", NetDll_WSAGetLastError());
			free(PacketHeader);
			return E_FAIL;
		}
		BytesLeft -= SendStatus;
		CurPos += SendStatus;
	}
	free(PacketHeader);
	return ERROR_SUCCESS;
}

DWORD Networking::Recv(void* Data, DWORD BytesExpected) {
	if (!Connected) 
		return E_FAIL;

	DWORD BytesLeft = BytesExpected;
	DWORD BytesRecieved = NULL;
	while (BytesLeft > NULL) {
		DWORD cbRecv = NetDll_recv(CALLER, sSocket, reinterpret_cast<CHAR*>(Data) + BytesRecieved, min(SEND_RECV_SIZE, BytesLeft), NULL);
		if (cbRecv == SOCKET_ERROR) {
			DebugPrint("Recv error %d\n", NetDll_WSAGetLastError());
			return E_FAIL;
		}

		if (cbRecv == NULL) 
			break;

		BytesLeft -= cbRecv;
		BytesRecieved += cbRecv;
	}
	if (BytesRecieved != BytesExpected) 
		return E_FAIL;
	BuildNetworkingHeader(reinterpret_cast<BYTE*>(Data), BytesRecieved);
	return ERROR_SUCCESS;
}

DWORD Networking::Send(PACKET_COMMAND CommandID, void* Request, DWORD RequestLen, void* Responce, DWORD ResponceLength, bool KeepConnectionOpen, bool ShouldRecieve) {
	if (!m_Initalized) 
		StartupServerCommunicator();
	EnterCriticalSection(&CriticalSection);

	while (Connected) 
		Native::Xam::Sleep(100);

	DWORD Now = GetTickCount();
	DWORD Return = ERROR_SUCCESS;

	if (FAILED(Connect())) {
		Return = E_FAIL;
		if (Return == E_FAIL) 
			DebugPrint("failed connect");
	}

	if (Return == ERROR_SUCCESS) 
		DebugPrint("connect took %i", GetTickCount() - Now);

	if (FAILED(Send(CommandID, Request, RequestLen))) {
		Return = E_FAIL;
		if (Return == E_FAIL) 
			DebugPrint("failed Send");
	}

	if (Return == ERROR_SUCCESS) 
		DebugPrint("send took %i", GetTickCount() - Now);

	if (ShouldRecieve && FAILED(Recv(Responce, ResponceLength))) {
		Return = E_FAIL;

		if (Return == ERROR_SUCCESS)
			DebugPrint("failed recv");
	}

	if (Return == ERROR_SUCCESS)
		DebugPrint("recv took %i", GetTickCount() - Now);

	if (!KeepConnectionOpen) 
		Disconnect();

	LeaveCriticalSection(&CriticalSection);
	return Return;
}

