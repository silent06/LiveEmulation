#pragma once

class Menu {
public:
	int m_welcomeMessageTimeout;
	bool m_needsToShowWelcomeMessage;
	char m_welcomeMessage[0x300];

	bool m_needsToShowChangelog;
	char m_changelogMessage[0x300];

	bool m_open;
	bool m_inputDisabled;
	bool m_disableAllInputThisFrame;

	bool m_isShowingAnotherWindow;

	Vector2 m_center;
	int m_maxOptions;
	int m_currentOption;
	int m_scrollOffset;
	int m_breakScroll;
	int m_openKey;
	int m_selectedTab;
	static const char* m_version;
	static int m_versionInt;
	bool m_fastPressLeft;
	bool m_fastPressRight;

	void Render();
	void ASyncUpdate();
	void ScrollEvent(int scroll);

	void Open() { m_open = true; GetSubmenuHandler()->SetSubmenuMain(); }
	void Close() { m_open = false; }
	void ToggleOpen() { Open(); }
	bool IsOpen() { return m_open; }
	void Init();
};

Menu* GetMenu();