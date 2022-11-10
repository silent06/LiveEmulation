#pragma once

#define LAUNCH_MOUNT "LEC:\\"

struct PLUGIN_LOAD_PATH {
	DWORD       magic;
	const char* devicePath;
	const char* iniPath;
};

class Launch {
public:
	static bool Initialize();
private:
	static PLUGIN_LOAD_PATH* m_launchPluginPath;
};