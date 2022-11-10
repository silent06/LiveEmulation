#include "stdafx.h"

void ToggleOption::Input(int pos) {

}

void ToggleOption::Render(int pos) {
	m_onUpdate();

	GetRenderer()->DrawString(m_name, 454, 198 + (pos * GetRenderer()->m_optionHeight), GetRenderer()->m_selectedFont, 0.58f, Color(255, 255, 255, 255), false, false, JustifyLeft);

	x_tempGreen = *m_bool ? 25.f : 0.f;
	//x_greenWidth = x_tempGreen;
	Utils::Ease(x_greenWidth, x_tempGreen, 4.f);

	x_tempRed = *m_bool ? 0.f : 25.f;
	//x_redWidth = x_tempRed;
	Utils::Ease(x_redWidth, x_tempRed, 4.f);

	x_tempBar = *m_bool ? 19.f : 0.f;
	//x_barPos = x_tempBar;
	Utils::Ease(x_barPos, x_tempBar, 4.f);

	if (x_greenWidth > 25.f) {
		if(*m_bool)
			x_greenWidth = 25.f;
		else x_greenWidth = 0.0f;
	}

	if (x_redWidth > 25.f) {
		if (*m_bool)
			x_redWidth = 25.f;
		else x_redWidth = 0.0f;
	}

	if (x_barPos > 19.f) {
		if (*m_bool)
			x_barPos = 19.f;
		else x_barPos = 0.0f;
	}

	GetRenderer()->DrawOutlinedRect(GetMenu()->m_center.x + 161, 182 + (pos * GetRenderer()->m_optionHeight), 25, 16, 1, Color(255, 255, 255, 255), Color(255, 255, 255, 255), true);
	GetRenderer()->DrawRect(GetMenu()->m_center.x + 161, 182 + (pos * GetRenderer()->m_optionHeight), x_greenWidth, 16, JustifyLeft, Color(0x37c775FF));
	GetRenderer()->DrawRect(GetMenu()->m_center.x + 161 + 25, 182 + (pos * GetRenderer()->m_optionHeight), x_redWidth, 16, JustifyRight, Color(0xc73737FF));

	GetRenderer()->DrawRect(GetMenu()->m_center.x + 161 + x_barPos, 182 + (pos * GetRenderer()->m_optionHeight) - 2, 6, 20, JustifyLeft, Color(195, 195, 195, 255));
}

void ToggleOption::RenderSelected(int pos) {
	m_currentOp();
	if (IsOptionPressed() && m_requirement()) {
		*m_bool ^= true;
		m_function();

		if (m_canBeSaved) {
			if (!*m_bool) {
				auto vit = find(GetConfig()->m_configItemsToSave.begin(), GetConfig()->m_configItemsToSave.end(), make_pair(GetSubmenuHandler()->GetCurrentSubmenu()->GetConfigName(), string(this->GetName())));
				if (vit != GetConfig()->m_configItemsToSave.end()) {
					GetConfig()->m_configItemsToSave.erase(vit);
				}
			} else {
				ConfigItems item;
				item.m_option = string(this->GetName());
				item.m_submenu = GetSubmenuHandler()->GetCurrentSubmenu()->GetConfigName();
				item.m_type = "bool";
				item.m_value = *m_bool ? "true" : "false";
				GetConfig()->m_configItemsToSave.push_back(item);
			}
		}
	}
};

void ToggleOption::LoadFromConfig(ConfigResolvedItems resolved) {
	if (m_canBeSaved) {
		if (resolved.m_type == eBOOL) {
			if (!strcmp(this->GetName(), resolved.m_option.c_str())) {
				*m_bool = resolved.m_bool;
				m_function();
			}
		}
	}
}