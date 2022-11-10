#include "stdafx.h"

void BreakOption::Input(int position) {}

void BreakOption::Render(int position) {
	GetRenderer()->DrawString(m_name, GetMenu()->m_center.x, 198 + (position * GetRenderer()->m_optionHeight), GetRenderer()->m_selectedFont, 0.58f, Color(255, 255, 255, 255), false, false, JustifyCenter);
}

void BreakOption::RenderSelected(int position) {
	switch (GetMenu()->m_breakScroll) {
		case 1: //Up
			if (GetMenu()->m_scrollOffset > 0 && GetMenu()->m_currentOption - GetMenu()->m_scrollOffset == 0)GetMenu()->m_scrollOffset--;
			if (GetMenu()->m_currentOption - 1 < 0)Bottom();
			else GetMenu()->m_currentOption--;
			break;
		case 2: //Down
			if (GetMenu()->m_scrollOffset < GetSubmenuHandler()->GetTotalOptions() - GetMenu()->m_maxOptions && GetMenu()->m_currentOption + 1 - GetMenu()->m_scrollOffset == GetMenu()->m_maxOptions) GetMenu()->m_scrollOffset++;
			if (GetMenu()->m_currentOption + 1 >= GetSubmenuHandler()->GetTotalOptions())Top();
			else GetMenu()->m_currentOption++;
			break;
		case 3: //Bottom
			if (GetMenu()->m_scrollOffset > 0 && GetMenu()->m_currentOption - GetMenu()->m_scrollOffset == 1)GetMenu()->m_scrollOffset--;
			GetMenu()->m_currentOption--; break;
		case 4: //Top
			if (GetMenu()->m_scrollOffset < GetSubmenuHandler()->GetTotalOptions() - GetMenu()->m_maxOptions && GetSubmenuHandler()->GetTotalOptions() >= GetMenu()->m_maxOptions && GetMenu()->m_currentOption - GetMenu()->m_scrollOffset == GetMenu()->m_maxOptions) GetMenu()->m_scrollOffset++;
			GetMenu()->m_currentOption++; break;
		default:
			break;
	}
}
