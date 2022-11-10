#pragma once

#define LOG(notify, fmt, ...) Log::Message(notify, fmt, ##__VA_ARGS__);

class Log {
public:
	static void Initialize();
	static const char* GetTimeAsString();
	static void Message(bool notify, const char* format, ...);
};