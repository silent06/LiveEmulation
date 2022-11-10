#pragma once

template<typename Type>
class ScrollOption : public Option {
public:
	void(*m_onUpdate)();
	void(*m_currentOp)();
	void(*m_function)();

	ScrollOptionType m_type;
	int* m_number;
	shared_ptr<int> m_selectedNumberStorage;
	int* m_selectedNumber;
	bool* m_bool;
	int m_min;
	int m_max;

	int m_scrollSpeed;

	bool m_hasMin;
	bool m_hasMax;
	bool m_loop;

	bool leftDisabled;
	bool rightDisabled;
	int leftTimer;
	int rightTimer;

	ScrollStruct<Type>* m_scrollStruct;

	float x_greenWidth;
	float x_tempGreen;

	float x_redWidth;
	float x_tempRed;

	float x_barPos;
	float x_tempBar;
public:
	ScrollOption(ScrollOptionType type, const char* name) : Option(name), m_type(type), m_hasMin(false), m_hasMax(false), m_loop(false), m_scrollSpeed(100), m_function(nullsub), m_onUpdate(nullsub), m_currentOp(nullsub) {}
	
	ScrollOption& addScroll(int& number, int min, int max, ScrollStruct<Type>* scrollStruct) {
		if (m_type == SELECT) {
			m_selectedNumberStorage = make_shared<int>(0);
			m_selectedNumber = m_selectedNumberStorage.get();
		}
		m_isScrollOption = true;
		m_hasMin = true;
		m_hasMax = true;
		m_min = min;
		m_max = max;
		m_number = &number;
		m_scrollStruct = scrollStruct;
		return *this;
	}

	ScrollOption& addToggle(bool& toggle) {
		m_bool = &toggle;
		return *this;
	}

	ScrollOption& addSelectedNumber(int&selectedNumber) {
		m_selectedNumber = selectedNumber;
		return *this;
	}

	ScrollOption& addFunction(void(*func)()) {
		m_function = func;
		return *this;
	}

	ScrollOption& addOnUpdate(void(*func)()) {
		m_onUpdate = func;
		return *this;
	}

	ScrollOption& addRequirement(bool(*func)()) {
		m_requirement = func;
		return *this;
	}

	ScrollOption& addOnHover(void(*func)()) {
		m_currentOp = func;
		return *this;
	}

	ScrollOption &canLoop() {
		m_loop = true;
		return *this;
	}

	ScrollOption& addTooltip(string tooltip) {
		m_tooltip = tooltip;
		return *this;
	}

	void Input() {

	}

	void Render(int pos) {
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
			GetRenderer()->DrawString(m_scrollStruct[*m_number].m_name, GetMenu()->m_center.x + 155, 198 + (pos * GetRenderer()->m_optionHeight), GetRenderer()->m_selectedFont, 0.58f, Color(255, 255, 255, 255), false, false, JustifyRight);
			break;
		}
		case SELECT:
			
			break;
		case SCROLL:
		case SCROLLSELECT:
			GetRenderer()->DrawString(m_scrollStruct[*m_number].m_name, GetMenu()->m_center.x + 161 + 25, 198 + (pos * GetRenderer()->m_optionHeight), GetRenderer()->m_selectedFont, 0.58f, Color(255, 255, 255, 255), false, false, JustifyRight);
			break;
		}

		GetRenderer()->DrawString(m_name, 454, 198 + (pos * GetRenderer()->m_optionHeight), GetRenderer()->m_selectedFont, 0.58f, Color(255, 255, 255, 255), false, false, JustifyLeft);
	}

	void RenderSelected(int pos) {
		m_currentOp();
		
		if (IsOptionPressed() && m_requirement()) {
			switch (m_type) {
			case TOGGLE: *m_bool ^= true; break;
			case SELECT: *m_selectedNumber = *m_number; break;
			case SCROLLSELECT: m_function(); break;
			}
		}

		if (IsJustPressed(DPAD_LEFT)) {
			if (!leftDisabled) {
				*m_number -= 1;
				if (m_loop) {
					if (*m_number < m_min) *m_number = m_max;
				}
				else if (*m_number < m_min && m_hasMin) *m_number = m_min;
				if (m_type == SCROLL) m_function();
				if (m_type == TOGGLE && *m_bool) m_function();
			}
			leftDisabled = false;
			leftTimer = 0;
		}

		if (IsJustPressed(DPAD_RIGHT)) {
			if (!rightDisabled) {
				*m_number += 1;
				if (m_loop) {
					if (*m_number > m_max) *m_number = m_min;
				}
				else if (*m_number > m_max && m_hasMax) *m_number = m_max;
				if (m_type == SCROLL) m_function();
				if (m_type == TOGGLE && *m_bool) m_function();
			}
			rightDisabled = false;
			rightTimer = 0;
		}

		static int TIMER;

		if (IsDown(DPAD_LEFT)) {
			if (++leftTimer > 20) {
				leftDisabled = true;
				if ((Utils::timeGetTime() - TIMER) > m_scrollSpeed) {
					*m_number -= 1;
					if (m_loop) {
						if (*m_number < m_min) *m_number = m_max;
					}
					else if (*m_number < m_min && m_hasMin) *m_number = m_min;
					if (m_type == SCROLL) m_function();
					if (m_type == TOGGLE && *m_bool) m_function();
					TIMER = Utils::timeGetTime();
				}
			}
		}

		if (IsDown(DPAD_RIGHT)) {
			if (++rightTimer > 20) {
				rightDisabled = true;
				if ((Utils::timeGetTime() - TIMER) > m_scrollSpeed) {
					*m_number += 1;
					if (m_loop) {
						if (*m_number > m_max) *m_number = m_min;
					}
					else if (*m_number > m_max && m_hasMax) *m_number = m_max;
					if (m_type == SCROLL) m_function();
					if (m_type == TOGGLE && *m_bool) m_function();
					TIMER = Utils::timeGetTime();
				}
			}
		}

		GetRenderer()->SetToolip(m_tooltip);
	}
};
