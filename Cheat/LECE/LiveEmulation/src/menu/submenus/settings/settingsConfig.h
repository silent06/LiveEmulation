#pragma once
#include "menu/submenu.h"

class SettingsConfigMenu : public Submenu {
public:
	static SettingsConfigMenu* GetInstance();
	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	SettingsConfigMenu() : Submenu() {}
	~SettingsConfigMenu();
};

namespace SettingsConfigMenuVars
{
	struct Vars {
		vector<string> m_files;
		const char* m_selectedFile;
	};

	extern Vars vars;
}