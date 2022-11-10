#include "stdafx.h"
#include "visualRadar.h"
#include "visualESP.h"
#include "visualRadarSettings.h"

using namespace VisualRadarMenuVars;

namespace VisualRadarMenuVars {
	Vars vars;

	void Patch() {
		static DWORD last_hash = 0;

		if (!vars.m_radar) {
			*(int*)AuthData::g_addresses[AuthData::PATCH_CompassOverdrawn] = 0x48197925; // Compass ownerdraw had width or height of 0
			*(int*)AuthData::g_addresses[AuthData::PATCH_CompassScanLineBorder] = 0x4BFED0A5; // Scan line border
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawTickertape1] = 0x4BFD90A9; // CG_CompassDrawTickertape
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawTickertape2] = 0x4BFD9071; // CG_CompassDrawTickertape
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawPlayerMap] = 0x4BFD7FA9; // CG_CompassDrawPlayerMap
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawPlayer] = 0x4BFD8755; // CG_CompassDrawPlayer
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawCrates] = 0x4BFD9601; // CG_CompassDrawCrates
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawFriendlies] = 0x4BFDADB5; // CG_CompassDrawFriendlies
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawEnemies] = 0x4BFDB611; // CG_CompassDrawEnemies
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawTurrets] = 0x4BFDBB7D; // CG_CompassDrawTurrets
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawMissiles] = 0x4BFDD001; // CG_CompassDrawMissiles
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawHelicopter] = 0x4827A339; // CG_CompassDrawHelicopter
		} else {
			*(int*)AuthData::g_addresses[AuthData::PATCH_CompassOverdrawn] = 0x60000000; // Compass ownerdraw had width or height of 0
			*(int*)AuthData::g_addresses[AuthData::PATCH_CompassScanLineBorder] = 0x60000000; // Scan line border
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawTickertape1] = 0x60000000; // CG_CompassDrawTickertape
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawTickertape2] = 0x60000000; // CG_CompassDrawTickertape
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawPlayerMap] = 0x60000000; // CG_CompassDrawPlayerMap
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawPlayer] = 0x60000000; // CG_CompassDrawPlayer
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawCrates] = 0x60000000; // CG_CompassDrawCrates
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawFriendlies] = 0x60000000; // CG_CompassDrawFriendlies
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawEnemies] = 0x60000000; // CG_CompassDrawEnemies
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawTurrets] = 0x60000000; // CG_CompassDrawTurrets
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawMissiles] = 0x60000000; // CG_CompassDrawMissiles
			*(int*)AuthData::g_addresses[AuthData::PATCH_CG_CompassDrawHelicopter] = 0x60000000; // CG_CompassDrawHelicopter

			if (last_hash != Utils::Joaat(Native::Engine::Dvar_GetString("ui_mapname"))) {
				last_hash = Utils::Joaat(Native::Engine::Dvar_GetString("ui_mapname"));
				Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_CompassMaxRange], Utils::GetDvar<float>(AuthData::g_addresses[AuthData::DVAR_CompassMaxRange]) * 1.5f);
			}
		}
	}

	void YawVectors2D(float yaw, float *forward, float *right) {
		float sy = sinf(yaw * 0.017453292f);
		float cy = cosf(yaw * 0.017453292f);

		if (forward) {
			forward[0] = cy;
			forward[1] = sy;
		}

		if (right) {
			right[0] = sy;
			right[1] = -cy;
		}
	}

	void WorldPosToCompassPos(RectDef* radar, float* world, Vector2* compass) {
		auto cg_t = Native::Custom::GetCG();
		float north[2] = { 0, 0 };
		YawVectors2D(cg_t->m_refdefViewAngles.y, north, 0);

		float out[2] = { 0, 0 };
		Native::Engine::CG_WorldPosToCompass(COMPASS_TYPE_PARTIAL, cg_t, radar, north, world, cg_t->m_refdef.m_origin, 0, out);

		compass->x = (radar->x + (radar->w / 2)) - out[0];
		compass->y = (radar->x + (radar->w / 2)) - out[1];
	}
}

void VisualRadarMenu::Init() {
	SetParentSubmenu<VisualsMenu>();
	SetName("Visual Radar");

	addOption(SubmenuOption("Settings")
		.addSubmenu<VisualRadarSettingsMenu>()
		.addTooltip("Settings"));

	addOption(ToggleOption("Render Radar")
		.addToggle(vars.m_radar).canBeSaved()
		.addFunction(Patch)
		.addTooltip("Enable radar mods"));

	addOption(ToggleOption("Render Outline")
		.addToggle(vars.m_outline).canBeSaved()
		.addTooltip("Draw an outline over the radar"));
}

void VisualRadarMenu::InitVars() {}

/*Update once when submenu is opened*/
void VisualRadarMenu::UpdateOnce() {}

/*Update while submenu is active*/
void VisualRadarMenu::Update() {}

/*Background update*/
void VisualRadarMenu::FeatureUpdate() {
	if (vars.m_radar) {
		if (Utils::IsInGame()) {
			if (Backend::m_canRenderRadar) {
				if (vars.m_outline) {
					GetRenderer()->DrawOutlinedRect(28.f, 20.f, 300, 301, 2, Color(0, 0, 0, 0), VisualRadarSettingsMenuVars::vars.m_outline, true);
				}

				RectDef radarPosition;
				radarPosition.x = -88.f;
				radarPosition.y = 14.f;
				radarPosition.w = 200.f;
				radarPosition.h = 200.f;

				Native::Engine::CG_CompassDrawPlayerMap(0, COMPASS_TYPE_PARTIAL, nullptr, &radarPosition, GetRenderer()->m_whiteMaterial, VisualRadarSettingsMenuVars::vars.m_map.GetFloatPtr());
				Native::Engine::CG_CompassDrawCrates(0, COMPASS_TYPE_PARTIAL, nullptr, &radarPosition, GetRenderer()->m_whiteMaterial, VisualRadarSettingsMenuVars::vars.m_crates.GetFloatPtr());
				Native::Engine::CG_CompassDrawPlayer(0, COMPASS_TYPE_PARTIAL, nullptr, &radarPosition, VisualRadarSettingsMenuVars::vars.m_selfIcon.GetFloatPtr());
				Native::Engine::CG_CompassDrawFriendlies(0, COMPASS_TYPE_PARTIAL, nullptr, &radarPosition, VisualRadarSettingsMenuVars::vars.m_friendlyIcon.GetFloatPtr());
				Native::Engine::CG_CompassDrawEnemies(0, COMPASS_TYPE_PARTIAL, nullptr, &radarPosition, VisualRadarSettingsMenuVars::vars.m_enemyIcon.GetFloatPtr());
				Native::Engine::CG_CompassDrawTurrets(0, COMPASS_TYPE_PARTIAL, nullptr, &radarPosition, VisualRadarSettingsMenuVars::vars.m_turrets.GetFloatPtr());
				Native::Engine::CG_CompassDrawMissiles(0, COMPASS_TYPE_PARTIAL, nullptr, &radarPosition, VisualRadarSettingsMenuVars::vars.m_missiles.GetFloatPtr());
				Native::Engine::CG_CompassDrawHelicopter(0, COMPASS_TYPE_PARTIAL, nullptr, &radarPosition, VisualRadarSettingsMenuVars::vars.m_helicopters.GetFloatPtr());

				/*auto CG_DrawRotatedPic = [](int placement, float x, float  y, float w, float h, int alignx, int aligny, float angle, float* color, Material* shader) {
					((void(*)(int, float, float, float, float, int, int, float, float*, Material*))0x82117810)(placement, x, y, w, h, alignx, aligny, angle, color, shader);
				};

				for (int i = 0; i < 2048; i++) {
					auto entity = Native::Custom::GetCEntity()[i];
					if (Utils::IsAlive(i, true)) {
						if (entity.m_type == ET_ITEM || entity.m_type == ET_MISSLE) {
							auto shader = Utils::GetWeaponShader(entity.m_weaponIndex);
							if (shader) {
								Vector2 size;
								size.x = 10.f;
								size.y = 10.f;

								RectDef radar = { 28.f, 20.f, 200.f, 200.f, 8, 8 };
								Vector2 compass;
								WorldPosToCompassPos(&radar, entity.m_origin, &compass);

								float angle = Native::Custom::GetCEntity()[Native::Custom::GetCG()->m_clientNum].m_angles.y - entity.m_angles.y;
								CG_DrawRotatedPic(0x827322C0, compass.x - (size.x / 2), compass.y - (size.y / 2), size.x, size.y, 8, 8, angle, Color(0xFFFFFFFF).GetFloatPtr(), shader);
							}
						}
					}
				}*/

				Backend::m_canRenderRadar = false;
			}
		}
	}

	Utils::InitializeOnceInGame(&vars.m_onceFlag, Patch);
}

/*Singleton*/
VisualRadarMenu* _instance;
VisualRadarMenu* VisualRadarMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VisualRadarMenu();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
VisualRadarMenu::~VisualRadarMenu() { delete _instance; }