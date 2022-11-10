#include "stdafx.h"
#include <unordered_map>
#ifdef DEBUG
CRITICAL_SECTION dbgLock;
bool dbgInit = FALSE;
void Utilities::LOG(CONST CHAR* strFormat, ...) {
	if (!Utilities::FileExists(LOGPath)) 
		Utilities::CWriteFile(LOGPath, NULL, NULL);

	if (dbgInit == FALSE) {
		InitializeCriticalSection(&dbgLock);
		dbgInit = TRUE;
	}

	CHAR buffer[0x3E8];

	va_list pArgList;
	va_start(pArgList, strFormat);
	vsprintf_s(buffer, 0x3E8, strFormat, pArgList);
	va_end(pArgList);

	char TimeBuff[20];
	struct tm *sTm;
	time_t now = time(NULL);
	sTm = gmtime(&now);

	strftime(TimeBuff, sizeof(TimeBuff), "%Y-%m-%d %H:%M:%S", sTm);

	EnterCriticalSection(&dbgLock);
	ofstream writeLog;
	writeLog.open(LOGPath, ofstream::app);
	if (writeLog.is_open()) {
		writeLog.write(TimeBuff, strlen(TimeBuff));
		writeLog.write(" ", 0x1);
		writeLog.write(buffer, strlen(buffer));
		writeLog.write("\r\n", 2);
	}
	writeLog.close();
	LeaveCriticalSection(&dbgLock);
}
#endif

string Utilities::GetModuleNameFromAddress(DWORD dwAddress) {
	auto ldr = reinterpret_cast<PLDR_DATA_TABLE_ENTRY>(GetModuleHandle("xboxkrnl.exe"));
	auto CurrentEntry = ldr->InLoadOrderLinks.Flink;
	PLDR_DATA_TABLE_ENTRY Current = nullptr;

	char buffer[100];
	while (CurrentEntry != &ldr->InLoadOrderLinks && CurrentEntry != nullptr) {
		Current = CONTAINING_RECORD(CurrentEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		if (!Current)
			break;

		if (dwAddress >= reinterpret_cast<DWORD>(Current->ImageBase)) {
			if (dwAddress <= reinterpret_cast<DWORD>(Current->ImageBase) + Current->SizeOfFullImage) {
				wcstombs(buffer, Current->BaseDllName.Buffer, sizeof(buffer));
				return std::string(buffer);
			}
		}
		CurrentEntry = CurrentEntry->Flink;
	}
	return nullptr;
}

char m_hookSection[0x500];
int m_hookCount;
FARPROC ResolveFunction2(HMODULE hHandle, DWORD Ordinal) {
	return (hHandle == NULL) ? NULL : GetProcAddress(hHandle, (LPCSTR)Ordinal);
}
/*DWORD Utilities::Resolve(HMODULE Module, int ordinal) {
	auto ldr = reinterpret_cast<PLDR_DATA_TABLE_ENTRY>(Module);
	auto header = reinterpret_cast<_IMAGE_XEX_HEADER*>(ldr->XexHeaderBase);
	if (header) {
		auto sec = reinterpret_cast<PXEX_SECURITY_INFO>(header->SecurityInfo);
		if (sec) {
			PIMAGE_EXPORT_ADDRESS_TABLE exports = sec->ImageInfo.ExportTableAddress;//ExportTableAddress
			if (exports)
				return exports->ordOffset[ordinal - 1] + (exports->ImageBaseAddress << 16);
		}
	}
	return ERROR_SUCCESS;
}*/

void Utilities::Reverse(BYTE* pbData, DWORD cbData) {
	DWORD dwMidpoint = cbData >> 1;
	DWORD dwReverseCounter = NULL;
	BYTE* pbPtr = pbData;

	if (dwMidpoint) {
		while (dwReverseCounter < dwMidpoint) {
			DWORD i = (cbData - dwReverseCounter) - 1 & 0xFFFFFFFF;

			BYTE b1 = *pbPtr;
			BYTE b2 = pbData[i];
			*pbPtr = b2;
			pbData[i] = b1;

			dwReverseCounter++;
			pbPtr++;
		}
	}
}

#pragma code_seg(push, r1, ".ptext")
DWORD Utilities::GetFunctionSize(DWORD* pdwAddress) {
	DWORD dwAddress = (DWORD)pdwAddress;
	DWORD dwSize = 0;

	while (*(DWORD*)dwAddress != 0x0000000 && *(DWORD*)dwAddress != 0x4E800020) {
		dwAddress += 4;
		dwSize += 4;
	}
	return dwSize > 0 ? (dwSize - 4) : 0;
} 
#pragma code_seg(pop, r1) 

bool Utilities::CompareData(BYTE* target1, BYTE* target2) {
	if (sizeof(target1) != sizeof(target2)) 
		return false;

	for (int i = 0; i < sizeof(target1); i++) 
		if (target1[i] != target2[i])
			return false;
	return true;
}

bool Utilities::IsNumberBetween(INT Max, INT Min, INT Num) {
	return ((Num < Max && !(Num <= Min)) || (Num > Max && !(Num >= Min)));
}

bool Utilities::IsBufferEmpty(CHAR* Buffer) {
	for (DWORD i = 0; i < sizeof(Buffer); i++)
	if (Buffer[i] != NULL)
		return false;
	return true;
}

bool Utilities::IsBufferEmpty(BYTE* Buffer) {
	for (DWORD i = 0; i < sizeof(Buffer); i++)
	if (Buffer[i] != NULL) 
		return false;
	return true;
}

DWORD Utilities::SetupResources() {
	/* Masterkey: 0x9B, 0xBC, 0x90, 0xA6, 0xAE, 0x90, 0xA1, 0x3D, 0xAC, 0x7F, 0x36, 0xC6, 0xE8, 0x0A, 0x8C, 0x02 */

	BYTE MasterKey[0x10];
	BYTE DesKey[0x10] = { 0x8B, 0xD3, 0xCC, 0x2D, 0x37, 0x5D, 0xD9, 0x75, 0xB0, 0x8C, 0xB2, 0x70, 0x05, 0xC6, 0xAA, 0x47 };
	BYTE AesKey[0x10] = { 0xCF, 0x65, 0x30, 0x9F, 0x07, 0xE4, 0x2F, 0xA6, 0xB7, 0x0C, 0x0A, 0x8C, 0x9B, 0x7D, 0x50, 0xF8 };
	BYTE AesIV[0x10] = { 0x94, 0xDA, 0xA3, 0xF5, 0xBB, 0x58, 0x6B, 0x58, 0x14, 0xAD, 0xFD, 0x68, 0x8C, 0x78, 0x2D, 0x7F };
	
	DWORD Aes_SboxKey = 0x90BC;
	DWORD Aes_SboxSz = 0x9;
	CryptoTools::CreateAesSbox(Aes_SboxKey, Aes_SboxSz);
	for (DWORD i = 0; i < sizeof(AesKey); i++) 
		AesKey[i] ^= Aes_SboxKey;

	DWORD Des_SboxKey = 0x653;
	DWORD Des_SboxSz = 0x4;
	CryptoTools::CreateAesSbox(Des_SboxKey, Des_SboxSz);
	for (DWORD i = 0; i < sizeof(DesKey); i++) 
		DesKey[i] ^= Des_SboxKey;

	CryptoTools::AesCbcEncrypt(AesKey, AesIV, AesKey, true, sizeof(AesKey));
	CryptoTools::Des3Ecb(DesKey, DesKey, true);
	CryptoTools::EncryptedMemcpy(AesKey, AesKey, 0x10);
	Native::Kernel::XeCryptSha((BYTE*)DesKey, sizeof(DesKey), AesKey, sizeof(AesKey), AesIV, sizeof(AesIV), MasterKey, 0x10);

	Native::Kernel::XeCryptRc4(MasterKey, sizeof(MasterKey), Xui::XuiData, sizeof(Xui::XuiData));
	//Utilities::CWriteFile(XEX_Path "MasterKey.bin", MasterKey, sizeof(MasterKey));
	return ERROR_SUCCESS;
}

void Utilities::SetSignInStrings() {
	BYTE SignInOrOutEdit[0xA] = { 0xD4, 0xE6, 0xEF, 0xE0, 0xEC, 0xEE, 0xE6, 0xA3, 0xD7, 0xEC };
	BYTE ChooseProfileEdit[0x13] = { 0xD9, 0xE3, 0xEE, 0xF9, 0xCE, 0xEF, 0xED, 0xE8, 0xEF, 0xE4, 0xD2, 0xE4, 0xF3, 0xF7, 0xE4, 0xF3, 0xA1, 0xA0, 0xA1 };

	for (DWORD x = 0; x < sizeof(SignInOrOutEdit); x++) 
		SignInOrOutEdit[x] ^= 0x83;
	for (DWORD x = 0; x < sizeof(ChooseProfileEdit); x++) 
		ChooseProfileEdit[x] ^= 0x81;

	strcpy((CHAR*)0xC248E010, (CHAR*)SignInOrOutEdit);
	strcpy((CHAR*)0xC248DFFC, (CHAR*)ChooseProfileEdit);
}

void Utilities::SendToDash(PWCHAR Text, ...) {
	XLaunchNewImage(nullptr, NULL);
	Utilities::XNotifyUI(Text);
}

BYTE* Utilities::GetHVCpukey() {
	BYTE bCpukey[0x10];

	HV::HvPeekBytes(0x20, bCpukey, 0x10);
	return bCpukey;
}

void Utilities::FreezeXbox() {
	HV::HvxGetVersions(0x72627472, 5, 0x12345678, 0x8000, 0x12345678, 0x12345678);
}

BYTE* Utilities::GetFuseCpukey() {
		
	BYTE bCpukey[0x10];

	QWORD Tmp1 = HV::HvGetFuseLine(3) | HV::HvGetFuseLine(4);
	QWORD Tmp2 = HV::HvGetFuseLine(5) | HV::HvGetFuseLine(6);
	memcpy(bCpukey, &Tmp1, 0x08);
	memcpy(bCpukey + 0x08, &Tmp2, 0x08);
	return bCpukey;
}

void Utilities::StartThread(LPTHREAD_START_ROUTINE lpStartAddress) {
	HANDLE hThread = nullptr;
	DWORD dwThreadId = NULL;

	Native::Kernel::ExCreateThread(&hThread, NULL, &dwThreadId, reinterpret_cast<void*>(XapiThreadStartup), lpStartAddress, nullptr, 0x2 | CREATE_SUSPENDED);
	XSetThreadProcessor(hThread, 0x4);
	SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
	ResumeThread(hThread);
}

void __declspec(naked) GLPR(void) {
	__asm {
		std     r14, -0x98(sp)
		std     r15, -0x90(sp)
		std     r16, -0x88(sp)
		std     r17, -0x80(sp)
		std     r18, -0x78(sp)
		std     r19, -0x70(sp)
		std     r20, -0x68(sp)
		std     r21, -0x60(sp)
		std     r22, -0x58(sp)
		std     r23, -0x50(sp)
		std     r24, -0x48(sp)
		std     r25, -0x40(sp)
		std     r26, -0x38(sp)
		std     r27, -0x30(sp)
		std     r28, -0x28(sp)
		std     r29, -0x20(sp)
		std     r30, -0x18(sp)
		std     r31, -0x10(sp)
		stw     r12, -0x8(sp)
		blr
	}
}

DWORD RelinkGPLR(DWORD SFSOffset, DWORD* SaveStubAddress, DWORD* OriginalAddress) {
	DWORD Instruction = NULL, Replacing;
	DWORD* Saver = (DWORD*)GLPR;

	if (SFSOffset & 0x2000000) 
		SFSOffset |= 0xFC000000;

	Replacing = OriginalAddress[SFSOffset / 0x04];
	for (DWORD i = NULL; i < 0x14; i++) {
		if (Replacing == Saver[i]) {
			DWORD NewAddress = (DWORD)&Saver[i] - (DWORD)SaveStubAddress;
			Instruction = (0x48000001 | (NewAddress & 0x03FFFFFC));
		}
	} 
	return Instruction;
}

void Utilities::HookFunctionStart(DWORD* Address, DWORD* SaveStub, DWORD Destination) {
	if ((Address != NULL) && (SaveStub != NULL)) {
		DWORD Relocation = (DWORD)(&Address[0x04]);
		SaveStub[0x00] = (0x3D600000 + ((Relocation >> 0x10) & 0xFFFF));
		SaveStub[0x01] = (0x616B0000 + (Relocation & 0xFFFF));
		SaveStub[0x02] = 0x7D6903A6;

		for (DWORD i = 0; i < 0x04; i++) {
			if ((Address[i] & 0x48000003) == 0x48000001) 
				SaveStub[i + 0x03] = RelinkGPLR((Address[i] & ~0x48000003), &SaveStub[i + 0x03], &Address[i]);
			else SaveStub[i + 0x03] = Address[i];
		}

		SaveStub[0x07] = 0x4E800420;
		__dcbst(NULL, SaveStub);
		__sync();
		__isync();
		Utilities::PatchInJump(Address, Destination, false);
	}
}

DWORD Utilities::HookFunctionStub(DWORD _Address, void* Function) {
	DWORD* startStub = (DWORD*)&m_hookSection[m_hookCount * 32];
	m_hookCount++;

	for (auto i = 0; i < 7; i++)
		startStub[i] = 0x60000000;
	startStub[7] = 0x4E800020;

	HookFunctionStart((DWORD*)_Address, startStub, (DWORD)Function);
	return (DWORD)startStub;
}

DWORD Utilities::HookFunctionStub(CHAR* ModuleName, DWORD Ordinal, void* Destination) {
	return Utilities::HookFunctionStub((DWORD)Utilities::ResolveFunction(ModuleName, Ordinal), Destination);
}
void Utilities::XNotifyQueueUI(unsigned int Icon, wchar_t* Notification)
{
	((void(*)(...))0x816AAC08)(Icon, 0xFF, 0x2, Notification, 0);// old -> 0x816AAAC8
}
void Utilities::ApplyPatchData(void* buffer) {
	DWORD PatchCount = NULL;
	DWORD* PatchData = (DWORD*)buffer;

	while (*PatchData != 0xFFFFFFFF) {
		memcpy((void*)PatchData[0], &PatchData[2], PatchData[1] * sizeof(DWORD));
		PatchData += (PatchData[1] + 2);
		PatchCount++;
	}
}
PWCHAR Utilities::vaS(const char* Text, ...) {
	CHAR Buffer[0x1000];
	CHAR MessageBuffer[0x100];
	static WCHAR Message[0x100];
	va_list pArgList;
	va_start(pArgList, Text);
	RtlVsprintf(Buffer, Text, pArgList);
	va_end(pArgList);
	RtlSprintf(MessageBuffer, Buffer);
	mbstowcs(Message, MessageBuffer, strlen(MessageBuffer) + 1);
	ZeroMemory(Buffer, sizeof(Buffer));
	ZeroMemory(MessageBuffer, sizeof(MessageBuffer));
	return Message;
}
std::string BytesToString(const unsigned char *data, size_t len) {
	std::stringstream str;
	str.setf(std::ios_base::hex, std::ios::basefield);
	str.setf(std::ios_base::uppercase);
	str.fill('0');

	for (size_t i = 0; i < len; ++i) {
		str << std::setw(2) << (unsigned short)data[i];
	}

	return str.str();
}

void Utilities::PatchInJump(DWORD* Address, DWORD Destination, bool Linked) {
	Address[0] = 0x3D600000 + ((Destination >> 16) & 0xFFFF);
	if (Destination & 0x8000) 
		Address[0] += 1;
	Address[1] = 0x396B0000 + (Destination & 0xFFFF);
	Address[2] = 0x7D6903A6;
	Address[3] = Linked ? 0x4E800421 : 0x4E800420;
}

DWORD Utilities::PatchModuleImport(CHAR* Module, CHAR* ImportedModuleName, DWORD Ordinal, DWORD PatchAddress) {
	LDR_DATA_TABLE_ENTRY* moduhLE = (LDR_DATA_TABLE_ENTRY*)GetModuleHandle(Module);
	return (moduhLE == NULL) ? S_FALSE : PatchModuleImport(moduhLE, ImportedModuleName, Ordinal, PatchAddress);
}

DWORD Utilities::PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, CHAR* ImportedModuleName, DWORD Ordinal, DWORD PatchAddress) {
	DWORD Address = (DWORD)Utilities::ResolveFunction(ImportedModuleName, Ordinal);
	if (Address == NULL)
		return S_FALSE;

	PXEX_IMPORT_DESCRIPTOR importDesc = (PXEX_IMPORT_DESCRIPTOR)RtlImageXexHeaderField(Module->XexHeaderBase, 0x000103FF);
	if (importDesc == NULL) 
		return S_FALSE;

	DWORD Result = 0x2;
	CHAR* stringTable = (CHAR*)(importDesc + 1);
	XEX_IMPORT_TABLE_ORG* importTable = (XEX_IMPORT_TABLE_ORG*)(stringTable + importDesc->NameTableSize);

	for (DWORD x = 0; x < importDesc->ModuleCount; x++) {
		DWORD* importAdd = (DWORD*)(importTable + 1);
		for (DWORD y = 0; y < importTable->ImportTable.ImportCount; y++) {

			if (*(DWORD*)importAdd[y] == Address) {
				CopyMemory((DWORD*)importAdd[y], &PatchAddress, 0x4);
				DWORD newCode[0x4];
				PatchInJump(newCode, PatchAddress, FALSE);
				CopyMemory((DWORD*)importAdd[y + 1], newCode, 0x10);
				Result = S_OK;
			}
		} 
		importTable = (XEX_IMPORT_TABLE_ORG*)(((BYTE*)importTable) + importTable->TableSize);
	}
	return Result;
}

DWORD Utilities::MountPath(std::string &device, std::string &link, bool System) {
	STRING deviceName;
	STRING linkName;

	Native::Kernel::RtlInitAnsiString(&deviceName, device.c_str());
	Native::Kernel::RtlInitAnsiString(&linkName, link.c_str());
	return (SUCCEEDED(Native::Kernel::ObCreateSymbolicLink(&linkName, &deviceName, System))) ? ERROR_SUCCESS : E_FAIL;
}

BOOL CReadFile(CONST PCHAR FilePath, MemoryBuffer& Buffer)
{
	HANDLE Handle = CreateFile(FilePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (Handle == INVALID_HANDLE_VALUE) return FALSE;
	DWORD FileSize = GetFileSize(Handle, 0);
	PBYTE FileBuffer = (PBYTE)malloc(FileSize);
	if (FileBuffer == 0)
	{
		CloseHandle(Handle);
		return FALSE;
	}
	DWORD ReadSize = 0;
	if (!ReadFile(Handle, FileBuffer, FileSize, &ReadSize, 0))
	{
		free(FileBuffer);
		CloseHandle(Handle);
		return FALSE;
	}
	else if (ReadSize != FileSize)
	{
		free(FileBuffer);
		CloseHandle(Handle);
		return FALSE;
	}
	Buffer.Add(FileBuffer, FileSize);
	free(FileBuffer);
	CloseHandle(Handle);
	return TRUE;
}


bool Utilities::CReadFile(const char* FilePath, BYTE* pBuffer, DWORD dwSize) {

	HANDLE hFile = CreateFileA(FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwNumberOfBytesRead;
	BYTE* TempBuffer = nullptr;

	if (hFile == INVALID_HANDLE_VALUE) {

		CloseHandle(hFile);
		return false;
	}

	if (dwSize != GetFileSize(hFile, NULL)) {
		CloseHandle(hFile);
		return false;
	}

	TempBuffer = (BYTE*)malloc(dwSize);
	if (TempBuffer == NULL) {

		free(TempBuffer);
		CloseHandle(hFile);
		return false;
	}

	if (!ReadFile(hFile, TempBuffer, dwSize, &dwNumberOfBytesRead, NULL)) {
		free(TempBuffer);
		CloseHandle(hFile);
		return false;
	}

	memcpy(pBuffer, TempBuffer, dwSize);
	free(TempBuffer);
	CloseHandle(hFile);
	return true;
}

bool Utilities::CWriteFile(const char* FilePath, const void* pData, DWORD dwSize) {
	HANDLE hFile = CreateFileA(FilePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		return false;
	}

	if (WriteFile(hFile, pData, dwSize, &dwSize, NULL) != true) {
		CloseHandle(hFile);
		return false;
	}

	CloseHandle(hFile);
	return true;
}

bool Utilities::FileExists(LPCSTR FileName) {
	if (GetFileAttributes(FileName) == -1) {
		DWORD lastError = GetLastError();
		if (lastError == ERROR_FILE_NOT_FOUND || lastError == ERROR_PATH_NOT_FOUND)
			return false;
	}
	return true;
}

void ToggleNotify(bool Status) {
	bool pfShow = (bool)0xDEADBEEF;
	bool pfShowMovie;
	bool pfPlaySound;
	bool pfShowIPTV;

	if ((int)pfShow == 0xDEADBEEF)  
		Native::Xam::XNotifyUIGetOptions(&pfShow, &pfShowMovie, &pfPlaySound, &pfShowIPTV);
	(!Status) ? 
		Native::Xam::XNotifyUISetOptions(pfShow, pfShowMovie, pfPlaySound, pfShowIPTV) : Native::Xam::XNotifyUISetOptions(TRUE, TRUE, TRUE, TRUE);
	Native::Xam::Sleep(500);
}

void NotifyDoQueueUI(PWCHAR pwszStringParam) {
	ToggleNotify(true);
	Native::Xam::XNotifyQueueUI(pwszStringParam);
}

void Utilities::XNotifyUI(PWCHAR Text) {
	if ((UCHAR)Native::Kernel::KeGetCurrentProcessType() != PROC_USER) {
		HANDLE th = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NotifyDoQueueUI, (void*)Text, CREATE_SUSPENDED, NULL);
		(th != NULL) ? ResumeThread(th) : NULL;
	}
	else 
		NotifyDoQueueUI(Text);
}

void Utilities::XNotifyUI(CHAR* Text, ...) {
	CHAR Buffer[0x100];
	PWCHAR Buffer2 = new WCHAR[0x100];

	va_list pArgList;
	va_start(pArgList, Text);
	vsprintf(Buffer, Text, pArgList);
	va_end(pArgList);

	mbstowcs(Buffer2, Buffer, strlen(Buffer) + 1);
	XNotifyUI(Buffer2);
}

int Utilities::RandomInRange(int Min, int Max) {
	int n = Min - Max + 1;
	int remainder = RAND_MAX % n;
	int x;
	do { 
		x = rand(); 
	} while (x >= RAND_MAX - remainder);
	return Min + x % n;
}

FARPROC Utilities::ResolveFunction(HMODULE hHandle, DWORD Ordinal) {
	return (hHandle == NULL) ? NULL : GetProcAddress(hHandle, (LPCSTR)Ordinal);
}

FARPROC Utilities::ResolveFunction(CHAR* ModuleName, DWORD Ordinal) {
	HMODULE hHandle = GetModuleHandle(ModuleName);
	return (hHandle == NULL) ? NULL : GetProcAddress(hHandle, (LPCSTR)Ordinal);
}

bool Utilities::GetSectionInfo(DWORD* pdwAddress, DWORD* pdwLength, const char* pchSectionName) {
	DWORD SectionInfoOffset = BASEADDY;
	while (!strcmp(FormatUtils::toCHAR(L".rdata"), (CHAR*)SectionInfoOffset) == FALSE) 
		SectionInfoOffset += 0x4;

	PIMAGE_SECTION_HEADER DefaultSections = (PIMAGE_SECTION_HEADER)SectionInfoOffset;
	for (DWORD i = 0; strlen((CHAR*)DefaultSections[i].Name); i++) {
		if (!strcmp(pchSectionName, (CHAR*)DefaultSections[i].Name) == TRUE) {
			*pdwAddress = BASEADDY + _byteswap_ulong(DefaultSections[i].VirtualAddress);
			*pdwLength = _byteswap_ulong(DefaultSections[i].Misc.VirtualSize);
			return true;
		}
	}
	return false;
}

void Utilities::SetLiveBlock(bool Enabled) {
	if (Enabled) {
	    WCHAR* nullStr = L"NO.%sNO.NO\0";
		CopyMemory(reinterpret_cast<BYTE*>(KV::bIsDevkit ? 0x816021C4 : 0x815FF1F4), FormatUtils::toCHAR(nullStr), 0x14); //siflc
		CopyMemory(reinterpret_cast<BYTE*>(KV::bIsDevkit ? 0x816021DC : 0x815FF20C), FormatUtils::toCHAR(nullStr), 0x14); //piflc
		CopyMemory(reinterpret_cast<BYTE*>(KV::bIsDevkit ? 0x816021F4 : 0x815FF224), FormatUtils::toCHAR(nullStr), 0x11); //notice
		CopyMemory(reinterpret_cast<BYTE*>(KV::bIsDevkit ? 0x81602208 : 0x815FF238), FormatUtils::toCHAR(nullStr), 0x14); //xexds
		CopyMemory(reinterpret_cast<BYTE*>(KV::bIsDevkit ? 0x81602238 : 0x815FF250), FormatUtils::toCHAR(nullStr), 0x13); //xeas
		CopyMemory(reinterpret_cast<BYTE*>(KV::bIsDevkit ? 0x81602220 : 0x815FF268), FormatUtils::toCHAR(nullStr), 0x14); //xetgs
		CopyMemory(reinterpret_cast<BYTE*>(KV::bIsDevkit ? 0x8160224C : 0x815FF27C), FormatUtils::toCHAR(nullStr), 0x15); //xemacs
	}
	else {
		CopyMemory(reinterpret_cast<BYTE*>(KV::bIsDevkit ? 0x816021C4 : 0x815FF1F4), FormatUtils::toCHAR(L"SIFLC.%sXBOXLIVE.COM"), 0x14); //siflc
		CopyMemory(reinterpret_cast<BYTE*>(KV::bIsDevkit ? 0x816021DC : 0x815FF20C), FormatUtils::toCHAR(L"PIFLC.%sXBOXLIVE.COM"), 0x14); //piflc
		CopyMemory(reinterpret_cast<BYTE*>(KV::bIsDevkit ? 0x816021F4 : 0x815FF224), FormatUtils::toCHAR(L"NOTICE.%sXBOX.COM"), 0x11); //notice
		CopyMemory(reinterpret_cast<BYTE*>(KV::bIsDevkit ? 0x81602208 : 0x815FF238), FormatUtils::toCHAR(L"XEXDS.%sXBOXLIVE.COM"), 0x14); //xexds
		CopyMemory(reinterpret_cast<BYTE*>(KV::bIsDevkit ? 0x81602220 : 0x815FF250), FormatUtils::toCHAR(L"XETGS.%sXBOXLIVE.COM"), 0x14); //xetgs
		CopyMemory(reinterpret_cast<BYTE*>(KV::bIsDevkit ? 0x81602238 : 0x815FF268), FormatUtils::toCHAR(L"XEAS.%sXBOXLIVE.COM"), 0x13); //xeas
		CopyMemory(reinterpret_cast<BYTE*>(KV::bIsDevkit ? 0x8160224C : 0x815FF27C), FormatUtils::toCHAR(L"XEMACS.%sXBOXLIVE.COM"), 0x15); //xemacs
	} 
}
