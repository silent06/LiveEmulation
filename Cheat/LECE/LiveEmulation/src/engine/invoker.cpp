#include "stdafx.h"

NativeRegistration Invoker::g_natives[256];
DWORD Invoker::m_invokingAddress;

void Invoker::RegisterNative(DWORD NativeAddress, NativeHandler Handler) {
	NativeRegistration* Registration = &g_natives[NativeAddress & 0xFF];
	if (Registration != nullptr) {
		while (Registration->m_next != nullptr) {
			Registration = Registration->m_next;
		}

		if (Registration->m_count == 9) {
			Registration->m_next = (NativeRegistration*)malloc(sizeof(NativeRegistration));
			Registration = Registration->m_next;
		}

		Registration->m_address[Registration->m_count] = NativeAddress;
		Registration->m_function[Registration->m_count] = Handler;
		Registration->m_count++;

		GetIntegrityManager()->Add((void*)Handler, 4);
	}
}

NativeHandler Invoker::GetNativeHandler(DWORD NativeAddress){
	NativeHandler Handler = nullptr;
	NativeRegistration* Registration = &g_natives[NativeAddress & 0xFF];

	if (Registration != nullptr) {
		for (; Registration; Registration = Registration->m_next) {
			for (int i = 0; i < Registration->m_count; i++) {
				if (NativeAddress == Registration->m_address[i]) {
					Handler = Registration->m_function[i];
					break;
				}
			}
		}
	}

	return Handler;
}

namespace NativeHandlers {
	// Custom
	void GetCG(NativeCallContext* Context) {
		CgArray* cg = *(CgArray**)AuthData::g_addresses[AuthData::CUSTOM_GetCG];
		Context->SetResult(0, cg);
	}

	void GetCEntity(NativeCallContext* Context) {
		CEntity* ent = *(CEntity**)AuthData::g_addresses[AuthData::CUSTOM_GetCEntity];
		if (ent != nullptr) {
			Context->SetResult(0, ent);
			return;
		}
		Context->SetResult(0, nullptr);
	}

	void GetClientInfo(NativeCallContext* Context) {
		int index = Context->GetArgument<int>(0);

		CgArray* cg = Native::Custom::GetCG();
		if (cg != nullptr) {
			if (cg->m_clientInfo) {
				Context->SetResult(0, cg->m_clientInfo[index]);
				return;
			}
		}

		Context->SetResult(0, ClientInfo());
	}

	void GetServerInfo(NativeCallContext* Context) {
		ServerInfo* server = *(ServerInfo**)AuthData::g_addresses[AuthData::CUSTOM_GetServerInfo];
		Context->SetResult(0, server);
	}

	void GetClientActive(NativeCallContext* Context) {
		ClientActive* active = *(ClientActive**)AuthData::g_addresses[AuthData::CUSTOM_GetClientActive];
		Context->SetResult(0, active);
	}

	void GetXenonUserData(NativeCallContext* Context) {
		int index = Context->GetArgument<int>(0);
		_XenonUserData* XenonUserData = (_XenonUserData*)AuthData::g_addresses[AuthData::CUSTOM_GetXenonUserData];
		Context->SetResult(0, XenonUserData[index]);
	}

	void GetUIContext(NativeCallContext* Context) {
		Context->SetResult(0, Backend::m_uiContext);
	}

	void GetPingData(NativeCallContext* Context) {
		ClientPingData* clientPingData = (ClientPingData*)AuthData::g_addresses[AuthData::CUSTOM_GetPingData];
		Context->SetResult(0, clientPingData);
	}

	// Engine
	void R_RegisterMaterial(NativeCallContext* Context) {
		const char* Font = Context->GetArgument<const char*>(0);
		int ImageTrack = Context->GetArgument<int>(1);

		Material* Result = ((Material*(*)(const char*, int))AuthData::g_addresses[AuthData::R_RegisterMaterial])(Font, ImageTrack);
		Context->SetResult(0, Result);
	}

	void R_RegisterFont(NativeCallContext* Context) {
		char* Font = Context->GetArgument<char*>(0);
		int ImageTrack = Context->GetArgument<int>(1);
		
		Font_s* Result = ((Font_s*(*)(char*, int))AuthData::g_addresses[AuthData::R_RegisterFont])(Font, ImageTrack);
		Context->SetResult(0, Result);
	}

	void R_AddCmdDrawStretchPic(NativeCallContext* Context) {
		Vector2 pos; // x, y
		pos.x = Context->GetArgument<float>(0);
		pos.y = Context->GetArgument<float>(1);

		Vector2 scaleA; // width, height
		scaleA.x = Context->GetArgument<float>(2);
		scaleA.y = Context->GetArgument<float>(3);

		Vector2 scaleB; // xscale, yscale
		scaleB.x = Context->GetArgument<float>(4);
		scaleB.y = Context->GetArgument<float>(5);

		Vector2 scaleC; //xay, yay
		scaleC.x = Context->GetArgument<float>(6);
		scaleC.y = Context->GetArgument<float>(7);

		const float* color = Context->GetArgument<float*>(8);
		Material* material = Context->GetArgument<Material*>(9);

		void* Return = ((void*(*)(float, float, float, float, float, float, float, float, const float*, Material*))AuthData::g_addresses[AuthData::R_AddCmdDrawStretchPic])(pos.x, pos.y, scaleA.x, scaleA.y, scaleB.x, scaleB.y, scaleC.x, scaleC.y, color, material);
		Context->SetResult(0, Return);
	}

	void R_AddCmdDrawText(NativeCallContext* Context) {
		const char* text = Context->GetArgument<const char*>(0);
		int maxChars = Context->GetArgument<int>(1);
		Font_s* font = Context->GetArgument<Font_s*>(2);

		float posX = Context->GetArgument<float>(3);
		float posY = Context->GetArgument<float>(4);

		float scaleX = Context->GetArgument<float>(5);
		float scaleY = Context->GetArgument<float>(6);

		float rotation = Context->GetArgument<float>(7);
		const float* color = Context->GetArgument<const float*>(8);
		int style = Context->GetArgument<int>(9);

		if (text == nullptr || text == "") {
			text = "--";
		}

		void* Return = ((void*(*)(const char*, int, Font_s*, float, float, float, float, float, const float*, int))AuthData::g_addresses[AuthData::R_AddCmdDrawText])(text, maxChars, font, posX, posY, scaleX, scaleY, rotation, color, style);
		Context->SetResult(0, Return);
	}

	void R_TextWidth(NativeCallContext* Context) {
		const char* text = Context->GetArgument<const char*>(0);
		Font_s* font = Context->GetArgument<Font_s*>(1);

		if (text && text != nullptr) {
			int Return = ((int(*)(const char*, int, Font_s*))AuthData::g_addresses[AuthData::R_TextWidth])(text, 0x7FFFFFFF, font);
			Context->SetResult(0, Return);
			return;
		}

		Context->SetResult(0, 0);
	}

	void Dvar_GetBool(NativeCallContext* Context) {
		const char* dvar = Context->GetArgument<const char*>(0);

		bool Return = ((bool(*)(const char*))AuthData::g_addresses[AuthData::Dvar_GetBool])(dvar);
		Context->SetResult(0, Return);
	}

	void Dvar_GetString(NativeCallContext* Context) {
		const char* dvar = Context->GetArgument<const char*>(0);
		
		const char* Return = ((const char*(*)(const char*))AuthData::g_addresses[AuthData::Dvar_GetString])(dvar);
		Context->SetResult(0, Return);
	}

	void Cbuf_AddText(NativeCallContext* Context) {
		int localPlayer = Context->GetArgument<int>(0);
		const char* command = Context->GetArgument<const char*>(1);

		void* Result = ((void*(*)(int, const char*))AuthData::g_addresses[AuthData::Cbuf_AddText])(localPlayer, command);
		Context->SetResult(0, Result);
	}

	void CG_DrawRotatedPicPhysical(NativeCallContext* Context) {
		int scrPlacement = Context->GetArgument<int>(0);
		
		Vector2 pos; // x, y
		pos.x = Context->GetArgument<float>(1);
		pos.y = Context->GetArgument<float>(2);

		Vector2 scale; // width, height
		scale.x = Context->GetArgument<float>(3);
		scale.y = Context->GetArgument<float>(4);

		float angle = Context->GetArgument<float>(5);
		const float* color = Context->GetArgument<const float*>(6);
		Material* material = Context->GetArgument<Material*>(7);

		void* Return = ((void*(*)(int, float, float, float, float, float, const float *, Material*))AuthData::g_addresses[AuthData::CG_DrawRotatedPicPhysical])(scrPlacement, pos.x, pos.y, scale.x, scale.y, angle, color, material);
		Context->SetResult(0, Return);
	}

	void CG_DObjGetWorldTagPos(NativeCallContext* Context) {
		CEntity* entity = Context->GetArgument<CEntity*>(0);
		int dObj = Context->GetArgument<int>(1);
		unsigned int tag = Context->GetArgument<unsigned int>(2);
		D3DXVECTOR3* out = Context->GetArgument<D3DXVECTOR3*>(3);

		bool Return = ((bool(*)(CEntity*, int, unsigned int, D3DXVECTOR3*))AuthData::g_addresses[AuthData::CG_DObjGetWorldTagPos])(entity, dObj, tag, out);
		Context->SetResult(0, Return);
	}

	void Key_IsDown(NativeCallContext* Context) {
		int localClient = Context->GetArgument<int>(0);
		int key = Context->GetArgument<int>(1);

		int Return = ((int(*)(int, int))AuthData::g_addresses[AuthData::Key_IsDown])(localClient, key);
		Context->SetResult(0, Return);
	}

	void CG_CanSeePlayer(NativeCallContext* Context) {
		int client = Context->GetArgument<int>(0);
		CEntity* entity = Context->GetArgument<CEntity*>(1);

		int contestMask = 0x803003; // 0x2807823, 0x2807821

		bool Return = ((bool(*)(int, CEntity*, int))AuthData::g_addresses[AuthData::CG_CanSeePlayer])(client, entity, contestMask);
		Context->SetResult(0, Return);
	}

	void Com_GetClientDObj(NativeCallContext* Context) {
		int handle = Context->GetArgument<int>(0);
		int clientNum = Context->GetArgument<int>(1);

		int Return = ((int(*)(int, int))AuthData::g_addresses[AuthData::Com_GetClientDObj])(handle, clientNum);
		Context->SetResult(0, Return);
	}

	void SL_GetString(NativeCallContext* Context) {
		const char* str = Context->GetArgument<const char*>(0);

		unsigned int Return = ((unsigned int(*)(const char*, int))AuthData::g_addresses[AuthData::SL_GetString])(str, 0);
		Context->SetResult(0, Return);
	}

	void SL_GetString_0(NativeCallContext* Context) {
		const char* str = Context->GetArgument<const char*>(0);

		unsigned int Return = ((unsigned int(*)(const char*))AuthData::g_addresses[AuthData::SL_GetString_0])(str);
		Context->SetResult(0, Return);
	}

	void GetTagPosition(NativeCallContext* Context) {
		int client = Context->GetArgument<int>(0);
		const char* tag = Context->GetArgument<const char*>(1);

		CEntity entity = Native::Custom::GetCEntity()[client];

		D3DXVECTOR3 OutPut;

		unsigned int _tag = Native::Engine::SL_GetString(tag);

		int dobj = Native::Engine::Com_GetClientDObj(entity.m_handle, 0);
		if (!dobj) {
			Context->SetResult(0, D3DXVECTOR3());
			return;
		}

		bool Return = Native::Engine::CG_DObjGetWorldTagPos(&entity, dobj, _tag, &OutPut);
		if(Return)
			Context->SetResult(0, OutPut);
		else
			Context->SetResult(0, D3DXVECTOR3());
	}

	void CG_RegisterTag(NativeCallContext* Context) {
		const char* tag = Context->GetArgument<const char*>(0);

		int Return = ((int(*)(const char*))AuthData::g_addresses[AuthData::CG_RegisterTag])(tag);
		Context->SetResult(0, Return);
	}

	void AimAssist_GetTagPos(NativeCallContext* Context) {
		int mask = Context->GetArgument<int>(0);
		CEntity* entity = Context->GetArgument<CEntity*>(1);
		short tag = Context->GetArgument<short>(2);
		D3DXVECTOR3* out = Context->GetArgument<D3DXVECTOR3*>(3);

		int Return = ((int(*)(int, CEntity*, short, D3DXVECTOR3*))AuthData::g_addresses[AuthData::AimAssist_GetTagPos])(mask, entity, tag, out);
		Context->SetResult(0, Return);
	}

	void CG_WorldPosToCompass(NativeCallContext* Context) {
		CompassType compassType = Context->GetArgument<CompassType>(0);
		CgArray* cg = Context->GetArgument<CgArray*>(1);
		RectDef* mapRect = Context->GetArgument<RectDef*>(2);
		float* north = Context->GetArgument<float*>(3);
		float* world = Context->GetArgument<float*>(4);
		D3DXVECTOR3 origin = Context->GetArgument<D3DXVECTOR3>(5);
		float* out = Context->GetArgument<float*>(6);
		float* outClipped = Context->GetArgument<float*>(7);

		bool Return = ((bool(*)(CompassType, CgArray*, RectDef*, float*, float*, D3DXVECTOR3, float*, float*))AuthData::g_addresses[AuthData::CG_WorldPosToCompass])(compassType, cg, mapRect, north, world, origin, out, outClipped);
		Context->SetResult(0, Return);
	}

	void CG_DrawGrenadePointer(NativeCallContext* Context) {
		int localClientNum = Context->GetArgument<int>(0);
		float centerX = Context->GetArgument<float>(1);
		float centerY = Context->GetArgument<float>(2);
		D3DXVECTOR3* grenadeOffset = Context->GetArgument<D3DXVECTOR3*>(3);
		float* color = Context->GetArgument<float*>(4);

		((void(*)(int, float, float, D3DXVECTOR3*, float*))AuthData::g_addresses[AuthData::CG_DrawGrenadePointer])(localClientNum, centerX, centerY, grenadeOffset, color);
		Context->SetResult(0, 0);
	}

	void CG_DrawGrenadeIcon(NativeCallContext* Context) {
		int localClientNum = Context->GetArgument<int>(0);
		float centerX = Context->GetArgument<float>(1);
		float centerY = Context->GetArgument<float>(2);
		D3DXVECTOR3* grenadeOffset = Context->GetArgument<D3DXVECTOR3*>(3);
		float* color = Context->GetArgument<float*>(4);
		Material* material = Context->GetArgument<Material*>(5);

		((void(*)(int, float, float, D3DXVECTOR3*, float*, Material*))AuthData::g_addresses[AuthData::CG_DrawGrenadeIcon])(localClientNum, centerX, centerY, grenadeOffset, color, material);
		Context->SetResult(0, 0);
	}

	void CG_Trace(NativeCallContext* Context) {
		Trace* trace = Context->GetArgument<Trace*>(0);
		D3DXVECTOR3* start = Context->GetArgument<D3DXVECTOR3*>(1);
		D3DXVECTOR3* end = Context->GetArgument<D3DXVECTOR3*>(2);
		int ignore = Context->GetArgument<int>(3);
		int mask = Context->GetArgument<int>(4);

		((void(*)(Trace*, D3DXVECTOR3*, D3DXVECTOR3*, int, int))AuthData::g_addresses[AuthData::CG_Trace])(trace, start, end, ignore, mask);
		Context->SetResult(0, 0);
	}

	void CG_TraceIgnoreSetup(NativeCallContext* Context) {
		Trace* trace = Context->GetArgument<Trace*>(0);
		float* start = Context->GetArgument<float*>(1);
		float* end = Context->GetArgument<float*>(2);
		float* origin = Context->GetArgument<float*>(3);
		int ignore = Context->GetArgument<int>(4);
		int mask = Context->GetArgument<int>(5);
		bool unk1 = Context->GetArgument<bool>(6);
		bool unk2 = Context->GetArgument<bool>(7);

		((void(*)(Trace*, float*, float*, float*, int, int, bool, bool))AuthData::g_addresses[AuthData::CG_TraceIgnoreSetup])(trace, start, end, origin, ignore, mask, unk1, unk2);
		Context->SetResult(0, 0);
	}

	void CG_TraceIgnoreSetup2(NativeCallContext* Context) {
		Trace* trace = Context->GetArgument<Trace*>(0);
		float* start = Context->GetArgument<float*>(1);
		float* end = Context->GetArgument<float*>(2);
		float* origin = Context->GetArgument<float*>(3);
		int ignore = Context->GetArgument<int>(4);
		int mask = Context->GetArgument<int>(5);

		((void(*)(Trace*, float*, float*, float*, int, int))AuthData::g_addresses[AuthData::CG_TraceIgnoreSetup2])(trace, start, end, origin, ignore, mask);
		Context->SetResult(0, 0);
	}

	void CG_LocationalTrace(NativeCallContext* Context) {
		Trace* _trace = Context->GetArgument<Trace*>(0);
		D3DXVECTOR3 start = Context->GetArgument<D3DXVECTOR3>(1);
		D3DXVECTOR3 end = Context->GetArgument<D3DXVECTOR3>(2);
		D3DXVECTOR3 origin = Context->GetArgument<D3DXVECTOR3>(3);
		int clientNum = Context->GetArgument<int>(4);
		int mask = Context->GetArgument<int>(5);

		((void(*)(Trace*, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3, int, int))AuthData::g_addresses[AuthData::CG_LocationalTrace])(_trace, start, end, origin, clientNum, mask);
		Context->SetResult(0, 0);
	}

	void BulletTrace(NativeCallContext* Context) {
		int client = Context->GetArgument<int>(0);
		BulletFireParams* bullet = Context->GetArgument<BulletFireParams*>(1);
		CEntity* entity = Context->GetArgument<CEntity*>(2);
		BulletTraceResults* trace = Context->GetArgument<BulletTraceResults*>(3);
		int unk = Context->GetArgument<int>(4);

		bool Return = ((bool(*)(int, BulletFireParams*, CEntity*, BulletTraceResults*, int))AuthData::g_addresses[AuthData::BulletTrace])(client, bullet, entity, trace, unk);
		Context->SetResult(0, Return);
	}

	void FireBulletPenetrate(NativeCallContext* Context) {
		int client = Context->GetArgument<int>(0);
		BulletFireParams* bullet = Context->GetArgument<BulletFireParams*>(1);
		int weapon = Context->GetArgument<int>(2);
		int attacker = Context->GetArgument<int>(3);
		CEntity* entity = Context->GetArgument<CEntity*>(4);
		int alwaysMinusOne = Context->GetArgument<int>(5);
		float* viewOrigin = Context->GetArgument<float*>(6);
		bool unkBool = Context->GetArgument<bool>(7);

		((void(*)(int, BulletFireParams*, int, int, CEntity*, int, float*, bool))AuthData::g_addresses[AuthData::FireBulletPenetrate])(client, bullet, weapon, attacker, entity, alwaysMinusOne, viewOrigin, unkBool);
		Context->SetResult(0, 0);
	}

	void Trace_GetEntityHitId(NativeCallContext* Context) {
		Trace* trace = Context->GetArgument<Trace*>(0);

		int Return = ((int(*)(Trace*))AuthData::g_addresses[AuthData::Trace_GetEntityHitId])(trace);
		Context->SetResult(0, Return);
	}

	void RandomBulletDir(NativeCallContext* Context) {
		int* randomSeed = Context->GetArgument<int*>(0);
		float* x = Context->GetArgument<float*>(1);
		float* y = Context->GetArgument<float*>(2);

		((void(*)(int*, float*, float*))AuthData::g_addresses[AuthData::RandomBulletDir])(randomSeed, x, y);
		Context->SetResult(0, 0);
	}

	void BG_SeedRandWithGameTime(NativeCallContext* Context) {
		int* seed = Context->GetArgument<int*>(0);

		((void(*)(int*))AuthData::g_addresses[AuthData::BG_SeedRandWithGameTime])(seed);
		Context->SetResult(0, 0);
	}

	void BG_GetSpreadForWeapon(NativeCallContext* Context) {
		CgArray* ps = Context->GetArgument<CgArray*>(0);
		int weapon = Context->GetArgument<int>(1);
		float* minSpread = Context->GetArgument<float*>(2);
		float* maxSpread = Context->GetArgument<float*>(3);

		((void(*)(CgArray*, int, float*, float*))AuthData::g_addresses[AuthData::BG_GetSpreadForWeapon])(ps, weapon, minSpread, maxSpread);
		Context->SetResult(0, 0);
	}

	void CG_GetPlayerViewOrigin(NativeCallContext* Context) {
		int clientNum = Context->GetArgument<int>(0);
		CgArray* ps = Context->GetArgument<CgArray*>(1);
		D3DXVECTOR3* out = Context->GetArgument<D3DXVECTOR3*>(2);

		((void(*)(int, CgArray*, D3DXVECTOR3*))AuthData::g_addresses[AuthData::CG_GetPlayerViewOrigin])(clientNum, ps, out);
		Context->SetResult(0, 0);
	}

	void R_TextHeight(NativeCallContext* Context) {
		Font_s* font = Context->GetArgument<Font_s*>(0);
		int size = font ? font->m_pixelHeight : 0;
		Context->SetResult(0, size);
	}

	void Key_GetBindingForCmd(NativeCallContext* Context) {
		const char* keyString = Context->GetArgument<const char*>(0);

		int Return = ((int(*)(const char*))AuthData::g_addresses[AuthData::Key_GetBindingForCmd])(keyString);
		Context->SetResult(0, Return);
	}

	void CL_ExecuteKey(NativeCallContext* Context) {
		int client = Context->GetArgument<int>(0);
		int bind = Context->GetArgument<int>(1);
		int key = Context->GetArgument<int>(2);

		((void(*)(int, int, int))AuthData::g_addresses[AuthData::CL_ExecuteKey])(client, bind, key);
		Context->SetResult(0, 0);
	}

	void CG_GetRankIcon(NativeCallContext* Context) {
		int rank = Context->GetArgument<int>(0);
		int prestige = Context->GetArgument<int>(1);
		Material** material = Context->GetArgument<Material**>(2);

		((void(*)(int, int, Material**))AuthData::g_addresses[AuthData::CG_GetRankIcon])(rank, prestige, material);
		Context->SetResult(0, 0);
	}

	void CG_CompassDrawPlayerMap(NativeCallContext* Context) {
		int localClientNum = Context->GetArgument<int>(0);
		CompassType compassType = Context->GetArgument<CompassType>(1);
		RectDef* parentRect = Context->GetArgument<RectDef*>(2);
		RectDef* rect = Context->GetArgument<RectDef*>(3);
		Material* material = Context->GetArgument<Material*>(4);
		float* color = Context->GetArgument<float*>(5);

		((void(*)(int, CompassType, RectDef*, RectDef*, Material*, float*))AuthData::g_addresses[AuthData::CG_CompassDrawPlayerMap])(localClientNum, compassType, parentRect, rect, material, color);
		Context->SetResult(0, 0);
	}

	void CG_CompassDrawPlayer(NativeCallContext* Context) {
		int localClientNum = Context->GetArgument<int>(0);
		CompassType compassType = Context->GetArgument<CompassType>(1);
		RectDef* parentRect = Context->GetArgument<RectDef*>(2);
		RectDef* rect = Context->GetArgument<RectDef*>(3);
		float* color = Context->GetArgument<float*>(4);

		((void(*)(int, CompassType, RectDef*, RectDef*, float*))AuthData::g_addresses[AuthData::CG_CompassDrawPlayer])(localClientNum, compassType, parentRect, rect, color);
		Context->SetResult(0, 0);
	}

	void CG_CompassDrawCrates(NativeCallContext* Context) {
		int localClientNum = Context->GetArgument<int>(0);
		CompassType compassType = Context->GetArgument<CompassType>(1);
		RectDef* parentRect = Context->GetArgument<RectDef*>(2);
		RectDef* rect = Context->GetArgument<RectDef*>(3);
		Material* material = Context->GetArgument<Material*>(4);
		float* color = Context->GetArgument<float*>(5);

		((void(*)(int, CompassType, RectDef*, RectDef*, Material*, float*))AuthData::g_addresses[AuthData::CG_CompassDrawCrates])(localClientNum, compassType, parentRect, rect, material, color);
		Context->SetResult(0, 0);
	}

	void CG_CompassDrawPlanes(NativeCallContext* Context) {
		int localClientNum = Context->GetArgument<int>(0);
		CompassType compassType = Context->GetArgument<CompassType>(1);
		RectDef* parentRect = Context->GetArgument<RectDef*>(2);
		RectDef* rect = Context->GetArgument<RectDef*>(3);
		Material* material = Context->GetArgument<Material*>(4);
		float* color = Context->GetArgument<float*>(5);

		((void(*)(int, CompassType, RectDef*, RectDef*, Material*, float*))AuthData::g_addresses[AuthData::CG_CompassDrawPlanes])(localClientNum, compassType, parentRect, rect, material, color);
		Context->SetResult(0, 0);
	}

	void CG_CompassDrawFriendlies(NativeCallContext* Context) {
		int localClientNum = Context->GetArgument<int>(0);
		CompassType compassType = Context->GetArgument<CompassType>(1);
		RectDef* parentRect = Context->GetArgument<RectDef*>(2);
		RectDef* rect = Context->GetArgument<RectDef*>(3);
		float* color = Context->GetArgument<float*>(4);

		((void(*)(int, CompassType, RectDef*, RectDef*, float*))AuthData::g_addresses[AuthData::CG_CompassDrawFriendlies])(localClientNum, compassType, parentRect, rect, color);
		Context->SetResult(0, 0);
	}

	void CG_CompassDrawEnemies(NativeCallContext* Context) {
		int localClientNum = Context->GetArgument<int>(0);
		CompassType compassType = Context->GetArgument<CompassType>(1);
		RectDef* parentRect = Context->GetArgument<RectDef*>(2);
		RectDef* rect = Context->GetArgument<RectDef*>(3);
		float* color = Context->GetArgument<float*>(4);

		((void(*)(int, CompassType, RectDef*, RectDef*, float*))AuthData::g_addresses[AuthData::CG_CompassDrawEnemies])(localClientNum, compassType, parentRect, rect, color);
		Context->SetResult(0, 0);
	}

	void CG_CompassDrawTurrets(NativeCallContext* Context) {
		int localClientNum = Context->GetArgument<int>(0);
		CompassType compassType = Context->GetArgument<CompassType>(1);
		RectDef* parentRect = Context->GetArgument<RectDef*>(2);
		RectDef* rect = Context->GetArgument<RectDef*>(3);
		float* color = Context->GetArgument<float*>(4);

		((void(*)(int, CompassType, RectDef*, RectDef*, float*))AuthData::g_addresses[AuthData::CG_CompassDrawTurrets])(localClientNum, compassType, parentRect, rect, color);
		Context->SetResult(0, 0);
	}

	void CG_CompassDrawMissiles(NativeCallContext* Context) {
		int localClientNum = Context->GetArgument<int>(0);
		CompassType compassType = Context->GetArgument<CompassType>(1);
		RectDef* parentRect = Context->GetArgument<RectDef*>(2);
		RectDef* rect = Context->GetArgument<RectDef*>(3);
		float* color = Context->GetArgument<float*>(4);

		((void(*)(int, CompassType, RectDef*, RectDef*, float*))AuthData::g_addresses[AuthData::CG_CompassDrawMissiles])(localClientNum, compassType, parentRect, rect, color);
		Context->SetResult(0, 0);
	}

	void CG_CompassDrawHelicopter(NativeCallContext* Context) {
		int localClientNum = Context->GetArgument<int>(0);
		CompassType compassType = Context->GetArgument<CompassType>(1);
		RectDef* parentRect = Context->GetArgument<RectDef*>(2);
		RectDef* rect = Context->GetArgument<RectDef*>(3);
		float* color = Context->GetArgument<float*>(4);

		((void(*)(int, CompassType, RectDef*, RectDef*, float*))AuthData::g_addresses[AuthData::CG_CompassDrawHelicopter])(localClientNum, compassType, parentRect, rect, color);
		Context->SetResult(0, 0);
	}

	void BG_GetWeaponName(NativeCallContext* Context) {
		int weapon = Context->GetArgument<int>(0);
		char* buffer = Context->GetArgument<char*>(1);
		int maxLength = Context->GetArgument<int>(2);

		char* Return = ((char*(*)(int, char*, int))AuthData::g_addresses[AuthData::BG_GetWeaponName])(weapon, buffer, maxLength);
		Context->SetResult(0, Return);
	}

	void CG_BulletEndPos(NativeCallContext* Context) {
		int* seed = Context->GetArgument<int*>(0);
		float spread = Context->GetArgument<float>(1);
		float* viewOrigin = Context->GetArgument<float*>(2);
		float* spreadEndVec = Context->GetArgument<float*>(3);
		float* spreadDirection = Context->GetArgument<float*>(4);
		float* viewAxisX = Context->GetArgument<float*>(5);
		float* viewAxisY = Context->GetArgument<float*>(6);
		float* viewAxisZ = Context->GetArgument<float*>(7);
		float maxRange = Context->GetArgument<float>(8);

		((void(*)(int*, float, float*, float*, float*, float*, float*, float*, float))AuthData::g_addresses[AuthData::CG_BulletEndPos])(seed, spread, viewOrigin, spreadEndVec, spreadDirection, viewAxisX, viewAxisY, viewAxisZ, maxRange);
		Context->SetResult(0, 0);
	}

	// System
	void NetDll_socket(NativeCallContext* Context) {
		XNCALLER_TYPE xnCaller = XNCALLER_SYSAPP;
		DWORD af = Context->GetArgument<DWORD>(1);
		DWORD type = Context->GetArgument<DWORD>(2);
		DWORD protocol = Context->GetArgument<DWORD>(3);

		SOCKET Return = ((SOCKET(*)(XNCALLER_TYPE, int, int, int))Utils::ResolveFunction(g_modules["xam.xex"], 0x03))(xnCaller, af, type, protocol);
		Context->SetResult(0, Return);
	}

	void NetDll_closesocket(NativeCallContext* Context) {
		XNCALLER_TYPE xnCaller = XNCALLER_SYSAPP;
		SOCKET socket = Context->GetArgument<SOCKET>(1);

		DWORD Return = ((DWORD(*)(XNCALLER_TYPE, SOCKET))Utils::ResolveFunction(g_modules["xam.xex"], 0x04))(xnCaller, socket);
		Context->SetResult(0, Return);
	}

	void NetDll_shutdown(NativeCallContext* Context) {
		XNCALLER_TYPE xnCaller = XNCALLER_SYSAPP;
		SOCKET socket = Context->GetArgument<SOCKET>(1);
		DWORD method = Context->GetArgument<DWORD>(2);

		DWORD Return = ((DWORD(*)(XNCALLER_TYPE, SOCKET, DWORD))Utils::ResolveFunction(g_modules["xam.xex"], 0x05))(xnCaller, socket, method);
		Context->SetResult(0, Return);
	}

	void NetDll_setsockopt(NativeCallContext* Context) {
		XNCALLER_TYPE xnCaller = XNCALLER_SYSAPP;
		SOCKET socket = Context->GetArgument<SOCKET>(1);
		DWORD level = Context->GetArgument<DWORD>(2);
		DWORD option = Context->GetArgument<DWORD>(3);
		const char* value = Context->GetArgument<const char*>(4);
		DWORD length = Context->GetArgument<DWORD>(5);

		DWORD Return = ((DWORD(*)(XNCALLER_TYPE, SOCKET, DWORD, DWORD, const char*, DWORD))Utils::ResolveFunction(g_modules["xam.xex"], 0x07))(xnCaller, socket, level, option, value, length);
		Context->SetResult(0, Return);
	}

	void NetDll_connect(NativeCallContext* Context) {
		XNCALLER_TYPE xnCaller = XNCALLER_SYSAPP;
		SOCKET socket = Context->GetArgument<SOCKET>(1);
		sockaddr* name = Context->GetArgument<sockaddr*>(2);
		DWORD length = Context->GetArgument<DWORD>(3);

		DWORD Return = ((DWORD(*)(XNCALLER_TYPE, SOCKET, sockaddr*, DWORD))Utils::ResolveFunction(g_modules["xam.xex"], 0x0C))(xnCaller, socket, name, length);
		Context->SetResult(0, Return);
	}

	void NetDll_recv(NativeCallContext* Context) {
		XNCALLER_TYPE xnCaller = XNCALLER_SYSAPP;
		SOCKET socket = Context->GetArgument<SOCKET>(1);
		char* buffer = Context->GetArgument<char*>(2);
		int length = Context->GetArgument<int>(3);
		int flags = Context->GetArgument<int>(4);

		int Return = ((int(*)(XNCALLER_TYPE, SOCKET, char*, int, int))Utils::ResolveFunction(g_modules["xam.xex"], 0x12))(xnCaller, socket, buffer, length, flags);
		Context->SetResult(0, Return);
	}

	void NetDll_send(NativeCallContext* Context) {
		XNCALLER_TYPE xnCaller = XNCALLER_SYSAPP;
		SOCKET socket = Context->GetArgument<SOCKET>(1);
		char* buffer = Context->GetArgument<char*>(2);
		int length = Context->GetArgument<int>(3);
		int flags = Context->GetArgument<int>(4);

		int Return = ((int(*)(XNCALLER_TYPE, SOCKET, char*, int, int))Utils::ResolveFunction(g_modules["xam.xex"], 0x16))(xnCaller, socket, buffer, length, flags);
		Context->SetResult(0, Return);
	}

	void NetDll_WSAStartupEx(NativeCallContext* Context) {
		XNCALLER_TYPE xnCaller = XNCALLER_SYSAPP;
		WORD versionA = Context->GetArgument<WORD>(1);
		WSADATA* wsad = Context->GetArgument<WSADATA*>(2);
		DWORD versionB = Context->GetArgument<DWORD>(3);

		DWORD Return = ((DWORD(*)(XNCALLER_TYPE, WORD, WSADATA*, DWORD))Utils::ResolveFunction(g_modules["xam.xex"], 0x24))(xnCaller, versionA, wsad, versionB);
		Context->SetResult(0, Return);
	}

	void NetDll_XNetStartup(NativeCallContext* Context) {
		XNCALLER_TYPE xnCaller = XNCALLER_SYSAPP;
		XNetStartupParams* xnsp = Context->GetArgument<XNetStartupParams*>(1);

		DWORD Return = ((DWORD(*)(XNCALLER_TYPE, XNetStartupParams*))Utils::ResolveFunction(g_modules["xam.xex"], 0x33))(xnCaller, xnsp);
		Context->SetResult(0, Return);
	}

	void ObCreateSymbolicLink(NativeCallContext* Context) {
		PSTRING SymbolicLinkName = Context->GetArgument<PSTRING>(0);
		PSTRING DeviceName = Context->GetArgument<PSTRING>(1);

		HRESULT Return = ((HRESULT(*)(PSTRING, PSTRING))Utils::ResolveFunction(g_modules["xboxkrnl.exe"], 0x103))(SymbolicLinkName, DeviceName);
		Context->SetResult(0, Return);
	}

	void RtlInitAnsiString(NativeCallContext* Context) {
		PANSI_STRING DestinationString = Context->GetArgument<PANSI_STRING>(0);
		PCSZ DeviceName = Context->GetArgument<PCSZ>(1);

		PVOID Return = ((PVOID(*)(PANSI_STRING, PCSZ))Utils::ResolveFunction(g_modules["xboxkrnl.exe"], 0x12C))(DestinationString, DeviceName);
		Context->SetResult(0, Return);
	}

	void ExCreateThread(NativeCallContext* Context) {
		HANDLE* handle = Context->GetArgument<HANDLE*>(0);
		DWORD dwStackSize = Context->GetArgument<DWORD>(1);
		LPDWORD lpThreadId = Context->GetArgument<LPDWORD>(2);
		PVOID apiThreadStartup = Context->GetArgument<PVOID>(3);
		LPTHREAD_START_ROUTINE lpStartAddress = Context->GetArgument<LPTHREAD_START_ROUTINE>(4);
		LPVOID lpParameter = Context->GetArgument<LPVOID>(5);
		DWORD dwCreationFlagsMod = Context->GetArgument<DWORD>(6);

		DWORD Return = ((DWORD(*)(HANDLE*, DWORD, LPDWORD, PVOID, LPTHREAD_START_ROUTINE, LPVOID, DWORD))Utils::ResolveFunction(g_modules["xboxkrnl.exe"], 13))(handle, dwStackSize, lpThreadId, apiThreadStartup, lpStartAddress, lpParameter, dwCreationFlagsMod);
		Context->SetResult(0, Return);
	}

	void Sleep(NativeCallContext* Context) {
		DWORD ms = Context->GetArgument<DWORD>(0);
		::Sleep(ms);
		Context->SetResult(0, 0);
	}

	void XamGetCurrentTitleID(NativeCallContext* Context) {
		DWORD Return = ((DWORD(*)())Utils::ResolveFunction(g_modules["xam.xex"], 0x1CF))();
		Context->SetResult(0, Return);
	}

	void XSetThreadProcessor(NativeCallContext* Context) {
		HANDLE thread = Context->GetArgument<HANDLE>(0);
		DWORD hardwareThread = Context->GetArgument<DWORD>(1);
		DWORD Return = ::XSetThreadProcessor(thread, hardwareThread);
		Context->SetResult(0, Return);
	}

	void ResumeThread(NativeCallContext* Context) {
		HANDLE thread = Context->GetArgument<HANDLE>(0);
		::ResumeThread(thread);
		Context->SetResult(0, 0);
	}

	void CloseHandle(NativeCallContext* Context) {
		HANDLE hObj = Context->GetArgument<HANDLE>(0);
		BOOL Return = ::CloseHandle(hObj);
		Context->SetResult(0, Return);
	}

	void XexGetProcedureAddress(NativeCallContext* Context) {
		HANDLE handle = Context->GetArgument<HANDLE>(0);
		DWORD ordinal = Context->GetArgument<DWORD>(1);
		PVOID address = Context->GetArgument<PVOID>(2);

		DWORD Return = ::XexGetProcedureAddress(handle, ordinal, address);
		Context->SetResult(0, Return);
	}

	void HalReturnToFirmware(NativeCallContext* Context) {
		DWORD powerMode = Context->GetArgument<DWORD>(0);
		::HalReturnToFirmware(powerMode);
		Context->SetResult(0, 0);
	}

	void XEncryptedAlloc(NativeCallContext* Context) {
		SIZE_T size = Context->GetArgument<SIZE_T>(0);
		void* Return = ::XEncryptedAlloc(size);
		Context->SetResult(0, Return);
	}

	void XEncryptedFree(NativeCallContext* Context) {
		void* address = Context->GetArgument<void*>(0);
		::XEncryptedFree(address);
		Context->SetResult(0, 0);
	}

	// LE Server
	void AddRegionToWhitelist(NativeCallContext* Context) {
		DWORD from = Context->GetArgument<DWORD>(0);
		DWORD size = Context->GetArgument<DWORD>(1);

		DWORD address = *(DWORD*)0x817417A4;
		if (address != 0)
			((void(*)(DWORD, DWORD))address)(from, size);
	}

	// XeCrypt
	void XeCryptRc4(NativeCallContext* Context) {
		BYTE* pbKey = Context->GetArgument<BYTE*>(0);
		DWORD cbKey = Context->GetArgument<DWORD>(1);
		BYTE* pbInpOut = Context->GetArgument<BYTE*>(2);
		DWORD cbInpOut = Context->GetArgument<DWORD>(3);

		((void(*)(BYTE*, DWORD, BYTE*, DWORD))Utils::ResolveFunction(g_modules["xboxkrnl.exe"], 397))(pbKey, cbKey, pbInpOut, cbInpOut);
		Context->SetResult(0, 0);
	}

	// Self
	void RegisterThread(NativeCallContext* Context) {
		const char* name = Context->GetArgument<const char*>(0);
		void(*func)() = Context->GetArgument<void(*)()>(1);
		bool security = Context->GetArgument<bool>(2);

		auto Return = GetThreadPool()->RegisterThread(name, func, security);
		Context->SetResult(0, Return);
	}

	void IntegrityManagerAdd(NativeCallContext* Context) {
		void* function = Context->GetArgument<void*>(0);
		int size = Context->GetArgument<int>(1);

		GetIntegrityManager()->Add(function, size);
		Context->SetResult(0, 0);
	}

	void HookXbdmCommand(NativeCallContext* Context) {
		const char* command = Context->GetArgument<const char*>(0);
		DWORD function = Context->GetArgument<DWORD>(1);

		GetXbdm()->HookCommand(command, function);
		Context->SetResult(0, 0);
	}

	void InitializeXbdmCommands(NativeCallContext* Context) {
		GetXbdm()->InitializeCommands();
		Context->SetResult(0, 0);
	}
}

__declspec(noinline) void Invoker::RegisterPostAuthNatives() {
	// Custom
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_GetCG], NativeHandlers::GetCG);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_GetCEntity], NativeHandlers::GetCEntity);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_GetClientInfo], NativeHandlers::GetClientInfo);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_GetServerInfo], NativeHandlers::GetServerInfo);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_GetClientActive], NativeHandlers::GetClientActive);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_GetXenonUserData], NativeHandlers::GetXenonUserData);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_GetUIContext], NativeHandlers::GetUIContext);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_GetPingData], NativeHandlers::GetPingData);

	// Engine
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_AimAssist_GetTagPos], NativeHandlers::AimAssist_GetTagPos);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_RegisterTag], NativeHandlers::CG_RegisterTag);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_R_RegisterMaterial], NativeHandlers::R_RegisterMaterial);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_R_RegisterFont], NativeHandlers::R_RegisterFont);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_R_AddCmdDrawStretchPic], NativeHandlers::R_AddCmdDrawStretchPic);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_R_AddCmdDrawText], NativeHandlers::R_AddCmdDrawText);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_R_TextWidth], NativeHandlers::R_TextWidth);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_Dvar_GetBool], NativeHandlers::Dvar_GetBool);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_Dvar_GetString], NativeHandlers::Dvar_GetString);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_Cbuf_AddText], NativeHandlers::Cbuf_AddText);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_DrawRotatedPicPhysical], NativeHandlers::CG_DrawRotatedPicPhysical);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_Com_GetClientDObj], NativeHandlers::Com_GetClientDObj);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_DObjGetWorldTagPos], NativeHandlers::CG_DObjGetWorldTagPos);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_Key_IsDown], NativeHandlers::Key_IsDown);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_CanSeePlayer], NativeHandlers::CG_CanSeePlayer);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_GetTagPosition], NativeHandlers::GetTagPosition);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_WorldPosToCompass], NativeHandlers::CG_WorldPosToCompass);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_DrawGrenadePointer], NativeHandlers::CG_DrawGrenadePointer);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_DrawGrenadeIcon], NativeHandlers::CG_DrawGrenadeIcon);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_TraceIgnoreSetup], NativeHandlers::CG_TraceIgnoreSetup);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_TraceIgnoreSetup2], NativeHandlers::CG_TraceIgnoreSetup2);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_Trace], NativeHandlers::CG_Trace);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_LocationalTrace], NativeHandlers::CG_LocationalTrace);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_BulletTrace], NativeHandlers::BulletTrace);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_FireBulletPenetrate], NativeHandlers::FireBulletPenetrate);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_Trace_GetEntityHitId], NativeHandlers::Trace_GetEntityHitId);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_RandomBulletDir], NativeHandlers::RandomBulletDir);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_BG_SeedRandWithGameTime], NativeHandlers::BG_SeedRandWithGameTime);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_BG_GetSpreadForWeapon], NativeHandlers::BG_GetSpreadForWeapon);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_SL_GetString], NativeHandlers::SL_GetString);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_SL_GetString_0], NativeHandlers::SL_GetString_0);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_GetPlayerViewOrigin], NativeHandlers::CG_GetPlayerViewOrigin);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_R_TextHeight], NativeHandlers::R_TextHeight);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_Key_GetBindingForCmd], NativeHandlers::Key_GetBindingForCmd);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CL_ExecuteKey], NativeHandlers::CL_ExecuteKey);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_GetRankIcon], NativeHandlers::CG_GetRankIcon);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_CompassDrawPlayerMap], NativeHandlers::CG_CompassDrawPlayerMap);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_CompassDrawPlayer], NativeHandlers::CG_CompassDrawPlayer);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_CompassDrawCrates], NativeHandlers::CG_CompassDrawCrates);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_CompassDrawPlanes], NativeHandlers::CG_CompassDrawPlanes);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_CompassDrawFriendlies], NativeHandlers::CG_CompassDrawFriendlies);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_CompassDrawEnemies], NativeHandlers::CG_CompassDrawEnemies);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_CompassDrawMissiles], NativeHandlers::CG_CompassDrawMissiles);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_CompassDrawHelicopter], NativeHandlers::CG_CompassDrawHelicopter);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_CompassDrawTurrets], NativeHandlers::CG_CompassDrawTurrets);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_BG_GetWeaponName], NativeHandlers::BG_GetWeaponName);
	RegisterNative(AuthData::g_natives[AuthData::eNatives::NATIVE_CG_BulletEndPos], NativeHandlers::CG_BulletEndPos);
}

__declspec(noinline) void Invoker::RegisterSystemNatives() {
	RegisterNative(0xf5fe680f, NativeHandlers::NetDll_socket);
	RegisterNative(0x75229afc, NativeHandlers::NetDll_closesocket);
	RegisterNative(0x956396f8, NativeHandlers::NetDll_shutdown);
	RegisterNative(0x96627da7, NativeHandlers::NetDll_setsockopt);
	RegisterNative(0x2fc51727, NativeHandlers::NetDll_connect);
	RegisterNative(0xbcd9702b, NativeHandlers::NetDll_recv);
	RegisterNative(0xc290de6d, NativeHandlers::NetDll_send);
	RegisterNative(0x4578f873, NativeHandlers::NetDll_WSAStartupEx);
	RegisterNative(0x97c394b0, NativeHandlers::NetDll_XNetStartup);
	RegisterNative(0xfe805bca, NativeHandlers::ObCreateSymbolicLink);
	RegisterNative(0xd8f258c5, NativeHandlers::RtlInitAnsiString);
	RegisterNative(0x8546aae1, NativeHandlers::ExCreateThread);
	RegisterNative(0xeefa61ff, NativeHandlers::Sleep);
	RegisterNative(0x6e19abd, NativeHandlers::XamGetCurrentTitleID);
	RegisterNative(0x1757a1b1, NativeHandlers::XSetThreadProcessor);
	RegisterNative(0xbc9414b8, NativeHandlers::ResumeThread);
	RegisterNative(0x44fb9f81, NativeHandlers::CloseHandle);
	RegisterNative(0xfa1b79f7, NativeHandlers::XexGetProcedureAddress);
	RegisterNative(0xbcd16d0b, NativeHandlers::HalReturnToFirmware);
	RegisterNative(0x70c3113f, NativeHandlers::AddRegionToWhitelist);
	RegisterNative(0xd1f3bce2, NativeHandlers::XeCryptRc4);
	RegisterNative(0x6f6c8a3, NativeHandlers::XEncryptedAlloc);
	RegisterNative(0xbe1f66f5, NativeHandlers::XEncryptedFree);
	RegisterNative(0x40a331df, NativeHandlers::RegisterThread);
	RegisterNative(0xaead5cd7, NativeHandlers::IntegrityManagerAdd);
	RegisterNative(0xd9887a6c, NativeHandlers::HookXbdmCommand);
	RegisterNative(0x48f0e8a1, NativeHandlers::InitializeXbdmCommands);
}