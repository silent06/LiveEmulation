#include "stdafx.h"
#include "menu/submenus/visuals/visualInformationMenu.h"

static SubmenuHandler g_submenuHandler;

void SubmenuHandler::Init() {
#ifdef DEVELOPER_BUILD
	LOG(false, "[SubmenuHandler] Initializing submenu handler");
#endif

	m_mainSubmenu = m_currentSubmenu = MainMenu::GetInstance();
	VisualsMenu::GetInstance();
	SettingsMenu::GetInstance();
	PlayersMenu::GetInstance();
	AimbotMenu::GetInstance();
}

void SubmenuHandler::Update() {
	m_currentSubmenu->MenuUpdate();
	if (m_nextSubmenu != nullptr) {
		GetMenu()->m_currentOption = m_nextCurrentOption;
		GetMenu()->m_scrollOffset = m_nextScrollOffset;
		m_currentSubmenu = m_nextSubmenu;
		m_nextSubmenu = nullptr;
		m_currentSubmenu->UpdateOnce();
	}
}

void SubmenuHandler::ASyncUpdate() {
	for (size_t i = 0; i < m_submenus.size(); i++) {
		m_submenus.at(i)->FeatureUpdate();
	}
}

void SubmenuHandler::SetSubmenu(Submenu* submenu) {
	m_currentSubmenu->m_oldScrollOffset = GetMenu()->m_currentOption;
	m_currentSubmenu->m_oldScrollOffset = GetMenu()->m_scrollOffset;
	m_nextCurrentOption = 0;
	m_nextScrollOffset = 0;
	m_nextSubmenu = submenu;
}

void SubmenuHandler::SetSubmenuMain() {
	m_nextSubmenu = m_mainSubmenu;
	m_nextCurrentOption = 0;
	m_nextScrollOffset = 0;
}

void SubmenuHandler::SetSubmenuPrevious(bool resetPos) {
	if (m_currentSubmenu->GetParentSubmenu() == nullptr) {
		GetSubmenuHandler()->SetSubmenuMain();
		GetMenu()->Close();
		GetMenu()->m_selectedTab = 0;
	} else {
		m_nextSubmenu = m_currentSubmenu->GetParentSubmenu();
		m_nextCurrentOption = resetPos ? 0 : m_nextSubmenu->m_oldCurrentOption;
		m_nextScrollOffset = resetPos ? 0 : m_nextSubmenu->m_oldScrollOffset;
	}
}

SubmenuHandler* GetSubmenuHandler() {
	return &g_submenuHandler;
}