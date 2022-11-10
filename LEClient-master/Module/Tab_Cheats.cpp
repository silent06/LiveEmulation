#include "stdafx.h"

DWORD HUD_UI::Tab_Cheats::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	INI::Read();

	this->GetChildById(L"lblDesc", &labeldesc);
	this->GetChildById(L"btnSave", &btnSave);
	btnSave.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#star_full.png", Xui::XuiData, sizeof(Xui::XuiData)));
	btnSave.SetEnable(FALSE);

	bHandled = TRUE;
	return ERROR_SUCCESS;
}

DWORD HUD_UI::Tab_Cheats::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	bHandled = TRUE;
	return ERROR_SUCCESS;
}