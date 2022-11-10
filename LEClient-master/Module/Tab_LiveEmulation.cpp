#include "stdafx.h"

VOID DisplayLiveBlockMessage() {
	PCWSTR Buttons[0x2] = { L"Enable LiveBlock", L"Disable LiveBlock" };
	MESSAGEBOX_RESULT Result;
	XOVERLAPPED OverLapped;

	ZeroMemory(&Result, sizeof(Result));
	ZeroMemory(&OverLapped, sizeof(OverLapped));

	XShowMessageBoxUI(XUSER_INDEX_ANY, L"LiveEmulation | Are you sure?", L"You are about to block XboxLives Dns which will result in a disconnection from XboxLive.\nPressing yes will Enable the block feature, while pressing Disable will disable the block feature.\nNote LiveBlock will be Enabled until you disable it here again!", ARRAYSIZE(Buttons), Buttons, NULL, XMB_ALERTICON, &Result, &OverLapped);
	while (!XHasOverlappedIoCompleted(&OverLapped))
		Native::Xam::Sleep(500);

	if (Result.dwButtonPressed == 0) {
		INI::EnableBlockXblDns = TRUE;
		INI::UpdateSettings();
		Utilities::SetLiveBlock(INI::EnableBlockXblDns);
		Utilities::XNotifyUI(L"LiveEmulation - The LiveDns has been [blocked]!");
	}
	if (Result.dwButtonPressed == 1) {
		INI::EnableBlockXblDns = FALSE;
		INI::UpdateSettings();
		Utilities::SetLiveBlock(INI::EnableBlockXblDns);
		Utilities::XNotifyUI(L"LiveEmulation - The LiveDns has been [Unblocked]!");
	}
}

VOID DisplayKvLifeDurationMessage() {
	PCWSTR Button[0x1] = { L"Okay" };
	MESSAGEBOX_RESULT Result;
	XOVERLAPPED OverLapped;

	ZeroMemory(&Result, sizeof(Result));
	ZeroMemory(&OverLapped, sizeof(OverLapped));

	XShowMessageBoxUI(XUSER_INDEX_ANY, L"LiveEmulation | KV Duration",
		FormatUtils::toWCHAR("Your Keyvault has been unbanned since: %s.\nWhich is %i Days %i Hours  %i Mins of unban time!\nConsoles using this KV %i.", Requests::m_TimeKvFirstUnbannedTimeStamp, Requests::m_Time.UnBanTime_Days, Requests::m_Time.UnBanTime_Hours, Requests::m_Time.UnBanTime_Mins, Requests::m_Time.KvUsedOn), ARRAYSIZE(Button), Button, NULL, XMB_ALERTICON, &Result, &OverLapped);
	while (!XHasOverlappedIoCompleted(&OverLapped))
		Native::Xam::Sleep(500);
}

DWORD HUD_UI::Tab_LiveEmulation::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	this->GetChildById(L"btnRedeemTkn", &btnRedeemTkn);
	this->GetChildById(L"btnKVOverview", &btnCheckTime);
	this->GetChildById(L"btnResetXbdm", &btnResetXbdm);
	this->GetChildById(L"btnGurdian", &btnGurdian);
	this->GetChildById(L"btnBlockLiveDns", &btnBlockLiveDns);

	btnRedeemTkn.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#Tokens.png", Xui::XuiData, sizeof(Xui::XuiData)));
	btnCheckTime.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#CheckTime.png", Xui::XuiData, sizeof(Xui::XuiData)));
	btnResetXbdm.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#XbdmRefresh.png", Xui::XuiData, sizeof(Xui::XuiData)));
	btnGurdian.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#KvGuard.png", Xui::XuiData, sizeof(Xui::XuiData)));
	btnBlockLiveDns.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#entherneticon.png", Xui::XuiData, sizeof(Xui::XuiData)));
	
	/*btnRedeemTkn.SetImage(L"file://LE:\\LEUI\\Tokens.png");
	btnCheckTime.SetImage(L"file://LE:\\LEUI\\kvoverview.png");
	btnResetXbdm.SetImage(L"file://LE:\\LEUI\\XbdmRefresh.png");
	btnGurdian.SetImage(L"file://LE:\\LEUI\\KvGuard.png");
	btnBlockLiveDns.SetImage(L"file://LE:\\LEUI\\entherneticon.png");*/
	btnGurdian.SetEnable(FALSE);

	bHandled = TRUE;
	return ERROR_SUCCESS;
}

DWORD HUD_UI::Tab_LiveEmulation::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	if (hObjPressed == btnRedeemTkn)
		Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(Xui::VerifyToken));
	else if (hObjPressed == btnCheckTime)
		Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(DisplayKvLifeDurationMessage));
	else if (hObjPressed == btnResetXbdm) {
		XexUnloadImage(GetModuleHandle("xbdm.xex"));
		if (SUCCEEDED(XexLoadImage("LE:\\xbdm.xex", XEX_LOADIMG_FLAG_DLL, NULL, nullptr)))
			Utilities::XNotifyUI("LiveEmulation - Successfully reset xbdm!");
	}
	else if (hObjPressed == btnGurdian)
		Utilities::XNotifyUI("LiveEmulation - This feature is not yet completed!");
	else if (hObjPressed == btnBlockLiveDns)
		Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(DisplayLiveBlockMessage));

	bHandled = TRUE;
	return ERROR_SUCCESS;
}