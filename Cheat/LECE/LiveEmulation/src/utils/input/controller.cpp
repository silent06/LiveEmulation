#include "stdafx.h"

Controller g_controller;

void Controller::Update() {
	m_state[A].m_isDown = Native::Engine::Key_IsDown(0, Key_A) || m_aBypass;
	m_state[X].m_isDown = Native::Engine::Key_IsDown(0, Key_X);
	m_state[Y].m_isDown = Native::Engine::Key_IsDown(0, Key_Y);
	m_state[B].m_isDown = Native::Engine::Key_IsDown(0, Key_B);
	m_state[DPAD_UP].m_isDown = Native::Engine::Key_IsDown(0, Key_Up);
	m_state[DPAD_DOWN].m_isDown = Native::Engine::Key_IsDown(0, Key_Down);
	m_state[DPAD_LEFT].m_isDown = Native::Engine::Key_IsDown(0, Key_Left) || m_leftBypass;
	m_state[DPAD_RIGHT].m_isDown = Native::Engine::Key_IsDown(0, Key_Right) || m_rightBypass;
	m_state[START].m_isDown = Native::Engine::Key_IsDown(0, Key_Start);
	m_state[BACK].m_isDown = Native::Engine::Key_IsDown(0, Key_Back);
	m_state[LEFT_THUMB].m_isDown = Native::Engine::Key_IsDown(0, Key_LS);
	m_state[RIGHT_THUMB].m_isDown = Native::Engine::Key_IsDown(0, Key_RS);
	m_state[LEFT_SHOULDER].m_isDown = Native::Engine::Key_IsDown(0, Key_LB);
	m_state[RIGHT_SHOULDER].m_isDown = Native::Engine::Key_IsDown(0, Key_RB);
	m_state[LEFT_TRIGGER].m_isDown = Native::Engine::Key_IsDown(0, Key_LT);
	m_state[RIGHT_TRIGGER].m_isDown = Native::Engine::Key_IsDown(0, Key_RT);

	for (int i = 0; i < 16; i++) {
		m_state[i].m_isJustPressed = !m_state[i].m_wasJustDown && m_state[i].m_isDown;
		m_state[i].m_wasJustDown = m_state[i].m_isDown;
	}

	m_leftBypass = false;
	m_rightBypass = false;
	m_aBypass = false;
}

bool Controller::IsDown(int key) {
	return m_state[key].m_isDown;
}

bool Controller::IsJustPressed(int key) {
	return m_state[key].m_isJustPressed;
}

Controller* GetController() {
	return &g_controller;
}

bool IsDown(int key) {
	return GetController()->IsDown(key);
}

bool IsJustPressed(int key) {
	return GetController()->IsJustPressed(key);
}