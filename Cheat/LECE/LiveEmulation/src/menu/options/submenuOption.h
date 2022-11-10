#pragma once

class Submenu;

class SubmenuOption : public Option {
public:
	void(*m_onUpdate)();
	void(*m_currentOp)();
	void(*m_function)();
	Submenu* m_submenu;

	SubmenuOption() : Option(), m_submenu(nullptr), m_function(nullsub), m_currentOp(nullsub), m_onUpdate(nullsub) {}
	SubmenuOption(const char* name) : Option(name), m_submenu(nullptr), m_function(nullsub), m_currentOp(nullsub), m_onUpdate(nullsub) {}

	template<typename T>
	SubmenuOption& addSubmenu() {
		m_submenu = T::GetInstance();
		return *this;
	}

	SubmenuOption& addSubmenu(Submenu* submenu) {
		m_submenu = submenu;
		return *this;
	}

	SubmenuOption& addFunction(void(*func)()) {
		m_function = func;
		return *this;
	}

	SubmenuOption& addOnUpdate(void(*func)()) {
		m_onUpdate = func;
		return *this;
	}

	SubmenuOption& addOnHover(void(*func)()) {
		m_currentOp = func;
		return *this;
	}

	SubmenuOption& setVisible(bool visible) {
		m_visible = visible;
		return *this;
	}

	SubmenuOption& addName(const char* name) {
		strcpy(m_name, name);
		return *this;
	}

	SubmenuOption& addTooltip(string tooltip) {
		m_tooltip = tooltip;
		return *this;
	}

	void Input(int pos);
	void Render(int pos);
	void RenderSelected(int pos);
};