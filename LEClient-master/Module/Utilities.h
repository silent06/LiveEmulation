#include "stdafx.h"
#pragma once

#ifdef DEBUG
#define DebugPrint(x, ...) Utilities::LOG("[LE \"%s\":%d] %s -> " x "\n", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#else
#define DebugPrint(...)
#endif

class MemoryBuffer {
public:
	MemoryBuffer(DWORD Size = 0x200)
	{
		m_Buffer = 0;
		m_DataLength = 0;
		m_BufferSize = 0;
		if ((Size != 0) && (Size < UINT_MAX))
		{
			m_Buffer = (PBYTE)malloc(Size + 1);
			if (m_Buffer)
			{
				m_BufferSize = Size;
				m_Buffer[0] = 0;
			}
		}
	}
	~MemoryBuffer()
	{
		if (m_Buffer) free(m_Buffer);
		m_Buffer = 0;
		m_DataLength = 0;
		m_BufferSize = 0;
	}
	BOOL CheckSize(DWORD Size)
	{
		if (m_BufferSize >= (m_DataLength + Size)) return TRUE;
		else
		{
			DWORD NewSize = max((m_DataLength + Size), (m_BufferSize * 2));
			PBYTE NewBuffer = (PBYTE)realloc(m_Buffer, NewSize + 1);
			if (NewBuffer)
			{
				m_BufferSize = NewSize;
				m_Buffer = NewBuffer;
				return TRUE;
			}
			else return FALSE;
		}
	}
	VOID Add(CONST PVOID Buffer, DWORD Size)
	{
		if (CheckSize(Size))
		{
			memcpy(m_Buffer + m_DataLength, Buffer, Size);
			m_DataLength += Size;
			*(m_Buffer + m_DataLength) = 0;
		}
	}
	DWORD GetSize() CONST { return m_DataLength; }
	PBYTE GetBuffer() CONST { return m_Buffer; }
private:
	PBYTE m_Buffer;
	DWORD m_DataLength;
	DWORD m_BufferSize;
};
BOOL CReadFile(CONST PCHAR FilePath, MemoryBuffer& Buffer);

class Utilities {
public:
	static string GetModuleNameFromAddress(DWORD dwAddress);
	//static DWORD Resolve(HMODULE Module, int ordinal);
	static void XNotifyQueueUI(unsigned int Icon, wchar_t* Notification);
#ifdef DEBUG
	static void LOG(const CHAR* strFormat, ...);
#endif
	static bool GetSectionInfo(DWORD* pdwAddress, DWORD* pdwLength, const char* pchSectionName);
	static PWCHAR vaS(const char* Text, ...);
	static void Reverse(BYTE* pbData, DWORD cbData);
	static void FreezeXbox();
	static void SendToDash(PWCHAR Text, ...);
	static void StartThread(LPTHREAD_START_ROUTINE lpStartAddress);
	static void XNotifyUI(PWCHAR Text);
	static void XNotifyUI(CHAR* Text, ...);
	static void PatchInJump(DWORD* Address, DWORD Destination, bool Linked);
	static void SetSignInStrings();
	static void ApplyPatchData(void* buffer);
	static void HookFunctionStart(DWORD* Address, DWORD* SaveStub, DWORD Destination);
	static void SetLiveBlock(bool enabled);
	static std::string BytesToString(const unsigned char *data, size_t len);
	static bool CompareData(BYTE* target1, BYTE* target2);
	static bool IsNumberBetween(INT Max, INT Min, INT Num);
	static bool IsBufferEmpty(CHAR* Buffer);
	static bool IsBufferEmpty(BYTE* Buffer);
	static bool CReadFile(const char* FilePath, BYTE* pBuffer, DWORD dwSize);
	static bool CWriteFile(const char* FilePath, const void* pData, DWORD dwSize);
	static bool FileExists(LPCSTR FileName);

	static DWORD GetFunctionSize(DWORD* Function);
	static DWORD SetupResources();
	static DWORD MountPath(std::string &device, std::string &link, bool System);
	static DWORD PatchModuleImport(CHAR* Module, CHAR* ImportedModuleName, DWORD Ordinal, DWORD PatchAddress);
	static DWORD PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, CHAR* ImportedModuleName, DWORD Ordinal, DWORD PatchAddress);
	static DWORD HookFunctionStub(CHAR* ModuleName, DWORD Ordinal, void* Destination);
	static DWORD HookFunctionStub(DWORD _Address, void* Function);
	static int RandomInRange(int Min, int Max);
	static BYTE* GetHVCpukey();
	static BYTE* GetFuseCpukey();
	static FARPROC SecureResolveFunction(DWORD dwOrdinal);
	static FARPROC ResolveFunction(HMODULE hHandle, DWORD Ordinal);
	static FARPROC ResolveFunction(CHAR* ModuleName, DWORD Ordinal);
};

std::string BytesToString(const unsigned char *data, size_t len);