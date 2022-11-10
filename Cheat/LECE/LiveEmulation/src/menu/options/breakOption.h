#pragma once
class BreakOption : public Option {
private:
	bool m_line;
	void(*m_function)();

public:
	BreakOption() : Option("________________________"), m_line(true) {}
	BreakOption(const char* name) : Option(name), m_line(false) {}

	BreakOption& addRequirement(bool(*func)()) {
		m_requirement = func;
		return *this;
	}

	void Input(int position);
	void Render(int position);
	void RenderSelected(int position);

	void SetName(const char* name) {
		strcpy(m_name, name);
		m_line = false;
	}
	void SetLine() { m_line = true; SetName("________________________"); }
};