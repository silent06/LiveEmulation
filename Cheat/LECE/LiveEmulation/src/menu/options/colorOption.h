#pragma once

class ColorOption : public Option {
private:
	void(*m_function)();
	void(*m_currentOp)();
	void(*m_onUpdate)();

	Color m_default;

public:
	Color* m_color;
	bool m_canBeSaved;

	ColorOption(const char* name) : Option(name), m_canBeSaved(false), m_function(nullsub), m_onUpdate(nullsub), m_currentOp(nullsub) {}

	ColorOption& addColor(Color& color) {
		m_color = &color;
		return *this;
	}

	ColorOption& addFunction(void(*func)()) {
		m_function = func;
		return *this;
	}

	ColorOption& addOnUpdate(void(*func)()) {
		m_onUpdate = func;
		return *this;
	}

	ColorOption& addOnHover(void(*func)()) {
		m_currentOp = func;
		return *this;
	}

	ColorOption& addTooltip(string tooltip) {
		m_tooltip = tooltip;
		return *this;
	}

	ColorOption& addDefault(Color color) {
		m_default = color;
		return *this;
	}

	ColorOption& canBeSaved() {
		m_canBeSaved = true;
		return *this;
	}

	void LoadFromConfig(ConfigResolvedItems resolved);
	void Input(int pos);
	void Render(int pos);
	void RenderSelected(int pos);
};

void ColorOptionSaveConfig(ColorOption* ptr);