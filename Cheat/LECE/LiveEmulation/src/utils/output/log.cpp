#include "stdafx.h"

void Log::Initialize() {
	printf("[LE_CE] Log allocated\n");


}

const char* Log::GetTimeAsString() {
	tm timeStruct;
	time_t currentTime = time(nullptr);
	localtime_s(&timeStruct, &currentTime);

	static char buffer[50];
	sprintf(buffer, "[%02d:%02d:%02d]", timeStruct.tm_hour, timeStruct.tm_min, timeStruct.tm_sec);
	return buffer;
}

void Log::Message(bool notify, const char* format, ...) {
	char buffer[0x700];

	va_list va;

	va_start(va, format);
	vsprintf_s(buffer, format, va);
	va_end(va);

	char buffer2[0x700];
	sprintf(buffer2, "%s[LE_CE] %s\n", GetTimeAsString(), buffer);

	//LogToFile(buffer2);
	printf(buffer2);

	if (notify) {
		Notify((char*)buffer);
	}
}