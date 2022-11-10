#include "stdafx.h"
static Menu g_menu;
const char* Menu::m_version = "1.0.0";
int Menu::m_versionInt = 1;

void Menu::Init() {
#ifdef DEVELOPER_BUILD
	LOG(false, "[Menu] Initializing menu");
#endif

	m_center.x = 1280 / 2;
	m_center.y = 720 / 2;
	m_maxOptions = 14;
	m_openKey = DPAD_LEFT;

	GetSubmenuHandler()->Init();
}

void Menu::ScrollEvent(int scroll) {
	if (!m_open) return;
	if (scroll > 0 && GetMenu()->m_scrollOffset > 0) {
		GetMenu()->m_scrollOffset--;
		GetMenu()->m_currentOption--;
	}
	if (scroll < 0 && GetMenu()->m_scrollOffset < GetSubmenuHandler()->GetTotalOptions() - GetMenu()->m_maxOptions) {
		GetMenu()->m_scrollOffset++;
		GetMenu()->m_currentOption++;
	}
	if (GetSubmenuHandler()->GetTotalOptions() >= GetMenu()->m_maxOptions && GetMenu()->m_scrollOffset > GetSubmenuHandler()->GetTotalOptions() - GetMenu()->m_maxOptions) {
		GetMenu()->m_scrollOffset = GetSubmenuHandler()->GetTotalOptions() - GetMenu()->m_maxOptions;
		GetMenu()->m_currentOption = GetSubmenuHandler()->GetTotalOptions() - GetMenu()->m_maxOptions;
	}
}

void Menu::Render() {
	if (!m_open) return;

	if (GetMenu()->m_isShowingAnotherWindow) {
		/*Render color input*/
		GetMenuInput()->Col();
	} else {
		GetRenderer()->Render();
		
		GetSubmenuHandler()->Update();
	}
}

void Menu::ASyncUpdate() {
	MenuControls();

	if (IsOpenPressed() && !IsOpen()) {
		ToggleOpen();
	}

	if (m_disableAllInputThisFrame) {
		m_inputDisabled = true;
		m_disableAllInputThisFrame = false;
	}
	else m_inputDisabled = false;
}

Menu* GetMenu() { return &g_menu; }