#include "stdafx.h"
#include "visualIndicator.h"
#include "visualESP.h"
#include "visualIndicatorSettings.h"

using namespace VisualIndicatorMenuVars;

namespace VisualIndicatorMenuVars {
	Vars vars;

	void InitRadius() {
		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadeIconOffset], VisualIndicatorSettingsMenuVars::vars.cg_hudGrenadeIconOffset);
		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadeIconWidth], VisualIndicatorSettingsMenuVars::vars.cg_hudGrenadeIconWidth);
		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadeIconHeight], VisualIndicatorSettingsMenuVars::vars.cg_hudGrenadeIconHeight);
		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadePointerWidth], VisualIndicatorSettingsMenuVars::vars.cg_hudGrenadePointerWidth);
		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadePointerHeight], VisualIndicatorSettingsMenuVars::vars.cg_hudGrenadePointerHeight);
	}

	void Draw(CEntity entity, Material* material, Color pointer, Color icon, bool drawPointer = true, Vector2 newIconScale = Vector2(30.f, 30.f)) {
		D3DXVECTOR3 offset = entity.m_origin - Native::Custom::GetCG()->m_origin;

		if (drawPointer) Native::Engine::CG_DrawGrenadePointer(0, GetMenu()->m_center.x, GetMenu()->m_center.y, &offset, pointer.GetFloatPtr());

		Vector2 origScale;
		origScale.x = Utils::GetDvar<float>(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadeIconWidth]);
		origScale.y = Utils::GetDvar<float>(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadeIconHeight]);

		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadeIconWidth], newIconScale.x);
		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadeIconHeight], newIconScale.y);

		Native::Engine::CG_DrawGrenadeIcon(0, GetMenu()->m_center.x, GetMenu()->m_center.y, &offset, icon.GetFloatPtr(), material);

		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadeIconWidth], origScale.x);
		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadeIconHeight], origScale.y);
	}

	void DrawPointer(CEntity entity, Color pointer, Vector2 newScale = Vector2(30.f, 12.f)) {
		D3DXVECTOR3 offset = entity.m_origin - Native::Custom::GetCG()->m_origin;

		float beforeW = Utils::GetDvar<float>(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadePointerWidth]);
		float beforeH = Utils::GetDvar<float>(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadePointerHeight]);

		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadePointerWidth], newScale.x);
		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadePointerHeight], newScale.y);

		Native::Engine::CG_DrawGrenadePointer(0, GetMenu()->m_center.x, GetMenu()->m_center.y, &offset, pointer.GetFloatPtr());

		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadePointerWidth], beforeW);
		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_cg_hudGrenadePointerHeight], beforeH);
	}
}

void VisualIndicator::Init() {
	SetParentSubmenu<VisualsMenu>();
	SetName("Indicators");

	addOption(SubmenuOption("Settings")
		.addSubmenu<VisualIndicatorSettings>()
		.addTooltip("Edit the settings for the indicators"));

	addOption(ToggleOption("Players")
		.addToggle(vars.m_player).canBeSaved()
		.addTooltip("Player arrows"));

	addOption(ToggleOption("Scavenger Packs")
		.addToggle(vars.m_scav).canBeSaved()
		.addTooltip("Draw scavenger packs"));

	addOption(ToggleOption("Weapons")
		.addToggle(vars.m_weapons).canBeSaved()
		.addTooltip("Weapon arrows"));

	addOption(ToggleOption("Throwable/Static")
		.addToggle(vars.m_throwable).canBeSaved()
		.addTooltip("Grenades, flashbangs, knifes, C4 etc"));
}

void VisualIndicator::InitVars() {}

/*Update once when submenu is opened*/
void VisualIndicator::UpdateOnce() {}

/*Update while submenu is active*/
void VisualIndicator::Update() {}

/*Background update*/
void VisualIndicator::FeatureUpdate() {
	if (Utils::IsInGame()) {
		if (vars.m_player) {
			auto cg_t = Native::Custom::GetCG();
			for (int i = 0; i < Native::Custom::GetServerInfo()->m_maxClients; i++) {
				if (i == cg_t->m_clientNum) continue;

				if (Utils::IsAlive(i, true)) {
					auto entity = Native::Custom::GetCEntity()[i];

					Color color = Utils::IsFriend(i) ? VisualIndicatorSettingsMenuVars::vars.m_friendlyPlayerPointerColor : VisualIndicatorSettingsMenuVars::vars.m_enemyPlayerPointerColor;
					if (i == VisualESPMenuVars::vars.m_aimbotTarget) {
						color = VisualIndicatorSettingsMenuVars::vars.m_selectedPlayerTargetPointerColor;
					}

					DrawPointer(entity, color, VisualIndicatorSettingsMenuVars::vars.m_playerPointerSize);
				}
			}
		}

		if (vars.m_weapons || vars.m_scav) {
			for (int i = 0; i < 2048; i++) {
				auto entity = Native::Custom::GetCEntity()[i];
				if (Utils::IsAlive(i, true)) {
					if (entity.m_type == ET_ITEM || entity.m_type == ET_MISSLE) {
						if (Utils::GetDistance(entity.m_origin, Native::Custom::GetCG()->m_origin) <= 60.f) {
							auto wepDef = Utils::BG_GetWeaponDef(entity.m_weaponIndex);
							if (wepDef) {
								if (vars.m_scav) {
									if (entity.m_weaponIndex == 74) {
										static const auto material = Native::UI::R_RegisterMaterial("specialty_scavenger", 0);

										DrawPointer(entity, Color(0xc155e8FF), VisualIndicatorSettingsMenuVars::vars.m_playerPointerSize);
										Draw(entity, material, Color(0xFF0000FF), Color(0xFFFFFFFF), false, Vector2(18.f, 18.f));
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
											DrawPointer(entity, VisualIndicatorSettingsMenuVars::vars.m_throwablePointerColor, VisualIndicatorSettingsMenuVars::vars.m_playerPointerSize);
											Draw(entity, wepDef->m_killIcon, Color(0xFF0000FF), Color(0xFFFFFFFF), false, Vector2(18.f, 18.f));
											break;
										}
									}

									if (cont) {
										continue;
									}
								}

								if (vars.m_weapons) {
									if (wepDef->m_killIcon) {
										DrawPointer(entity, VisualIndicatorSettingsMenuVars::vars.m_weaponPointerColor, VisualIndicatorSettingsMenuVars::vars.m_playerPointerSize);
										Draw(entity, wepDef->m_killIcon, Color(0xFF0000FF), VisualIndicatorSettingsMenuVars::vars.m_weaponIconColor, false, Vector2(VisualIndicatorSettingsMenuVars::vars.cg_hudGrenadeIconWidth, VisualIndicatorSettingsMenuVars::vars.cg_hudGrenadeIconHeight));
									}
								}
							}
						}
					}
				}
			}
		}
	}

	Utils::InitializeOnceInGame(&vars.m_onceInGameFlag, InitRadius);
}

/*Singleton*/
VisualIndicator* _instance;
VisualIndicator* VisualIndicator::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VisualIndicator();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
VisualIndicator::~VisualIndicator() { delete _instance; }