#include "stdafx.h"

class Networking {
public:
	static void DecryptNetworking();
	static void Disconnect();
	static bool m_Initalized;
	static bool Connected;
	static SOCKET sSocket;
	static DWORD Connect();
	static DWORD Recv(void* Data, DWORD BytesExpected);
	static DWORD Send(PACKET_COMMAND CommandID, void* Request, DWORD RequestLen, void* Responce, DWORD ResponceLength, bool KeepConnectionOpen = FALSE, bool ShouldRecieve = TRUE);

private:
	static BYTE rgSessionKeySalt[];
	static BYTE rgSessionKeySaltSeed[];
	static BYTE rgSessionKey[];
	static BYTE PrimaryDec[];

	static INT XorMagic(INT Key);
	static void BuildNetworkingHeader(BYTE* Data, DWORD DataLen);
	static void XorMagic(BYTE* Key);
	static DWORD StartupServerCommunicator();
	static DWORD Send(PACKET_COMMAND CommandID, void* Data, DWORD DataLen);
};
