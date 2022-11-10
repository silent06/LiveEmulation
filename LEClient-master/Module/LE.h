#include "stdafx.h"

class MemoryManager {
public:
	__declspec(noinline) int GetKeySet(int Index, int Key = 0x50) {
		for (int i = 0; i < Index; i++) {
			Index++;
			Index ^= Key;
		}
		Key ^= Index;
	}

	MemoryManager(void* Function) {
		m_Function = Function;
		m_FunctSize = Utilities::GetFunctionSize((DWORD*)Function);
		m_FunctionData.resize(m_FunctSize);

		for (int i = 0; i < m_FunctSize; i++) {
			m_FunctionData.push_back(*(BYTE*)((DWORD)m_Function + i));
			m_FunctionData.at(i) = m_FunctionData.at(i) ^ (m_FunctSize ^= GetKeySet(m_FunctSize));
			*(BYTE*)((DWORD)m_Function + i) = NULL; // null
		}
	}

	__declspec(noinline) MemoryManager& DeleteAfterCall() {
		m_DeleteAfterCall = true;
		return *this;
	}

	__declspec(noinline) void Begin() {
		// decrypts 
		for (int i = 0; i < m_FunctSize; i++) {
			m_FunctionData.at(i) = m_FunctionData.at(i) ^ (m_FunctSize ^= GetKeySet(m_FunctSize));// de-xor
			*(BYTE*)((DWORD)m_Function + i) = m_FunctionData.at(i); // put back to function
			m_FunctionData.at(i) = m_FunctionData.at(i) ^ (m_FunctSize ^= GetKeySet(m_FunctSize)); // soon as it's written, xor it again
		}
	}

	__declspec(noinline) void End() {
		for (int i = 0; i < m_FunctSize; i++) {
			*(BYTE*)((DWORD)m_Function + i) = NULL; // execution is complete; null again
		}

		if (m_DeleteAfterCall) {
			m_DeleteAfterCall = false;
			for (int i = 0; i < m_FunctSize; i++) {
				*(BYTE*)((DWORD)m_Function + i) = NULL;
			}

			m_FunctionData.clear();
			m_Function = nullptr;
			m_FunctSize = -1;
		}
	}

	template<typename R>
	__declspec(noinline) R Call() {
		Begin();
		R response = ((R(*)())m_Function)();
		End();
		return response;
	}

	template<typename R, typename T1>
	__declspec(noinline) R Call(T1 p1) {
		Begin();
		R response = ((R(*)(T1))m_Function)(p1);
		End();
		return response;
	}

	template<typename R, typename T1, typename T2>
	__declspec(noinline) R Call(T1 p1, T2 p2) {
		Begin();
		R response = ((R(*)(T1, T2))m_Function)(p1, p2);
		End();
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3>
	__declspec(noinline) R Call(T1 p1, T2 p2, T3 p3) {
		Begin();
		R response = ((R(*)(T1, T2, T3))m_Function)(p1, p2, p3);
		End();
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4>
	__declspec(noinline) R Call(T1 p1, T2 p2, T3 p3, T4 p4) {
		Begin();
		R response = ((R(*)(T1, T2, T3, T4))m_Function)(p1, p2, p3, p4);
		End();
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
	__declspec(noinline) R Call(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5) {
		Begin();
		R response = ((R(*)(T1, T2, T3, T4, T5))m_Function)(p1, p2, p3, p4, p5);
		End();
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	__declspec(noinline) R Call(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) {
		Begin();
		R response = ((R(*)(T1, T2, T3, T4, T5, T6))m_Function)(p1, p2, p3, p4, p5, p6);
		End();
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	__declspec(noinline) R Call(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7) {
		Begin();
		R response = ((R(*)(T1, T2, T3, T4, T5, T6, T7))m_Function)(p1, p2, p3, p4, p5, p6, p7);
		End();
		return response;
	}
private:
	bool m_DeleteAfterCall;
	void* m_Function;
	int m_FunctSize;
	vector<BYTE> m_FunctionData;
};

class LE {
public:

	static HANDLE hLE;
	static HANDLE hXam;
	static HANDLE hKernel;

	static AUTH_STATUS s_Auth_status;
	static DWORD dwMyModuleSize;

	static void Init_thread();
	static DWORD Init();
};
