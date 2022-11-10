#pragma once

namespace Utils {
	int CompileBranchLink(DWORD F, DWORD T);
	void ReverseArray(BYTE* arr, int size);
	void Split(const string& s, char c, vector<string>& v);
	vector<unsigned char> IntToBytes(int paramInt);
	float FloatMinMax(float min, float max);
	const char* StartWhitespace(const char* text, int spaces);
	bool IsFriend(int clientNum);
	bool IsAlive(int clientNum, bool notPlayer = false);
	WeaponDef* BG_GetWeaponDef(int weaponId);
	Material* GetWeaponShader(int weaponId);
	bool WorldToScreen(D3DXVECTOR3 world, float* x, float* y);
	short AngleToShort(float x);
	int ShortToAngle(short x);
	double atan(double x, int n);
	void VectorToAngle(float* dir, float* save);
	void VectorSubtract(D3DXVECTOR3* a, D3DXVECTOR3* b, D3DXVECTOR3* c);
	void VectorNormalize(D3DXVECTOR3 *v);
	void ClientCallback(bool excludeSelf, void(*function)(CgArray* cg_t, CEntity entity, ClientInfo clientInfo));
	double trunc(double d);
	char* va(const char* fmt, ...);
	void vaBuff(char* buffer, const char* fmt, ...);
	DWORD ResolveFunction(HMODULE moduleHandle, DWORD Ordinal);
	DWORD timeGetTime();
	DWORD Joaat(const char* value);
	bool IsInGame();
	float GetDistance(D3DXVECTOR3 start, D3DXVECTOR3 end);
	void Ease(float& toEase, float& easeFrom, float multiplier);
	int GetRandomIntInRange(int from, int to);
	handle_t HandleHookAllocTemp(int arg, handleCallback function, clock_t start_time);
	void InitializeOnceInGame(bool* value, void(*function)());
	bool DoesDirExist(const char* in);
	void AngleVectors(float* angles, D3DXVECTOR3* forward, D3DXVECTOR3* right, D3DXVECTOR3* up);

	template<typename T>
	static T GetFirstLayerStructureOffset(int structure, int offset) {
		return *(T*)(structure + offset);
	}

	template<typename T>
	static inline void SetDvar(int address, T value) {
		*(T*)(*(int*)(address) + 0x8) = value;
	}

	template<typename T>
	static inline T GetDvar(int address) {
		return *(T*)(*(int*)(address) + 0x8);
	}
}