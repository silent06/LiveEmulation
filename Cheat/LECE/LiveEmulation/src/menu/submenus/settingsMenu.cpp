#include "stdafx.h"
#include "settingsMenu.h"
#include "settings/settingsColorMenu.h"
#include "settings/settingsConfig.h"

using namespace SettingsMenuVars;

namespace SettingsMenuVars {
	Vars vars;
}

void SettingsMenu::Init() {
	SetName("LiveEmulation Cheat Engine", "Settings");

	addOption(SubmenuOption("Colors")
		.addSubmenu<SettingsColorMenu>()
		.addTooltip("Color options"));

	addOption(SubmenuOption("Config")
		.addSubmenu<SettingsConfigMenu>()
		.addTooltip("Load/save config"));

	addOption(ToggleOption("Developer Customisation")
		.addToggle(vars.m_developer).canBeSaved()
		.addTooltip("Enable editing of options that aren't visible by default"));
}

void SettingsMenu::InitVars() { }

/*Update once when submenu is opened*/
void SettingsMenu::UpdateOnce() {}

/*Update while submenu is active*/
void SettingsMenu::Update() {}

/*Background update*/
void SettingsMenu::FeatureUpdate() { };

/*Singleton*/
SettingsMenu* _instance;
SettingsMenu* SettingsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new SettingsMenu();
		_instance->InitVars();
		_instance->Init();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
SettingsMenu::~SettingsMenu() { delete _instance; }