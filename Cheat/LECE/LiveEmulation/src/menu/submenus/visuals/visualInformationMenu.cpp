#include "stdafx.h"
#include "visualInformationMenu.h"
#include "visualESP.h"

using namespace VisualInformationMenuVars;

namespace VisualInformationMenuVars {
	Vars vars;

	const char* GetReadableName(const char* name) {
		DWORD hash = Utils::Joaat(name);
		switch (hash) {
			/*Game Modes*/
		case 0xdd6f4a20:
			return "Free-For-All (Standard)";
		case 0xfa24b367:
			return "Team Deathmatch (Standard)";
		case 0x1148a15e:
			return "Search and Destroy (Standard)";
		case 0xfc0cca60:
			return "Sabotage (Standard)";
		case 0xbbf2d511:
			return "Domination (Standard)";
		case 0x81688a9e:
			return "Headquarters (Standard)";
		case 0xc7391121:
			return "Capture the Flag (Standard)";
		case 0x7688fc55:
			return "Demolition (Standard)";
		case 0x450ba8b6:
			return "Kill Confirmed (Standard)";
		case 0xa2198c58:
			return "Team Defender (Standard)";
		case 0x25fe0213:
			return "Drop Zone (Alternate)";
		case 0x14e3dbb6:
			return "Team Juggernaut (Alternate)";
		case 0xa86abbdb:
			return "Juggernaut (Alternate)";
		case 0x439a2ae5:
			return "Gun Game (Alternate)";
		case 0x19a2cad5:
			return "Infected (Alternate)";
		case 0x71735b69:
			return "One In The Chamber (Alternate)";

			/*Maps*/
		case 0x84a165e7:
			return "Lockdown";
		case 0xd738122c:
			return "Bootleg";
		case 0xcaa6476:
			return "Mission";
		case 0x3bf2559:
			return "Carbon";
		case 0x70626767:
			return "Dome";
		case 0x896275f4:
			return "Downturn";
		case 0xc0330651:
			return "Hardhat";
		case 0x4240e6c9:
			return "Interchange";
		case 0x6da8a9c0:
			return "Fallen";
		case 0xd1a780c2:
			return "Bakaara";
		case 0x53bc0e83:
			return "Resistance";
		case 0xadce8f20:
			return "Arkaden";
		case 0x7cbdca36:
			return "Outpost";
		case 0x7556ddbb:
			return "Seatown";
		case 0x88895841:
			return "Underground";
		case 0x1085ab50:
			return "Village";
		case 0xdf58f9d1:
			return "Aground";
		case 0xf85e32d9:
			return "Aqueduct";
		case 0x8d9a6db1:
			return "Foundation";
		case 0x94be79ef:
			return "Getaway";
		case 0xa0283854:
			return "Piazza";
		case 0x656ca4d2:
			return "Sanctuary";
		case 0x7a68b8a2:
			return "Black Box";
		case 0xc12358b5:
			return "Overwatch";
		case 0x859018bf:
			return "Liberation";
		case 0xb29bb927:
			return "Oasis";
		case 0x4be89d05:
			return "Lookout";
		case 0xf88f4847:
			return "Terminal";
		default:
			return name;
		}
	}
}

void VisualInformationMenu::Init() {
	SetParentSubmenu<VisualsMenu>();
	SetName("Debug");

	addOption(ToggleOption("Target Details")
		.addToggle(vars.m_targetDetails).canBeSaved()
		.addTooltip("Draw information on the aimed target"));

	addOption(ToggleOption("Screen Resolution")
		.addToggle(vars.m_screenResolution).canBeSaved()
		.addTooltip("Draw your screens resolution"));

	addOption(ToggleOption("FPS")
		.addToggle(vars.m_fps).canBeSaved()
		.addTooltip("Draw your current FPS"));

	addOption(ToggleOption("Ping")
		.addToggle(vars.m_ping).canBeSaved()
		.addTooltip("Draw your current ping"));

	addOption(ToggleOption("Map")
		.addToggle(vars.m_map).canBeSaved()
		.addTooltip("Draw the map you're playing in"));

	addOption(ToggleOption("Game Mode")
		.addToggle(vars.m_gameMode).canBeSaved()
		.addTooltip("Draw the game mode you're playing in"));

	addOption(ToggleOption("Game Host")
		.addToggle(vars.m_gameHost).canBeSaved()
		.addTooltip("Draw the host of your game"));

	addOption(ToggleOption("Nat Type")
		.addToggle(vars.m_natType).canBeSaved()
		.addTooltip("Draw your nat type"));

	addOption(ToggleOption("Client Count")
		.addToggle(vars.m_clientCount).canBeSaved()
		.addTooltip("Draw the amount of connected clients"));
}

void VisualInformationMenu::InitVars() {}

/*Update once when submenu is opened*/
void VisualInformationMenu::UpdateOnce() {}

/*Update while submenu is active*/
void VisualInformationMenu::Update() {}

/*Background update*/
void VisualInformationMenu::FeatureUpdate() {
	GetRenderer()->DrawTopRight(Utils::va("LiveEmulation Cheat Engine v%s", GetMenu()->m_version), 1);

	vars.m_displayIndex = 2;

	if (vars.m_screenResolution) {
		auto cg = Native::Custom::GetCG();
		if (cg != nullptr) {
			Utils::vaBuff(vars.m_displayOptions[0], "Screen Resolution: %i x %i", cg->m_refdef.m_width, cg->m_refdef.m_height);
			GetRenderer()->DrawTopRight(vars.m_displayOptions[0], vars.m_displayIndex);
			vars.m_displayIndex++;
		}
	}

	if (vars.m_fps) {
		static int delay = 0;
		static float fps = 0.0f;

		if (Native::Custom::GetUIContext() != nullptr) {
			if (Native::Custom::GetUIContext()->m_fps != 0.0f) {
				if (GetTickCount() - delay > 2000) {
					delay = GetTickCount();
					fps = Native::Custom::GetUIContext()->m_fps;
				}

				Utils::vaBuff(vars.m_displayOptions[1], "FPS: %.0f", Utils::IsInGame() ? fps / 2.f : fps);
				GetRenderer()->DrawTopRight(vars.m_displayOptions[1], vars.m_displayIndex);
			}
		}
		vars.m_displayIndex++;
	}

	if (Utils::IsInGame()) {
		if (vars.m_map) {
			Utils::vaBuff(vars.m_displayOptions[2], "Map: %s", GetReadableName(Native::Engine::Dvar_GetString("ui_mapname")));
			GetRenderer()->DrawTopRight(vars.m_displayOptions[2], vars.m_displayIndex);
			vars.m_displayIndex++;
		}

		if (vars.m_gameMode) {
			Utils::vaBuff(vars.m_displayOptions[3], "Game Mode: %s", GetReadableName(Native::Engine::Dvar_GetString("ui_gametype")));
			GetRenderer()->DrawTopRight(vars.m_displayOptions[3], vars.m_displayIndex);
			vars.m_displayIndex++;
		}

		if (vars.m_gameHost) {
			auto serverInfo = Native::Custom::GetServerInfo();
			if (serverInfo != nullptr) {
				Utils::vaBuff(vars.m_displayOptions[4], "Host: %s", serverInfo->m_hostName);
				GetRenderer()->DrawTopRight(vars.m_displayOptions[4], vars.m_displayIndex);
				vars.m_displayIndex++;
			}
		}

		if (vars.m_clientCount) {
			auto serverInfo = Native::Custom::GetServerInfo();
			if (serverInfo != nullptr) {
				int currentCount = 0;

				for (int i = 0; i < serverInfo->m_maxClients; i++) {
					if (Utils::IsAlive(i)) currentCount++;
				}

				Utils::vaBuff(vars.m_displayOptions[7], "Number of Clients: %i / %i", currentCount, serverInfo->m_maxClients);
				GetRenderer()->DrawTopRight(vars.m_displayOptions[7], vars.m_displayIndex);
				vars.m_displayIndex++;
			}
		}
	}

	if (vars.m_natType) {
		XONLINE_NAT_TYPE natType = (XONLINE_NAT_TYPE)(*(DWORD*)0x825AFA5C);
		switch (natType) {
		case XONLINE_NAT_MODERATE:
			Utils::vaBuff(vars.m_displayOptions[5], "Nat Type: ^3Moderate");
			break;
		case XONLINE_NAT_OPEN:
			Utils::vaBuff(vars.m_displayOptions[5], "Nat Type: ^2Open");
			break;
		case XONLINE_NAT_STRICT:
			Utils::vaBuff(vars.m_displayOptions[5], "Nat Type: ^1Strict");
			break;
		}
		GetRenderer()->DrawTopRight(vars.m_displayOptions[5], vars.m_displayIndex);
		vars.m_displayIndex++;
	}

	if (vars.m_ping) {
		if (Utils::IsInGame()) {
			Utils::vaBuff(vars.m_displayOptions[6], "Ping: %i", Native::Custom::GetPingData()[Native::Custom::GetCG()->m_clientNum].m_ping);
			GetRenderer()->DrawTopRight(vars.m_displayOptions[6], vars.m_displayIndex);
			vars.m_displayIndex++;
		}
	}

	if (vars.m_targetDetails) {
		if (Utils::IsInGame()) {
			if (VisualESPMenuVars::vars.m_aimbotTarget != -1 && VisualESPMenuVars::vars.m_aimbotTarget != Native::Custom::GetCG()->m_clientNum) {
				auto entity = Native::Custom::GetCEntity()[VisualESPMenuVars::vars.m_aimbotTarget];

				vars.m_displayIndex++;
				GetRenderer()->DrawTopRight("Target Details:", vars.m_displayIndex);
				vars.m_displayIndex++;

				GetRenderer()->DrawTopRight(Utils::va("Name: %s", Native::Custom::GetCG()->m_clientInfo[VisualESPMenuVars::vars.m_aimbotTarget].m_name), vars.m_displayIndex);
				vars.m_displayIndex++;

				GetRenderer()->DrawTopRight(Utils::va("Weapons: %s | %s", Utils::BG_GetWeaponDef(entity.m_weaponIndex)->m_overlayName, Utils::BG_GetWeaponDef(*(BYTE*)((int)&entity + 0x12B))->m_overlayName), vars.m_displayIndex);
				vars.m_displayIndex++;

				GetRenderer()->DrawTopRight(Utils::va("Client: %i", VisualESPMenuVars::vars.m_aimbotTarget), vars.m_displayIndex);
				vars.m_displayIndex++;

				GetRenderer()->DrawTopRight(Utils::va("Target Bone: %s", AimbotMenuVars::vars.m_usingTag), vars.m_displayIndex);
				vars.m_displayIndex++;

				float distance = Utils::GetDistance(entity.m_origin, Native::Custom::GetCEntity()[Native::Custom::GetCG()->m_clientNum].m_origin);
				GetRenderer()->DrawTopRight(Utils::va("Distance: %.2f m", distance), vars.m_displayIndex);
				vars.m_displayIndex++;

				const char* stance = 
					entity.m_flags & 0x08 ? "Prone" :
					entity.m_flags & 0x04 ? "Crouched" : "Standing";

				GetRenderer()->DrawTopRight(Utils::va("Stance: %s", stance), vars.m_displayIndex);
				vars.m_displayIndex++;

				GetRenderer()->DrawTopRight(Utils::va("Is Hittable: %s", AimbotMenuVars::vars.m_hittable ? "YES" : "NO"), vars.m_displayIndex);
				vars.m_displayIndex++;

				GetRenderer()->DrawTopRight(Utils::va("Is Visible: %s", AimbotMenuVars::vars.m_visible ? "YES" : "NO"), vars.m_displayIndex);
				vars.m_displayIndex++;
			}
		}
	}
};

/*Singleton*/
VisualInformationMenu* _instance;
VisualInformationMenu* VisualInformationMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VisualInformationMenu();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
VisualInformationMenu::~VisualInformationMenu() { delete _instance; }