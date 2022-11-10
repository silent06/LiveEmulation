#include "stdafx.h"
#include "visualESPSettings.h"
#include "visualESP.h"

using namespace VisualESPSettingsMenuVars;

namespace VisualESPSettingsMenuVars {
	Vars vars;
}

void VisualESPSettingsMenu::Init() {
	SetParentSubmenu<VisualESPMenu>();
	SetName("Visual ESP Settings");

	addOption(ColorOption("Friendly Name Text")
		.addColor(VisualESPMenuVars::vars.m_nameTextFriendlies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Enemy Name Text")
		.addColor(VisualESPMenuVars::vars.m_nameTextEnemies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Friendly Name Box Shader")
		.addColor(VisualESPMenuVars::vars.m_nameBoxFriendlies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Enemy Name Box Shader")
		.addColor(VisualESPMenuVars::vars.m_nameBoxEnemies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Friendly Snapline")
		.addColor(VisualESPMenuVars::vars.m_snaplineFriendlies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Enemy Snapline")
		.addColor(VisualESPMenuVars::vars.m_snaplineEnemies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Snapline Aimbot Target")
		.addColor(VisualESPMenuVars::vars.m_snaplineAimbotTarget)
		.addTooltip("Change this color"));

	addOption(ColorOption("Friendly Bones")
		.addColor(VisualESPMenuVars::vars.m_bonesFriendlies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Enemy Bones")
		.addColor(VisualESPMenuVars::vars.m_bonesEnemies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Bones Aimbot Target")
		.addColor(VisualESPMenuVars::vars.m_bonesAimbotTarget)
		.addTooltip("Change this color"));

	addOption(ColorOption("Friendly Corner")
		.addColor(VisualESPMenuVars::vars.m_cornerFriendlies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Enemy Corner")
		.addColor(VisualESPMenuVars::vars.m_cornerEnemies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Corner Aimbot Target")
		.addColor(VisualESPMenuVars::vars.m_cornerAimbotTarget)
		.addTooltip("Change this color"));

	addOption(ColorOption("Friendly Weapon Shader")
		.addColor(VisualESPMenuVars::vars.m_weaponShadersFriendlies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Enemy Weapon Shader")
		.addColor(VisualESPMenuVars::vars.m_weaponShadersEnemies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Weapon Shader Aimbot Target")
		.addColor(VisualESPMenuVars::vars.m_weaponShadersAimbotTarget)
		.addTooltip("Change this color"));

	addOption(ColorOption("Friendly Filled")
		.addColor(VisualESPMenuVars::vars.m_filledFriendlies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Enemy Filled")
		.addColor(VisualESPMenuVars::vars.m_filledEnemies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Filled Aimbot Target")
		.addColor(VisualESPMenuVars::vars.m_filledAimbotTarget)
		.addTooltip("Change this color"));

	addOption(ColorOption("Friendly 2D")
		.addColor(VisualESPMenuVars::vars.m_2dFriendlies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Enemy 2D")
		.addColor(VisualESPMenuVars::vars.m_2dEnemies)
		.addTooltip("Change this color"));

	addOption(ColorOption("2D Aimbot Target")
		.addColor(VisualESPMenuVars::vars.m_2dAimbotTarget)
		.addTooltip("Change this color"));

	addOption(ColorOption("Friendly 3D")
		.addColor(VisualESPMenuVars::vars.m_3dFriendlies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Enemy 3D")
		.addColor(VisualESPMenuVars::vars.m_3dEnemies)
		.addTooltip("Change this color"));

	addOption(ColorOption("3D Aimbot Target")
		.addColor(VisualESPMenuVars::vars.m_3dAimbotTarget)
		.addTooltip("Change this color"));

	addOption(ColorOption("Friendly Pyramid")
		.addColor(VisualESPMenuVars::vars.m_pyramidFriendlies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Enemy Pyramid")
		.addColor(VisualESPMenuVars::vars.m_pyramidEnemies)
		.addTooltip("Change this color"));

	addOption(ColorOption("Pyramid Aimbot Target")
		.addColor(VisualESPMenuVars::vars.m_pyramidAimbotTarget)
		.addTooltip("Change this color"));
}

void VisualESPSettingsMenu::InitVars() {}

/*Update once when submenu is opened*/
void VisualESPSettingsMenu::UpdateOnce() {}

/*Update while submenu is active*/
void VisualESPSettingsMenu::Update() {}

/*Background update*/
void VisualESPSettingsMenu::FeatureUpdate() {}

/*Singleton*/
VisualESPSettingsMenu* _instance;
VisualESPSettingsMenu* VisualESPSettingsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VisualESPSettingsMenu();
		_instance->InitVars();
		_instance->Init();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
VisualESPSettingsMenu::~VisualESPSettingsMenu() { delete _instance; }