#pragma once
struct Hook {
	const char* m_name;
	DWORD m_address;
	BYTE m_originalAssembly[0x10];
	void* m_hook;
};

class Hooking {
public:
	static DWORD HookFunctionStub(DWORD Address, void* Function, const char* name);
	static DWORD HookFunctionStub(AuthData::eAddresses Address, void* Function, const char* name);
	static void HookFunctionPatch(DWORD Address, void* Function, BOOL Linked = 0);
	static void HookFunctionStart(PDWORD Address, PDWORD SaveStub, DWORD Destination, DWORD expected = -1);

	static vector<Hook> GetHooks();
private:
	static vector<Hook> m_hooks;
	static unsigned char m_hookSection[0x500];
	static unsigned int m_hookCount;
};