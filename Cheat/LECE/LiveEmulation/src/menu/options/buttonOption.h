#pragma once

class ButtonOption : public Option {
private:
	void(*m_function)();
	void(*m_currentOp)();
	void(*m_onUpdate)();

	// sprite
	bool m_sprite;
	Material* m_material;
	Vector2 m_materialOffset;
	Vector2 m_materialScale;

	bool m_hasKeyboard;
	void(*m_keyboardCallback)(const char*);
public:
	ButtonOption(const char* name) : Option(name), m_hasKeyboard(false), m_function(nullsub), m_currentOp(nullsub), m_onUpdate(nullsub) {}

	ButtonOption& addFunction(void(*func)()) {
		m_function = func;
		return *this;
	}

	ButtonOption& addOnUpdate(void(*func)()) {
		m_onUpdate = func;
		return *this;
	}

	ButtonOption& addOnHover(void(*func)()) {
		m_currentOp = func;
		return *this;
	}

	ButtonOption& addRequirement(bool(*func)()) {
		m_requirement = func;
		return *this;
	}

	ButtonOption& addTooltip(string tooltip) {
		m_tooltip = tooltip;
		return *this;
	}

	ButtonOption& addKeyboard(void(*func)(const char* str)) {
		m_keyboardCallback = func;
		m_hasKeyboard = true;
		return *this;
	}

	ButtonOption& addSprite(Material* material, float xOffset, float yOffset, float w, float h) {
		m_sprite = true;
		m_material = material;
		m_materialOffset.x = xOffset;
		m_materialOffset.y = yOffset;
		m_materialScale.x = w;
		m_materialScale.y = h;
		return *this;
	}

	void Input(int pos);
	void Render(int pos);
	void RenderSelected(int pos);
};