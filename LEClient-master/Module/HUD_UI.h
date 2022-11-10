#pragma once
#include "stdafx.h"

namespace HUD_UI {
	typedef INT(*tSendNotifyPress)(HXUIOBJ r3, WORD r4, WORD r5);
	typedef HRESULT(*tXuiElementBeginRender)(HXUIOBJ hObj, XUIMessageRender *pRenderData, XUIRenderStruct *pRenderStruct);

	extern tSendNotifyPress SendNotifyPressStub;
	extern tXuiElementBeginRender HUD_XuiElementBeginRenderStub;

	extern HRESULT SetFill(HXUIOBJ hObj, DWORD Color1, DWORD Color2, BOOL Gradient);

	extern HRESULT XuiElementBeginRenderHook(HXUIOBJ hObj, XUIMessageRender *pRenderData, XUIRenderStruct *pRenderStruct);
	extern INT SendNotifyPressHook(HXUIOBJ r3, WORD r4, WORD r5);

	class Tab_LiveEmulation : CXuiSceneImpl {
	public:
		CXuiControl btnRedeemTkn, btnCheckTime, btnResetXbdm, btnGurdian, btnBlockLiveDns;

		XUI_IMPLEMENT_CLASS(Tab_LiveEmulation, L"iniTab1", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()
			XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
			XUI_ON_XM_INIT(OnInit)
			XUI_END_MSG_MAP()

			DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
	};

	class Tab_UI : CXuiSceneImpl {
	public:
		CXuiSlider slider_R, slider_G, slider_B, slider_Rotation, slider_R2, slider_G2, slider_B2;
		HXUIOBJ hbtnPreview;

		CXuiTextElement labeldesc;
		CXuiCheckbox chkCustomNotify, chkLEImage, chkGuideInfo, chkColorFade, chkGradient, chkGradientBtns, chkGradientBG, checkboxDashUiModifications;
		CXuiControl btnSave;

		XUI_IMPLEMENT_CLASS(Tab_UI, L"iniTab2", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()
			XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
			XUI_ON_XM_INIT(OnInit)
			XUI_ON_XM_NOTIFY_VALUE_CHANGED(OnNotifyValueChanged)
			XUI_END_MSG_MAP()

			DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
		HRESULT OnNotifyValueChanged(HXUIOBJ hObjSource, XUINotifyValueChanged *pNotifyValueChangedData, BOOL& bHandled);
	};

	class Tab_Bypasses : CXuiSceneImpl {
	public:
		CXuiTextElement labeldesc;
		CXuiCheckbox checkboxMachineIDSpoofing, checkboxGhosts, checkboxAw, checkboxBO2, checkboxBO3;
		CXuiControl btnSave;

		XUI_IMPLEMENT_CLASS(Tab_Bypasses, L"iniTab3", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()
			XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
			XUI_ON_XM_INIT(OnInit)
			XUI_END_MSG_MAP()

			DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
	};

	class Tab_Cheats : CXuiSceneImpl {
	public:
		CXuiTextElement labeldesc;
		CXuiControl btnSave;

		XUI_IMPLEMENT_CLASS(Tab_Cheats, L"iniTab4", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()
			XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
			XUI_ON_XM_INIT(OnInit)
			XUI_END_MSG_MAP()

			DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
	};

	class Tab_Changes : CXuiSceneImpl {
	public:
		CXuiListItem ListChanges;
		CXuiTextElement labeldesc;

		XUI_IMPLEMENT_CLASS(Tab_Changes, L"iniTab5", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()
			XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
			XUI_ON_XM_INIT(OnInit)
			XUI_END_MSG_MAP()

			DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
	};

	class ScnGuideInfo : CXuiSceneImpl {
	public:
		CXuiControl ui_footer;
		CXuiTextElement txt_TimeLabel, txt_Time, txt_KvUsedOnLabel, txt_KvUsedOn;

		XUI_IMPLEMENT_CLASS(ScnGuideInfo, L"GuideMainAdditionalScene", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()
			XUI_ON_XM_INIT(OnInit)
			XUI_ON_XM_RENDER(Render)
			XUI_END_MSG_MAP()

			DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		DWORD Render(XUIMessageRender *PRenderData, BOOL& bHandled);
	};
}