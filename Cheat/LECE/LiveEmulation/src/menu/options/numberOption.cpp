#include "stdafx.h"

static bool leftDisabled = false;
static bool rightDisabled = false;
static int leftTimer = 0;
static int rightTimer = 0;

template <class Type>
void NumberOption<Type>::Input(int pos) {

}

template <class Type>
void NumberOption<Type>::Render(int pos) {
	m_onUpdate();

	switch (m_type) {
	case TOGGLE: {
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
			if (*m_bool)
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

		GetRenderer()->DrawOutlinedRect(GetMenu()->m_center.x + 161, 182 + (pos * GetRenderer()->m_optionHeight), 25, 16, 1, Color(0, 0, 0, 0), Color(255, 255, 255, 255), true);
		GetRenderer()->DrawRect(GetMenu()->m_center.x + 161, 182 + (pos * GetRenderer()->m_optionHeight), x_greenWidth, 16, JustifyLeft, Color(0x37c775FF));
		GetRenderer()->DrawRect(GetMenu()->m_center.x + 161 + 25, 182 + (pos * GetRenderer()->m_optionHeight), x_redWidth, 16, JustifyRight, Color(0xc73737FF));

		GetRenderer()->DrawRect(GetMenu()->m_center.x + 161 + x_barPos, 182 + (pos * GetRenderer()->m_optionHeight) - 2, 6, 20, JustifyLeft, Color(195, 195, 195, 255));
		GetRenderer()->DrawString(Utils::va(m_format, *m_number), GetMenu()->m_center.x + 155, 198 + (pos * GetRenderer()->m_optionHeight), GetRenderer()->m_selectedFont, 0.58f, Color(255, 255, 255, 255), false, false, JustifyRight);
		break;
	}
	case SELECT:

		break;
	case SCROLL:
	case SCROLLSELECT:
		if (m_sliderHud) {
			static const int testVal = 8;
			static const int widthOfLine = 100;
			bool isHovering = pos == GetMenu()->m_currentOption - GetMenu()->m_scrollOffset;
			float hmm = (float)(((widthOfLine * *m_number) / m_max));

			GetRenderer()->DrawOutlinedRect(testVal + (GetMenu()->m_center.x + 177) - widthOfLine, 187 + (pos * GetRenderer()->m_optionHeight), widthOfLine, 4, 1, Color(0, 0, 0, 150), isHovering ? Color(255, 255, 255) : GetRenderer()->UIColors.sliderBoxOutlineColor, true);

			GetRenderer()->DrawRect(testVal + (GetMenu()->m_center.x + 177) - widthOfLine, 187 + (pos * GetRenderer()->m_optionHeight), hmm, 4, JustifyLeft, Color(255, 255, 255));

			GetRenderer()->DrawString(Utils::va(m_format, *m_number), testVal + 810 - widthOfLine, 198 + (pos * GetRenderer()->m_optionHeight), GetRenderer()->m_selectedFont, 0.58f, Color(255, 255, 255, 255), false, false, JustifyRight);
		} else {
			GetRenderer()->DrawString(Utils::va(m_format, *m_number), GetMenu()->m_center.x + 175, 198 + (pos * GetRenderer()->m_optionHeight), GetRenderer()->m_selectedFont, 0.58f, Color(255, 255, 255, 255), false, false, JustifyRight);
		}
		break;
	}

	GetRenderer()->DrawString(m_name, 454, 198 + (pos * GetRenderer()->m_optionHeight), GetRenderer()->m_selectedFont, 0.58f, Color(255, 255, 255, 255), false, false, JustifyLeft);
}

template <class Type>
void NumberOption<Type>::RenderSelected(int pos) {
	m_currentOp();
	if (IsOptionPressed() && m_requirement()) {
		switch (m_type) {
		case TOGGLE: *m_bool ^= true; m_function(); break;
		case SELECT: *m_selectedNumber = *m_number; break;
		case SCROLL:
		case SCROLLSELECT: m_function();  break;
		}
	}

	bool canMonitor = false;

	if (IsJustPressed(DPAD_LEFT)) {
		if (!leftDisabled) {
			*m_number -= m_step;
			if (m_loop) {
				if (*m_number < m_min) *m_number = m_max;
			}
			else if (*m_number < m_min && m_hasMin) *m_number = m_min;
			if (m_type == SCROLL) m_function();
			if (m_type == TOGGLE && *m_bool) m_function();
			canMonitor = true;
		}
		leftDisabled = false;
		leftTimer = 0;
	}

	if (IsJustPressed(DPAD_RIGHT)) {
		if (!rightDisabled) {
			*m_number += m_step;
			if (m_loop) {
				if (*m_number > m_max) *m_number = m_min;
			}
			else if (*m_number > m_max && m_hasMax) *m_number = m_max;
			if (m_type == SCROLL) m_function();
			if (m_type == TOGGLE && *m_bool) m_function();
			canMonitor = true;
		}
		rightDisabled = false;
		rightTimer = 0;
	}

	static int TIMER;

	if (IsDown(DPAD_LEFT)) {
		if (++leftTimer > 20) {
			leftDisabled = true;
			if ((Utils::timeGetTime() - TIMER) > m_scrollSpeed) {
				*m_number -= m_step;
				if (m_loop) {
					if (*m_number < m_min) *m_number = m_max;
				}
				else if (*m_number < m_min && m_hasMin) *m_number = m_min;
				if (m_type == SCROLL) m_function();
				if (m_type == TOGGLE && *m_bool) m_function();
				TIMER = Utils::timeGetTime();
				canMonitor = true;
			}
		}
	}

	if (IsDown(DPAD_RIGHT)) {
		if (++rightTimer > 20) {
			rightDisabled = true;
			if ((Utils::timeGetTime() - TIMER) > m_scrollSpeed) {
				*m_number += m_step;
				if (m_loop) {
					if (*m_number > m_max) *m_number = m_min;
				}
				else if (*m_number > m_max && m_hasMax) *m_number = m_max;
				if (m_type == SCROLL) m_function();
				if (m_type == TOGGLE && *m_bool) m_function();
				TIMER = Utils::timeGetTime();
				canMonitor = true;
			}
		}
	}

	if (canMonitor) {
		if (m_canBeSaved) {
			if (m_type == SCROLL || m_type == TOGGLE) {
				if (m_originalValue != *m_number) {
					auto vit = find(GetConfig()->m_configItemsToSave.begin(), GetConfig()->m_configItemsToSave.end(), make_pair(GetSubmenuHandler()->GetCurrentSubmenu()->GetConfigName(), string(this->GetName())));
					if (vit != GetConfig()->m_configItemsToSave.end()) {
						GetConfig()->m_configItemsToSave.erase(vit);
					}

					ConfigItems item;
					item.m_option = string(this->GetName());
					item.m_submenu = GetSubmenuHandler()->GetCurrentSubmenu()->GetConfigName();

					if (m_type == TOGGLE) {
						item.m_type = Utils::va("%s%s", m_optionTypeName, "toggle");
						item.m_value = *m_bool ? "true" : "false";

						if (strstr(m_optionTypeName, "int")) {
							item.m_extra = Utils::va("%i", *m_number);
						} else {
							item.m_extra = Utils::va("%.f", *m_number);
						}
					} else {
						item.m_type = m_optionTypeName;

						if (strstr(m_optionTypeName, "int")) {
							item.m_value = Utils::va("%i", *m_number);
						} else {
							item.m_value = Utils::va("%.f", *m_number);
						}
					}

					GetConfig()->m_configItemsToSave.push_back(item);
				}
			}
		}
	}
}

template class NumberOption<int>;
template class NumberOption<float>;
template class NumberOption<BYTE>;

template <class Type>
void NumberOption<Type>::LoadFromConfig(ConfigResolvedItems resolved) {
	if (m_canBeSaved) {
		if (!strcmp(this->GetName(), resolved.m_option.c_str())) {
			if (strstr(m_optionTypeName, "int")) {
				if (resolved.m_type == eINT || resolved.m_type == eINTTOGGLE) {
					*m_number = resolved.m_int;

					if (m_bool != nullptr) {
						if (resolved.m_type == eINTTOGGLE) {
							*m_bool = resolved.m_bool;
						}
					}
				}
			}

			if (strstr(m_optionTypeName, "float")) {
				if (resolved.m_type == eFLOAT || resolved.m_type == eFLOATTOGGLE) {
					*m_number = resolved.m_float;

					if (m_bool != nullptr) {
						if (resolved.m_type == eFLOATTOGGLE) {
							*m_bool = resolved.m_bool;
						}
					}
				}
			}
		}
	}
}