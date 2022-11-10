#include "stdafx.h"
#include "visualRadarSettings.h"
#include "visualRadar.h"

using namespace VisualRadarSettingsMenuVars;

namespace VisualRadarSettingsMenuVars {
	Vars vars;
}

void VisualRadarSettingsMenu::Init() {
	SetParentSubmenu<VisualRadarMenu>();
	SetName("Radar Settings");

	addOption(ColorOption("Outline")
		.addColor(vars.m_outline)
		.addTooltip("Change this color"));

	addOption(ColorOption("Map")
		.addColor(vars.m_map)
		.addTooltip("Change this color"));

	addOption(ColorOption("Self Icon")
		.addColor(vars.m_selfIcon)
		.addTooltip("Change this color"));

	addOption(ColorOption("Friendly Icons")
		.addColor(vars.m_friendlyIcon)
		.addTooltip("Change this color"));

	addOption(ColorOption("Enemy Icons")
		.addColor(vars.m_enemyIcon)
		.addTooltip("Change this color"));

	addOption(ColorOption("Turrets")
		.addColor(vars.m_turrets)
		.addTooltip("Change this color"));

	addOption(ColorOption("Missiles")
		.addColor(vars.m_missiles)
		.addTooltip("Change this color"));

	addOption(ColorOption("Helicopters")
		.addColor(vars.m_helicopters)
		.addTooltip("Change this color"));

	addOption(ColorOption("Crates")
		.addColor(vars.m_crates)
		.addTooltip("Change this color"));
}

void VisualRadarSettingsMenu::InitVars(){
	vars.m_selfIcon = Color(0x17c775FF);
	vars.m_friendlyIcon = Color(0x37c775FF);
	vars.m_enemyIcon = Color(0xc73737FF);
	vars.m_turrets = Color(0xFFFFFFFF);
	vars.m_missiles = Color(0xFFFFFFFF);
	vars.m_helicopters = Color(0xFFFFFFFF);
	vars.m_map = Color(0xFFFFFFFF);
	vars.m_crates = Color(0xFFFFFFFF);
	vars.m_outline = Color(0x00cc99FF);
}

/*Update once when submenu is opened*/
void VisualRadarSettingsMenu::UpdateOnce() {}

/*Update while submenu is active*/
void VisualRadarSettingsMenu::Update() {}

/*Background update*/
void VisualRadarSettingsMenu::FeatureUpdate() {}

/*Singleton*/
VisualRadarSettingsMenu* _instance;
VisualRadarSettingsMenu* VisualRadarSettingsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VisualRadarSettingsMenu();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
VisualRadarSettingsMenu::~VisualRadarSettingsMenu() { delete _instance; }