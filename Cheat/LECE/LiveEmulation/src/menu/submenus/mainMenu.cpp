#include "stdafx.h"
#include "mainMenu.h"
#include "main/mainChangeNameMenu.h"

using namespace MainMenuVars;

namespace MainMenuVars {
	Vars vars;

	ScrollStruct<int> m_changeTeam[] = {
		"Allies", 0,
		"Axis", 1,
		"Spectator", 2
	};

	void NoSpread(UserCommand* oldCommand) {
		CgArray* cg = Native::Custom::GetCG();

		auto TransformSeed = [](int* seed) -> void {
			*seed = 0x343FD * (0x343FD * (0x343FD * (0x343FD * *seed + 0x269EC3) + 0x269EC3) + 0x269EC3) + 0x269EC3;
		};

		auto BG_Random = [](int* seed) -> float {
			return ((float(*)(int*))0x820E7E20)(seed);
		};

		float MaxSpread, MinSpread;
		Native::Engine::BG_GetSpreadForWeapon(cg, Native::Custom::GetCEntity()[cg->m_clientNum].m_weaponIndex, &MinSpread, &MaxSpread);

		float spread = MinSpread + ((MaxSpread - MinSpread) * (cg->m_aimSpreadScale / 255.0f));
		float aimOffset = tan(spread * (3.14159265358979323846264338327950288f / 180.0f)) * 8192.f;

		int randSeed = oldCommand->m_time;
		TransformSeed(&randSeed);

		float rDir = (BG_Random(&randSeed) * 360.0f) * (3.14159265358979323846264338327950288f / 180.0f);
		float uDir = BG_Random(&randSeed);

		float spreadRight = uDir * cos(rDir) * aimOffset;
		float spreadUp = uDir * sin(rDir) * aimOffset;

		D3DXVECTOR3 DirF, DirR, DirU;
		Utils::AngleVectors(cg->m_viewAngles, &DirF, &DirR, &DirU);

		D3DXVECTOR3 EndVec = cg->m_refdef.m_origin + (DirF * 8192.f) + (DirR * spreadRight) + (DirU * spreadUp);

		D3DXVECTOR3 SpreadFix(0, 0, 0);
		D3DXVECTOR3 SpreadVector = EndVec - cg->m_refdef.m_origin;

		Utils::VectorToAngle(SpreadVector, SpreadFix);

		oldCommand->m_viewAngles[0] += Utils::AngleToShort(cg->m_viewAngles.x - SpreadFix.x);
		oldCommand->m_viewAngles[1] += Utils::AngleToShort(cg->m_viewAngles.y - SpreadFix.y);
	}

	void LeaveGame() {
		Native::Engine::Cbuf_AddText(0, "disconnect");
	}

	void EndGame() {
		Native::Engine::Cbuf_AddText(0, Utils::va("cmd mr %i 3 endround;", *(int*)(AuthData::g_addresses[AuthData::MISC_CmdMr] + 0x415E8)));
	}

	void SetJuggernaut() {
		Native::Engine::Cbuf_AddText(0, Utils::va("cmd mr %i 10 axis;", *(int*)(AuthData::g_addresses[AuthData::MISC_CmdMr] + 0x415E8)));
	}

	void ChangeTeam() {
		switch (vars.m_changeTeam) {
		case 0:
			Native::Engine::Cbuf_AddText(0, Utils::va("cmd mr %i 3 allies;", *(int*)(AuthData::g_addresses[AuthData::MISC_CmdMr] + 0x415E8)));
			break;
		case 1:
			Native::Engine::Cbuf_AddText(0, Utils::va("cmd mr %i 3 axis;", *(int*)(AuthData::g_addresses[AuthData::MISC_CmdMr] + 0x415E8)));
			break;
		case 2:
			Native::Engine::Cbuf_AddText(0, Utils::va("cmd mr %i 3 spectator;", *(int*)(AuthData::g_addresses[AuthData::MISC_CmdMr] + 0x415E8)));
			break;
		}
	}

	void SetClass() {
		Native::Engine::Cbuf_AddText(0, Utils::va("cmd mr %i 10 custom%i", *(int*)(AuthData::g_addresses[AuthData::MISC_CmdMr] + 0x415E8), vars.m_class));
	}

	namespace Bots {
		int getClient_s(int clientNum) {
			return *(int*)0x836C6310 + (clientNum * 0x68B80);
		}

		int bPort = 0;

		int SpawnAvaliableBot() {
			for (bPort = 0; bPort < 18; bPort++)
				if (!Native::Custom::GetCEntity()[bPort].m_alive)
					break;

			if (bPort == 18)
				return -1;

			printf("using entity %i\n", bPort);

			DWORD protocol = ((DWORD(*)())0x8232A320)();
			DWORD checksum = ((DWORD(*)())0x820E0B50)();

			DWORD rand1 = ((DWORD(*)(int, int))0x8226FC80)(0, INT_MAX);
			DWORD rand2 = ((DWORD(*)(int, int))0x8226FC80)(0, INT_MAX);

			char* connectBuffer = new char[0x400];
			sprintf(connectBuffer, "connect bot%d \"\\rate\\20000\\name\\bot%d\\natType\\1\\rank\\1\\prestige\\1\\ec_usingTag\\0\\ec_usingTitle\\0\\ec_TitleBg\\0\\ec_Level\\0\\ptd_prestige_black_ops\\0\\ptd_rank_black_ops\\0\\ptd_prestige_mw2\\0\\ptd_rank_mw2\\0\\ptd_prestige_world_at_war\\0\\ptd_rank_world_at_war\\0\\ptd_prestige_mw\\0\\ptd_rank_mw\\0\\protocol\\%i\\checksum\\%i\\challenge\\0\\statver\\26 3648679816\\invited\\1\\xuid\\%08x%08x\\onlineStats\\0\\migrating\\0\"", bPort - 1, bPort, protocol, checksum, rand1, rand2);
			((void(*)(const char*))0x822889F0)(connectBuffer);
			delete[] connectBuffer;

			((void(*)(int, int))0x822C8AB0)(0, bPort);
			((void(*)())0x82288A10)();

			int elPtr = getClient_s(bPort);
			if (elPtr == 0) return -1;

			*(short*)(elPtr + 0x352E6) = 0x3FF;
			*(int*)(elPtr + 0x352EC) = 1;

			UserCommand cmd;
			((void(*)(int))0x822C6DD0)(elPtr);
			((void(*)(int, UserCommand*))0x822C6F50)(elPtr, &cmd);

			return bPort;
		}

		void Scr_AddString(const char* value) {
			((void(*)(const char*))0x822BFC68)(value);
		}

		void Scr_NotifyNum(int entityNum, int classNum, int stringValue, int paramcount) {
			((void(*)(int, int, int, int))0x822BE548)(entityNum, classNum, stringValue, paramcount);
		}

		int SL_GetString(const char* string) {
			return ((int(*)(const char*, int))AuthData::g_addresses[AuthData::SL_GetString])(string, 0);
		}

		void botPickTeam(int botNum, handle_t* handle, handleCall_t type) {
			Scr_AddString("autoassign");
			Scr_AddString("team_marinesopfor");
			Scr_NotifyNum(botNum, 0, SL_GetString("menuresponse"), 2);
		}

		void botPickClass(int botNum, handle_t* handle, handleCall_t type) {
			Scr_AddString("class0");
			Scr_AddString("changeclass");
			Scr_NotifyNum(botNum, 0, SL_GetString("menuresponse"), 2);
		}

		void SpawnBot() {
			int bot = SpawnAvaliableBot();
			if (bot != -1) {
				Utils::HandleHookAllocTemp(bot, botPickTeam, 400); //delay this call 400 ms
				Utils::HandleHookAllocTemp(bot, botPickClass, 600); //delay this call 600 ms
			}
		}

		void spawn7bots() {
			SpawnBot();
			SpawnBot();
			SpawnBot();
			SpawnBot();
			SpawnBot();
			SpawnBot();
			SpawnBot();
		}
	}

	void SetData() {
		*(BYTE*)(AuthData::g_addresses[AuthData::PATCH_NoRecoil] + 0x3) = vars.m_noRecoil ? 0x0 : 0x7;
		*(int*)AuthData::g_addresses[AuthData::PATCH_Laser] = vars.m_laser ? 0x38600001 : 0x38600000;
		*(int*)AuthData::g_addresses[AuthData::PATCH_ConstantRadar] = vars.m_constantRadar ? 0x3B800001 : 0x557C87FE;

		*(int*)AuthData::g_addresses[AuthData::PATCH_CG_Flashbanged] = vars.m_noFlashbangScreenEffect ? 0x4e800020 : 0x3D400010; // CG_Flashbanged
		*(int*)AuthData::g_addresses[AuthData::PATCH_CG_DrawFlashDamage] = vars.m_noFlashbangScreenEffect ? 0x4e800020 : 0x7D8802A6; // CG_DrawFlashDamage
		*(int*)AuthData::g_addresses[AuthData::PATCH_Flashbang] = vars.m_noFlashbangScreenEffect ? 0x4e800020 : 0x7D8802A6;

		*(int*)AuthData::g_addresses[AuthData::PATCH_CL_CapTurnRate] = vars.m_noConcussionScreenEffect ? 0x4e800020 : 0x3D400000; // CL_CapTurnRate
		*(int*)AuthData::g_addresses[AuthData::PATCH_CG_StartShellShock] = vars.m_noConcussionScreenEffect ? 0x4e800020 : 0x3D600007; // CG_StartShellShock
		*(int*)AuthData::g_addresses[AuthData::PATCH_CG_UpdateShellShock] = vars.m_noConcussionScreenEffect ? 0x4e800020 : 0x7D8802A6; // CG_UpdateShellShock

		*(int*)AuthData::g_addresses[AuthData::PATCH_NoSway1] = vars.m_noSway ? 0x60000000 : 0x4BFFEA61;
		*(int*)AuthData::g_addresses[AuthData::PATCH_NoSway2] = vars.m_noSway ? 0x60000000 : 0x4BFFFB9D;
		*(int*)AuthData::g_addresses[AuthData::PATCH_NoFlinch] = vars.m_noFlinch ? 0x4e800020 : 0x7D8802A6;

		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_HearEveryone1], vars.m_hearEveryone);
		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_HearEveryone2], vars.m_hearEveryone);
		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_ThirdPerson], vars.m_thirdPerson);
		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_ThirdPersonRange], vars.m_thirdPersonRange);
		Utils::SetDvar(AuthData::g_addresses[AuthData::DVAR_FOV], vars.m_fov);
	}

	void test() {
		int options = 0;
		for (int i = 0; i < GetSubmenuHandler()->GetSubmenus().size(); i++) {
			options += GetSubmenuHandler()->GetSubmenus().at(i)->GetOptions().size();
		}

		printf("Total options: %i\n", options);
	}
}

void MainMenu::InitVars() {
	vars.m_fov = 65.f;
	vars.m_class = 1;
	vars.m_thirdPersonRange = 120.f;
}

void MainMenu::Init() {
	SetName("LiveEmulation Cheat Engine", "Main");

	addOption(ButtonOption("[Dev] Spawn Bot")
		.addFunction(Bots::SpawnBot)
		.addTooltip("Spawn el bot"));

	addOption(ButtonOption("[Dev] Test")
		.addFunction(test)
		.addTooltip("Test"));

	addOption(SubmenuOption("Change Name")
		.addSubmenu<MainChangeNameMenu>()
		.addTooltip("Change name"));

	addOption(ToggleOption("Constant Radar")
		.addToggle(vars.m_constantRadar).canBeSaved()
		.addFunction(SetData)
		.addTooltip("Always see enemies"));

	addOption(ToggleOption("Remove Sway")
		.addToggle(vars.m_noSway).canBeSaved()
		.addFunction(SetData)
		.addTooltip("Remove sway"));

	addOption(ToggleOption("Remove Flinch")
		.addToggle(vars.m_noFlinch).canBeSaved()
		.addFunction(SetData)
		.addTooltip("Remove flinch"));

	addOption(ToggleOption("Remove Recoil")
		.addToggle(vars.m_noRecoil).canBeSaved()
		.addFunction(SetData)
		.addTooltip("Remove recoil"));

	addOption(ToggleOption("Remove Spread")
		.addToggle(vars.m_noSpread).canBeSaved()
		.addFunction(SetData)
		.addTooltip("Remove spread"));

	addOption(ToggleOption("Remove Flashbang Screen Effect")
		.addToggle(vars.m_noFlashbangScreenEffect).canBeSaved()
		.addFunction(SetData)
		.addTooltip("Remove flashbang screen effect"));

	addOption(ToggleOption("Remove Concussion Screen Effect")
		.addToggle(vars.m_noConcussionScreenEffect).canBeSaved()
		.addFunction(SetData)
		.addTooltip("Remove concussion screen effect"));

	addOption(ToggleOption("Hear Everyone")
		.addToggle(vars.m_hearEveryone).canBeSaved()
		.addFunction(SetData)
		.addTooltip("Hear everyone"));

	addOption(ToggleOption("Spoof IP")
		.addToggle(vars.m_spoofIP).canBeSaved()
		.addTooltip("Spoof your ip to 1.3.3.7 to stop modders pulling your ip"));

	addOption(ToggleOption("Laser")
		.addToggle(vars.m_laser).canBeSaved()
		.addFunction(SetData)
		.addTooltip("Force render the weapon laser"));

	addOption(NumberOption<float>(SCROLL, "Field of View")
		.addNumber(vars.m_fov, "%.0f", 1.0f).addMin(0.0f).addMax(150.0f)
		.addFunction(SetData)
		.addTooltip("Set FOV"));

	addOption(NumberOption<int>(SCROLLSELECT, "Change Class")
		.addNumber(vars.m_class, "%d", 1).addMin(1).addMax(10)
		.addFunction(SetClass)
		.addTooltip("Set your class"));

	addOption(ScrollOption<int>(SCROLLSELECT, "Change Team")
		.addScroll(vars.m_changeTeam, 0, NUMOF(m_changeTeam) - 1, m_changeTeam)
		.addFunction(ChangeTeam)
		.addTooltip("Change your team"));

	addOption(NumberOption<float>(TOGGLE, "Third Person")
		.addNumber(vars.m_thirdPersonRange, "%.0f", 1.f).addMin(1.f).addMax(500.f).setScrollSpeed(10)
		.addToggle(vars.m_thirdPerson).canBeSaved()
		.addFunction(SetData)
		.addTooltip("Set your view as third person and control the range"));

	addOption(ButtonOption("Juggernaut")
		.addFunction(SetJuggernaut)
		.addTooltip("Set jugg"));

	addOption(ToggleOption("Auto Uninfect")
		.addToggle(vars.m_autoUninfect).canBeSaved()
		.addTooltip("Auto uninfect yourself in the Infected game mode"));

	addOption(ButtonOption("End Game")
		.addFunction(EndGame)
		.addTooltip("Force end the current match"));

	addOption(ButtonOption("Leave Game")
		.addFunction(LeaveGame)
		.addTooltip("Disconnect from the current match"));
}

/*Update once when submenu is opened*/
void MainMenu::UpdateOnce() {}

/*Update while submenu is active*/
void MainMenu::Update() {}

/*Background update*/
void MainMenu::FeatureUpdate() {
	Utils::InitializeOnceInGame(&vars.m_onceInGameFlag, SetData);

	if (Utils::IsInGame()) {
		static bool welcome = false;
		if (!welcome) {
			welcome = true;
			((void(*)(int, const char*))0x82127BF8)(0, "^1Welcome to the ^2LiveEmulation Cheat Engine^1!");
		}

		if (vars.m_autoUninfect) {
			if (Utils::Joaat(Native::Engine::Dvar_GetString("ui_gametype")) == 0x19a2cad5) { // if infected
				if (Native::Custom::GetCG()->m_clientInfo[Native::Custom::GetCG()->m_clientNum].m_team == 1) {
					Native::Engine::Cbuf_AddText(0, Utils::va("cmd mr %i 3 allies;", *(int*)(AuthData::g_addresses[AuthData::MISC_CmdMr] + 0x415E8)));
				}
			}
		}
	}
};

/*Singleton*/
MainMenu* _instance;
MainMenu* MainMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MainMenu();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
MainMenu::~MainMenu() { delete _instance; }