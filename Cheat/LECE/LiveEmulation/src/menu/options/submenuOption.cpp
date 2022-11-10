#include "stdafx.h"

void SubmenuOption::Input(int pos) {

}

void SubmenuOption::Render(int pos) {
	m_onUpdate();
	
	GetRenderer()->DrawString(m_name, 454, 198 + (pos * GetRenderer()->m_optionHeight), GetRenderer()->m_selectedFont, 0.58f, Color(255, 255, 255, 255), false, false, JustifyLeft);
	GetRenderer()->DrawString("+", GetMenu()->m_center.x + 175, 198 + (pos * GetRenderer()->m_optionHeight), GetRenderer()->Font.boldFont, 0.58f, Color(255, 255, 255, 255), false, false, JustifyLeft);
}

void SubmenuOption::RenderSelected(int position) {
	m_currentOp();
	if (IsOptionPressed()) {
		m_function();
		GetSubmenuHandler()->SetSubmenu(m_submenu);
	}
};