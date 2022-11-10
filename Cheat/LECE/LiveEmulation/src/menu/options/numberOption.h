#pragma once

template<typename Type>
class NumberOption : public Option {
private:
	void(*m_onUpdate)();
	void(*m_currentOp)();
	void(*m_function)();

	ScrollOptionType m_type;
	const char* m_format;
	Type* m_number;
	Type m_originalValue;
	bool* m_bool;
	Type m_step;
	Type m_min;
	Type m_max;

	shared_ptr<Type> m_selectedNumberStorage;
	Type* m_selectedNumber;

	DWORD m_scrollSpeed;

	bool m_hasMin;
	bool m_hasMax;
	bool m_loop;

	float x_greenWidth;
	float x_tempGreen;

	float x_redWidth;
	float x_tempRed;

	float x_barPos;
	float x_tempBar;

	bool m_sliderHud;

	bool m_canBeSaved;
	const char* m_optionTypeName;
public:
	NumberOption(ScrollOptionType type, const char* name) : Option(name), m_type(type), m_hasMin(false), m_hasMax(false), m_sliderHud(true), m_loop(false), m_scrollSpeed(100), m_function(nullsub), m_onUpdate(nullsub), m_currentOp(nullsub){}
	
	NumberOption& addToggle(bool& toggle) {
		m_bool = &toggle;
		return *this;
	}

	NumberOption& disableHud(bool disable) {
		m_sliderHud = !disable;
		return *this;
	}

	NumberOption& addNumber(Type& number, const char* format, Type step) {
		if (m_type == SELECT) {
			m_selectedNumberStorage = make_shared<Type>(0);
			m_selectedNumber = m_selectedNumberStorage.get();
		}
		m_isScrollOption = true;
		m_number = &number;
		m_originalValue = number;
		m_format = format;
		m_step = step;

		m_optionTypeName = typeid(Type).name();
		return *this;
	}

	NumberOption& addSelectedNumber(Type& selectedNumber) {
		m_selectedNumber = &selectedNumber;
		return *this;
	}

	NumberOption& addMin(Type min) {
		m_min = min;
		m_hasMin = true;
		return *this;
	}

	NumberOption& addMax(Type max) {
		m_max = max;
		m_hasMax = true;
		return *this;
	}

	NumberOption& addFunction(void(*func)()) {
		m_function = func;
		return *this;
	}

	NumberOption& addOnUpdate(void(*func)()) {
		m_onUpdate = func;
		return *this;
	}

	NumberOption& addRequirement(bool(*func)()) {
		m_requirement = func;
		return *this;
	}

	NumberOption& addOnHover(void(*func)()) {
		m_currentOp = func;
		return *this;
	}

	NumberOption& setScrollSpeed(int speed) {
		m_scrollSpeed = speed;
		return *this;
	}

	NumberOption& addTooltip(string tooltip) {
		m_tooltip = tooltip;
		return *this;
	}

	NumberOption& canBeSaved() {
		m_canBeSaved = true;
		return *this;
	}

	NumberOption& canLoop() {
		m_loop = true;
		return *this;
	}

	void LoadFromConfig(ConfigResolvedItems resolved);
	void Input(int pos);
	void Render(int pos);
	void RenderSelected(int pos);
};


