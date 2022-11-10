#pragma once
#include "stdafx.h"
#include "Config.h"
#include "Utilities.h"

class NativeCallContext {
protected:
	void* m_return;
	int m_argCount;
	void* m_args;

public:
	template<typename type>
	inline type GetArgument(int Index) {
		intptr_t* args = (intptr_t*)m_args;
		return *(type*)&args[Index];
	}

	template<typename type>
	inline void SetResult(int Index, type Value) {
		intptr_t* returnValue = (intptr_t*)m_return;
		*(type*)&returnValue[Index] = Value;
	}

	template<typename returnType>
	inline returnType GetResult(int Index) {
		intptr_t* returnValue = (intptr_t*)m_return;
		return *(returnType*)&returnValue[Index];
	}

	inline int GetArgCount() {
		return m_argCount;
	}
};

class NativeContext: public NativeCallContext {
private:
	enum {
		maxNativeParams = 20,
		argSize = 4 // PPC is always 4 bytes
	};

	byte m_tempStack[maxNativeParams * argSize];

public:
	inline NativeContext() {
		m_args = &m_tempStack;
		m_return = &m_tempStack;

		m_argCount = 0;

		memset(m_tempStack, 0, sizeof(m_tempStack));
	}

	template<typename type>
	inline void Push(type value) {
		if (sizeof(type) < argSize)
			*(uintptr_t*)(m_tempStack + argSize * m_argCount) = 0;

		*(type*)(m_tempStack + argSize * m_argCount) = value;
		m_argCount++;
	}

	inline void Reverse() {
		uintptr_t tempValues[maxNativeParams];
		uintptr_t* args = (uintptr_t*)m_args;

		for (int i = 0; i < m_argCount; i++) {
			int target = m_argCount - i - 1;
			tempValues[target] = args[i];
		}

		memcpy(m_tempStack, tempValues, sizeof(tempValues));
	}

	template<typename type>
	inline type GetResult() {
		return *(type*)(m_tempStack);
	}
};

class NativeManager : public NativeContext {
public:
	NativeManager()
		:NativeContext()
	{}

	void Reset() {
		m_argCount = 0;
	}

	inline void* GetResultPointer() {
		return m_return;
	}
};

static int NativeRegistrationKeys[28] = {
	0x73B7B7C2,0x65B7B7C3,0x65B7B7C4,0x65B7B7C5,0x65B7B7C6,0x65B7B7C7,0x65B7B7C8,0x65B7B7C9,0x65B7B7d2,0x65B7B7e2,0x65B7B7f2,0x65B7B8C2,0x65B7B9C2,0x65B7B0C2,
	0x65B7B712,0x73B7B722,0x65B7B732,0x73B7B742,0x65B6B7C2,0x65B7B7C2,0x65B5B7C2,0x65B4B7C2,0x65B3B7C2,0x65B327C2,0x71B7B7C2,0x72B7B7C2,0x73B7B7C2,0x65B7B7C2
};


typedef void(*NativeHandler)(NativeCallContext*);

struct NativeRegistration {
	NativeRegistration* m_next;
	NativeHandler m_function[10];
	DWORD m_count;
	DWORD m_address[10];
	bool m_first;

	inline NativeHandler GetFunction(int index) {
		DWORD func = (DWORD)m_function[index];

		func ^= (index + 12) ^ 10;
		func ^= index;
		func += 10;
		func ^= 69;
		func -= 1337;
		func ^= 0x20;
		func ^= 0x10;

		return (NativeHandler)func;
	}

	inline DWORD GetCount() {
		if (!m_first) return 0;
		DWORD count = m_count;

		count ^= NativeRegistrationKeys[2] + 10;
		count ^= NativeRegistrationKeys[3];
		count ^= NativeRegistrationKeys[1];
		count ^= 0x26;
		count ^= 0x25;
		count ^= 0x24;
		count ^= 0x23;
		count ^= 0x22;
		count ^= 0x21;
		count ^= 0x20;
		count -= 1337;
		count ^= 0x12;

		return count;
	}

	inline DWORD GetAddress(int index) {
		DWORD address = m_address[index];

		address ^= (index + 5) ^ 8;
		address ^= index;
		address += 5;
		address ^= 69;
		address -= 1327;
		address ^= 0x40;
		address ^= 0x11;

		return address;
	}

	inline void SetFunction(int index, NativeHandler func) {
		DWORD f = (DWORD)func;

		f ^= 0x10;
		f ^= 0x20;
		f += 1337;
		f ^= 69;
		f -= 10;
		f ^= index;
		f ^= (index + 12) ^ 10;

		m_function[index] = (NativeHandler)f;
	}

	inline void SetCount(DWORD count) {
		m_first = true;

		count ^= 0x12;
		count += 1337;
		count ^= 0x20;
		count ^= 0x21;
		count ^= 0x22;
		count ^= 0x23;
		count ^= 0x24;
		count ^= 0x25;
		count ^= 0x26;
		count ^= NativeRegistrationKeys[1];
		count ^= NativeRegistrationKeys[3];
		count ^= NativeRegistrationKeys[2] + 10;

		m_count = count;
	}

	inline void SetAddress(int index, DWORD address) {
		address ^= 0x11;
		address ^= 0x40;
		address += 1327;
		address ^= 69;
		address -= 5;
		address ^= index;
		address ^= (index + 5) ^ 8;

		m_address[index] = address;
	}
};

class Invoker {
public:
	
	static void test();
	static DWORD RegisterPreAuthNatives();
	static DWORD RegisterPostCryptoNatives();
	static void RegisterNative(DWORD NativeAddress, NativeHandler Handler);
	static NativeHandler GetNativeHandler(DWORD NativeAddress);

	static int* Call(DWORD NativeAddress, NativeManager* Context) {
		int* result = nullptr;
		NativeHandler handler = GetNativeHandler(NativeAddress);
		if (handler) {
			__try {
				handler(Context);
			}
			__except (true) {
#ifdef DEBUG=
				DebugPrint("Failed invoking 0x%X\n", NativeAddress);
#endif
			}
			result = (int*)Context->GetResultPointer();
		}
		return result;
	}

	template<typename R>
	static R Invoke(DWORD NativeAddress) {
		m_context.Reset();
		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1>
	static R Invoke(DWORD NativeAddress, T1 P1) {
		m_context.Reset();

		m_context.Push(P1);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);
		m_context.Push(P12);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);
		m_context.Push(P12);
		m_context.Push(P13);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);
		m_context.Push(P12);
		m_context.Push(P13);
		m_context.Push(P14);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);
		m_context.Push(P12);
		m_context.Push(P13);
		m_context.Push(P14);
		m_context.Push(P15);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);
		m_context.Push(P12);
		m_context.Push(P13);
		m_context.Push(P14);
		m_context.Push(P15);
		m_context.Push(P16);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);
		m_context.Push(P12);
		m_context.Push(P13);
		m_context.Push(P14);
		m_context.Push(P15);
		m_context.Push(P16);
		m_context.Push(P17);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);
		m_context.Push(P12);
		m_context.Push(P13);
		m_context.Push(P14);
		m_context.Push(P15);
		m_context.Push(P16);
		m_context.Push(P17);
		m_context.Push(P18);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18, T19 P19) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);
		m_context.Push(P12);
		m_context.Push(P13);
		m_context.Push(P14);
		m_context.Push(P15);
		m_context.Push(P16);
		m_context.Push(P17);
		m_context.Push(P18);
		m_context.Push(P19);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18, T19 P19, T20 P20) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);
		m_context.Push(P12);
		m_context.Push(P13);
		m_context.Push(P14);
		m_context.Push(P15);
		m_context.Push(P16);
		m_context.Push(P17);
		m_context.Push(P18);
		m_context.Push(P19);
		m_context.Push(P20);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18, T19 P19, T20 P20, T21 P21) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);
		m_context.Push(P12);
		m_context.Push(P13);
		m_context.Push(P14);
		m_context.Push(P15);
		m_context.Push(P16);
		m_context.Push(P17);
		m_context.Push(P18);
		m_context.Push(P19);
		m_context.Push(P20);
		m_context.Push(P21);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18, T19 P19, T20 P20, T21 P21, T22 P22) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);
		m_context.Push(P12);
		m_context.Push(P13);
		m_context.Push(P14);
		m_context.Push(P15);
		m_context.Push(P16);
		m_context.Push(P17);
		m_context.Push(P18);
		m_context.Push(P19);
		m_context.Push(P20);
		m_context.Push(P21);
		m_context.Push(P22);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18, T19 P19, T20 P20, T21 P21, T22 P22, T23 P23) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);
		m_context.Push(P12);
		m_context.Push(P13);
		m_context.Push(P14);
		m_context.Push(P15);
		m_context.Push(P16);
		m_context.Push(P17);
		m_context.Push(P18);
		m_context.Push(P19);
		m_context.Push(P20);
		m_context.Push(P21);
		m_context.Push(P22);
		m_context.Push(P23);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18, T19 P19, T20 P20, T21 P21, T22 P22, T23 P23, T24 P24) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);
		m_context.Push(P12);
		m_context.Push(P13);
		m_context.Push(P14);
		m_context.Push(P15);
		m_context.Push(P16);
		m_context.Push(P17);
		m_context.Push(P18);
		m_context.Push(P19);
		m_context.Push(P20);
		m_context.Push(P21);
		m_context.Push(P22);
		m_context.Push(P23);
		m_context.Push(P24);

		return *(R*)Call(NativeAddress, &m_context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25>
	static R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18, T19 P19, T20 P20, T21 P21, T22 P22, T23 P23, T24 P24, T25 P25) {
		m_context.Reset();

		m_context.Push(P1);
		m_context.Push(P2);
		m_context.Push(P3);
		m_context.Push(P4);
		m_context.Push(P5);
		m_context.Push(P6);
		m_context.Push(P7);
		m_context.Push(P8);
		m_context.Push(P9);
		m_context.Push(P10);
		m_context.Push(P11);
		m_context.Push(P12);
		m_context.Push(P13);
		m_context.Push(P14);
		m_context.Push(P15);
		m_context.Push(P16);
		m_context.Push(P17);
		m_context.Push(P18);
		m_context.Push(P19);
		m_context.Push(P20);
		m_context.Push(P21);
		m_context.Push(P22);
		m_context.Push(P23);
		m_context.Push(P24);
		m_context.Push(P25);

		return *(R*)Call(NativeAddress, &m_context);
	}

private:
	static NativeRegistration g_natives[256];
	static DWORD m_invokingAddress;
	static NativeManager m_context;
};
