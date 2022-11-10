#include "stdafx.h"

PLUGIN_LOAD_PATH* Launch::m_launchPluginPath;

bool Launch::Initialize() {
	if ((m_launchPluginPath = (PLUGIN_LOAD_PATH*)Utils::ResolveFunction(g_modules["launch.xex"], 14)) == NULL)
		return false;

	if (m_launchPluginPath->magic == 0x504C5041 && m_launchPluginPath->devicePath != NULL && m_launchPluginPath->iniPath != NULL) {
		string launchDevice;
		string launchFile;

		launchDevice.assign(m_launchPluginPath->devicePath);
		launchFile.assign(m_launchPluginPath->iniPath);

		string link("\\System??\\" LAUNCH_MOUNT);
		string directory(launchDevice);
		directory.append(launchFile.substr(1, launchFile.find_last_of('\\')));

		STRING deviceName;
		STRING linkName;

		RtlInitAnsiString(&deviceName, directory.c_str());
		RtlInitAnsiString(&linkName, link.c_str());

		return (SUCCEEDED(ObCreateSymbolicLink(&linkName, &deviceName)));
	}

	printf("failed to create symbolic link\n");

	return false;
}