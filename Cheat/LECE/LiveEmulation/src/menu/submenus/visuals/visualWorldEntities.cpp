#include "stdafx.h"
#include "visualWorldEntities.h"

using namespace VisualWorldEntitiesMenuVars;

namespace VisualWorldEntitiesMenuVars {
	Vars vars;

}

void VisualWorldEntities::Init() {
	SetName("World Entities");
	SetParentSubmenu<VisualsMenu>();
	
	addOption(ToggleOption("Scavenger Packs")
		.addToggle(vars.m_scav).canBeSaved()
		.addTooltip("Draw scavenger packs"));

	addOption(ToggleOption("Weapons")
		.addToggle(vars.m_weapons).canBeSaved()
		.addTooltip("Draw weapons"));

	addOption(ToggleOption("Throwable/Static")
		.addToggle(vars.m_throwable).canBeSaved()
		.addTooltip("Grenades, flashbangs, knifes, C4 etc"));
}

void VisualWorldEntities::InitVars() {}

/*Update once when submenu is opened*/
void VisualWorldEntities::UpdateOnce() {}

/*Update while submenu is active*/
void VisualWorldEntities::Update() {}

/*Background update*/
void VisualWorldEntities::FeatureUpdate() {
	if (Utils::IsInGame()) {
		if (vars.m_scav || vars.m_throwable || vars.m_weapons) {
			for (int i = 0; i < 2048; i++) {
				auto entity = Native::Custom::GetCEntity()[i];
				if (entity.m_type == ET_ITEM || entity.m_type == ET_MISSLE) {
					if (Utils::IsAlive(i, true)) {
						if (Utils::GetDistance(entity.m_origin, Native::Custom::GetCG()->m_origin) <= 60.f) {
							Vector2 screenPosition;
							if (Utils::WorldToScreen(entity.m_origin, &screenPosition.x, &screenPosition.y)) {
								if (vars.m_scav) {
									if (entity.m_weaponIndex == 74) {
										static const auto material = Native::UI::R_RegisterMaterial("specialty_scavenger", 0);
										Native::UI::CG_DrawRotatedPicPhysical(0x827322C0, screenPosition.x, screenPosition.y, 20, 20, 0, Color(0xFFFFFFFF).GetFloatPtr(), material);
									}
								}

								if (vars.m_throwable) {
									bool cont = false;
									auto wepDef = Utils::BG_GetWeaponDef(entity.m_weaponIndex);
									if (wepDef) {
										switch (entity.m_weaponIndex) {
										case 66: // Frag
										case 69: // Concussion
										case 68: // Smoke
										case 65: // Semtex
										case 71: // Knife
										case 72: // Bouncing Betty
										case 60: // Claymore
										case 61: // Trophy
										case 58: // C4
										case 78: // Portable Radar
											cont = true;
											Native::UI::CG_DrawRotatedPicPhysical(0x827322C0, screenPosition.x, screenPosition.y, 20, 20, 0, Color(0xFFFFFFFF).GetFloatPtr(), wepDef->m_killIcon);
											break;
										}
									}

									if (cont) {
										continue;
									}
								}

								if (vars.m_weapons) {
									auto wepDef = Utils::BG_GetWeaponDef(entity.m_weaponIndex);
									if (wepDef) {
										if (wepDef->m_killIcon) {
											Native::UI::CG_DrawRotatedPicPhysical(0x827322C0, screenPosition.x, screenPosition.y, 40, 20, 0, Color(0xFFFFFFFF).GetFloatPtr(), wepDef->m_killIcon);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
};

/*Singleton*/
VisualWorldEntities* _instance;
VisualWorldEntities* VisualWorldEntities::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VisualWorldEntities();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
VisualWorldEntities::~VisualWorldEntities() { delete _instance; }