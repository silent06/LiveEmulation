#pragma once

extern Color g_tempColor;

class MenuInput {
private:
	void Setup();
	vector<void(*)()> m_functionQueue;
public:
	MenuInput() {
		m_gridColor = GetRenderer()->m_baseColor;
	};

	void AddToQueue(void(*func)()) { m_functionQueue.push_back(func); }
	void Update();
	void Col();

	Color* m_changingColor;
	Color m_gridColor;
	int alphaSlider;
	void(*m_colorCallback)();
	ColorOption* m_classPtr;
	char m_name[100];
	bool m_accepted;

	static const int GRID_COUNT = 30;
	static const int SELECTION_COUNT = 30;
	static const int GRID_ITEM_WIDTH = 10;
	static const int GRID_ITEM_HEIGHT = 10;
	static const int SELECTION_ITEM_WIDTH = 10;
	static const int SELECTION_ITEM_HEIGHT = 10;
};

MenuInput* GetMenuInput();