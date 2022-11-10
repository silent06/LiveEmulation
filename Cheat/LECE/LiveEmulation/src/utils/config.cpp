#include "stdafx.h"

Config g_config;

vector<ConfigItems> Config::m_configItemsToSave;

Config* GetConfig() {
	return &g_config;
}