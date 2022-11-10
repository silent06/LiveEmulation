#include "stdafx.h"
#include "settingsConfig.h"
#include "../settingsMenu.h"

using namespace SettingsConfigMenuVars;

namespace SettingsConfigMenuVars {
	Vars vars;

	void SaveConfig(const char* fileName) {
		printf("Saving settings to config: %s.txt\n", fileName);

		for (int i = 0; i < GetConfig()->m_configItemsToSave.size(); i++) {
			auto item = GetConfig()->m_configItemsToSave.at(i);
			printf("%s|%s|%s|%s\n", item.m_submenu.c_str(), item.m_option.c_str(), item.m_type.c_str(), item.m_value.c_str());
		}
	}

	void LoadConfig() {
		vector<string> m_lines;

		vector<ConfigItems> m_items;
		for (int i = 0; i < m_lines.size(); i++) {
			int c = count(m_lines.at(i).begin(), m_lines.at(i).end(), '|');
			if (c >= 3) {
				vector<string> splitVals;
				Utils::Split(m_lines.at(i), '|', splitVals);

				ConfigItems item;
				item.m_submenu = splitVals[0];
				item.m_option = splitVals[1];
				item.m_type = splitVals[2];
				item.m_value = splitVals[3];

				switch (c) {
				case 4: // inttoggle
					item.m_extra = splitVals[4];
					break;
				}

				m_items.push_back(item);
			}
		}

		vector<ConfigResolvedItems> m_resolved;
		for (int i = 0; i < m_items.size(); i++) {
			ConfigResolvedItems item;

			if (!strcmp(m_items.at(i).m_type.c_str(), "inttoggle")) {
				item.m_type = eINTTOGGLE;
				item.m_int = stoi(m_items.at(i).m_extra);
				if (!strcmp(m_items.at(i).m_value.c_str(), "true"))
					item.m_bool = true;
			} else if (!strcmp(m_items.at(i).m_type.c_str(), "floattoggle")) {
				item.m_type = eFLOATTOGGLE;
				item.m_float = stof(m_items.at(i).m_extra);
				if (!strcmp(m_items.at(i).m_value.c_str(), "true"))
					item.m_bool = true;
			} else if (!strcmp(m_items.at(i).m_type.c_str(), "bool")) {
				item.m_type = eBOOL;
				if (!strcmp(m_items.at(i).m_value.c_str(), "true"))
					item.m_bool = true;
			} else if (!strcmp(m_items.at(i).m_type.c_str(), "int")) {
				item.m_type = eINT;
				item.m_int = stoi(m_items.at(i).m_value);
			} else if (!strcmp(m_items.at(i).m_type.c_str(), "float")) {
				item.m_type = eFLOAT;
				item.m_float = stof(m_items.at(i).m_value);
			} else if (!strcmp(m_items.at(i).m_type.c_str(), "color")) {
				item.m_type = eCOLOR;
				RGBA color;

				vector<string> tokens;
				Utils::Split(m_items.at(i).m_value, ',', tokens);
				if (tokens.size() == 4) {
					color.r = atoi(tokens[0].c_str());
					color.g = atoi(tokens[1].c_str());
					color.b = atoi(tokens[2].c_str());
					color.a = atoi(tokens[3].c_str());
				}

				item.m_color = color;
			}

			item.m_option = m_items.at(i).m_option;

			m_resolved.push_back(item);
		}

		for (int i = 0; i < GetSubmenuHandler()->GetSubmenus().size(); i++) {
			auto options = GetSubmenuHandler()->GetSubmenus().at(i)->GetOptions();
			for (int j = 0; j < options.size(); j++) {
				for (int k = 0; k < m_resolved.size(); k++) {
					if (!strcmp(m_resolved.at(k).m_submenu.c_str(), GetSubmenuHandler()->GetSubmenus().at(i)->GetConfigName())) {
						options.at(j).get()->LoadFromConfig(m_resolved.at(k));
					}
				}
			}
		}
	}
}

void SettingsConfigMenu::Init() {
	SetParentSubmenu<SettingsMenu>();
	SetName("Config");

	addOption(ButtonOption("Save Current Config")
		.addKeyboard(SaveConfig)
		.addTooltip("Save your current settings"));

	addOption(BreakOption("Load Config"));

	addOption(ButtonOption("f")
		.addFunction(LoadConfig)
		.addTooltip("hmmmmmmmmmm"));
}

void SettingsConfigMenu::InitVars() {
	vars.m_files.push_back("f");
}

/*Update once when submenu is opened*/
void SettingsConfigMenu::UpdateOnce() {}

/*Update while submenu is active*/
void SettingsConfigMenu::Update() {}

/*Background update*/
void SettingsConfigMenu::FeatureUpdate() {}

/*Singleton*/
SettingsConfigMenu* _instance;
SettingsConfigMenu* SettingsConfigMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new SettingsConfigMenu();
		_instance->InitVars();
		_instance->Init();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
SettingsConfigMenu::~SettingsConfigMenu() { delete _instance; }