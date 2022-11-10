#include "stdafx.h"
#include "visualReticle.h"
#include "visualReticleSettings.h"

using namespace VisualReticleMenuVars;

namespace VisualReticleMenuVars {
	Vars vars;

	void DisableGameReticle() {
		*(int*)0x82116E78 = vars.m_disableGameReticle ? 0x4e800020 : 0x7D8802A6;
	}
}

void VisualReticleMenu::Init() {
	SetParentSubmenu<VisualsMenu>();
	SetName("Reticle");

	addOption(SubmenuOption("Settings")
		.addSubmenu<VisualReticleSettingsMenu>()
		.addTooltip("Edit the settings of the reticle"));

	addOption(ToggleOption("Render Reticle")
		.addToggle(vars.m_render).canBeSaved()
		.addTooltip("Render the reticle"));

	addOption(ToggleOption("Disable Game Reticle")
		.addToggle(vars.m_disableGameReticle).canBeSaved()
		.addFunction(DisableGameReticle)
		.addTooltip("Disable the games crosshair"));
}

void VisualReticleMenu::InitVars() {
	vars.m_rotateSpeed = 0;
	vars.m_scale = 30;
	vars.m_rotateIncrement = 1;
	vars.m_color = Color(0xFFFFFFFF);
}

/*Update once when submenu is opened*/
void VisualReticleMenu::UpdateOnce() {}

/*Update while submenu is active*/
void VisualReticleMenu::Update() {}

/*Background update*/
void VisualReticleMenu::FeatureUpdate() {
	if (Utils::IsInGame()) {
		if (vars.m_render) {
			float cx = GetMenu()->m_center.x;
			float cy = GetMenu()->m_center.y;

			if (vars.m_rotate) {
				static int timer = 0;
				static int rotation = 0;
				if (Utils::timeGetTime() - timer > vars.m_rotateSpeed) {
					timer = Utils::timeGetTime();

					rotation += vars.m_rotateIncrement;
					if (rotation > 360) rotation = 0;
				}

				Native::UI::CG_DrawRotatedPicPhysical(0x827322C0, cx - (vars.m_scale / 2), cy, vars.m_scale, 2, rotation, vars.m_color.GetFloatPtr(), GetRenderer()->m_whiteMaterial);
				Native::UI::CG_DrawRotatedPicPhysical(0x827322C0, cx - (vars.m_scale / 2), cy, vars.m_scale, 2, rotation + 90, vars.m_color.GetFloatPtr(), GetRenderer()->m_whiteMaterial);
			} else {
				Native::UI::CG_DrawRotatedPicPhysical(0x827322C0, cx - (vars.m_scale / 2), cy, vars.m_scale, 2, vars.m_rotation, vars.m_color.GetFloatPtr(), GetRenderer()->m_whiteMaterial);
				Native::UI::CG_DrawRotatedPicPhysical(0x827322C0, cx - (vars.m_scale / 2), cy, vars.m_scale, 2, vars.m_rotation + 90, vars.m_color.GetFloatPtr(), GetRenderer()->m_whiteMaterial);
			}
		}
	}

	Utils::InitializeOnceInGame(&vars.m_onceFlag, DisableGameReticle);
}

/*Singleton*/
VisualReticleMenu* _instance;
VisualReticleMenu* VisualReticleMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VisualReticleMenu();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
VisualReticleMenu::~VisualReticleMenu() { delete _instance; }