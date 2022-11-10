#include "stdafx.h"
#include "visualReticleSettings.h"
#include "visualReticle.h"

using namespace VisualReticleSettingsMenuVars;

namespace VisualReticleSettingsMenuVars {
	Vars vars;
}

void VisualReticleSettingsMenu::Init() {
	SetParentSubmenu<VisualReticleMenu>();
	SetName("Reticle Settings");

	addOption(ColorOption("Color")
		.addColor(VisualReticleMenuVars::vars.m_color)
		.addTooltip("Set the color"));

	addOption(NumberOption<int>(SCROLL, "Scale")
		.addNumber(VisualReticleMenuVars::vars.m_scale, "%d", 1).addMin(0).addMax(250).setScrollSpeed(10)
		.addTooltip("Set the scale"));

	addOption(NumberOption<int>(SCROLL, "Rotation")
		.addNumber(VisualReticleMenuVars::vars.m_rotation, "%d", 1).addMin(0).addMax(360).setScrollSpeed(10)
		.addTooltip("Set the rotation"));

	addOption(ToggleOption("Loop Rotate Reticle")
		.addToggle(VisualReticleMenuVars::vars.m_rotate).canBeSaved()
		.addTooltip("Rotate the reticle"));

	addOption(NumberOption<int>(SCROLL, "Loop Rotation Delay")
		.addNumber(VisualReticleMenuVars::vars.m_rotateSpeed, "%d", 1).addMin(0).addMax(300).setScrollSpeed(10)
		.addTooltip("Set the looped rotation speed"));

	addOption(NumberOption<int>(SCROLL, "Loop Rotation Increment")
		.addNumber(VisualReticleMenuVars::vars.m_rotateIncrement, "%d", 1).addMin(1).addMax(50).setScrollSpeed(50)
		.addTooltip("Set the rotation increment per tick"));
}

void VisualReticleSettingsMenu::InitVars() {}

/*Update once when submenu is opened*/
void VisualReticleSettingsMenu::UpdateOnce() {}

/*Update while submenu is active*/
void VisualReticleSettingsMenu::Update() {}

/*Background update*/
void VisualReticleSettingsMenu::FeatureUpdate() {}

/*Singleton*/
VisualReticleSettingsMenu* _instance;
VisualReticleSettingsMenu* VisualReticleSettingsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VisualReticleSettingsMenu();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
VisualReticleSettingsMenu::~VisualReticleSettingsMenu() { delete _instance; }