#include "stdafx.h"

DWORD HUD_UI::Tab_Bypasses::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	INI::Read();

	this->GetChildById(L"lblDesc", &labeldesc);
	this->GetChildById(L"checkboxMachineIDSpoofing", &checkboxMachineIDSpoofing);
	this->GetChildById(L"checkboxGhosts", &checkboxGhosts);
	this->GetChildById(L"checkboxAw", &checkboxAw);
	this->GetChildById(L"checkboxBO2", &checkboxBO2);
	this->GetChildById(L"checkboxBO3", &checkboxBO3);
	this->GetChildById(L"btnSave", &btnSave);
	btnSave.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#star_full.png", Xui::XuiData, sizeof(Xui::XuiData)));
	checkboxMachineIDSpoofing.SetCheck(INI::MachineIDSpoofing);
	checkboxGhosts.SetCheck(INI::GhostBypass);
	checkboxAw.SetCheck(INI::AwBypass);
	checkboxBO2.SetCheck(INI::BO2Bypass);
	checkboxBO3.SetCheck(INI::B03Bypass);

	bHandled = TRUE;
	return ERROR_SUCCESS;
}

DWORD HUD_UI::Tab_Bypasses::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	if (hObjPressed == checkboxMachineIDSpoofing)
		INI::MachineIDSpoofing = checkboxMachineIDSpoofing.IsChecked();
	else if (hObjPressed == checkboxGhosts)
		INI::GhostBypass = checkboxGhosts.IsChecked();
	else if (hObjPressed == checkboxAw)
		INI::AwBypass = checkboxAw.IsChecked();
	else if (hObjPressed == checkboxBO2)
		INI::BO2Bypass = checkboxBO2.IsChecked();
	else if (hObjPressed == checkboxBO3)
		INI::B03Bypass = checkboxBO3.IsChecked();
	else if (hObjPressed == btnSave) {
		INI::UpdateSettings();
		//Utilities::XNotifyUI("LiveEmulation - Settings saved and updated!");
		Utilities::XNotifyQueueUI(0x10, L"LiveEmulation - Settings saved and updated!");
	}

	bHandled = TRUE;
	return ERROR_SUCCESS;
}