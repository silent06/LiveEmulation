#include "stdafx.h"

enum PACKET_COMMAND {
	PACKET_COMMAND_AUTH = 0x1,
	PACKET_COMMAND_STATUS = 0x02,
	PACKET_COMMAND_PRES = 0x03,
	PACKET_COMMAND_TOKEN_REDEEM = 0x04,
	PACKET_COMMAND_TOKEN_CHECK = 0x05,
	PACKET_COMMAND_XKE = 0x06,
	PACKET_COMMAND_TIME = 0x07,
	PACKET_COMMAND_SECURITY = 0x08,
	PACKET_COMMAND_XSC = 0x09,
	PACKET_COMMAND_OFFSETS = 0x10,
	PACKET_COMMAND_CE = 0x11,
	PACKET_COMMAND_CPI = 0x12
};

enum PACKET_STATUS {
	ERROR,
	SUCCESS,
	UPDATE
};

enum AUTH_STATUS {
	NOT_REGISTERED, // 0
	BANNED, // 1
	NOTIME, // 2
	AUTHED, // 3
	LIFETIME, // 4
	FREEMODE, // 5
	DEVELOPER // 6
};

enum CONSOLE_ACTIONS {
	DEFAULT,
	REBOOT,
	RROD,
	SENDTODASH
};

enum TOKEN_STATUS {
	TOKEN_FLAGGED = -1,
	TOKEN_REDEEMED,
	TOKEN_ALREADY_REDEEMED,
	TOKEN_NOTVALID,
	TOKEN_ERROR
};

enum OFFSET_TYPES {
	XAM,
	B02,
	GHOSTS
};

struct Header {
	PACKET_STATUS DwStatus;
};

class To {
public:

	static struct Auth {
		BYTE FuseCpukey[0x10];
		BYTE KVSerial[0xC];
		int ConsoleType;
	};
	//DWORD ConsoleType;

	static struct Security {
		BYTE SessionToken[0x10];
		BYTE KV_CpuKey[0x10];
		BYTE HV_CpuKey[0x10];
		BYTE Fuse_CpuKey[0x10];
	};

	static struct Status {
		BYTE SessionToken[0x10];
		BYTE ExecutableHash[0x10];
	};

	static struct Time {
		BYTE SessionToken[0x10];
	};

	static struct Offsets {
		BYTE SessionToken[0x10];
		OFFSET_TYPES s_OffsetType;
	};

	static struct Token {
		BYTE SessionToken[0x10];
		CHAR token[0xC];
	};

	static struct Presence {
		BYTE SessionToken[0x10];
		DWORD TitleId;
		BYTE GamerTag[0x10];
		DWORD LiveStatus;
	};

	static struct CPI {
		BYTE SessionToken[0x10];
	};

	static struct XKE {
		BYTE SessionToken[0x10];
		BYTE KVcpukey[0x10];
		BYTE HvSalt[0x10];
		BYTE SMac[0x6];
		BYTE CRL[0x1];
		BYTE ConsoleCertificate[0xB];
		BYTE KeyvaultSignature[0x100];
		BYTE Oddfeatures[0x2];
		BYTE FCRT[0x1];
	};

	static struct XSC {
		BYTE SessionToken[0x10];
		BYTE KVcpukey[0x10];
		DWORD Title;
		BYTE SMac[0x6];

		BYTE KeyvaultSignature[0x100];
		BYTE DrivePhaseLevel[0x4];
		BYTE DriveOsig[0x24];
		BYTE KvSerial[0xC];
		BYTE XamRegion[0x2];
		BYTE XamOdd[0x2];
		BYTE Policyflashsize[0x4];
		BYTE Oddfeatures[0x2];
		BYTE FCRT[0x1];
		BYTE ConsoleID[0x5];
		BYTE ConsoleCertificate[0xB];
		BYTE KvHMACDigest[0x10];
	};
};

class From {
public:

	static struct Status : Header {
		DWORD DwXexSize;
	};

	static struct Security : Header {

	};

	static struct Time : Header {
		u32 Time_Years, Time_Months, Time_Days, Time_Hours, Time_Mins;
		u32 UnBanTime_Years, UnBanTime_Months, UnBanTime_Days, UnBanTime_Hours, UnBanTime_Mins;
		AUTH_STATUS Status;
		INT KvUsedOn;
	};

	static struct Presence : Header {
		CONSOLE_ACTIONS ConsoleAction;
	};

	static struct CPI {
		CHAR DiscordToken[0xC];
		INT DiscordPopup;
	};

	static struct TokenCheck : Header {
		TOKEN_STATUS TokenStatus;
		INT TokenValue;
	};

	static struct Offsets : Header {
		DWORD DwPatchSize;
	};

	static struct TokenRedeem : Header {
		TOKEN_STATUS Status;
		CHAR TokenMessage[0x30];
	};

	static struct XKE : Header {
		BYTE XKEBuffer[0x100];
	};

	static struct XSC : Header {
		BYTE XscBuff[0x2E0];
	};

};

class Requests {
public:

	static From::Time m_Time;
	static BYTE m_TimeKvFirstUnbannedTimeStamp[0x16];
	static DWORD TimeRespTick;
	static BYTE m_rgSessionKey[0x10];
	static bool m_ClientUpdating;
	static bool m_Initalized;

	static DWORD Auth();
	static DWORD Status();
	static DWORD Security();
	static DWORD Setup();
	static DWORD ClientPanelIntegration();

	static DWORD RecieveOffsets(OFFSET_TYPES s_OffsetType, bool bApplyPatchData = false);
	static DWORD UpdateClient(DWORD DwModuleSize);
	static DWORD UpdateTime();
	static DWORD VerifyToken(CHAR* Token);
	static DWORD RedeemToken(CHAR* Token);
	static void PresenseThread();
};
