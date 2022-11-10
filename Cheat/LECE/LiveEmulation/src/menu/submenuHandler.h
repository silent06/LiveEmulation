#pragma once

class SubmenuHandler {
private:
	Submenu* m_mainSubmenu;
	Submenu* m_currentSubmenu;
	Submenu* m_nextSubmenu;

	int m_nextCurrentOption;
	int m_nextScrollOffset;

	vector<Submenu*> m_submenus;
public:
	//vector<Submenu*> m_submenus;

	void Init();
	void Update();
	void ASyncUpdate();

	void AddSubmenu(Submenu* submenu) { m_submenus.push_back(submenu); };
	int GetTotalOptions() { return m_currentSubmenu->GetOptionAmount(); };
	void SetSubmenu(Submenu* submenu);
	void SetSubmenuMain();
	void SetSubmenuPrevious(bool resetPos = false);
	Submenu* GetCurrentSubmenu() { return m_currentSubmenu; };

	template<typename T>
	bool IsCurrentSubmenuOfType() {
		return dynamic_cast<const T*>(GetCurrentSubmenu()) != nullptr;
	}

	vector<Submenu*> GetSubmenus() { return m_submenus; }

	int m_currentOptionsWithoutBreaks;
	int m_totalOptionsWithoutBreaks;
	int m_totalOptions;
};

SubmenuHandler* GetSubmenuHandler();