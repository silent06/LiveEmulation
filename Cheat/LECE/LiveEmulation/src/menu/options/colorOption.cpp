#include "stdafx.h"

void ColorOptionSaveConfig(ColorOption* ptr) {
	if (ptr->m_canBeSaved) {
		ConfigItems item;
		item.m_option = string(ptr->GetName());
		item.m_submenu = GetSubmenuHandler()->GetCurrentSubmenu()->GetConfigName();
		item.m_type = "color";
		item.m_value = Utils::va("%i,%i,%i,%i", ptr->m_color->m_color.r, ptr->m_color->m_color.g, ptr->m_color->m_color.b, ptr->m_color->m_color.a);
		GetConfig()->m_configItemsToSave.push_back(item);
	}
}

void ColorOption::Input(int pos) {

}

void ColorOption::Render(int pos) {
	m_onUpdate();
	GetRenderer()->DrawString(m_name, 454, 198 + (pos * GetRenderer()->m_optionHeight), GetRenderer()->m_selectedFont, 0.58f, Color(255, 255, 255, 255), false, false, JustifyLeft);

	GetRenderer()->DrawOutlinedRect(GetMenu()->m_center.x + 170, 182 + (pos * GetRenderer()->m_optionHeight), 16, 16, 1, *m_color, *m_color, true);
}

void ColorOption::RenderSelected(int pos) {
	m_currentOp();
	if (IsOptionPressed() && m_requirement()) {
		GetMenuInput()->m_colorCallback = m_function;
		GetMenuInput()->m_classPtr = this;
		strcpy(GetMenuInput()->m_name, m_name);
		GetMenuInput()->m_accepted = false;
		GetMenuInput()->m_changingColor = m_color;
		GetMenuInput()->alphaSlider = m_color->m_color.a;
		GetMenu()->m_isShowingAnotherWindow = true;
	}
}

void ColorOption::LoadFromConfig(ConfigResolvedItems resolved) {
	if (m_canBeSaved) {
		if (resolved.m_type == eCOLOR) {
			if (!strcmp(this->GetName(), resolved.m_option.c_str())) {
				m_color->m_color = resolved.m_color;
				m_function();
			}
		}
	}
}