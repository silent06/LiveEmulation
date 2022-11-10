#pragma once
#include "../submenus/aimbotMenu.h"
#include "../submenus/visualsMenu.h"
#include "../submenus/mainMenu.h"
#include "../submenus/settingsMenu.h"
#include "../submenus/playersMenu.h"

static bool IsOptionPressed() {
	if (GetMenu()->m_inputDisabled) return false;
	return GetController()->IsJustPressed(A);
}

static bool IsOpenPressed() {
	if (GetMenu()->m_inputDisabled) return false;
	return GetController()->IsJustPressed(GetMenu()->m_openKey);
}

static void Up() {
	if (GetMenu()->m_inputDisabled) return;
	if (GetSubmenuHandler()->GetTotalOptions() == 0) return;
	GetMenu()->m_currentOption--;
	if (GetMenu()->m_scrollOffset > 0 && GetMenu()->m_currentOption - GetMenu()->m_scrollOffset == -1) GetMenu()->m_scrollOffset--;
	GetMenu()->m_breakScroll = 1;
}

static void Down() {
	if (GetMenu()->m_inputDisabled) return;
	if (GetSubmenuHandler()->GetTotalOptions() == 0) return;
	GetMenu()->m_currentOption++;
	if (GetMenu()->m_scrollOffset < GetSubmenuHandler()->GetTotalOptions() - GetMenu()->m_maxOptions && GetMenu()->m_currentOption - GetMenu()->m_scrollOffset == GetMenu()->m_maxOptions) GetMenu()->m_scrollOffset++;
	if (GetSubmenuHandler()->GetTotalOptions() >= GetMenu()->m_maxOptions && GetMenu()->m_scrollOffset > GetSubmenuHandler()->GetTotalOptions() - GetMenu()->m_maxOptions) GetMenu()->m_scrollOffset = GetSubmenuHandler()->GetTotalOptions() - GetMenu()->m_maxOptions;
	GetMenu()->m_breakScroll = 2;
}

static void Bottom() {
	if (GetMenu()->m_inputDisabled) return;
	if (GetSubmenuHandler()->GetTotalOptions() == 0) return;
	GetMenu()->m_currentOption = GetSubmenuHandler()->GetTotalOptions() - 1;
	if (GetSubmenuHandler()->GetTotalOptions() >= GetMenu()->m_maxOptions) GetMenu()->m_scrollOffset = GetSubmenuHandler()->GetTotalOptions() - GetMenu()->m_maxOptions;
	GetMenu()->m_breakScroll = 3;
}

static void Top() {
	if (GetMenu()->m_inputDisabled) return;
	if (GetSubmenuHandler()->GetTotalOptions() == 0) return;
	GetMenu()->m_currentOption = 0;
	GetMenu()->m_scrollOffset = 0;
	GetMenu()->m_breakScroll = 4;
}

static void MenuControls() {
	GetMenu()->m_fastPressLeft = false;
	GetMenu()->m_fastPressRight = false;

	GetMenu()->m_fastPressLeft = IsDown(DPAD_LEFT);
	GetMenu()->m_fastPressRight = IsDown(DPAD_RIGHT);

	if (GetMenu()->m_isShowingAnotherWindow) return;

	static int twoHundredDelay = 0;
	if ((GetTickCount() - twoHundredDelay) > 200)  {
		if (IsDown(DPAD_UP))  {
			twoHundredDelay = GetTickCount();
			if (GetMenu()->m_currentOption == 0)  {
				Bottom();
			} else {
				Up();
			}
		}  else if (IsDown(DPAD_DOWN))  {
			twoHundredDelay = GetTickCount();
			if (GetMenu()->m_currentOption == GetSubmenuHandler()->GetTotalOptions() - 1) {
				Top();
			} else {
				Down();
			}
		}  else if (IsJustPressed(DPAD_LEFT)) {
			if (GetMenu()->IsOpen()) {
				if (!GetSubmenuHandler()->GetCurrentSubmenu()->GetOptions().at(GetMenu()->m_currentOption).get()->IsScroll()) {
					GetMenu()->m_currentOption = 0;
					GetMenu()->m_scrollOffset = 0;
					GetMenu()->m_selectedTab--;
					GetMenu()->m_selectedTab = GetMenu()->m_selectedTab < 0 ? 4 : GetMenu()->m_selectedTab;

					switch (GetMenu()->m_selectedTab) {
					case 0: GetSubmenuHandler()->SetSubmenu(MainMenu::GetInstance()); break;
					case 1: GetSubmenuHandler()->SetSubmenu(AimbotMenu::GetInstance()); break;
					case 2: GetSubmenuHandler()->SetSubmenu(VisualsMenu::GetInstance()); break;
					case 3: GetSubmenuHandler()->SetSubmenu(SettingsMenu::GetInstance()); break;
					case 4: GetSubmenuHandler()->SetSubmenu(PlayersMenu::GetInstance()); break;
					}
					twoHundredDelay = GetTickCount();
				} else {
					twoHundredDelay = GetTickCount();
				}
			} else {
				twoHundredDelay = GetTickCount();
				GetMenu()->m_currentOption = 0;
				GetMenu()->m_scrollOffset = 0;
				GetMenu()->m_selectedTab = 0;
			}
		} else if (IsJustPressed(DPAD_RIGHT)) {
			if (GetMenu()->IsOpen()) {
				if (!GetSubmenuHandler()->GetCurrentSubmenu()->GetOptions().at(GetMenu()->m_currentOption).get()->IsScroll()) {
					GetMenu()->m_currentOption = 0;
					GetMenu()->m_scrollOffset = 0;
					GetMenu()->m_selectedTab++;
					GetMenu()->m_selectedTab = GetMenu()->m_selectedTab > 4 ? 0 : GetMenu()->m_selectedTab;

					switch (GetMenu()->m_selectedTab) {
					case 0: GetSubmenuHandler()->SetSubmenu(MainMenu::GetInstance()); break;
					case 1: GetSubmenuHandler()->SetSubmenu(AimbotMenu::GetInstance()); break;
					case 2: GetSubmenuHandler()->SetSubmenu(VisualsMenu::GetInstance()); break;
					case 3: GetSubmenuHandler()->SetSubmenu(SettingsMenu::GetInstance()); break;
					case 4: GetSubmenuHandler()->SetSubmenu(PlayersMenu::GetInstance()); break;
					}
					twoHundredDelay = GetTickCount();
				}
			}
			twoHundredDelay = GetTickCount();
		} else if (IsDown(DPAD_LEFT)) {
			if (GetMenu()->IsOpen()) {
				if (!GetSubmenuHandler()->GetCurrentSubmenu()->GetOptions().at(GetMenu()->m_currentOption).get()->IsScroll()) {
					GetMenu()->m_currentOption = 0;
					GetMenu()->m_scrollOffset = 0;
					GetMenu()->m_selectedTab--;
					GetMenu()->m_selectedTab = GetMenu()->m_selectedTab < 0 ? 4 : GetMenu()->m_selectedTab;

					switch (GetMenu()->m_selectedTab) { // main, aim, vis
					case 0: GetSubmenuHandler()->SetSubmenu(MainMenu::GetInstance()); break;
					case 1: GetSubmenuHandler()->SetSubmenu(AimbotMenu::GetInstance()); break;
					case 2: GetSubmenuHandler()->SetSubmenu(VisualsMenu::GetInstance()); break;
					case 3: GetSubmenuHandler()->SetSubmenu(SettingsMenu::GetInstance()); break;
					case 4: GetSubmenuHandler()->SetSubmenu(PlayersMenu::GetInstance()); break;
					}
					twoHundredDelay = GetTickCount();
				}
				else {
					twoHundredDelay = GetTickCount();
				}
			}
			else {
				twoHundredDelay = GetTickCount();
			}
		}
		else if (IsDown(DPAD_RIGHT)) {
			if (GetMenu()->IsOpen()) {
				if (!GetSubmenuHandler()->GetCurrentSubmenu()->GetOptions().at(GetMenu()->m_currentOption).get()->IsScroll()) {
					GetMenu()->m_currentOption = 0;
					GetMenu()->m_scrollOffset = 0;
					GetMenu()->m_selectedTab++;
					GetMenu()->m_selectedTab = GetMenu()->m_selectedTab > 4 ? 0 : GetMenu()->m_selectedTab;

					switch (GetMenu()->m_selectedTab) {
					case 0: GetSubmenuHandler()->SetSubmenu(MainMenu::GetInstance()); break;
					case 1: GetSubmenuHandler()->SetSubmenu(AimbotMenu::GetInstance()); break;
					case 2: GetSubmenuHandler()->SetSubmenu(VisualsMenu::GetInstance()); break;
					case 3: GetSubmenuHandler()->SetSubmenu(SettingsMenu::GetInstance()); break;
					case 4: GetSubmenuHandler()->SetSubmenu(PlayersMenu::GetInstance()); break;
					}
					twoHundredDelay = GetTickCount();
				} else {
					twoHundredDelay = GetTickCount();
				}
			} else {
				twoHundredDelay = GetTickCount();
			}
		}
	}

	if (IsJustPressed(B)) {
		if (GetMenu()->m_isShowingAnotherWindow) {
			GetMenuInput()->m_accepted = false;
			GetMenu()->m_isShowingAnotherWindow = false;
		} else GetSubmenuHandler()->SetSubmenuPrevious();
	}
}