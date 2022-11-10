#include "stdafx.h"

DWORD HUD_UI::Tab_Changes::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	INI::Read();

	this->GetChildById(L"lblDesc", &labeldesc);
	this->GetChildById(L"ListChanges", &ListChanges);

	labeldesc.SetText(L"Release of LiveEmulation V3.0 with major changes!");

	bHandled = TRUE;
	return ERROR_SUCCESS;
}

DWORD HUD_UI::Tab_Changes::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	bHandled = TRUE;
	return ERROR_SUCCESS;
}