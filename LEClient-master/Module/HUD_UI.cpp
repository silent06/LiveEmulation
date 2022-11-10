#include "stdafx.h"

namespace HUD_UI {
	tSendNotifyPress SendNotifyPressStub;
	tXuiElementBeginRender HUD_XuiElementBeginRenderStub;

	HXUIOBJ hSettingsTab;
	HXUIOBJ hGuideMain;
	HXUIOBJ newScene;
	HXUIOBJ hSliderR, hSliderG, hSliderB;
	HANDLE hHudSkin;
	HXUIOBJ hChild;

	HRESULT SetProperty(HXUIOBJ Obj, LPCWSTR szPropName, XUIElementPropVal* propVal) {
		HRESULT result = E_FAIL;

		DWORD propID = 0;
		if (NT_SUCCESS(result = XuiObjectGetPropertyId(Obj, szPropName, &propID)))
			result = XuiObjectSetProperty(Obj, propID, 0, propVal);

		return result;
	}

	HRESULT GetProperty(HXUIOBJ Obj, LPCWSTR szPropName, XUIElementPropVal* propVal) {
		HRESULT result = E_FAIL;

		DWORD propID = 0;
		if (NT_SUCCESS(result = XuiObjectGetPropertyId(Obj, szPropName, &propID)))
			result = XuiObjectGetProperty(Obj, propID, 0, propVal);

		return result;
	}

	HRESULT AddButton(HXUIOBJ ObjToAttatchTo, HXUIOBJ* Obj, LPCWSTR Id, LPCWSTR Text) {
		XUIElementPropVal propVall;
		D3DXVECTOR3 pos;

		HXUIOBJ hBtnLastChild;
		LPCWSTR LastButtonId;
		XuiElementGetLastChild(ObjToAttatchTo, &hBtnLastChild);
		XuiElementGetId(hBtnLastChild, &LastButtonId);

		propVall.SetVal(Id);
		SetProperty(hBtnLastChild, L"NavDown", &propVall);

		XuiElementGetPosition(hBtnLastChild, &pos);

		XUIElementPropVal propVal;

		XuiCreateObject(XUI_CLASS_BUTTON, Obj);

		XuiElementSetBounds(*Obj, 323.0, 28.0);

		pos.y += 28;
		XuiElementSetPosition(*Obj, &pos);

		propVal.SetVal(Id);
		SetProperty(*Obj, L"Id", &propVal);

		propVal.SetVal(Text);
		SetProperty(*Obj, L"Text", &propVal);

		propVal.SetVal(L"XuiButtonGuide");
		SetProperty(*Obj, L"Visual", &propVal);

		propVal.SetVal(LastButtonId);
		SetProperty(*Obj, L"NavUp", &propVal);

		XuiElementAddChild(ObjToAttatchTo, *Obj);

		return ERROR_SUCCESS;
	}

	HRESULT SetFill(HXUIOBJ hObj, DWORD Color1, DWORD Color2, BOOL Gradient) {
		XUI_FILL_TYPE fillType = XUI_FILL_TYPE::XUI_FILL_SOLID;
		if (INI::Gradient)
			fillType = (Gradient) ? XUI_FILL_TYPE::XUI_FILL_LINEAR_GRADIENT : XUI_FILL_TYPE::XUI_FILL_SOLID;

		D3DXVECTOR2 pvScalingFactor(1.0f, 1.0f), pTrnas(0.0f, 0.0f);

		XUIGradientStop gs[2];
		gs[0].dwColor = Color1;
		gs[0].fPos = 0.0f;

		gs[1].dwColor = Color2;
		gs[1].fPos = 1.0f;

		return XuiFigureSetFill(hObj, fillType, Color1, gs, 2, 0, &pvScalingFactor, &pTrnas);
	}

	HRESULT XuiElementBeginRenderHook(HXUIOBJ hObj, XUIMessageRender *pRenderData, XUIRenderStruct *pRenderStruct) {
		HXUIOBJ hParentObj; LPCWSTR ObjID, ParentText;
		XuiElementGetId(hObj, &ObjID);
		XuiElementGetParent(hObj, &hParentObj);
		XuiElementGetId(hParentObj, &ParentText);

		if (NT_SUCCESS(HUD_XuiElementBeginRenderStub(hObj, pRenderData, pRenderStruct))) {
			if (lstrcmpW(ObjID, L"2") == 0)
				SetFill(hObj, Color2, Color1, INI::Gradient);
			else if (lstrcmpW(ObjID, L"RGBBlade") == 0)
				SetFill(hObj, Color2, Color1, INI::Gradient);
			else if (lstrcmpW(ObjID, L"background") == 0 || lstrcmpW(ObjID, L"GreenHighlight") == 0 || lstrcmpW(ObjID, L"GreenHighlight1") == 0
				|| lstrcmpW(ObjID, L"Scroller") == 0 || lstrcmpW(ObjID, L"Bar_mid1") == 0 || lstrcmpW(ObjID, L"Text") == 0)
				SetFill(hObj, Color2, Color1, INI::Gradient);

			if (lstrcmpW(ParentText, L"Tabscene") == 0) {
				XuiElementGetParent(hParentObj, &hGuideMain);

				if (!XuiHandleIsValid(hChild) && INI::DrawGuideInformation) {
					if (NT_SUCCESS(XuiSceneCreate(nullptr, FormatUtils::LinkWChar(L"memory://%X,%X#GuideInfo.xur", Xui::XuiData, sizeof(Xui::XuiData)), NULL, &hChild)))
						XuiElementAddChild(hGuideMain, hChild);
				}

				XuiTabSceneGetCurrentTab(hParentObj, &hSettingsTab);
				XuiElementGetFirstChild(hSettingsTab, &hSettingsTab);
				XuiElementGetNext(hSettingsTab, &hSettingsTab);
				XuiElementGetId(hSettingsTab, &ParentText);

				if (lstrcmpW(ParentText, L"iniTab5") == 0 || lstrcmpW(ParentText, L"iniTab5") == 0) {
					HXUIOBJ Btn;
					if (XuiElementGetChildById(hSettingsTab, L"btnLEMenu", &Btn) != 0) {
						HXUIOBJ hBtnXamShutdown;
						if (XuiElementGetChildById(hSettingsTab, L"btnXamShutdown", &hBtnXamShutdown) == 0)
							XuiDestroyObject(hBtnXamShutdown);

						AddButton(hSettingsTab, &Btn, L"btnLEMenu", L"LiveEmulation Menu");
					}
				}
			}
		}

		return ERROR_SUCCESS;
	}

	INT SendNotifyPressHook(HXUIOBJ r3, WORD r4, WORD r5) {
		XUIElementPropVal propVal;
		GetProperty(r3, L"Text", &propVal);

		if (lstrcmpW(propVal.szVal, L"LiveEmulation Menu") == 0) {
			//if (NT_SUCCESS(XuiSceneCreate(NULL, L"file://LE:\\LEUI\\LECustomScene.xur", NULL, &newScene)))
				//XuiSceneNavigateForward(hGuideMain, FALSE, newScene, 0xFF);
			XuiSceneCreate(nullptr, FormatUtils::LinkWChar(L"memory://%X,%X#LECustomScene.xur", Xui::XuiData, sizeof(Xui::XuiData)), NULL, &newScene) & XuiSceneNavigateForward(hGuideMain, FALSE, newScene, 0xFF);
			return 0;
		}

		if (lstrcmpW(propVal.szVal, L"LE Menu") == 0) {
			XuiSceneCreate(nullptr, FormatUtils::LinkWChar(L"memory://%X,%X#LECustomScene.xur", Xui::XuiData, sizeof(Xui::XuiData)), NULL, &newScene) & XuiSceneNavigateForward(hGuideMain, FALSE, newScene, 0xFF);
			return 0;
		}

		return SendNotifyPressStub(r3, r4, r5);
	}
}