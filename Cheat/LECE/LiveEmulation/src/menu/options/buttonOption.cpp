#include "stdafx.h"

DWORD WINAPI KeyboardHandlerThread(LPVOID lp) {
	XOVERLAPPED Overlapped;
	ZeroMemory(&Overlapped, sizeof(Overlapped));

	WCHAR wideBuffer[0x100];
	XShowKeyboardUI(0, VKBD_DEFAULT, L"", L"Emulation CE", L"Enter the name you want to set your config to", wideBuffer, 0x100, &Overlapped);

	while (!XHasOverlappedIoCompleted(&Overlapped)) Sleep(1000);

	char lowKeyboardBuffer[0x100];
	wcstombs(lowKeyboardBuffer, wideBuffer, 0x100);

	((void(*)(const char*))(DWORD)lp)(lowKeyboardBuffer);

	return 0;
}

void ButtonOption::Input(int pos) {

}

void ButtonOption::Render(int pos) {
	m_onUpdate();

	GetRenderer()->DrawString(m_name, 454, 198 + (pos * GetRenderer()->m_optionHeight), GetRenderer()->m_selectedFont, 0.58f, Color(255, 255, 255, 255), false, false, JustifyLeft);

	// sprite
	//if(m_sprite)
		//GetRenderer()->DrawShader(454 + m_materialOffset.x, 198 + (pos * GetRenderer()->m_optionHeight) + m_materialOffset.y, m_materialScale.x, m_materialScale.y, Color(0xFFFFFFFF), m_material);
}

void ButtonOption::RenderSelected(int pos) {
	m_currentOp();
	if (IsOptionPressed() && m_requirement()) {
		if (m_hasKeyboard) {
			CreateThread(0, 0, KeyboardHandlerThread, m_keyboardCallback, 0, 0);
		}
		m_function();
	}
}