#include "stdafx.h"
#include "visuals/visualInformationMenu.h"
#include "visuals/visualRadar.h"
#include "visuals/visualESP.h"
#include "visuals/visualReticle.h"
#include "visuals/visualIndicator.h"
#include "visuals/visualWorldEntities.h"

using namespace VisualsMenuVars;

namespace VisualsMenuVars {
	Vars vars;

	void _ProximityWarningCallback(CgArray* cg_t, CEntity entity, ClientInfo clientInfo) {
		if (vars.m_proximityWarning) {
			if (!Utils::IsFriend(entity.m_clientNumber)) {
				float distance = Utils::GetDistance(entity.m_origin, Native::Custom::GetCEntity()[cg_t->m_clientNum].m_origin);
				if (distance <= 20.0f) {
					vars.m_queue.push_back(Utils::va("%s is near (%.1f m)", clientInfo.m_name, distance));
				}
			}
		}
	}

	void _VisibleWarningCallback(CgArray* cg_t, CEntity entity, ClientInfo clientInfo) {
		if (vars.m_visibleWarning) {
			if(!Utils::IsFriend(entity.m_clientNumber)){
				if (AimbotMenuVars::IsTargetVisible(AimbotMenuVars::GetBoneFraction(entity.m_clientNumber, "j_head"))) {
					vars.m_queue.push_back(Utils::va("%s is visible", clientInfo.m_name));
				}
			}
		}
	}
}

void VisualsMenu::Init() {
	SetName("LiveEmulation Cheat Engine", "Visuals");

	addOption(SubmenuOption("Reticle")
		.addSubmenu<VisualReticleMenu>()
		.addTooltip("Show a reticle"));

	addOption(SubmenuOption("Indicators")
		.addSubmenu<VisualIndicator>()
		.addTooltip("Indicators"));

	addOption(SubmenuOption("Debug")
		.addSubmenu<VisualInformationMenu>()
		.addTooltip("Show information in the top right hand side of your screen"));

	addOption(SubmenuOption("Radar")
		.addSubmenu<VisualRadarMenu>()
		.addTooltip("Modify the original radar"));

	addOption(SubmenuOption("ESP")
		.addSubmenu<VisualESPMenu>()
		.addTooltip("Draw ESP"));

	addOption(SubmenuOption("World Entities")
		.addSubmenu<VisualWorldEntities>()
		.addTooltip("Draw world entities"));

	addOption(ToggleOption("Proximity Warning")
		.addToggle(vars.m_proximityWarning).canBeSaved()
		.addTooltip("Show when a player is close to you"));

	addOption(ToggleOption("Visibility Warning")
		.addToggle(vars.m_visibleWarning).canBeSaved()
		.addTooltip("Show when a player is visible"));

	addOption(ToggleOption("Health Bar")
		.addToggle(vars.m_health).canBeSaved()
		.addTooltip("Draw a health bar"));
}

void VisualsMenu::InitVars() {
	vars.m_healthColor = GetRenderer()->m_baseColor;
	vars.m_healthTextColor = Color(0xFFFFFFFF);
}

/*Update once when submenu is opened*/
void VisualsMenu::UpdateOnce() {}

/*Update while submenu is active*/
void VisualsMenu::Update() {}

/*Background update*/
void VisualsMenu::FeatureUpdate() {
	if (Utils::IsInGame()) {
		if (vars.m_health) {
			GetRenderer()->DrawOutlinedRect(GetMenu()->m_center.x - (250 / 2), GetMenu()->m_center.y + 322, (((250 * Native::Custom::GetCG()->m_health) / 100)), 18, 1, vars.m_healthColor.WithOpacity(150), vars.m_healthColor.WithOpacity(150), true);
			GetRenderer()->DrawString(Utils::va("Health (%i/%i)", Native::Custom::GetCG()->m_health, Native::Custom::GetCG()->m_maxHealth), 640, 700, GetRenderer()->Font.normalFont, 0.53f, vars.m_healthTextColor, false, false, JustifyCenter);
		}

		if (vars.m_proximityWarning || vars.m_visibleWarning) {
			vars.m_queue.clear();
			Utils::ClientCallback(true, _ProximityWarningCallback);
			Utils::ClientCallback(true, _VisibleWarningCallback);

			for (int i = 0; i < vars.m_queue.size(); i++) {
				GetRenderer()->DrawString(vars.m_queue[i], GetMenu()->m_center.x, (GetMenu()->m_center.y - 300) + (i * 20), GetRenderer()->Font.normalFont, 0.53f, Color(0xc73737FF), false, false, JustifyCenter);
			}
		}
	}
};

/*Singleton*/
VisualsMenu* _instance;
VisualsMenu* VisualsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VisualsMenu();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
VisualsMenu::~VisualsMenu() { delete _instance; }