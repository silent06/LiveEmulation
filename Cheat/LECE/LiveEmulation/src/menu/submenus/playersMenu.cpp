#include "stdafx.h"
#include "playersMenu.h"

using namespace PlayersMenuVars;

namespace PlayersMenuVars {
	Vars vars;

	const char* GenerateName(UserData& instance) {
		CgArray* cg = Native::Custom::GetCG();
		string name;
		if (instance.m_id == cg->m_clientNum) name += "[^1ME^7] ";
		name += instance.m_name;
		return name.c_str();
	}

	void ReloadPlayerList() {
		if (Utils::IsInGame()) {
			vars.players.clear();
			for (int i = 0; i < Native::Custom::GetServerInfo()->m_maxClients; i++) {
				if (Utils::IsAlive(i)) {
					vars.players.push_back(UserData(i, Native::Custom::GetCG()->m_clientInfo[i].m_name));
				}
			}
		}

		vars.m_needsToSort = true;
	}
}

void PlayersMenu::Init() {
	SetName("LiveEmulation Cheat Engine");
}

void PlayersMenu::InitVars() { }

/*Update once when submenu is opened*/
void PlayersMenu::UpdateOnce() {
	ReloadPlayerList();
}

/*Update while submenu is active*/
void PlayersMenu::Update() {
	if (vars.m_needsToSort) {
		vars.m_needsToSort = false;

		ClearOptionsOffset(0);

		if (Utils::IsInGame()) {
			for (int i = 0; i < vars.players.size(); i++) {
				addOption(ButtonOption(vars.players[i].m_name)
					.addTooltip("Do some shit to dis nigga"));
			}

			if (vars.players.size() < 18) {
				for (int i = 0; i < (18 - vars.players.size()); i++) {
					addOption(ButtonOption("Empty...")
						.addTooltip("Client isn't connected"));
				}
			}
		} else {
			addOption(ButtonOption(GenerateName(UserData(0, Native::Custom::GetXenonUserData(0).m_name)))
				.addTooltip("Do some shit to dis nigga"));

			for (int i = 1; i < 18; i++) {
				addOption(ButtonOption("Empty...")
					.addTooltip("Client isn't connected"));
			}
		}
	}
}

/*Background update*/
void PlayersMenu::FeatureUpdate() { };

/*Singleton*/
PlayersMenu* _instance;
PlayersMenu* PlayersMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new PlayersMenu();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
PlayersMenu::~PlayersMenu() { delete _instance; }