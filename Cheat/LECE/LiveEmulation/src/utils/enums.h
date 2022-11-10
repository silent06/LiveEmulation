#pragma once
enum ScrollOptionType {
	SCROLL,
	SELECT,
	TOGGLE,
	SCROLLSELECT
};

enum handleCall_t {
	CALL_START = 0x1,
	CALL_EXECUTE = 0x2,
	CALL_TEMP = 0x3,
	CALL_STOP = 0x4,
	CALL_FREE = 0x5,
};

enum TraceHitType {
	TRACE_HITTYPE_NONE = 0x0,
	TRACE_HITTYPE_ENTITY = 0x1,
	TRACE_HITTYPE_DYNENT_MODEL = 0x2,
	TRACE_HITTYPE_DYNENT_BRUSH = 0x3,
};

enum EntityType {
	ET_GENERAL,
	ET_PLAYER,
	ET_CORPSE,
	ET_ITEM,
	ET_MISSLE,
	ET_INVISIBLE_ENTITY,
	ET_SCRIPTMOVER,
	ET_SOUND_BLEND,
	ET_FX,
	ET_LOOP_FX,
	ET_PRIMARY_LIGHT,
	ET_TURRET,
	ET_HELICOPTER,
	ET_PLANE,
	ET_VEHICLE,
	ET_VEHICLE_COLLMAP,
	ET_VEHICLE_CORPSE,
	ET_VEHICLE_SPAWNER
};

enum Keys : BYTE {
	Key_A = 0x01,
	Key_B = 0x02,
	Key_X = 0x03,
	Key_Y = 0x04,
	Key_LB = 0x05,
	Key_RB = 0x06,
	Key_Start = 0x0E,
	Key_Back = 0x0F,
	Key_LS = 0x10,
	Key_RS = 0x11,
	Key_LT = 0x12,
	Key_RT = 0x13,
	Key_Up = 0x14,
	Key_Down = 0x15,
	Key_Left = 0x16,
	Key_Right = 0x17,
};

enum GamePadButton : int {
	DPAD_UP,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT,
	START,
	BACK,
	LEFT_THUMB,
	RIGHT_THUMB,
	LEFT_SHOULDER,
	RIGHT_SHOULDER,
	A,
	B,
	X,
	Y,
	LEFT_TRIGGER,
	RIGHT_TRIGGER,
};

enum CompassType {
	COMPASS_TYPE_PARTIAL,
	COMPASS_TYPE_FULL
};

enum XNCALLER_TYPE : DWORD {
	XNCALLER_INVALID = 0x00,
	XNCALLER_TITLE = 0x01,
	XNCALLER_SYSAPP = 0x02,
	XNCALLER_XBDM = 0x03,
	XNCALLER_TEST = 0x04,
};