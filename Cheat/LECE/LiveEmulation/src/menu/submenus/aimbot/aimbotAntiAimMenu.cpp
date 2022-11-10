#include "stdafx.h"
#include "aimbotAntiAimMenu.h"

using namespace AimbotAntiAimMenuVars;

namespace AimbotAntiAimMenuVars {
	Vars vars;

	ScrollStruct<int> m_xs[] = {
		"Angle Up", 0,
		"Angle Down", 1,
		"Spin", 2,
		"Static Fake", 3,
		"Shield", 4
	};

	ScrollStruct<int> m_ys[] = {
		"Spin Left", 0,
		"Spin Right", 1,
		"Random Jitter", 2,
	};

	bool XSpinSpeedRequirement() {
		return vars.m_x && vars.m_xSelection == 2;
	}

	bool YSpinSpeedRequirement() {
		return vars.m_y && (vars.m_ySelection == 0 || vars.m_ySelection == 1);
	}
}

void AimbotAntiAimMenu::Init() {
	SetParentSubmenu<AimbotMenu>();
	SetName("Anti Aim");

	addOption(ScrollOption<int>(TOGGLE, "X")
		.addScroll(vars.m_xSelection, 0, NUMOF(m_xs) - 1, m_xs)
		.addToggle(vars.m_x)
		.addTooltip("Adjust the x angle anti aim"));

	addOption(NumberOption<float>(SCROLL, "X Spin Speed")
		.addNumber(vars.m_spinXSpeed, "%.0f", 1.0f).addMin(1.0f).addMax(35.f)
		.addRequirement(XSpinSpeedRequirement)
		.addTooltip("Adjust the speed of the x spin"));

	addOption(ScrollOption<int>(TOGGLE, "Y")
		.addScroll(vars.m_ySelection, 0, NUMOF(m_ys) - 1, m_ys)
		.addToggle(vars.m_y)
		.addTooltip("Adjust the y angle anti aim"));

	addOption(NumberOption<float>(SCROLL, "Y Spin Speed")
		.addNumber(vars.m_spinYSpeed, "%.0f", 1.0f).addMin(1.0f).addMax(30.f)
		.addRequirement(YSpinSpeedRequirement)
		.addTooltip("Adjust the speed of the y spin"));

	addOption(NumberOption<int>(TOGGLE, "Fake Lag")
		.addNumber(vars.m_fakeLagAmount, "%i (ms)", 100).setScrollSpeed(10).addMin(100).addMax(500)
		.addToggle(vars.m_fakeLag)
		.addTooltip("Fake lag"));

	addOption(ScrollOption<int>(TOGGLE, "Crouch Event")
		.addToggle(AimbotMenuVars::vars.m_crouchEvent)
		.addScroll(AimbotMenuVars::vars.m_crouchEventSelection, 0, NUMOF(AimbotMenuVars::m_crouchEvent) - 1, AimbotMenuVars::m_crouchEvent)
		.addTooltip("Crouch when one of the events occur"));

	addOption(NumberOption<int>(SCROLL, "Crouch Delay")
		.addNumber(AimbotMenuVars::vars.m_crouchDelay, "%i ms", 50).addMin(0).addMax(1000)
		.addTooltip("Delay for the crouch"));
}

void AimbotAntiAimMenu::InitVars() {
	vars.m_spinXSpeed = 5.f;
	vars.m_spinYSpeed = 5.f;
	vars.m_fakeLagAmount = 100;
}

/*Update once when submenu is opened*/
void AimbotAntiAimMenu::UpdateOnce() {}

/*Update while submenu is active*/
void AimbotAntiAimMenu::Update() {}

/*Background update*/
void AimbotAntiAimMenu::FeatureUpdate() {}

/*Singleton*/
AimbotAntiAimMenu* _instance;
AimbotAntiAimMenu* AimbotAntiAimMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new AimbotAntiAimMenu();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
AimbotAntiAimMenu::~AimbotAntiAimMenu() { delete _instance; }