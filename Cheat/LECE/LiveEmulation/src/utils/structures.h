#pragma once

#define HANDLE_COUNT		0x100
#define INVALID_HANDLE		((handle_t)-1)

#define TIME_NOW			clock()
#define TIME_NOW_OFF(x)		(clock() + (x * (CLOCKS_PER_SEC / 1000)))

typedef int handle_t;
typedef void(*handleCallback)(int arg, handle_t* handle, handleCall_t type);

struct handle_s {
	int arg;
	handleCallback function;
	handleCall_t type;
	handle_t* handle;
	clock_t interval;
	clock_t time;
	clock_t startTime;
};

template<typename Type>
struct ScrollStruct {
	const char* m_name;
	Type m_result;
};

template<typename f, typename s>
struct _TypeFirstSecond {
	f first;
	s second;
};

struct ClientPingData {
	BYTE m_active;
	char _0x0001[3];
	int m_ping;
	int m_nextPingTime;
};

struct _XenonUserData {
	int m_signInState;
	char m_name[0x20];
	char unk1[0x4];
	__int64 m_xuid;
	char m_xuidString[0x10];
	char unk2[0x28];
};

struct UIContext {
	char _0x0000[0x2C];
	float m_fps;
};

struct RectDef {
	float x;
	float y;
	float w;
	float h;
	int horzAlign;
	int vertAlign;
};

struct Material {
	const char* m_name;
};

struct Font_s {
	const char* m_fontName;
	int m_pixelHeight;
	int m_glyphCount;
	Material* m_material;
	Material* m_glowMaterial;
	void* m_glyph;
};

struct Vector2 {
	float x;
	float y;

	Vector2() {};

	Vector2(float _x, float _y)
		: x(_x), y(_y)
	{}

	bool operator!= (Vector2& in) {
		return in.x != x && in.y != y;
	}
};

struct RefDef {
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	Vector2 m_fov;
	D3DXVECTOR3 m_origin;
	D3DXVECTOR3 m_viewAxis[3];
	char _0x0048[0x24];
	float m_zoomProgress;
	char _0x0070[0x4AC0];
};

struct ClientInfo {
	int m_valid;						//0x0
	char _0x0004[0x4];				//0x4
	int m_clientNum;
	char m_name[0x20];				//0xC
	int m_team;						//0x2C,
	int m_oldTeam;
	int m_rank;
	int m_prestige;
	char _0x003C[0x8];
	int m_perks[2];
	char m_clanTag[8];
	int m_score;
	int m_location;
	int m_health;
	char m_model[0x40];
	char m_headModel[0x40];
	char _0x00E0[0x3D0];
	int m_attacking;
	char _0x04B4[0x4];
	int m_zooming;
	char _0x04BC[0x60];
	int m_weapon;
	char _0x0520[0x74];

	int getAddress() {
		return (int)this;
	}
};

struct CgArray {
	int m_commandtime;                //0x0
	char padding00[0x8];            //0x4
	int m_flags;					//0xC
	char padding01[0xC];            //0x10
	D3DXVECTOR3 m_origin;                  //0x1C
	char padding02[0x128];          //0x28
	int m_clientNum;					//0x150
	char padding03[0x4];            //0x154
	D3DXVECTOR3 m_viewAngles;				//0x158
	char padding04[0x38];			//0x164
	int m_health;                     //0x19C   
	char padding08[0x4];			//0x1A0
	int m_maxHealth;					//0x1A4
	char padding05[0x1D4];          //0x1A8
	float m_aimSpreadScale;           //0x37C
	char padding06[0x6B72C];        //0x380
	RefDef m_refdef;              //0x6BAAC
	D3DXVECTOR3 m_refdefViewAngles;
	char _0x705E8[0x746C];
	float m_compassNorth[2];
	char _0x77A54[0x8652C];
	ClientInfo m_clientInfo[18];
};

struct ServerInfo {
	int m_x;
	int m_y;
	int m_w;
	int m_h;

	char _0x0000[20];				//0x0
	//char _0x0000[36];				//0x0
	char m_gametype[0x20];			//0x24
	char m_hostName[0x20];			//0x44 
	char _0x0064[0xE4];				//0x64
	int m_maxClients;				//0x148 
	int m_privateClients;			//0x14C 
	char _0x0150[0x8];				//0x150
	char m_mapname[0x34];			//0x158
};

struct XModel {
	char* m_name;
};

struct DObj_s {
	char _0x0000[0x98];
	XModel** m_models;
};

struct CEntity {
	char _0x0000[0x2];				//0x0
	char m_alive;						//0x2 
	char _0x0003[0x11];				//0x4
	D3DXVECTOR3 m_origin;					//0x14 
	D3DXVECTOR3 m_angles;					//0x20 
	char _0x002C[0x30];				//0x2C
	int m_flags;						//0x5C 
	char _0x0060[0x64];				//0x60
	int m_handle;						//0xC4
	EntityType m_type;				//0xC8
	char _0x00CC[0x88];				//0xCC
	int m_clientNumber;				//0x154 
	char _0x0158[0x34];				//0x158
	int m_weaponIndex;				//0x18C 
	char _0x0190[0x5C];				//0x190
};

struct WeaponDef {
	const char* m_weaponName;
	char _0x4[4];
	const char* m_overlayName;
	char _0xC[120];
	Material* m_killIcon;
};

struct WeaponVariant {
	char _0x0008[8];
	WeaponDef* m_weaponDef;
};

struct UserCommand {
	int m_time;
	unsigned int m_button;
	int m_viewAngles[3];
	int m_weapon;
	int m_offHand;
	signed char m_forwardmove;
	signed char m_rightmove;
	unsigned short m_airburstMarkDistance;
	unsigned short m_meleeChargeEnt;
	char m_meleeChargeDist;
	char m_selectedLoc[2];
	char m_selectedLocAngle;
	unsigned short m_sightedClientsMask;
	unsigned short m_spawnTraceEntIndex;
};

struct ClientActive {
	bool m_usingAds;
	char padding00[0xDF];
	float m_baseAngles[2];
	char paddig00[0x34C8];
	float m_viewAngles[2];
	char padding01[0x4];
	UserCommand m_usercmd[128];
	int m_curCmd;

	UserCommand* GetCommand(int cmdNum) {
		return &m_usercmd[cmdNum & 0x7F];
	}
};

struct BulletFireParams {
	int m_maxEntityNumber;
	int m_ignoreEntIndex;
	float m_damageMultiplier;
	int m_methodOfDeath;
	int m_unk;
	D3DXVECTOR3 m_originalStart;
	D3DXVECTOR3 m_start;
	D3DXVECTOR3 m_end;
	D3DXVECTOR3 m_dir;
};

struct Trace {
	float m_fraction;
	D3DXVECTOR3 m_normal;
	int m_surfaceFlags;
	int m_contents;
	Material* m_material;
	TraceHitType m_hitType;
	unsigned short m_hitId;
	unsigned short m_modelIndex;
	unsigned short m_partName;
	unsigned short m_partGroup;
	bool m_allsolid;
	bool m_startsolid;
	bool m_walkable;
	char _0x002B[5];
	D3DXVECTOR3 m_endpos;
	char _0x003C[4];
	unsigned long m_materialType;
	char _0x0044[0x20];
};

struct BulletTraceResults {
	Trace m_trace;
	int m_hitEnt;
	D3DXVECTOR3 m_hitPos;
	bool m_ignoreHitEnt;
	int m_surfaceType;
};

struct ImageLoadArguments {
	HMODULE m_kernelModule;
	HMODULE m_launchModule;
	HMODULE m_xamModule;

	char m_drive[10];
	char m_gameString[50];
};