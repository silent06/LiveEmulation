#include "stdafx.h"
#include "visualIndicator.h"
#include "visualESP.h"
#include "visualIndicatorSettings.h"

using namespace VisualIndicatorSettingsMenuVars;

namespace VisualIndicatorSettingsMenuVars {
	Vars vars;
}

void VisualIndicatorSettings::Init() {
	SetParentSubmenu<VisualIndicator>();
	SetName("Indicator Settings");

	addOption(NumberOption<float>(SCROLL, "Radius")
		.addNumber(vars.cg_hudGrenadeIconOffset, "%.0f", 1.0f).addMin(25.f).addMax(500.f).setScrollSpeed(10)
		.addFunction(VisualIndicatorMenuVars::InitRadius)
		.addTooltip("Edit the radius that the indicator draws"));

	addOption(NumberOption<float>(SCROLL, "Icon Width")
		.addNumber(vars.cg_hudGrenadeIconWidth, "%.0f", 1.0f).addMin(1.0f).addMax(100.f).setScrollSpeed(10)
		.addFunction(VisualIndicatorMenuVars::InitRadius)
		.addTooltip("Edit the width of the icons (weapons, scav packs etc)"));

	addOption(NumberOption<float>(SCROLL, "Icon Height")
		.addNumber(vars.cg_hudGrenadeIconHeight, "%.0f", 1.0f).addMin(1.0f).addMax(100.f).setScrollSpeed(10)
		.addFunction(VisualIndicatorMenuVars::InitRadius)
		.addTooltip("Edit the height of the icons (weapons, scav packs etc)"));

	addOption(NumberOption<float>(SCROLL, "Pointer Width")
		.addNumber(vars.cg_hudGrenadePointerWidth, "%.0f", 1.0f).addMin(1.0f).addMax(100.f).setScrollSpeed(10)
		.addFunction(VisualIndicatorMenuVars::InitRadius)
		.addTooltip("Edit the width of the pointer (weapons)"));

	addOption(NumberOption<float>(SCROLL, "Pointer Height")
		.addNumber(vars.cg_hudGrenadePointerHeight, "%.0f", 1.0f).addMin(1.0f).addMax(100.f).setScrollSpeed(10)
		.addFunction(VisualIndicatorMenuVars::InitRadius)
		.addTooltip("Edit the height of the pointer (weapons)"));

	addOption(NumberOption<float>(SCROLL, "Player Pointer Width")
		.addNumber(vars.m_playerPointerSize.x, "%.0f", 1.0f).addMin(1.0f).addMax(100.f).setScrollSpeed(10)
		.addTooltip("Edit the width of the pointer (players)"));

	addOption(NumberOption<float>(SCROLL, "Player Pointer Height")
		.addNumber(vars.m_playerPointerSize.y, "%.0f", 1.0f).addMin(1.0f).addMax(100.f).setScrollSpeed(10)
		.addTooltip("Edit the height of the pointer (players)"));

	addOption(ColorOption("Friendly Pointer")
		.addColor(vars.m_friendlyPlayerPointerColor)
		.addTooltip("Change the color of the friendly player indicator"));

	addOption(ColorOption("Enemy Pointer")
		.addColor(vars.m_enemyPlayerPointerColor)
		.addTooltip("Change the color of the enemy player indicator"));

	addOption(ColorOption("Selected Target Pointer")
		.addColor(vars.m_selectedPlayerTargetPointerColor)
		.addTooltip("Change the color of the selected target indicator (from aimbot)"));

	addOption(ColorOption("Weapon Pointer")
		.addColor(vars.m_weaponPointerColor)
		.addTooltip("Change the color of the weapon pointer"));

	addOption(ColorOption("Weapon Icon")
		.addColor(vars.m_weaponIconColor)
		.addTooltip("Change the color of the weapon icon"));

	addOption(ColorOption("Scavenger Pack Pointer")
		.addColor(vars.m_scavengerPointerColor)
		.addTooltip("Change the color of the scavenger pointer"));

	addOption(ColorOption("Throwable Pointer")
		.addColor(vars.m_throwablePointerColor)
		.addTooltip("Change the color of the throwable item pointer"));
}

void VisualIndicatorSettings::InitVars() {
	vars.cg_hudGrenadeIconOffset = 100.f; // default 50.f
	vars.cg_hudGrenadeIconWidth = 50.f; // default 25.f
	vars.cg_hudGrenadeIconHeight = 30.f; // default 25.f
	vars.cg_hudGrenadePointerWidth = 30.f; // default 25.f
	vars.cg_hudGrenadePointerHeight = 20.f; // default 12.f

	vars.m_friendlyPlayerPointerColor = Color(0x37c775FF);
	vars.m_enemyPlayerPointerColor = Color(0xc73737FF);
	vars.m_selectedPlayerTargetPointerColor = Color(0xFFFF00FF);
	vars.m_weaponPointerColor = Color(0xc155e8FF);
	vars.m_weaponIconColor = Color(0xFFFFFFFF);
	vars.m_scavengerPointerColor = Color(0xe155e8FF);
	vars.m_throwablePointerColor = Color(0xe455e8FF);

	vars.m_playerPointerSize = Vector2(25.f, 11.f);
}

/*Update once when submenu is opened*/
void VisualIndicatorSettings::UpdateOnce() {}

/*Update while submenu is active*/
void VisualIndicatorSettings::Update() {}

/*Background update*/
void VisualIndicatorSettings::FeatureUpdate() {}

/*Singleton*/
VisualIndicatorSettings* _instance;
VisualIndicatorSettings* VisualIndicatorSettings::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VisualIndicatorSettings();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
VisualIndicatorSettings::~VisualIndicatorSettings() { delete _instance; }