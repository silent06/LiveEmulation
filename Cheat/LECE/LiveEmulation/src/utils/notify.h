#pragma once

enum eNotifyTypes {
	NOTIFY_X,
	NOTIFY_BOX
};

static void XNotifyQueueUI(DWORD dwType, DWORD dwUserIndex, DWORD dwPriority, LPCWSTR pwszStringParam, ULONGLONG qwParam) {
	static const DWORD address = Utils::ResolveFunction(GetModuleHandleA("xam.xex"), 0x290);
	return ((void(*)(DWORD dwType, DWORD dwUserIndex, DWORD dwPriority, LPCWSTR pwszStringParam, ULONGLONG qwParam))address)(dwType, dwUserIndex, dwPriority, pwszStringParam, qwParam);
}

static void XNotifyThread(PWCHAR NotifyText) {
	XNotifyQueueUI(14, 0, 2, NotifyText, NULL);
}

class Notify {
public:
	Notify(const char* message, eNotifyTypes type = NOTIFY_X) {
		const size_t cSize = strlen(message) + 1;
		wchar_t* wc = new wchar_t[cSize];
		mbstowcs(wc, message, cSize);

		if (type == NOTIFY_X) {
			if (KeGetCurrentProcessType() != USER_PROC) {
				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)XNotifyThread, (LPVOID)wc, 0, NULL);
			} else XNotifyThread(wc);
		}

		delete[] wc;
	}
};