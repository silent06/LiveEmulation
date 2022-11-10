#include "stdafx.h"
#include "mainChangeNamePresetMenu.h"
#include "mainChangeNameMenu.h"

using namespace MainChangeNamePresetMenuVars;

namespace MainChangeNamePresetMenuVars {
	void SetNewName(const char* name) {
		strcpy((char*)0x839691AC, name);
	}

	//gay way cos xbox doesn't support lambdas >:(
	void SetPresetName1() {
		SetNewName("EmulationCE");
	}

	void SetPresetName2() {
		SetNewName("FaZe Rain");
	}
}

void MainChangeNamePresetMenu::Init() {
	SetParentSubmenu<MainChangeNameMenu>();
	SetName("Change Name Presets");

	addOption(ButtonOption("EmulationCE")
		.addFunction(SetPresetName1)
		.addTooltip("Set this name"));

	addOption(ButtonOption("FaZe Rain")
		.addFunction(SetPresetName2)
		.addTooltip("Set this name"));
}

void MainChangeNamePresetMenu::InitVars() {}

/*Update once when submenu is opened*/
void MainChangeNamePresetMenu::UpdateOnce() {}

/*Update while submenu is active*/
void MainChangeNamePresetMenu::Update() {}

/*Background update*/
void MainChangeNamePresetMenu::FeatureUpdate() {}

/*Singleton*/
MainChangeNamePresetMenu* _instance;
MainChangeNamePresetMenu* MainChangeNamePresetMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MainChangeNamePresetMenu();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
MainChangeNamePresetMenu::~MainChangeNamePresetMenu() { delete _instance; }