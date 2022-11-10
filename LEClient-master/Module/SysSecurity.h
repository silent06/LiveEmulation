#pragma once
#include "stdafx.h"

#define UNLINK(x)                    \
    (x).Flink->Blink = (x).Blink;    \
    (x).Blink->Flink = (x).Flink;
#define RELINK(x, real)            \
    (x).Flink->Blink = (real);    \
    (x).Blink->Flink = (real);    \
    (real)->Blink = (x).Blink;    \
    (real)->Flink = (x).Flink;

class Security {
public:

	static bool IntegrityFailed;
	static vector<pair<DWORD, DWORD>> m_AutomaticWriteBackList;
	static vector<pair<DWORD, DWORD>> hLEReadProtection;
	static vector<pair<DWORD, DWORD>> hLEReadProtectionWhitelist;

	static bool XBDMSanityCheck();

	static void SetupIntegrity();
	static void IntegrityThread();
	static void DebuggerDetectionThread();
	static void UnlinkFromKEB();
	static void ProtectMyMemorySpace();

	static bool MmDbgReadCheckHook(DWORD dwAddress);
	static void AddRegionToWhitelist(DWORD start, DWORD length);
};

struct XbdmCommandTable {
	const char* m_name;
	int m_type;
	DWORD m_functionAddress; // i never update this so i can use it for unhooking

	DWORD m_entryAddress;
	bool m_isHooked;
	int m_vectorIndex;

	friend bool operator==(const XbdmCommandTable& left, const char* right) {
		return !strcmp(left.m_name, right);
	}
};

class Xbdm {
public:
	static void InitializeCommands() {
		if (LE::dwMyModuleSize == NULL) {
#ifdef  DEBUG
			DebugPrint("xbdm hLESize was null!");
#endif
			Native::Kernel::VdDisplayFatalError();
		}

		m_tableAddress = 0x9101ABF8;

		for (auto i = 0; i < 63; i++) {
			DWORD ptr = m_tableAddress + (i * 12);
			XbdmCommandTable entry;

			DWORD strAddress = *reinterpret_cast<DWORD*>(ptr);
			entry.m_name = (char*)strAddress;
			entry.m_type = *(int*)(ptr + 4);
			entry.m_isHooked = false;
			entry.m_functionAddress = *(DWORD*)(ptr + 8);
			entry.m_entryAddress = ptr;
			entry.m_vectorIndex = m_commandTable.size();
			m_commandTable.push_back(entry);

#ifdef DEBUG
			DebugPrint("[XBDM] Found command \"%s\"", entry.m_name);
#endif
		}
	}

	static XbdmCommandTable GetCommandInfo(const char* command) {
		XbdmCommandTable ret;

		auto vit = find(m_commandTable.begin(), m_commandTable.end(), command);
		if (vit != m_commandTable.end()) {
			ret = (*vit);
		}

		return ret;
	}

	static vector<XbdmCommandTable> GetCommands() {
		return m_commandTable;
	}

	static DWORD HookCommand(const char* command, DWORD func) {
		DWORD original = 0;
		auto entry = GetCommandInfo(command);

		if (entry.m_entryAddress) {
			if (entry.m_isHooked) {
#ifdef DEBUG
				DebugPrint("[XBDM] Tried to hook command \"%s\", but is already hooked", command);
#endif
				return 0;
			}

			*(DWORD*)(entry.m_entryAddress + 8) = func;
			m_commandTable[entry.m_vectorIndex].m_isHooked = true;

#ifdef DEBUG
			DebugPrint("[XBDM] Hooked command \"%s\"", command);
#endif
		}

		return original;
	}

	static void UnhookCommand(const char* command) {
		auto entry = GetCommandInfo(command);
		if (entry.m_entryAddress) {
			if (entry.m_isHooked) {
				*(DWORD*)(entry.m_entryAddress + 8) = entry.m_functionAddress;
				m_commandTable[entry.m_vectorIndex].m_isHooked = false;

#ifdef DEBUG
				DebugPrint("[XBDM] Unhooked command \"%s\"", command);
#endif
			}
		}
	}

private:
	static DWORD m_tableAddress;
	static vector<XbdmCommandTable> m_commandTable;
};

class XbdmCommands {
public:
	static DWORD Getmemex(const char* commandString, const char* outBuffer, int len, DWORD* unk) {
		DWORD original = 0;

		string str = string(commandString);
		size_t commandOffset = str.find(' ');
		string command = str.substr(0, commandOffset);
		string next = str.substr(commandOffset + 1);

		size_t addrOffset = next.find(' ');
		string addr = next.substr(0, addrOffset);

		string addrValue = addr.substr(addr.find('=') + 1);

		transform(command.begin(), command.end(), command.begin(), ::tolower);

		auto ptr = Xbdm::GetCommandInfo("getmemex");
		if (ptr.m_functionAddress) {
			original = ptr.m_functionAddress;
		}

		DWORD address = stoul(addrValue);

		// if the start is within our module, we don't care about the length.
		if (address >= BASEADDY && address <= (BASEADDY + LE::dwMyModuleSize)) {
			return 0x82DA000E; // access denied - gives error box that says "Object reference not set to an instance of an object."
		}

		if (original) {
			return reinterpret_cast<DWORD(*)(const char*, const char*, int, DWORD*)>(original)(commandString, outBuffer, len, unk);
		}
		return NULL;
	}

	static DWORD Getmem(const char* commandString, const char* outBuffer, int len, DWORD* unk) {
		DWORD original = 0;

		string str = string(commandString);
		size_t commandOffset = str.find(' ');
		string command = str.substr(0, commandOffset);
		string next = str.substr(commandOffset + 1);

		size_t addrOffset = next.find(' ');
		string addr = next.substr(0, addrOffset);

		string addrValue = addr.substr(addr.find('=') + 1);

		transform(command.begin(), command.end(), command.begin(), ::tolower);

		auto ptr = Xbdm::GetCommandInfo("getmem");
		if (ptr.m_functionAddress) {
			original = ptr.m_functionAddress;
		}

		DWORD address = stoul(addrValue);

		// if the start is within our module, we don't care about the length.
		if (address >= BASEADDY && address <= (BASEADDY + LE::dwMyModuleSize)) {
			return 0x82DA000E; // access denied - gives error box that says "Object reference not set to an instance of an object."
		}

		if (original) {
			return ((DWORD(*)(const char*, const char*, int, DWORD*))original)(commandString, outBuffer, len, unk);
		}
		return NULL;
	}

	static DWORD Setmem(const char* commandString, const char* outBuffer, int len, DWORD* unk) {
		DWORD original = 0;

		string str = string(commandString);
		size_t commandOffset = str.find(' ');
		string command = str.substr(0, commandOffset);
		string next = str.substr(commandOffset + 1);

		size_t addrOffset = next.find(' ');
		string addr = next.substr(0, addrOffset);

		string addrValue = addr.substr(addr.find('=') + 1);

		transform(command.begin(), command.end(), command.begin(), ::tolower);

		auto ptr = Xbdm::GetCommandInfo("setmem");
		if (ptr.m_functionAddress) {
			original = ptr.m_functionAddress;
		}

		// unlike getmem and getmemex, setmem gives us the address in hex format, so we parse it a little different.
		char *_ = 0;
		DWORD address = strtoul(addrValue.c_str(), &_, 0);

		// if the start is within our module, we don't care about the length.
		if (address >= BASEADDY && address <= (BASEADDY + LE::dwMyModuleSize)) {
			return 0x82DA000E; // access denied - gives error box that says "Object reference not set to an instance of an object."
		}

		if (original)
			return reinterpret_cast<DWORD(*)(const char*, const char*, int, DWORD*)>(original)(commandString, outBuffer, len, unk);
		return NULL;
	}

	static DWORD Break(const char* commandString, const char* outBuffer, int len, DWORD* unk) {
		return 0x82DA000E; // access denied - gives error box that says "Object reference not set to an instance of an object."
	}

	static DWORD Continue(const char* commandString, const char* outBuffer, int len, DWORD* unk) {
		return 0x82DA000E; // access denied - gives error box that says "Object reference not set to an instance of an object."
	}

	static DWORD Debugger(const char* commandString, const char* outBuffer, int len, DWORD* unk) {
#ifdef USE_SECURITY
		// when attaching to vs debugger we flag
		if (strstr(commandString, "Visual Studio")) {
			//ProcessMetric(new MetricDebuggerPresent());
		}
		else {
			/* watson or another type of debugger we haven't logged. Let's just restart.
			//Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine); */

			DWORD original = 0;
			auto ptr = Xbdm::GetCommandInfo("debugger");
			if (ptr.m_functionAddress) {
				original = ptr.m_functionAddress;
			}

			if (original)
				return ((DWORD(*)(const char*, const char*, int, DWORD*))original)(commandString, outBuffer, len, unk);
		}
		return 0x82DA000E; // access denied - gives error box that says "Object reference not set to an instance of an object."
#else
		DWORD original = 0;
		auto ptr = Xbdm::GetCommandInfo("debugger");
		if (ptr.m_functionAddress) {
			original = ptr.m_functionAddress;
		}

		if (original)
			return reinterpret_cast<DWORD(*)(const char*, const char*, int, DWORD*)>(original)(commandString, outBuffer, len, unk);
#endif
		return NULL;
	}
};

enum eObfuscationSeverity {
	OBF_DEFAULT,
	OBF_SUPER
};

class FunctionObfuscation {
public:

	FunctionObfuscation(void* func, int size, eObfuscationSeverity severity = OBF_DEFAULT) {
		m_function = func;
		m_severity = severity;
		m_size = size;
		m_functionBytes.resize(size);
		m_deleteNextCall = false;

		switch (m_severity) {
		case OBF_DEFAULT: {
			for (auto i = 0; i < m_size; i++) { // miss BLR
				*reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(m_function) + i) = *reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(m_function) + i) ^ 0x43; // for default we'll just encrypt the function
			}
			break;
		}
		case OBF_SUPER: {
			// the super severity copies the bytes locally, then nulls the func. Then when it's called, they get put back, then called, then repeat.
			for (auto i = 0; i < m_size; i++) {
				m_functionBytes.push_back(*reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(m_function) + i));
				m_functionBytes.at(i) = m_functionBytes.at(i) ^ 0x45; // static xor for now
				*reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(m_function) + i) = 0x0; // null
			}
			break;
		 }
	   }
	}

	__declspec(noinline) FunctionObfuscation& DeleteNextCall() {
		m_deleteNextCall = true;
		return *this;
	}

	__declspec(noinline) void Begin() {
		switch (m_severity) {
		case OBF_DEFAULT: {
			for (auto i = 0; i < m_size; i++) {
				*reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(m_function) + i) = *reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(m_function) + i) ^ 0x43; // de-xor
			}
			break;
		}
		case OBF_SUPER: {
			for (auto i = 0; i < m_size; i++) {
				m_functionBytes.at(i) = m_functionBytes.at(i) ^ 0x45; // de-xor
				*reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(m_function) + i) = m_functionBytes.at(i); // put back to function
				m_functionBytes.at(i) = m_functionBytes.at(i) ^ 0x45; // soon as it's written, xor it again
			}
			break;
		}
		}
	}

	__declspec(noinline) void End() {
		switch (m_severity) {
		case OBF_DEFAULT: {
			for (auto i = 0; i < m_size; i++) {
				*reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(m_function) + i) = *reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(m_function) + i) ^ 0x43; // re-xor
			}
			break;
		}
		case OBF_SUPER: {
			for (auto i = 0; i < m_size; i++) {
				*reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(m_function) + i) = 0x0; // execution is complete; null again
			}
			break;
		 }
		}

		if (m_deleteNextCall) {
			m_deleteNextCall = false;
			for (auto i = 0; i < m_size; i++) {
				*reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(m_function) + i) = 0x0;
			}

			m_functionBytes.clear();
			m_function = nullptr;
			m_size = -1;
		}
	}

	template<typename R>
	__declspec(noinline) R Call() {
		Begin();
		R response = ((R(*)())m_function)();
		End();
		return response;
	}

	template<typename R, typename T1>
	__declspec(noinline) R Call(T1 p1) {
		Begin();
		R response = ((R(*)(T1))m_function)(p1);
		End();
		return response;
	}

	template<typename R, typename T1, typename T2>
	__declspec(noinline) R Call(T1 p1, T2 p2) {
		Begin();
		R response = ((R(*)(T1, T2))m_function)(p1, p2);
		End();
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3>
	__declspec(noinline) R Call(T1 p1, T2 p2, T3 p3) {
		Begin();
		R response = ((R(*)(T1, T2, T3))m_function)(p1, p2, p3);
		End();
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4>
	__declspec(noinline) R Call(T1 p1, T2 p2, T3 p3, T4 p4) {
		Begin();
		R response = ((R(*)(T1, T2, T3, T4))m_function)(p1, p2, p3, p4);
		End();
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
	__declspec(noinline) R Call(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5) {
		Begin();
		R response = ((R(*)(T1, T2, T3, T4, T5))m_function)(p1, p2, p3, p4, p5);
		End();
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	__declspec(noinline) R Call(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) {
		Begin();
		R response = ((R(*)(T1, T2, T3, T4, T5, T6))m_function)(p1, p2, p3, p4, p5, p6);
		End();
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	__declspec(noinline) R Call(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7) {
		Begin();
		R response = ((R(*)(T1, T2, T3, T4, T5, T6, T7))m_function)(p1, p2, p3, p4, p5, p6, p7);
		End();
		return response;
	}

private:
	bool m_deleteNextCall;
	void* m_function;
	int m_size;
	eObfuscationSeverity m_severity;
	vector<BYTE> m_functionBytes;
};

