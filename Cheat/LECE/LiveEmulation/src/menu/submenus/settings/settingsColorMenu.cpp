#include "stdafx.h"
#include "settingsColorMenu.h"
#include "../settingsMenu.h"
#include "settingsColorRainbow.h"
#include "menu/submenus/visuals/visualESP.h"
#include "menu/submenus/visuals/visualIndicatorSettings.h"

using namespace SettingsColorMenuVars;

namespace SettingsColorMenuVars {
	Vars vars;

	void SyncAimbotColor() {
		VisualESPMenuVars::vars.m_snaplineAimbotTarget = vars.m_syncAimbot;
		VisualESPMenuVars::vars.m_bonesAimbotTarget = vars.m_syncAimbot;
		VisualESPMenuVars::vars.m_2dAimbotTarget = vars.m_syncAimbot;
		VisualESPMenuVars::vars.m_3dAimbotTarget = vars.m_syncAimbot;
		VisualESPMenuVars::vars.m_pyramidAimbotTarget = vars.m_syncAimbot;
		VisualESPMenuVars::vars.m_filledAimbotTarget = vars.m_syncAimbot;
		VisualESPMenuVars::vars.m_cornerAimbotTarget = vars.m_syncAimbot;
		VisualESPMenuVars::vars.m_weaponShadersAimbotTarget = vars.m_syncAimbot;

		VisualIndicatorSettingsMenuVars::vars.m_selectedPlayerTargetPointerColor = vars.m_syncAimbot;
	}
}

void SettingsColorMenu::Init() {
	SetParentSubmenu<SettingsMenu>();
	SetName("Color");

	addOption(SubmenuOption("Global Rainbow Manager")
		.addSubmenu<SettingsColorRainbow>()
		.addTooltip("Manage rainbow on a global scale"));

	addOption(BreakOption("Global Editing"));

	addOption(ColorOption("Sync Aimbot Target Color")
		.addColor(vars.m_syncAimbot).canBeSaved()
		.addFunction(SyncAimbotColor)
		.addTooltip("Sync every aimbot target color to this color"));

	addOption(BreakOption("Misc"));

	addOption(ColorOption("Visual Health Bar")
		.addColor(VisualsMenuVars::vars.m_healthColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Visual Health Bar Text")
		.addColor(VisualsMenuVars::vars.m_healthTextColor)
		.addTooltip("Change this color"));

	addOption(BreakOption("User Interface"));

	addOption(ColorOption("Title")
		.addColor(GetRenderer()->UIColors.titleColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Color Grid Border")
		.addColor(GetMenuInput()->m_gridColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Slider Option Border")
		.addColor(GetRenderer()->UIColors.sliderBoxOutlineColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Header Background")
		.addColor(GetRenderer()->UIColors.headerBackgroundColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Header Border")
		.addColor(GetRenderer()->UIColors.headerBorderColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Sub Header Border")
		.addColor(GetRenderer()->UIColors.subHeaderBorderColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Sub Header Background")
		.addColor(GetRenderer()->UIColors.subHeaderBackgroundColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Sub Header Selector")
		.addColor(GetRenderer()->UIColors.subHeaderSelectorColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Sub Header Selector Crevice")
		.addColor(GetRenderer()->UIColors.subHeaderSelectorCreviceColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Sub Header Text")
		.addColor(GetRenderer()->UIColors.subHeaderTextColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Top Right Border")
		.addColor(GetRenderer()->UIColors.topRightBorderColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Top Right Background")
		.addColor(GetRenderer()->UIColors.topRightBackgroundColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Top Right Text")
		.addColor(GetRenderer()->UIColors.topRightTextColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Background Border")
		.addColor(GetRenderer()->UIColors.backgroundBorderColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Background")
		.addColor(GetRenderer()->UIColors.backgroundColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Scroll Bar Border")
		.addColor(GetRenderer()->UIColors.scrollBarBorderColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Scroll Bar Background")
		.addColor(GetRenderer()->UIColors.scrollBarBackgroundColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Scroll Bar")
		.addColor(GetRenderer()->UIColors.scrollBarColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Option Scroll Bar")
		.addColor(GetRenderer()->UIColors.optionScrollBarColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Footer Border")
		.addColor(GetRenderer()->UIColors.footerBorderColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Footer Background")
		.addColor(GetRenderer()->UIColors.footerBackgroundColor)
		.addTooltip("Change this color"));

	addOption(ColorOption("Tooltip Text")
		.addColor(GetRenderer()->UIColors.tooltipColor)
		.addTooltip("Change this color"));
}

void SettingsColorMenu::InitVars() {
	vars.m_syncAimbot = Color(255, 255, 0);
}

/*Update once when submenu is opened*/
void SettingsColorMenu::UpdateOnce() {}

/*Update while submenu is active*/
void SettingsColorMenu::Update() {}

/*Background update*/
void SettingsColorMenu::FeatureUpdate() {}

/*Singleton*/
SettingsColorMenu* _instance;
SettingsColorMenu* SettingsColorMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new SettingsColorMenu();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
SettingsColorMenu::~SettingsColorMenu() { delete _instance; }