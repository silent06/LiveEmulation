#include "stdafx.h"
#include <sstream>

char vaBuffer[0x1000];
char lowKeyboardBuffer[512];

namespace Utils {
#define PI (3.14159265358979323846264338327950288f)

	void ReverseArray(BYTE* arr, int size) {
		BYTE* reversed = new BYTE[size];
		for (int i = 0; i < size; i++) {
			reversed[i] = arr[size - 1 - i];
		}

		for (int i = 0; i < size; i++) {
			arr[i] = reversed[i];
		}

		delete[] reversed;
	}

	vector<unsigned char> IntToBytes(int paramInt) {
		vector<unsigned char> arrayOfByte(4);
		for (int i = 0; i < 4; i++)
			arrayOfByte[3 - i] = (paramInt >> (i * 8));
		return arrayOfByte;
	}

	void Split(const string& s, char c, vector<string>& v) {
		string::size_type i = 0;
		string::size_type j = s.find(c);

		while (j != string::npos) {
			v.push_back(s.substr(i, j - i));
			i = ++j;
			j = s.find(c, j);

			if (j == string::npos)
				v.push_back(s.substr(i, s.length()));
		}
	}

	double atan(double x, int n) {
		double a = 0.0;
		double sum = 0.0;

		if (x == 1.0) return PI / 4.0;
		if (x == -1.0) return -PI / 4.0;

		if (n > 0) {
			if ((x < -1.0) || (x > 1.0)) {
				if (x > 1.0)
					sum = PI / 2.0;
				else
					sum = -PI / 2.0;
				a = -1.0 / x;
				for (int j = 1; j <= n; j++) {
					sum += a;
					a *= -1.0*(2.0*j - 1) / ((2.0*j + 1)*x*x);
				}
			}
			else {
				sum = 0.0;
				a = x;
				for (int j = 1; j <= n; j++) {
					sum += a;
					a *= -1.0*(2.0*j - 1)*x*x / (2.0*j + 1);
				}
			}
		}

		return sum;
	}

	double atan2(double y, double x) {
		double u = atan(y / x, 24);
		if (x < 0.0) {
			if (u > 0.0)
				u -= PI;
			else
				u += PI;
		}
		return u;
	}

	void VectorToAngle(float* dir, float* save) {
		((void(*)(float*, float*))0x823155F0)(dir, save);
	}

	void VectorNormalize(D3DXVECTOR3 *v) {
		float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
		if (length != 0) {
			v->x = v->x / length;
			v->y = v->y / length;
			v->z = v->z / length;
		}
	}

	WeaponDef* BG_GetWeaponDef(int weaponId) {
		int WeapDefId = ((int(*)(...))0x820D5A10)(weaponId, 0);
		return *(WeaponDef**)(0x825E0318 + ((WeapDefId * 4) & 0x3FC));
	}

	bool IsAlive(int clientNum, bool notPlayer) {
		if (notPlayer) {
			return (Native::Custom::GetCEntity()[clientNum].m_alive == 1) || (Native::Custom::GetCEntity()[clientNum].m_alive == 3) || (Native::Custom::GetCEntity()[clientNum].m_alive == 4);
		}

		return (Native::Custom::GetCEntity()[clientNum].m_alive == 1) && strcmp(Native::Custom::GetCG()->m_clientInfo[clientNum].m_headModel, "");
	}

	bool IsFriend(int clientNum) {
		return Native::Custom::GetCG()->m_clientInfo[clientNum].m_team == Native::Custom::GetCG()->m_clientInfo[Native::Custom::GetCG()->m_clientNum].m_team && strcmp(Native::Engine::Dvar_GetString("ui_gametype"), "dm");
	}

	float FloatMinMax(float min, float max) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float range = max - min;
		return (random * range) + min;
	}

	const char* StartWhitespace(const char* text, int spaces) {
		string newMessage = text;
		for (int i = 0; i < spaces; i++) newMessage = " " + newMessage;
		return newMessage.c_str();
	}

	Material* GetWeaponShader(int weaponId) {
		WeaponDef *WeapDef = BG_GetWeaponDef(weaponId);
		return WeapDef->m_killIcon;
	}

	bool WorldToScreen(D3DXVECTOR3 world, float* x, float* y) {
		CgArray* cg = Native::Custom::GetCG();

		D3DXVECTOR3 position;
		position.x = world.x - cg->m_refdef.m_origin.x;
		position.y = world.y - cg->m_refdef.m_origin.y;
		position.z = world.z - cg->m_refdef.m_origin.z;

		D3DXVECTOR3 transform;
		transform.x = (position.x * cg->m_refdef.m_viewAxis[1].x) + (position.y * cg->m_refdef.m_viewAxis[1].y) + (position.z * cg->m_refdef.m_viewAxis[1].z);
		transform.y = (position.x * cg->m_refdef.m_viewAxis[2].x) + (position.y * cg->m_refdef.m_viewAxis[2].y) + (position.z * cg->m_refdef.m_viewAxis[2].z);
		transform.z = (position.x * cg->m_refdef.m_viewAxis[0].x) + (position.y * cg->m_refdef.m_viewAxis[0].y) + (position.z * cg->m_refdef.m_viewAxis[0].z);

		if (transform.z < 0.1f) {
			return false;
		}

		*x = GetMenu()->m_center.x * (1 - (transform.x / cg->m_refdef.m_fov.x / transform.z));
		*y = GetMenu()->m_center.y * (1 - (transform.y / cg->m_refdef.m_fov.y / transform.z));

		return true;
	}

	void ClientCallback(bool excludeSelf, void(*function)(CgArray* cg_t, CEntity entity, ClientInfo clientInfo)) {
		if (IsInGame()) {
			auto cg_t = Native::Custom::GetCG();
			for (int i = 0; i < Native::Custom::GetServerInfo()->m_maxClients; i++) {
				if (i == cg_t->m_clientNum && excludeSelf) continue;

				auto entity = Native::Custom::GetCEntity()[i];
				
				if (Utils::IsAlive(i)) {
					function(cg_t, entity, cg_t->m_clientInfo[i]);
				}
			}
		}
	}

	int GetRandomIntInRange(int from, int to) {
		srand(time(NULL));

		if (from == 0) {
			return rand() % (to + 1);
		}

		return rand() % to + from;
	}

	char* va(const char* fmt, ...) {
		memset(vaBuffer, 0, 0x1000);
		va_list ap;
		va_start(ap, fmt);
		vsprintf(vaBuffer, fmt, ap);
		va_end(ap);
		return vaBuffer;
	}

	bool IsInGame() {
		return Native::Engine::Dvar_GetBool("cl_ingame") && Native::Custom::GetCG() != nullptr;
	}

	DWORD Joaat(const char* value) {
		size_t len = strlen(value);
		unsigned int hash, i;
		for (hash = i = 0; i < len; ++i) {
			hash += tolower(value[i]);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);
		return hash;
	}

	DWORD timeGetTime() {
		return ((DWORD(*)())0x824F0458)();
	}

	void vaBuff(char* buffer, const char* fmt, ...) {
		va_list ap;
		va_start(ap, fmt);
		vsprintf(buffer, fmt, ap);
		va_end(ap);
	}

	float GetDistance(D3DXVECTOR3 start, D3DXVECTOR3 end) {
		float dx = end.x - start.x;
		float dy = end.y - start.y;
		float dz = end.z - start.z;
		return (float)sqrt((dx * dx) + (dy * dy) + (dz * dz)) * 0.03048f;
	}

	void Ease(float& toEase, float& easeFrom, float multiplier) {
		toEase += toEase < easeFrom ? abs(toEase - easeFrom) / multiplier : -abs(toEase - easeFrom) / multiplier;
	}

	DWORD ResolveFunction(HMODULE moduleHandle, DWORD Ordinal) {
		DWORD address;
		Native::System::XexGetProcedureAddress(moduleHandle, Ordinal, &address);
		return address;
	}

	double trunc(double d) {
		return (d > 0) ? floor(d) : ceil(d);
	}

	handle_t HandleHookAllocTemp(int arg, handleCallback function, clock_t start_time) {
		if (function == NULL)
			return INVALID_HANDLE;
		handle_s* ev;
		for (int i = 1; i < HANDLE_COUNT; i++) {
			ev = &Backend::m_hookEvents[i];
			if (ev->function == function && ev->arg == arg)
				return i;
			else if (ev->function == NULL) {
				ev->arg = arg;
				ev->function = function;
				ev->type = CALL_TEMP;
				ev->handle = NULL;
				ev->interval = 0;
				ev->startTime = *(int*)(0x82FDA080 + 0x4A8) + start_time;
				ev->time = ev->startTime;
				return (handle_t)i;
			}
		}
		return INVALID_HANDLE;
	}

	void InitializeOnceInGame(bool* value, void(*function)()) {
		if (IsInGame()) {
			if (!*value) {
				*value = true;
				function();
			}
		}
		else {
			if (*value)
				*value = false;
		}
	}

	bool DoesDirExist(const char* in) {
		DWORD ftyp = GetFileAttributesA(in);
		if (ftyp == -1)
			return false;

		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
			return true;

		return false;
	}

	void AngleVectors(float* angles, D3DXVECTOR3* forward, D3DXVECTOR3* right, D3DXVECTOR3* up) {
		float angle;
		float sr, sp, sy, cr, cp, cy;
		angle = angles[1] * (PI * 2.0f / 360.0f);
		sy = sinf(angle);
		cy = cosf(angle);
		angle = angles[0] * (PI * 2.0f / 360.0f);
		sp = sinf(angle);
		cp = cosf(angle);
		angle = angles[2] * (PI * 2.0f / 360.0f);
		sr = sinf(angle);
		cr = cosf(angle);

		if (forward) {
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}
		if (right) {
			right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
			right->y = (-1 * sr*sp*sy + -1 * cr*cy);
			right->z = -1 * sr*cp;
		}
		if (up) {
			up->x = (cr*sp*cy + -sr * -sy);
			up->y = (cr*sp*sy + -sr * cy);
			up->z = cr * cp;
		}
	}

	short AngleToShort(float x) {
		return ((int)((x) * 65536 / 360) & 65535);
	}

	int ShortToAngle(short x) {
		return ((x)*(360.0 / 65536));
	}

	void VectorSubtract(D3DXVECTOR3* a, D3DXVECTOR3* b, D3DXVECTOR3* c) {
		c->x = a->x - b->x;
		c->y = a->y - b->y;
		c->z = a->z - b->z;
	}

	int CompileBranchLink(DWORD F, DWORD T) {
		if (F > T)
			return 0x4C000000 - (F - T) + 1;
		else if (F < T)
			return T - F + 0x48000000 + 1;
		else
			return 0x48000001;
	}
}