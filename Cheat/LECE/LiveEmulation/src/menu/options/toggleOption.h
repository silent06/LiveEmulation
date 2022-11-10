#pragma once

class ToggleOption : public Option {
private:
	void(*m_onUpdate)();
	void(*m_currentOp)();
	void(*m_function)();
	bool* m_bool;

	float x_greenWidth;
	float x_tempGreen;

	float x_redWidth;
	float x_tempRed;

	float x_barPos;
	float x_tempBar;

	bool m_canBeSaved;
public:
	ToggleOption(const char* name) : Option(name), m_function(nullsub), m_canBeSaved(false), m_onUpdate(nullsub), m_bool(nullptr), m_currentOp(nullsub) {}

	ToggleOption &addToggle(bool& toggle) {
		m_bool = &toggle;
		return *this;
	}

	ToggleOption& addFunction(void(*func)()) {
		m_function = func;
		return *this;
	}

	ToggleOption& addOnUpdate(void(*func)()) {
		m_onUpdate = func;
		return *this;
	}

	ToggleOption& addOnHover(void(*func)()) {
		m_currentOp = func;
		return *this;
	}

	ToggleOption& addRequirement(bool(*func)()) {
		m_requirement = func;
		return *this;
	}

	ToggleOption& addTooltip(string tooltip) {
		m_tooltip = tooltip;
		return *this;
	}

	ToggleOption& canBeSaved() {
		m_canBeSaved = true;
		return *this;
	}

	void LoadFromConfig(ConfigResolvedItems resolved);
	void Input(int pos);
	void Render(int pos);
	void RenderSelected(int pos);
};