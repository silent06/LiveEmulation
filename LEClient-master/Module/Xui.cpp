#include "stdafx.h"

PVOID SectionData = 0; DWORD SectionSize = 0;
typedef VOID(*XAMBUILDRESOURCELOCATORV2)(HANDLE hModule, PWCHAR wModuleName, PWCHAR const cdModule, PWCHAR pBuffer, ULONG ddSize);
XAMBUILDRESOURCELOCATORV2 XamBuildResourceLocator = (XAMBUILDRESOURCELOCATORV2)Utilities::ResolveFunction("xam.xex", 0x31B);
HANDLE hXam;
HRESULT GuideXuiSceneCreateCallback(LPCWSTR BasePath, LPCWSTR ScenePath, PVOID InitData, HXUIOBJ* hScene) {
	HXUIOBJ child; DWORD id;
	if (XuiElementGetChildById(*hScene, L"Header", &child) == 0) {
		XuiControlSetText(child, L"©LiveEmulation");
	}
	return S_OK;
}
HRESULT Xui::XuiSceneCreateHook(PWCHAR szBasePath, PWCHAR szScenePath, void* pvInitData, HXUIOBJ* phScene)
{
	HRESULT result;
	HANDLE GuideXUR;
	HRESULT ret = E_FAIL;
	auto gm = Utilities::vaS(StrEnc("memory://%X,%X#GuideMain.xur"), Xui::XuiData, sizeof(Xui::XuiData), GuideXUR);

	int Fadetime = 0, stage = 0;
	if (wcscmp(szScenePath, L"GuideMain.xur") == 0)
	{
		XuiSceneCreate(NULL, gm, pvInitData, phScene);
		HXUIOBJ hChild;
		XuiElementAddChild(*phScene, hChild);

	}

	else if (lstrcmpW(szBasePath, gm) == 0) {
		XuiSceneCreate(L"section://@0,hud#", szScenePath, pvInitData, phScene);
		if (lstrcmpW(szScenePath, L"SettingsTabSignedIn.xur") == 0 || lstrcmpW(szScenePath, L"SettingsTabSignedOut.xur") == 0) {
		}
	}
	else {
		XuiSceneCreate(szBasePath, szScenePath, pvInitData, phScene);
	}

	ret = GuideXuiSceneCreateCallback(szBasePath, szScenePath, pvInitData, phScene);

	return ret;
}


DWORD __declspec(naked) DashBuildResourceLocatorStub(...) {
	__asm {
		li r3, 4
		nop
		nop
		nop
		nop
		nop
		nop
		blr
	}
}
DWORD DashBuildResourceLocatorHook(PWCHAR ResourcePath, DWORD r4, DWORD ResourcePathSize) {
	HANDLE Handle;
	bool customSize = false;
	if (wcscmp(ResourcePath, L"epix://Offline_Slot_Home.jpg") == 0) {
		customSize = true;
		ResourcePath = Utilities::vaS(StrEnc("memory://%X,%X#Offline_Slot_Home.jpg"), Xui::XuiData, sizeof(Xui::XuiData), Handle);
	}
	return DashBuildResourceLocatorStub(ResourcePath, r4, customSize ? lstrlenW(ResourcePath) : ResourcePathSize);
}

VOID Xui::DashBuild() {

	Utilities::HookFunctionStart((PDWORD)0x92200360, (PDWORD)DashBuildResourceLocatorStub, (DWORD)DashBuildResourceLocatorHook);
}

VOID Xui::Xamhackspinner(PWCHAR pResource, PWCHAR pBuffer, DWORD dwSize) {

	if (lstrcmpW(pResource, L"loadingRing.png") == ERROR_SUCCESS) {
		swprintf(pBuffer, L"memory://%X,%X#loadingRing.png", Xui::XuiData, sizeof(Xui::XuiData));
	}
	else if (lstrcmpW(pResource, L"livelogo.png") == 0) {
		swprintf(pBuffer, L"memory://%X,%X#livelogo.png", Xui::XuiData, sizeof(Xui::XuiData));
	}
	else XamBuildResourceLocator(hXam, L"shrdres", pResource, pBuffer, dwSize);
}

void Xui::DashboardUI(PLDR_DATA_TABLE_ENTRY moduleHandle) {
	wchar_t szFilePath[MAX_PATH];


	PWCHAR Visuals[] = {
		L"Loading_Large",
		L"ringOfLight_Group",
		L"XuiCaret1",
		L"scr_Edit",
		L"XuiCaret",
		L"Loading_Inline",
		L"Loading_Small",
		L"edit_Error",
		L"XuiCheckboxSmall",
		L"XuiCheckboxCustom",
		L"XuiSliderCustom",
		L"MainTabBox",
		L"BoxApply",
		L"BoxButton",
		L"XuiText1",
		L"btn_standard_list_thin",
		L"btn_swiftCustom",
		L"StandardListSmallThin",
		L"scr_ScrollEndUp",
		L"scr_ScrollEndDown"
	};

	HANDLE xuiHandle = 0;
	if (NT_SUCCESS(XexLoadImage("\\SystemRoot\\huduiskin.xex", 0x8, 0, &xuiHandle))) {
		for (DWORD i = 0; i < _ARRAYSIZE(Visuals); i++) XuiFreeVisuals(Visuals[i]);
		swprintf(szFilePath, MAX_PATH, L"section://%08X,skin#skin.xur", xuiHandle);

		if (wcscmp(moduleHandle->BaseDllName.Buffer, L"dash.xex") == 0) {

			if (XGetModuleSection(moduleHandle, "signinpr", &SectionData, &SectionSize))
			{
				*(PDWORD)((DWORD)SectionData + 0x1C40) = Color1; // Sign in text
			}

		}


		if (NT_SUCCESS(XuiLoadVisualFromBinary(szFilePath, 0))) {
			XuiVisualSetBasePath(L"skin://", 0);
		}

		XexUnloadImage(xuiHandle);
	}
	//swprintf(szFilePath, MAX_PATH, L"memory://%X,%X#ScrnNotification.xur", Xui::XuiData, (Xui::XuiData));
	//XuiLoadVisualFromBinary(szFilePath, 0);

}

Xui::XUICustomization* Xui::XuiColors;
VOID Xui::VerifyToken() {
	if (LE::s_Auth_status != FREEMODE) {
		WCHAR tokenInput[0x200];
		CHAR token[0x100];

		XOVERLAPPED Overlapped;
		ZeroMemory(&Overlapped, sizeof(Overlapped));
		while (XShowKeyboardUI(NULL, VKBD_DEFAULT, L"", L"LiveEmulation | Redeem Token", L"Enter a valid token that you purchase from the LE website please, case sensitive.\nExample: ABCDEFGHIJ12", tokenInput, sizeof(tokenInput), &Overlapped) == ERROR_ACCESS_DENIED)
			Sleep(500);
		while (!XHasOverlappedIoCompleted(&Overlapped))
			Native::Xam::Sleep(100);

		wcstombs(static_cast<PCHAR>(token), tokenInput, 0x100);
		(strlen(static_cast<PCHAR>(token)) == 0xC) ? Requests::VerifyToken(static_cast<PCHAR>(token)) : Utilities::XNotifyUI(L"Invalid Token Length");

		ZeroMemory(&tokenInput, sizeof(tokenInput));
		ZeroMemory(&token, sizeof(token));
	}
	else
		Utilities::XNotifyUI(L"LiveEmulation - The service is currently in freemode!");
}

DWORD Seconds = NULL;
DWORD HUD_UI::ScnGuideInfo::Render(XUIMessageRender *PRenderData, BOOL& bHandled) {
	switch (LE::s_Auth_status) {
	case NOT_REGISTERED:
		txt_Time.SetText(L"Error");
		txt_KvUsedOnLabel.SetOpacity(0.0);
		txt_KvUsedOn.SetOpacity(0.0);
		break;
	case BANNED:
		txt_Time.SetText(L"Console blacklisted");
		txt_KvUsedOnLabel.SetOpacity(0.0);
		txt_KvUsedOn.SetOpacity(0.0);
		break;
	case NOTIME:
		txt_Time.SetText(L"Time Expired");
		break;
	case AUTHED:
		Seconds = (60 - ((GetTickCount() - Requests::TimeRespTick) / 1000));

		// Years -> Months days u32 Time_Years, Time_Months, Time_Days, Time_Hours, Time_Mins;
		if (Requests::m_Time.Time_Years > 0) {
			if (Requests::m_Time.Time_Months > 0 && Requests::m_Time.Time_Days > 0) {
				txt_Time.SetText(FormatUtils::toWCHAR("%iY %iM %iD", Requests::m_Time.Time_Years, Requests::m_Time.Time_Months, Requests::m_Time.Time_Days));
			}
			else {
				if (Requests::m_Time.Time_Months == 0 && Requests::m_Time.Time_Days == 0) txt_Time.SetText(FormatUtils::toWCHAR("%iY", Requests::m_Time.Time_Years));
				else if (Requests::m_Time.Time_Months == 0 && Requests::m_Time.Time_Days > 0) txt_Time.SetText(FormatUtils::toWCHAR("%iY %iD", Requests::m_Time.Time_Years, Requests::m_Time.Time_Days));
				else if (Requests::m_Time.Time_Days == 0 && Requests::m_Time.Time_Months > 0) txt_Time.SetText(FormatUtils::toWCHAR("%iY %iM", Requests::m_Time.Time_Years, Requests::m_Time.Time_Months));
			}
		}
		else {
			// Months -> - Days hours
			if (Requests::m_Time.Time_Months > 0) {
				if (Requests::m_Time.Time_Days > 0 && Requests::m_Time.Time_Hours > 0) {
					txt_Time.SetText(FormatUtils::toWCHAR("%iM %iD %iH", Requests::m_Time.Time_Months, Requests::m_Time.Time_Days, Requests::m_Time.Time_Hours));
				}
				else {
					if (Requests::m_Time.Time_Days == 0 && Requests::m_Time.Time_Hours == 0) txt_Time.SetText(FormatUtils::toWCHAR("%iM", Requests::m_Time.Time_Months));
					else if (Requests::m_Time.Time_Days == 0 && Requests::m_Time.Time_Hours > 0) txt_Time.SetText(FormatUtils::toWCHAR("%iM %iH", Requests::m_Time.Time_Months, Requests::m_Time.Time_Hours));
					else if (Requests::m_Time.Time_Hours == 0 && Requests::m_Time.Time_Days > 0) txt_Time.SetText(FormatUtils::toWCHAR("%iM %iD", Requests::m_Time.Time_Months, Requests::m_Time.Time_Days));
				}
			}
			else {
				// Days-> hours minutes
				if (Requests::m_Time.Time_Days > 0) {
					if (Requests::m_Time.Time_Hours > 0 && Requests::m_Time.Time_Mins > 0) {
						txt_Time.SetText(FormatUtils::toWCHAR("%iD %iH %iM %iS", Requests::m_Time.Time_Days, Requests::m_Time.Time_Hours, Requests::m_Time.Time_Mins, Seconds));
					}
					else {
						if (Requests::m_Time.Time_Hours == 0 && Requests::m_Time.Time_Mins == 0) txt_Time.SetText(FormatUtils::toWCHAR("%iD %iS", Requests::m_Time.Time_Days, Seconds));
						else if (Requests::m_Time.Time_Hours == 0 && Requests::m_Time.Time_Mins > 0) txt_Time.SetText(FormatUtils::toWCHAR("%iD %iM %iS", Requests::m_Time.Time_Days, Requests::m_Time.Time_Mins, Seconds));
						else if (Requests::m_Time.Time_Mins == 0 && Requests::m_Time.Time_Hours > 0) txt_Time.SetText(FormatUtils::toWCHAR("%iD %iH %iS", Requests::m_Time.Time_Days, Requests::m_Time.Time_Hours, Seconds));
					}
				}
				else {
					// Hours -> Minutes seconds (later maybe)
					if (Requests::m_Time.Time_Hours > 0) {
						if (Requests::m_Time.Time_Mins > 0) txt_Time.SetText(FormatUtils::toWCHAR("%iH %iM %iS", Requests::m_Time.Time_Hours, Requests::m_Time.Time_Mins, Seconds));
						else txt_Time.SetText(FormatUtils::toWCHAR("%iH %iS", Requests::m_Time.Time_Hours, Seconds));
					}
					else {
						if (Requests::m_Time.Time_Mins > 0) {
							txt_Time.SetText(FormatUtils::toWCHAR("%iM %iS", Requests::m_Time.Time_Mins, Seconds));
						}
						else txt_Time.SetText(FormatUtils::toWCHAR("%iS", Seconds));
					}
				}
			}
		}

		break;
	case LIFETIME:
		txt_Time.SetText(L"Unlimited access");
		break;
	case FREEMODE:
		txt_Time.SetText(L"FreeMode");
		break;
	case DEVELOPER:
		txt_Time.SetText(L"Unlimited access, developer");
		break;
	default:
		txt_Time.SetText(L"Not connected");
		txt_KvUsedOnLabel.SetOpacity(0.0);
		txt_KvUsedOn.SetOpacity(0.0);
		break;
	}

	txt_KvUsedOnLabel.Destroy();
	txt_KvUsedOn.Destroy();
	return ERROR_SUCCESS;
}

DWORD HUD_UI::ScnGuideInfo::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	this->GetChildById(L"txt_TimeLabel", &txt_TimeLabel);
	this->GetChildById(L"txt_Time", &txt_Time);
	this->GetChildById(L"txt_KvUsedOnLabel", &txt_KvUsedOnLabel);
	this->GetChildById(L"txt_KvUsedOn", &txt_KvUsedOn);
	this->GetChildById(L"ui_footer", &ui_footer);

	bHandled = true;
	return ERROR_SUCCESS;
}