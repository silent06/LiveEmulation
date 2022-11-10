#pragma once

bool defaultRequirement();

class Option {
protected:
	bool(*m_requirement)();

	char m_name[100];
	string m_tooltip;
	bool m_visible;
	BYTE m_hoverTimer;
	
	bool m_isScrollOption;
public:
	Option() : m_visible(true), m_isScrollOption(false), m_requirement(defaultRequirement), m_hoverTimer(0) {}
	Option(const char* name) : m_visible(true), m_isScrollOption(false), m_requirement(defaultRequirement), m_hoverTimer(0) { SetName(name); }

	virtual void Input(int pos);
	virtual void Render(int pos);
	virtual void RenderSelected(int pos);
	virtual void LoadFromConfig(ConfigResolvedItems resolved);

	virtual void SetName(const char* name) {
		strcpy(m_name, name);
	}

	void SetVisible(bool visible) { m_visible = visible; }
	bool IsVisible() { return m_visible && m_requirement(); }
	bool IsScroll() { return m_isScrollOption; }
	const char* GetName() { return m_name; }
	string GetTooltip() { return m_tooltip; }
};

void nullsub();
bool nullsub_b();