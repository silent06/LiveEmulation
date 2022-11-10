#pragma once
#include <unordered_map>

enum Packets {
	PACKET_WELCOME = 1,
	PACKET_HEARTBEAT,
	PACKET_STATUS,
	PACKET_LEAVE,
	PACKET_CHALLENGE,
	PACKET_VERSION,
	PACKET_GETDATA,
	PACKET_GETFUNC,
	PACKET_GETWELCOMEMSG,
	PACKET_GETCHANGELOG,
	PACKET_GETNATIVES,
	PACKET_PRESENCE
};

namespace AuthData {
	enum eFunctions {
		HookFunctionPatch
	};

	enum eNatives {
		NATIVE_GetCG,
		NATIVE_GetCEntity,
		NATIVE_GetClientInfo,
		NATIVE_GetServerInfo,
		NATIVE_GetClientActive,
		NATIVE_GetXenonUserData,
		NATIVE_GetUIContext,
		NATIVE_GetPingData,
		NATIVE_AimAssist_GetTagPos,
		NATIVE_CG_RegisterTag,
		NATIVE_R_RegisterMaterial,
		NATIVE_R_RegisterFont,
		NATIVE_R_AddCmdDrawStretchPic,
		NATIVE_R_AddCmdDrawText,
		NATIVE_R_TextWidth,
		NATIVE_Dvar_GetBool,
		NATIVE_Dvar_GetString,
		NATIVE_Cbuf_AddText,
		NATIVE_CG_DrawRotatedPicPhysical,
		NATIVE_Com_GetClientDObj,
		NATIVE_CG_DObjGetWorldTagPos,
		NATIVE_Key_IsDown,
		NATIVE_CG_CanSeePlayer,
		NATIVE_GetTagPosition,
		NATIVE_CG_WorldPosToCompass,
		NATIVE_CG_DrawGrenadePointer,
		NATIVE_CG_DrawGrenadeIcon,
		NATIVE_CG_TraceIgnoreSetup,
		NATIVE_CG_TraceIgnoreSetup2,
		NATIVE_CG_Trace,
		NATIVE_CG_LocationalTrace,
		NATIVE_BulletTrace,
		NATIVE_FireBulletPenetrate,
		NATIVE_Trace_GetEntityHitId,
		NATIVE_RandomBulletDir,
		NATIVE_BG_SeedRandWithGameTime,
		NATIVE_BG_GetSpreadForWeapon,
		NATIVE_SL_GetString,
		NATIVE_SL_GetString_0,
		NATIVE_CG_GetPlayerViewOrigin,
		NATIVE_R_TextHeight,
		NATIVE_Key_GetBindingForCmd,
		NATIVE_CL_ExecuteKey,
		NATIVE_CG_GetRankIcon,
		NATIVE_CG_CompassDrawPlayerMap,
		NATIVE_CG_CompassDrawPlayer,
		NATIVE_CG_CompassDrawCrates,
		NATIVE_CG_CompassDrawPlanes,
		NATIVE_CG_CompassDrawFriendlies,
		NATIVE_CG_CompassDrawEnemies,
		NATIVE_CG_CompassDrawMissiles,
		NATIVE_CG_CompassDrawHelicopter,
		NATIVE_CG_CompassDrawTurrets,
		NATIVE_BG_GetWeaponName,
		NATIVE_CG_BulletEndPos
	};

	enum eAddresses {
		Menu_PaintAll,
		R_AddCmdDrawStretchPic,
		ClientThink_Real,
		SV_DropClient,
		UI_SafeTranslateString,
		CL_WritePacket,
		Dvar_GetUnpackedColorByName,
		R_SetFrameFog,
		R_RegisterMaterial,
		R_RegisterFont,
		R_AddCmdDrawText,
		R_TextWidth,
		Dvar_GetBool,
		Dvar_GetString,
		Cbuf_AddText,
		CG_DrawRotatedPicPhysical,
		CG_DObjGetWorldTagPos,
		Key_IsDown,
		CG_CanSeePlayer,
		Com_GetClientDObj,
		SL_GetString,
		SL_GetString_0,
		CG_RegisterTag,
		AimAssist_GetTagPos,
		CG_WorldPosToCompass,
		CG_DrawGrenadePointer,
		CG_DrawGrenadeIcon,
		CG_Trace,
		CG_TraceIgnoreSetup,
		CG_TraceIgnoreSetup2,
		CG_LocationalTrace,
		BulletTrace,
		FireBulletPenetrate,
		Trace_GetEntityHitId,
		RandomBulletDir,
		BG_SeedRandWithGameTime,
		BG_GetSpreadForWeapon,
		CG_GetPlayerViewOrigin,
		Key_GetBindingForCmd,
		CL_ExecuteKey,
		CG_GetRankIcon,
		CUSTOM_GetPingData,
		CUSTOM_GetUIContext,
		CUSTOM_GetXenonUserData,
		CUSTOM_GetClientActive,
		CUSTOM_GetServerInfo,
		CUSTOM_GetCEntity,
		CUSTOM_GetCG,
		MISC_CmdMr,
		PATCH_NoRecoil,
		PATCH_Laser,
		PATCH_ConstantRadar,
		PATCH_CG_Flashbanged,
		PATCH_CG_DrawFlashDamage,
		PATCH_Flashbang,
		PATCH_CL_CapTurnRate,
		PATCH_CG_StartShellShock,
		PATCH_CG_UpdateShellShock,
		PATCH_NoSway1,
		PATCH_NoSway2,
		PATCH_NoFlinch,
		DVAR_HearEveryone1,
		DVAR_HearEveryone2,
		DVAR_ThirdPerson,
		DVAR_ThirdPersonRange,
		DVAR_FOV,
		PATCH_AutoWall1,
		PATCH_AutoWall2,
		PATCH_AutoWall3,
		PATCH_AutoWall4,
		PATCH_VisualESPName,
		DVAR_cg_hudGrenadeIconOffset,
		DVAR_cg_hudGrenadeIconWidth,
		DVAR_cg_hudGrenadeIconHeight,
		DVAR_cg_hudGrenadePointerWidth,
		DVAR_cg_hudGrenadePointerHeight,
		CG_CompassDrawPlayerMap,
		CG_CompassDrawPlayer,
		CG_CompassDrawCrates,
		CG_CompassDrawPlanes,
		CG_CompassDrawFriendlies,
		CG_CompassDrawEnemies,
		CG_CompassDrawTurrets,
		CG_CompassDrawMissiles,
		CG_CompassDrawHelicopter,
		PATCH_CompassOverdrawn,
		PATCH_CompassScanLineBorder,
		PATCH_CG_CompassDrawTickertape1,
		PATCH_CG_CompassDrawTickertape2,
		PATCH_CG_CompassDrawPlayerMap,
		PATCH_CG_CompassDrawPlayer,
		PATCH_CG_CompassDrawCrates,
		PATCH_CG_CompassDrawFriendlies,
		PATCH_CG_CompassDrawEnemies,
		PATCH_CG_CompassDrawTurrets,
		PATCH_CG_CompassDrawMissiles,
		PATCH_CG_CompassDrawHelicopter,
		DVAR_CompassMaxRange,
		XamInputGetState,
		BG_GetWeaponName,
		CG_BulletEndPos,
		CG_Respawn,
	};

	extern map<eAddresses, DWORD> g_addresses;
	extern map<eNatives, DWORD> g_natives;
}

namespace Request {
	extern byte token[0x20];
	extern byte requestCount;
#pragma pack(push)
#pragma pack(1)
	struct Header {
		Packets m_packet;
		int m_size;
		int m_title;
		byte m_CPU[0x10];
		byte m_hvCPU[0x10];
		byte m_token[0x20];

		// Encryption
		int m_key1;
		int m_key2;
		int m_key3;
		int m_key4;
		int m_key5;
		int m_key6;
		int m_key7;
		int m_key8;
		int m_key9;
		int m_key10;
		int m_hash;
	};

	struct ServerPacketVersion : Header {
		DWORD m_version;
	};

	struct ServerPacketGetFunc : Header {
		AuthData::eFunctions m_index;
	};

	struct ServerPacketPresence : Header {
		bool m_isLiveEmulationServerRunning;
		BYTE m_totalConnections;
		BYTE m_functionCountToCheckForBLR;
		BYTE m_functionsToCheckForBLR[15][4];
	};
#pragma pack(pop)
}

namespace Response {
#pragma pack(push)
#pragma pack(1)
	struct GenericResponse {
		bool m_successful;
		char m_error[0x100];
	};

	struct WelcomePacketResponse {
		bool m_successful;
		char m_error[0x100];
		BYTE m_token[0x20];
	};

	struct GetWelcomeMessage {
		bool m_isMessage;
		char m_message[0x300];
	};

	struct GenericGet {
		bool m_successful;
		byte m_count;
		byte m_data[0x250];
	};
#pragma pack(pop)
}

class Requests {
public:
	static bool ServerPacketWelcome();
	static void ServerPacketLeave();
	static bool ServerPacketStatus();
	static bool ServerPacketVersion();
	static bool ServerPacketGetData();
	static bool ServerPacketGetFunc();
	static bool ServerPacketGetNatives();
	static void ServerPacketWelcomeMessage();
	static void ServerPacketChangelog();
	static bool ServerPacketPresence();
	static void PresenceThread();
};