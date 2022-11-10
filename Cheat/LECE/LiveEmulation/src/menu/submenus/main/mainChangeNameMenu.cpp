#include "stdafx.h"
#include "mainChangeNameMenu.h"
#include "../mainMenu.h"
#include "mainChangeNamePresetMenu.h"

using namespace MainChangeNameMenuVars;

namespace MainChangeNameMenuVars {
	Vars vars;

	shared_ptr<ButtonOption> m_buttonOption;

	ScrollStruct<const char*> m_nameColors[] = {
		"^7White", "^7",
		"^1Red^7", "^1",
		"^2Green^7", "^2",
		"^3Yellow^7", "^3",
		"^4Blue^7", "^4",
		"^5Cyan^7", "^5",
		"^6Pink^7", "^6",
		"^0Black^7", "^0",
		"^;Yale Blue^7", "^;",
		"^:Light Yellow^7", "^:"
	};

	void SetNewName() {
		strcpy((char*)0x839691AC, vars.m_precompiled);
	}

	void SetDefaultName() {
		strcpy((char*)0x839691AC, vars.m_defaultName);
	}

	void CustomInputName() {
		XOVERLAPPED Overlapped;
		WCHAR wideBuffer[30];
		XShowKeyboardUI(0, VKBD_DEFAULT, L"", L"Emulation CE", L"Enter the name you want to change to", wideBuffer, 30, &Overlapped);

		while (!XHasOverlappedIoCompleted(&Overlapped));
		Sleep(1000);

		char lowKeyboardBuffer[30];
		wcstombs(lowKeyboardBuffer, wideBuffer, 30);

		vars.m_name = lowKeyboardBuffer;
	}
}

void MainChangeNameMenu::Init() {
	SetParentSubmenu<MainMenu>();
	SetName("Change Name");

	addOption(SubmenuOption("Preset Names")
		.addSubmenu<MainChangeNamePresetMenu>()
		.addTooltip("Preset Names"));

	addOption(ToggleOption("Rainbow Gamer tag")
		.addToggle(vars.m_rainbowName)
		.addTooltip("Start a rainbow on your ORIGINAL gamer tag"));

	addOption(ButtonOption("Revert To Default")
		.addFunction(SetDefaultName)
		.addTooltip("Reset your name back to your default name"));

	addOption(ScrollOption<const char*>(SCROLL, "Color")
		.addScroll(vars.m_nameColor, 0, NUMOF(m_nameColors) - 1, m_nameColors)
		.addTooltip("Set the color of your name"));

	addOption(ButtonOption("Custom Input")
		.addFunction(CustomInputName)
		.addTooltip("Enter a custom name"));

	m_buttonOption = addOption(ButtonOption("")
		.addFunction(SetNewName)
		.addTooltip("Set this name"));
}

void MainChangeNameMenu::InitVars() {}

/*Update once when submenu is opened*/
void MainChangeNameMenu::UpdateOnce() {
	if (vars.m_name == nullptr || vars.m_name == "") {
		vars.m_defaultName = (char*)0x839691AC; // xenonuserdata
		vars.m_name = (char*)0x839691AC;
	}
}

/*Update while submenu is active*/
void MainChangeNameMenu::Update() {
	Utils::vaBuff(vars.m_precompiled, "%s%s", m_nameColors[vars.m_nameColor].m_result, vars.m_name);
	m_buttonOption->SetName(Utils::va("Set: %s", vars.m_precompiled));
}

/*Background update*/
void MainChangeNameMenu::FeatureUpdate() {
	if (vars.m_rainbowName) {
		char temp[0x30];
		sprintf(temp, "%s%s", m_nameColors[Utils::GetRandomIntInRange(0, NUMOF(m_nameColors) - 1)].m_result, vars.m_defaultName);
		SetName(temp);
	}
}

/*Singleton*/
MainChangeNameMenu* _instance;
MainChangeNameMenu* MainChangeNameMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MainChangeNameMenu();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
MainChangeNameMenu::~MainChangeNameMenu() { delete _instance; }