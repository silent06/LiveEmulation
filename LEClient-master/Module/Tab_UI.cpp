#include "stdafx.h"

bool SettingsTabSceneInit = false;
DWORD HUD_UI::Tab_UI::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	SettingsTabSceneInit = false;
	INI::Read();

	this->GetChildById(L"slider_R", &slider_R);
	this->GetChildById(L"slider_G", &slider_G);
	this->GetChildById(L"slider_B", &slider_B);
	this->GetChildById(L"slider_Rotation", &slider_Rotation);
	this->GetChildById(L"slider_R2", &slider_R2);
	this->GetChildById(L"slider_G2", &slider_G2);
	this->GetChildById(L"slider_B2", &slider_B2);

	this->GetChildById(L"lblDesc", &labeldesc);

	this->GetChildById(L"checkboxCustomNotification", &chkCustomNotify);
	this->GetChildById(L"ckhLEImage", &chkLEImage);
	this->GetChildById(L"checkboxDrawGuideInformation", &chkGuideInfo);
	this->GetChildById(L"chkGradientBG", &chkGradientBG);
	this->GetChildById(L"chkGradientBtns", &chkGradientBtns);
	this->GetChildById(L"chkColorFade", &chkColorFade);
	this->GetChildById(L"checkboxHudColorModifications", &chkGradient);
	this->GetChildById(L"checkboxDashUiModifications", &checkboxDashUiModifications);
	this->GetChildById(L"btnPreview", &hbtnPreview);
	this->GetChildById(L"btnSave", &btnSave);

	btnSave.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#star_full.png", Xui::XuiData, sizeof(Xui::XuiData)));

	chkCustomNotify.SetCheck(INI::CustomNotification);
	chkLEImage.SetCheck(INI::LEImage);
	chkGuideInfo.SetCheck(INI::DrawGuideInformation);
	chkGradientBG.SetCheck(INI::GradientBG);
	chkGradientBtns.SetCheck(INI::GradientBtns);
	chkColorFade.SetCheck(INI::ColorFade);
	chkGradient.SetCheck(INI::Gradient);
	checkboxDashUiModifications.SetCheck(0);/*Does Nothing*/

	slider_R.SetValue((int)((Color1 >> 16) & 0xFF));
	slider_G.SetValue((int)((Color1 >> 8) & 0xFF));
	slider_B.SetValue((int)((Color1) & 0xFF));

	slider_R2.SetValue((int)((Color2 >> 16) & 0xFF));
	slider_G2.SetValue((int)((Color2 >> 8) & 0xFF));
	slider_B2.SetValue((int)((Color2) & 0xFF));

	HUD_UI::SetFill(hbtnPreview, Color2, Color1, INI::Gradient);

	SettingsTabSceneInit = true;
	bHandled = TRUE;
	return ERROR_SUCCESS;
}

DWORD HUD_UI::Tab_UI::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	if (hObjPressed == chkCustomNotify)
		INI::CustomNotification = chkCustomNotify.IsChecked();
	//else if (hObjPressed == chkLEImage)
		//INI::LEImage = chkLEImage.IsChecked();
	else if (hObjPressed == chkGuideInfo)
		INI::DrawGuideInformation = chkGuideInfo.IsChecked();
	else if (hObjPressed == chkGradientBG)
		INI::GradientBG = chkCustomNotify.IsChecked();
	else if (hObjPressed == chkGradientBtns)
		INI::GradientBtns = chkGradientBtns.IsChecked();
	else if (hObjPressed == chkGradient) {
		INI::Gradient = chkGradient.IsChecked();

		HUD_UI::SetFill(hbtnPreview, Color2, Color1, INI::Gradient);
	}
	else if (hObjPressed == chkColorFade) {
		INI::ColorFade = chkColorFade.IsChecked();
		//if (Fade) CreateThread((LPTHREAD_START_ROUTINE)HUD::DoRainbow);
	}
	else if (hObjPressed == btnSave) {
		INI::UpdateSettings();
		Utilities::XNotifyUI("LiveEmulation - Settings saved and updated!");
	}

	bHandled = TRUE;
	return ERROR_SUCCESS;
}

HRESULT HUD_UI::Tab_UI::OnNotifyValueChanged(HXUIOBJ hObjSource, XUINotifyValueChanged *pNotifyValueChangedData, BOOL& bHandled) {
	if (SettingsTabSceneInit) {
		if (hObjSource == slider_R.m_hObj || hObjSource == slider_G.m_hObj || hObjSource == slider_B.m_hObj) {
			int R, G, B;

			slider_R.GetValue(&R);
			slider_G.GetValue(&G);
			slider_B.GetValue(&B);

			Color1 = D3DCOLOR_RGBA(R, G, B, 0xFF);
		}
		else if (hObjSource == slider_Rotation.m_hObj || hObjSource == slider_R2.m_hObj || hObjSource == slider_G2.m_hObj || hObjSource == slider_B2.m_hObj) {
			int R2, G2, B2;

			slider_R2.GetValue(&R2);
			slider_G2.GetValue(&G2);
			slider_B2.GetValue(&B2);

			Color2 = D3DCOLOR_RGBA(R2, G2, B2, 0xFF);
		}

		HUD_UI::SetFill(hbtnPreview, Color2, Color1, INI::Gradient);
	}

	bHandled = TRUE;
	return ERROR_SUCCESS;
}