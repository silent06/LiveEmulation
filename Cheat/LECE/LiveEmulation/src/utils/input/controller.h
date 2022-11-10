#pragma once

struct GamePadState {
	int m_isDown;
	bool m_isJustPressed;
	bool m_wasJustDown;

	void reset() {
		this->m_isDown = false;
		this->m_wasJustDown = false;
	}
};

class Controller {
public:
	GamePadState m_state[16];
	bool m_leftBypass;
	bool m_rightBypass;
	bool m_aBypass;

	void Update();
	bool IsJustPressed(int key);
	bool IsDown(int key);
};

Controller* GetController();

bool IsDown(int key);
bool IsJustPressed(int key);