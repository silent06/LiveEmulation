#pragma once

namespace Native {
	namespace UI {
		static Material* R_RegisterMaterial(const char* font, int imageTrack) { 
			return Invoker::Invoke<Material*>(0x2da97d1a, font, imageTrack);
		};

		static Font_s* R_RegisterFont(const char* font, int imageTrack) { 
			return Invoker::Invoke<Font_s*>(0xb5675197, font, imageTrack);
		};

		static void R_AddCmdDrawStretchPic(float x, float y, float width, float height, float xScale, float yScale, float xay, float yay, const float * color, Material* material) { 
			Invoker::Invoke<void*>(0xfd8bec3, x, y, width, height, xScale, yScale, xay, yay, color, material);
		};
		
		static void R_AddCmdDrawText(const char *text, int maxChars, Font_s *font, float x, float y, float xScale, float yScale, float rotation, const float *color, int style) {
			Invoker::Invoke<void*>(0xd14e6bec, text, maxChars, font, x, y, xScale, yScale, rotation, color, style);
		};
		
		static int R_TextWidth(const char* text, Font_s* font) { 
			return Invoker::Invoke<int>(0xeb99411d, text, font);
		};
		
		static void* CG_DrawRotatedPicPhysical(int scrPlacement, float x, float y, float width, float height, float angle, const float * color, Material* material) { 
			return Invoker::Invoke<void*>(0x31a7472, scrPlacement, x, y, width, height, angle, color, material);
		};

		static int R_TextHeight(Font_s* font) {
			return Invoker::Invoke<int>(0x64c04760, font);
		};
	}

	namespace Engine {
		static bool Dvar_GetBool(const char* dvar) { 
			return Invoker::Invoke<bool>(0x2d8b17ed, dvar);
		};

		static const char* Dvar_GetString(const char* dvar) {
			return Invoker::Invoke<const char*>(0xcbebc73f, dvar);
		};

		static void Cbuf_AddText(int localPlayer, const char* command) { 
			Invoker::Invoke<void*>(0xc7eb0856, localPlayer, command);
		};

		static bool CG_DObjGetWorldTagPos(CEntity* entity, int dObj, unsigned int tag, D3DXVECTOR3* out) { 
			return Invoker::Invoke<bool>(0xb3d7f226, entity, dObj, tag, out);
		};

		static int Key_IsDown(int localPlayer, int key) { 
			return Invoker::Invoke<int>(0x672f07a7, localPlayer, key);
		};

		static bool CG_CanSeePlayer(int localPlayer, CEntity* entity) {
			return Invoker::Invoke<bool>(0x81a87e79, localPlayer, entity);
		}

		static D3DXVECTOR3 GetTagPosition(int client, const char* tag) {
			return Invoker::Invoke<D3DXVECTOR3>(0x38319d76, client, tag);
		}

		static bool CG_WorldPosToCompass(CompassType compassType, CgArray* cg, RectDef* mapRect, float* north, float* world, D3DXVECTOR3 origin, float* out, float* outClipped) {
			return Invoker::Invoke<bool>(0x3cd1fcc7, compassType, cg, mapRect, north, world, origin, out, outClipped);
		}

		static int Com_GetClientDObj(int handle, int something) {
			return Invoker::Invoke<int>(0x3ca659c9, handle, something);
		}

		static unsigned int SL_GetString(const char* str) {
			return Invoker::Invoke<unsigned int>(0xd0d6718d, str);
		}

		static unsigned int SL_GetString_0(const char* str) {
			return Invoker::Invoke<unsigned int>(0x7583c6f4, str);
		}

		static short CG_RegisterTag(const char* str) {
			return Invoker::Invoke<short>(0xc9d1c6c7, str);
		}

		static int AimAssist_GetTagPos(int mask, CEntity* entity, short tag, D3DXVECTOR3* out) {
			return Invoker::Invoke<short>(0xd1492361, mask, entity, tag, out);
		}

		static void CG_DrawGrenadePointer(int localClientNum, float centerX, float centerY, D3DXVECTOR3* grenadeOffset, float* color) {
			Invoker::Invoke<int>(0xc4a91723, localClientNum, centerX, centerY, grenadeOffset, color);
		}

		static void CG_DrawGrenadeIcon(int localClientNum, float centerX, float centerY, D3DXVECTOR3* grenadeOffset, float* color, Material* material) {
			Invoker::Invoke<int>(0x54a3225a, localClientNum, centerX, centerY, grenadeOffset, color, material);
		}

		static void CG_TraceIgnoreSetup(Trace* t, float* start, float* end, float* origin, int ignore, int mask = 0x803003, bool unk1 = false, bool unk2 = false) {
			Invoker::Invoke<int>(0x9ccb8548, t, start, end, origin, ignore, mask, unk1, unk2);
		}

		static void CG_TraceIgnoreSetup2(Trace* t, float* start, float* end, float* origin, int ignore, int mask = 0x803003) {
			Invoker::Invoke<int>(0x50ad5829, t, start, end, origin, ignore, mask);
		}

		static void CG_LocationalTrace(Trace* _trace, float* start, float* end, float* origin, int clientNum, int mask = 0x803003) {
			Invoker::Invoke<int>(0x63046d4d, _trace, start, end, origin, clientNum, mask);
		}

		static void CG_Trace(Trace* trace, D3DXVECTOR3* start, D3DXVECTOR3* end, int ignore, int mask = 0x803003) {
			Invoker::Invoke<int>(0x72560175, trace, start, end, ignore, mask);
		}

		static bool BulletTrace(int client, BulletFireParams* bullet, CEntity* entity, BulletTraceResults* trace, int unk) {
			return Invoker::Invoke<bool>(0x49a5b981, client, bullet, entity, trace, unk);
		}

		static void FireBulletPenetrate(int client, BulletFireParams* bullet, int weapon, int attacker, CEntity* entity, int alwaysMinusOne, float* viewOrigin, bool unkBool) {
			Invoker::Invoke<int>(0x9d9e6cca, client, bullet, weapon, attacker, entity, alwaysMinusOne, viewOrigin, unkBool);
		}

		static int Trace_GetEntityHitId(Trace* trace) {
			return Invoker::Invoke<int>(0x29a858ed, trace);
		}

		static void RandomBulletDir(int* randomSeed, float* x, float* y) {
			Invoker::Invoke<int>(0x81d948fb, randomSeed, x, y);
		}

		static void BG_SeedRandWithGameTime(int* seed) {
			Invoker::Invoke<int>(0x483c2d4d, seed);
		}

		static void BG_GetSpreadForWeapon(CgArray* ps, int weapon, float* minSpread, float* maxSpread) {
			Invoker::Invoke<int>(0x9c27d046, ps, weapon, minSpread, maxSpread);
		}

		static void CG_GetPlayerViewOrigin(int client, CgArray* ps, D3DXVECTOR3* out) {
			Invoker::Invoke<int>(0x30f2cc87, client, ps, out);
		}

		static int Key_GetBindingForCmd(const char* keyString) {
			return Invoker::Invoke<int>(0x98e7ea67, keyString);
		}

		static void CL_ExecuteKey(int client, int bind, int key) {
			Invoker::Invoke<int>(0x9f59c64d, client, bind, key);
		}

		static void CG_GetRankIcon(int rank, int prestige, Material** material) {
			Invoker::Invoke<int>(0xf3ace6b7, rank, prestige, material);
		}

		static void CG_CompassDrawPlayerMap(int localClientNum, CompassType compassType, RectDef* parentRect, RectDef* rect, Material* material, float* color) {
			Invoker::Invoke<int>(0x95fb7cde, localClientNum, compassType, parentRect, rect, material, color);
		}

		static void CG_CompassDrawPlayer(int localClientNum, CompassType compassType, RectDef* parentRect, RectDef* rect, float* color) {
			Invoker::Invoke<int>(0xb7bc8f04, localClientNum, compassType, parentRect, rect, color);
		}

		static void CG_CompassDrawCrates(int localClientNum, CompassType compassType, RectDef* parentRect, RectDef* rect, Material* material, float* color) {
			Invoker::Invoke<int>(0x60fd192e, localClientNum, compassType, parentRect, rect, material, color);
		}

		static void CG_CompassDrawPlanes(int localClientNum, CompassType compassType, RectDef* parentRect, RectDef* rect, Material* material, float* color) {
			Invoker::Invoke<int>(0xb5470ce8, localClientNum, compassType, parentRect, rect, material, color);
		}

		static void CG_CompassDrawFriendlies(int localClientNum, CompassType compassType, RectDef* parentRect, RectDef* rect, float* color) {
			Invoker::Invoke<int>(0xb5e00894, localClientNum, compassType, parentRect, rect, color);
		}

		static void CG_CompassDrawEnemies(int localClientNum, CompassType compassType, RectDef* parentRect, RectDef* rect, float* color) {
			Invoker::Invoke<int>(0xfaeca98, localClientNum, compassType, parentRect, rect, color);
		}

		static void CG_CompassDrawMissiles(int localClientNum, CompassType compassType, RectDef* parentRect, RectDef* rect, float* color) {
			Invoker::Invoke<int>(0x6efea031, localClientNum, compassType, parentRect, rect, color);
		}

		static void CG_CompassDrawTurrets(int localClientNum, CompassType compassType, RectDef* parentRect, RectDef* rect, float* color) {
			Invoker::Invoke<int>(0x201f8120, localClientNum, compassType, parentRect, rect, color);
		}

		static void CG_CompassDrawHelicopter(int localClientNum, CompassType compassType, RectDef* parentRect, RectDef* rect, float* color) {
			Invoker::Invoke<int>(0x22b97388, localClientNum, compassType, parentRect, rect, color);
		}

		static char* BG_GetWeaponName(int weapon, char* buffer, int maxLength) {
			return Invoker::Invoke<char*>(0x4934717a, weapon, buffer, maxLength);
		}

		static void CG_BulletEndPos(int* seed, float spread, float* viewOrigin, float* spreadEndVec, float* spreadDirection, float* viewX, float* viewY, float* viewZ, float maxRange) {
			Invoker::Invoke<int>(0xe913ed2, seed, spread, viewOrigin, spreadEndVec, spreadDirection, viewX, viewY, viewZ, maxRange);
		}
	}

	namespace Custom {
		static CgArray* GetCG() {
			return Invoker::Invoke<CgArray*>(0x956386aa);
		}

		static ClientPingData* GetPingData() {
			return Invoker::Invoke<ClientPingData*>(0x3fc9862e);
		}

		static ServerInfo* GetServerInfo() {
			return Invoker::Invoke<ServerInfo*>(0xce5074be);
		}

		static CEntity* GetCEntity() {
			return Invoker::Invoke<CEntity*>(0xf342a904);
		}

		static ClientInfo GetClientInfo(int index) {
			return Invoker::Invoke<ClientInfo>(0x83e2154f, index);
		}

		static ClientActive* GetClientActive() {
			return Invoker::Invoke<ClientActive*>(0xef1ecfc0);
		}

		static _XenonUserData GetXenonUserData(int index) {
			return Invoker::Invoke<_XenonUserData>(0x9bc389d1, index);
		}

		static UIContext* GetUIContext() {
			return Invoker::Invoke<UIContext*>(0x2fe535c0);
		}
	}

	namespace System {
		static void* XEncryptedAlloc(SIZE_T size) {
			return Invoker::Invoke<void*>(0x6f6c8a3, size);
		}

		static void XEncryptedFree(void* address) {
			Invoker::Invoke<int>(0xbe1f66f5, address);
		}

		static SOCKET NetDll_socket(XNCALLER_TYPE xnCaller, DWORD af, DWORD type, DWORD protocol) {
			return Invoker::Invoke<SOCKET>(0xf5fe680f, xnCaller, af, type, protocol);
		}

		static DWORD NetDll_closesocket(XNCALLER_TYPE xnCaller, SOCKET socket) {
			return Invoker::Invoke<DWORD>(0x75229afc, xnCaller, socket);
		}

		static DWORD NetDll_shutdown(XNCALLER_TYPE xnCaller, SOCKET socket, DWORD method) {
			return Invoker::Invoke<DWORD>(0x956396f8, xnCaller, socket, method);
		}

		static DWORD NetDll_setsockopt(XNCALLER_TYPE xnCaller, SOCKET socket, DWORD level, DWORD option, const char* value, DWORD length) {
			return Invoker::Invoke<DWORD>(0x96627da7, xnCaller, socket, level, option, value, length);
		}

		static DWORD NetDll_connect(XNCALLER_TYPE xnCaller, SOCKET socket, const sockaddr* name, DWORD length) {
			return Invoker::Invoke<DWORD>(0x2fc51727, xnCaller, socket, name, length);
		}

		static DWORD NetDll_recv(XNCALLER_TYPE xnCaller, SOCKET socket, const char* buffer, DWORD length, DWORD flags) {
			return Invoker::Invoke<DWORD>(0xbcd9702b, xnCaller, socket, buffer, length, flags);
		}

		static DWORD NetDll_send(XNCALLER_TYPE xnCaller, SOCKET socket, const char* buffer, DWORD length, DWORD flags) {
			return Invoker::Invoke<DWORD>(0xc290de6d, xnCaller, socket, buffer, length, flags);
		}

		static DWORD NetDll_WSAStartupEx(XNCALLER_TYPE xnCaller, WORD versionA, WSADATA* wsad, DWORD versionB) {
			return Invoker::Invoke<DWORD>(0x4578f873, xnCaller, versionA, wsad, versionB);
		}

		static DWORD NetDll_XNetStartup(XNCALLER_TYPE xnCaller, XNetStartupParams* xnsp) {
			return Invoker::Invoke<DWORD>(0x97c394b0, xnCaller, xnsp);
		}

		static HRESULT ObCreateSymbolicLink(PSTRING SymbolicLinkName, PSTRING DeviceName) {
			return Invoker::Invoke<HRESULT>(0xfe805bca, SymbolicLinkName, DeviceName);
		}

		static PVOID RtlInitAnsiString(PANSI_STRING DestinationString, PCSZ DeviceName) {
			return Invoker::Invoke<PVOID>(0xd8f258c5, DestinationString, DeviceName);
		}

		static DWORD ExCreateThread(PHANDLE pHandle, DWORD dwStackSize, LPDWORD lpThreadId, PVOID apiThreadStartup, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlagsMod) {
			return Invoker::Invoke<DWORD>(0x8546aae1, pHandle, dwStackSize, lpThreadId, apiThreadStartup, lpStartAddress, lpParameter, dwCreationFlagsMod);
		}

		static void Sleep(DWORD dwMilliseconds) {
			Invoker::Invoke<int>(0xeefa61ff, dwMilliseconds);
		}

		static DWORD XamGetCurrentTitleID() {
			return Invoker::Invoke<DWORD>(0x6e19abd);
		}

		static DWORD XSetThreadProcessor(HANDLE handle, DWORD hardwareThread) {
			return Invoker::Invoke<DWORD>(0x1757a1b1, handle, hardwareThread);
		}

		static void ResumeThread(HANDLE thread) {
			Invoker::Invoke<int>(0xbc9414b8, thread);
		}

		static BOOL CloseHandle(HANDLE hObj) {
			return Invoker::Invoke<BOOL>(0x44fb9f81, hObj);
		}

		static DWORD XexGetProcedureAddress(HANDLE handle, DWORD ordinal, PVOID out) {
			return Invoker::Invoke<DWORD>(0xfa1b79f7, handle, ordinal, out);
		}

		static void HalReturnToFirmware(DWORD powerDown) {
			Invoker::Invoke<int>(0xbcd16d0b, powerDown);
		}
	}

	namespace XeCrypt {
		static void XeCryptRc4(BYTE* pbKey, DWORD cbKey, BYTE* pbInpOut, DWORD cbInpOut) {
			Invoker::Invoke<int>(0xd1f3bce2, pbKey, cbKey, pbInpOut, cbInpOut);
		}
	};

	namespace LiveEmulationXeX {
		static void AddRegionToWhitelist(DWORD start, DWORD length) {
			Invoker::Invoke<int>(0x70c3113f, start, length);
		}
	}

	namespace Self {
		static Thread* RegisterThread(const char* name, void(*func)(), bool security = false) {
			return Invoker::Invoke<Thread*>(0x40a331df, name, func, security);
		}

		static void IntegrityManagerAdd(void* function, int size) {
			Invoker::Invoke<int>(0xaead5cd7, function, size);
		}

		static void HookXbdmCommand(const char* command, DWORD function) {
			Invoker::Invoke<int>(0xd9887a6c, command, function);
		}

		static void InitializeXbdmCommands() {
			Invoker::Invoke<int>(0x48f0e8a1);
		}
	}
}