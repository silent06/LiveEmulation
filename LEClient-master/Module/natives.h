#pragma once
#include "stdafx.h"

namespace Native {
	namespace Kernel {

		static DWORD XexLoadImageFromMemory(void* Image, DWORD ImageSize, LPCSTR ImageName) {
			return Invoker::Invoke<DWORD>(0xf2520dc8, Image, ImageSize, ImageName);
		}

		static void* RtlImageXexHeaderField(void* XexHeaderBase, DWORD ImageField) {
			return Invoker::Invoke<void*>(0x83ae755b, XexHeaderBase, ImageField);
		}

		static bool MmIsAddressValid(void* Address) {
			return Invoker::Invoke<bool>(0xdb1bc5c2, Address);
		}

		static void* XeCryptHmacShaFinal(PXECRYPT_HMACSHA_STATE pHmacShaState, BYTE* pbOut, DWORD cbOut) {
			return Invoker::Invoke<void*>(0x4c0c7fea, pHmacShaState, pbOut, cbOut);
		}

		static void* XeCryptHmacShaUpdate(PXECRYPT_HMACSHA_STATE pHmacShaState, const BYTE* pbInp, DWORD cbInp) {
			return Invoker::Invoke<void*>(0x4bf3197d, pHmacShaState, pbInp, cbInp);
		}

		static void* XeCryptHmacShaInit(PXECRYPT_HMACSHA_STATE pHmacShaState, const BYTE* pbKey, DWORD cbKey) {
			return Invoker::Invoke<void*>(0xd632634b, pHmacShaState, pbKey, cbKey);
		}

		static UCHAR KeGetCurrentProcessType() {
			return Invoker::Invoke<UCHAR>(0x81439a1c);
		}

		static void* XeCryptDes3Ecb(CONST PXECRYPT_DES3_STATE pDes3State, CONST BYTE* pbInp, BYTE* pbOut, bool fEncrypt) {
			return Invoker::Invoke<void*>(0x6ea2ec59, pDes3State, pbInp, pbOut, fEncrypt);
		}

		static void* XeCryptDes3Key(PXECRYPT_DES3_STATE pDes3State, CONST BYTE* pbKey) {
			return Invoker::Invoke<void*>(0xd837b9a9, pDes3State, pbKey);
		}

		static void* XeCryptAesEcb(CONST PXECRYPT_AES_STATE pAesState, CONST BYTE* pbInp, BYTE* pbOut, bool fEncrypt) {
			return Invoker::Invoke<void*>(0xaf78a41, pAesState, pbInp, pbOut, fEncrypt);
		}

		static void* XeCryptAesCbc(PXECRYPT_AES_STATE pAesState, CONST BYTE* pbInp, DWORD cbInp, BYTE* pbOut, BYTE* pbFeed, bool fEncrypt) {
			return Invoker::Invoke<void*>(0xc64271e5, pAesState, pbInp, cbInp, pbOut, pbFeed, fEncrypt);
		}

		static void* XeCryptAesKey(PXECRYPT_AES_STATE pAesState, BYTE* pbKey) {
			return Invoker::Invoke<void*>(0xa9c2c539, pAesState, pbKey);
		}

		static void* XeCryptHmacSha(CONST BYTE* pbKey, DWORD cbKey, CONST BYTE* pbInp1, DWORD cbInp1, BYTE* pbOut, DWORD cbOut) {
			return Invoker::Invoke<void*>(0x8dd778c6, pbKey, cbKey, pbInp1, cbInp1, pbOut, cbOut);
		}

		static void* XeCryptRotSumSha(CONST BYTE* pbInp1, DWORD cbInp1, CONST BYTE* pbInp2, DWORD cbInp2, BYTE* pbOut, DWORD cbOut) {
			return Invoker::Invoke<void*>(0x5a629980, pbInp1, cbInp1, pbInp2, cbInp2, pbOut, cbOut);
		}

		static void* XeCryptMd5(CONST BYTE* pbInp1, DWORD cbInp1, CONST BYTE* pbInp2, DWORD cbInp2, CONST BYTE* pbInp3, DWORD cbInp3, BYTE* pbOut, DWORD cbOut) {
			return Invoker::Invoke<void*>(0x69e0a6d1, pbInp1, cbInp1, pbInp2, cbInp2, pbInp3, cbInp3, pbOut, cbOut);
		}

		static void* XeCryptSha(CONST BYTE* pbInp1, DWORD cbInp1, CONST BYTE* pbInp2, DWORD cbInp2, CONST BYTE* pbInp3, DWORD cbInp3, BYTE* pbOut, DWORD cbOut) {
			return Invoker::Invoke<void*>(0xb815b8e4, pbInp1, cbInp1, pbInp2, cbInp2, pbInp3, cbInp3, pbOut, cbOut);
		}

		static void* XeCryptRc4(CONST BYTE* pbKey, DWORD cbKey, BYTE* pbInpOut, DWORD cbInpOut) {
			return Invoker::Invoke<void*>(0xd1f3bce2, pbKey, cbKey, pbInpOut, cbInpOut);
		}

		static void* XexUnloadImage(HANDLE ModuhLE) {
			return Invoker::Invoke<void*>(0xdfe009f8, ModuhLE);
		}

		static NTSTATUS XexLoadExecutable(CHAR* szXexName, PHANDLE pHandle, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion) {
			return Invoker::Invoke<NTSTATUS>(0xbd329ed7, szXexName, pHandle, dwModuleTypeFlags, dwMinimumVersion);
		}

		static NTSTATUS XexLoadImage(LPCSTR szXexName, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion, PHANDLE pHandle) {
			return Invoker::Invoke<NTSTATUS>(0x43664b40, szXexName, dwModuleTypeFlags, dwMinimumVersion, pHandle);
		}

		static bool XexGetModuleSection(HANDLE ModuhLE, LPCSTR szSectionName, void** pSectionData, ULONG* pSectionSize) {
			return Invoker::Invoke<bool>(0x5e48a4f8, ModuhLE, szSectionName, pSectionData, pSectionSize);
		}

		static void* XexPcToFileHeader(void* Address, PLDR_DATA_TABLE_ENTRY* ldatOut) {
			return Invoker::Invoke<void*>(0x77a9b548, Address, ldatOut);
		}

		static DWORD ExCreateThread(PHANDLE pHandle, DWORD dwStackSize, DWORD* lpThreadId, void* apiThreadStartup, LPTHREAD_START_ROUTINE lpStartAddress, void* lpParameter, DWORD dwCreationFlagsMod) {
			return Invoker::Invoke<DWORD>(0x8546aae1, pHandle, dwStackSize, lpThreadId, apiThreadStartup, lpStartAddress, lpParameter, dwCreationFlagsMod);
		}

		static HRESULT ObCreateSymbolicLink(PSTRING SymbolicLinkName, PSTRING DeviceName, bool System) {
			return Invoker::Invoke<HRESULT>(0xfe805bca, SymbolicLinkName, DeviceName, System);
		}

		static void* RtlInitAnsiString(PANSI_STRING DestinationString, PCSZ DeviceName) {
			return Invoker::Invoke<void*>(0xd8f258c5, DestinationString, DeviceName);
		}

		static NTSTATUS ExSetXConfigSetting(WORD Category, WORD Setting, void* Buffer, WORD szSetting) {
			return Invoker::Invoke<NTSTATUS>(0xa2ec093b, Category, Setting, Buffer, szSetting);
		}

		static NTSTATUS ExGetXConfigSetting(WORD Category, WORD Setting, void* Buffer, WORD Size, PWORD szSetting) {
			return Invoker::Invoke<NTSTATUS>(0x3c2abdb8, Category, Setting, Buffer, Size, szSetting);
		}

		static void* MmGetPhysicalAddress(void* Address) {
			return Invoker::Invoke<void*>(0x267c460a, Address);
		}

		static void* VdDisplayFatalError() {
			return Invoker::Invoke<void*>(0x393f3210);
		}

		static void* HalReturnToFirmware(FIRMWARE_REENTRY PowerDownMode) {
			return Invoker::Invoke<void*>(0xbcd16d0b, PowerDownMode);
		}

		static void* XeCryptRandom(BYTE* Bytes, DWORD AmountOfBytes) {
			return Invoker::Invoke<void*>(0x81f34970, Bytes, AmountOfBytes);
		}
	}
	namespace Xam {

		static DWORD XNetLogonGetExtendedStatus(DWORD* In, DWORD* Out) {
			return Invoker::Invoke<DWORD>(0x2b3d1421, In, Out);
		}

		static bool XamCacheReset(XAM_CACHE_FILE_TYPE FileType) {
			return Invoker::Invoke<bool>(0x26d06702, FileType);
		}

		static INT NetDll_XNetXnAddrToMachineId(XNCALLER_TYPE xnc, CONST XNADDR pxnaddr, ULONGLONG pqwMachineId) {
			return Invoker::Invoke<INT>(0x97a22c07, xnc, pxnaddr, pqwMachineId);
		}

		static void* XNotifyQueueUI(PWCHAR pwszStringParam) {
			return Invoker::Invoke<void*>(0xc9711e8b, pwszStringParam);
		}

		static void* XNotifyUIGetOptions(bool* pfShow, bool* pfShowMovie, bool* pfPlaySound, bool* pfShowIPTV) {
			return Invoker::Invoke<void*>(0xb523ea72, pfShow, pfShowMovie, pfPlaySound, pfShowIPTV);
		}

		static void* XNotifyUISetOptions(bool pfShow, bool pfShowMovie, bool pfPlaySound, bool pfShowIPTV) {
			return Invoker::Invoke<void*>(0xf11ab7cf, pfShow, pfShowMovie, pfPlaySound, pfShowIPTV);
		}

		static DWORD XamGetCurrentTitleId() {
			return Invoker::Invoke<DWORD>(0x6e19abd);
		}

		static void* Sleep(DWORD Milliseconds) {
			return Invoker::Invoke<void*>(0xeefa61ff, Milliseconds);
		}

		static DVD_TRAY_STATE XamLoaderGetDvdTrayState() {
			return Invoker::Invoke<DVD_TRAY_STATE>(0x5fe9b7e2);
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
	}
}
