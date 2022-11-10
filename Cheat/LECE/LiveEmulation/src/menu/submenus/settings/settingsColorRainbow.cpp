#include "stdafx.h"
#include "settingsColorRainbow.h"
#include "settingsColorMenu.h"
#include "menu/submenus/visuals/visualIndicatorSettings.h"
#include "menu/submenus/visuals/visualReticle.h"
#include "menu/submenus/visuals/visualRadarSettings.h"

using namespace SettingsColorRainbowVars;

namespace SettingsColorRainbowVars {
	Vars vars;
	
	void UpdateColors() {
		for (int i = 0; i < vars.m_colors.size(); i++) {
			auto colorPtr = vars.m_colors.at(i).first;
			bool toggle = *vars.m_colors.at(i).second;

			if (toggle) {
				if (!vars.m_rainbow.Exists(colorPtr)) {
					vars.m_rainbow.Add(colorPtr);
				}
			} else {
				if (vars.m_rainbow.Exists(colorPtr)) {
					vars.m_rainbow.Remove(colorPtr);
				}
			}
		}
	}

	void ToggleCallback() {
		if (vars.m_enable) {
			vars.m_rainbow.Start();
		} else {
			vars.m_rainbow.Stop();
		}
	}
}

void SettingsColorRainbow::Init() {
	SetParentSubmenu<SettingsColorMenu>();
	SetName("Global Rainbow Manager");

	addOption(ToggleOption("Enable Rainbow")
		.addToggle(vars.m_enable).canBeSaved()
		.addFunction(ToggleCallback)
		.addTooltip("Enable the rainbow"));

	addOption(BreakOption("User Interface"));

	addOption(ToggleOption("Title")
		.addToggle(vars.m_ui[0]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Color Grid Border")
		.addToggle(vars.m_ui[20]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Slider Option Border")
		.addToggle(vars.m_ui[21]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Header Background")
		.addToggle(vars.m_ui[1]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Header Border")
		.addToggle(vars.m_ui[2]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Sub Header Border")
		.addToggle(vars.m_ui[3]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Sub Header Background")
		.addToggle(vars.m_ui[4]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Sub Header Selector")
		.addToggle(vars.m_ui[5]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Sub Header Selector Crevice")
		.addToggle(vars.m_ui[6]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Sub Header Text")
		.addToggle(vars.m_ui[7]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Top Right Border")
		.addToggle(vars.m_ui[8]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Top Right Background")
		.addToggle(vars.m_ui[9]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Top Right Text")
		.addToggle(vars.m_ui[10]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Background Border")
		.addToggle(vars.m_ui[11]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Background")
		.addToggle(vars.m_ui[12]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Scroll Bar Border")
		.addToggle(vars.m_ui[13]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Scroll Bar Background")
		.addToggle(vars.m_ui[14]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Scroll Bar")
		.addToggle(vars.m_ui[15]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Option Scroll Bar")
		.addToggle(vars.m_ui[16]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Footer Border")
		.addToggle(vars.m_ui[17]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Footer Background")
		.addToggle(vars.m_ui[18]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Tooltip Text")
		.addToggle(vars.m_ui[19]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));
	
	addOption(BreakOption("Visuals"));

	addOption(ToggleOption("Visual Health Bar")
		.addToggle(vars.m_visuals[0]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Visual Health Bar Text")
		.addToggle(vars.m_visuals[1]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(BreakOption("Indicators"));

	addOption(ToggleOption("Friendly Pointer")
		.addToggle(vars.m_indicators[0]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Enemy Pointer")
		.addToggle(vars.m_indicators[1]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Selected Target Pointer")
		.addToggle(vars.m_indicators[2]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Weapon Pointer")
		.addToggle(vars.m_indicators[3]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Weapon Icon")
		.addToggle(vars.m_indicators[4]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Scavenger Pack Pointer")
		.addToggle(vars.m_indicators[5]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Throwable Pointer")
		.addToggle(vars.m_indicators[6]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(BreakOption("Reticle"));

	addOption(ToggleOption("Reticle")
		.addToggle(vars.m_reticle).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(BreakOption("Radar"));

	addOption(ToggleOption("Outline")
		.addToggle(vars.m_radar[0]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Map")
		.addToggle(vars.m_radar[1]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Self Icon")
		.addToggle(vars.m_radar[2]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Friendly Icons")
		.addToggle(vars.m_radar[3]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Enemy Icons")
		.addToggle(vars.m_radar[4]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Turrets")
		.addToggle(vars.m_radar[5]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Missiles")
		.addToggle(vars.m_radar[6]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Helicopters")
		.addToggle(vars.m_radar[7]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));

	addOption(ToggleOption("Crates")
		.addToggle(vars.m_radar[8]).canBeSaved()
		.addFunction(UpdateColors)
		.addTooltip("Change this color"));
}

void SettingsColorRainbow::InitVars() {
	vars.m_colors.push_back(make_pair(&VisualRadarSettingsMenuVars::vars.m_outline.m_color, &vars.m_radar[0]));
	vars.m_colors.push_back(make_pair(&VisualRadarSettingsMenuVars::vars.m_map.m_color, &vars.m_radar[1]));
	vars.m_colors.push_back(make_pair(&VisualRadarSettingsMenuVars::vars.m_selfIcon.m_color, &vars.m_radar[2]));
	vars.m_colors.push_back(make_pair(&VisualRadarSettingsMenuVars::vars.m_friendlyIcon.m_color, &vars.m_radar[3]));
	vars.m_colors.push_back(make_pair(&VisualRadarSettingsMenuVars::vars.m_enemyIcon.m_color, &vars.m_radar[4]));
	vars.m_colors.push_back(make_pair(&VisualRadarSettingsMenuVars::vars.m_turrets.m_color, &vars.m_radar[5]));
	vars.m_colors.push_back(make_pair(&VisualRadarSettingsMenuVars::vars.m_missiles.m_color, &vars.m_radar[6]));
	vars.m_colors.push_back(make_pair(&VisualRadarSettingsMenuVars::vars.m_helicopters.m_color, &vars.m_radar[7]));
	vars.m_colors.push_back(make_pair(&VisualRadarSettingsMenuVars::vars.m_crates.m_color, &vars.m_radar[8]));

	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.titleColor.m_color, &vars.m_ui[0]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.headerBackgroundColor.m_color, &vars.m_ui[1]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.headerBorderColor.m_color, &vars.m_ui[2]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.subHeaderBorderColor.m_color, &vars.m_ui[3]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.subHeaderBackgroundColor.m_color, &vars.m_ui[4]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.subHeaderSelectorColor.m_color, &vars.m_ui[5]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.subHeaderSelectorCreviceColor.m_color, &vars.m_ui[6]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.subHeaderTextColor.m_color, &vars.m_ui[7]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.topRightBorderColor.m_color, &vars.m_ui[8]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.topRightBackgroundColor.m_color, &vars.m_ui[9]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.topRightTextColor.m_color, &vars.m_ui[10]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.backgroundBorderColor.m_color, &vars.m_ui[11]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.backgroundColor.m_color, &vars.m_ui[12]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.scrollBarBorderColor.m_color, &vars.m_ui[13]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.scrollBarBackgroundColor.m_color, &vars.m_ui[14]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.scrollBarColor.m_color, &vars.m_ui[15]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.optionScrollBarColor.m_color, &vars.m_ui[16]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.footerBorderColor.m_color, &vars.m_ui[17]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.footerBackgroundColor.m_color, &vars.m_ui[18]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.tooltipColor.m_color, &vars.m_ui[19]));
	vars.m_colors.push_back(make_pair(&GetMenuInput()->m_gridColor.m_color, &vars.m_ui[20]));
	vars.m_colors.push_back(make_pair(&GetRenderer()->UIColors.sliderBoxOutlineColor.m_color, &vars.m_ui[21]));

	vars.m_colors.push_back(make_pair(&VisualIndicatorSettingsMenuVars::vars.m_friendlyPlayerPointerColor.m_color, &vars.m_indicators[0]));
	vars.m_colors.push_back(make_pair(&VisualIndicatorSettingsMenuVars::vars.m_enemyPlayerPointerColor.m_color, &vars.m_indicators[1]));
	vars.m_colors.push_back(make_pair(&VisualIndicatorSettingsMenuVars::vars.m_selectedPlayerTargetPointerColor.m_color, &vars.m_indicators[2]));
	vars.m_colors.push_back(make_pair(&VisualIndicatorSettingsMenuVars::vars.m_weaponPointerColor.m_color, &vars.m_indicators[3]));
	vars.m_colors.push_back(make_pair(&VisualIndicatorSettingsMenuVars::vars.m_weaponIconColor.m_color, &vars.m_indicators[4]));
	vars.m_colors.push_back(make_pair(&VisualIndicatorSettingsMenuVars::vars.m_scavengerPointerColor.m_color, &vars.m_indicators[5]));
	vars.m_colors.push_back(make_pair(&VisualIndicatorSettingsMenuVars::vars.m_throwablePointerColor.m_color, &vars.m_indicators[6]));

	vars.m_colors.push_back(make_pair(&VisualReticleMenuVars::vars.m_color.m_color, &vars.m_reticle));

	vars.m_colors.push_back(make_pair(&VisualsMenuVars::vars.m_healthColor.m_color, &vars.m_visuals[0]));
	vars.m_colors.push_back(make_pair(&VisualsMenuVars::vars.m_healthTextColor.m_color, &vars.m_visuals[1]));
}

/*Update once when submenu is opened*/
void SettingsColorRainbow::UpdateOnce() {}

/*Update while submenu is active*/
void SettingsColorRainbow::Update() {}

/*Background update*/
void SettingsColorRainbow::FeatureUpdate() {
	if (vars.m_enable) {
		vars.m_rainbow.Run();
	}
}

/*Singleton*/
SettingsColorRainbow* _instance;
SettingsColorRainbow* SettingsColorRainbow::GetInstance() {
	if (_instance == nullptr) {
		_instance = new SettingsColorRainbow();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
SettingsColorRainbow::~SettingsColorRainbow() { delete _instance; }