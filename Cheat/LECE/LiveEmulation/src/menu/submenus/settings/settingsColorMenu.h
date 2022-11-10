#pragma once
#include "../../submenu.h"

class SettingsColorMenu : public Submenu {
public:
	static SettingsColorMenu* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	SettingsColorMenu() : Submenu() {}
	~SettingsColorMenu();
};

namespace SettingsColorMenuVars
{
	struct Vars {
		Color m_syncAimbot;
	};
}