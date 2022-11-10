#include "stdafx.h"

//DWORD BrandColor = 0xFF0A7562;

DWORD Color1 = 0xFF209079;
DWORD Color2 = 0xFF30C6AA;

namespace Dashboard_UI {
	tXuiElementBeginRender XuiElementBeginRenderStub;

	HXUIOBJ objLEImage;
	HRESULT AddImage(HXUIOBJ ObjToAttatchTo, HXUIOBJ* Obj, LPCWSTR Id, FLOAT x, FLOAT y, LPCWSTR ImagePath) {
		D3DXVECTOR3 pos;
		Dashboard_Structures::XUIElementPropVal propVal;

		Dashboard_Calls::XuiCreateObject(XUI_CLASS_IMAGE, Obj);

		Dashboard_Calls::XuiElementSetBounds(*Obj, 255.0, 28.0);

		pos.x = x; pos.y = y; pos.z = 0;
		Dashboard_Calls::XuiElementSetPosition(*Obj, &pos);

		Dashboard_Calls::XUIElementPropVal_SetString(&propVal, Id);
		Dashboard_Calls::SetProperty(*Obj, L"Id", &propVal);

		Dashboard_Calls::XUIElementPropVal_SetString(&propVal, ImagePath);
		Dashboard_Calls::SetProperty(*Obj, L"ImagePath", &propVal);

		return Dashboard_Calls::XuiElementAddChild(ObjToAttatchTo, *Obj);
	}

	HRESULT XuiElementBeginRenderHook(HXUIOBJ hObj, XUIMessageRender *pRenderData, XUIRenderStruct *pRenderStruct) {
		HXUIOBJ hParentObj; LPCWSTR ObjID, ParentText;
		Dashboard_Calls::XuiElementGetId(hObj, &ObjID);
		Dashboard_Calls::XuiElementGetParent(hObj, &hParentObj);
		Dashboard_Calls::XuiElementGetId(hParentObj, &ParentText);

		if (NT_SUCCESS(XuiElementBeginRenderStub(hObj, pRenderData, pRenderStruct))) {
			HXUIOBJ randomasshandlefunction = ((HXUIOBJ(*)(...))(DWORD)Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10681))();
			if (INI::LEImage && XuiElementGetChildById(randomasshandlefunction, L"imgLE", &objLEImage) != 0) {
				AddImage(randomasshandlefunction, &objLEImage, L"imgLE", 12.0f, 94.00, L"common://ico_64x_console.png");
			}

			if (lstrcmpW(ObjID, L"Front") == 0)
				Dashboard_Calls::SetFill(hObj, Color2, Color1, INI::Gradient);
			else if (lstrcmpW(ObjID, L"Background") == 0 || lstrcmpW(ObjID, L"GreenHighlight") == 0 || lstrcmpW(ObjID, L"_Background") == 0 || lstrcmpW(ParentText, L"DescriptionGrid") == 0 || lstrcmpW(ObjID, L"figSearchBox") == 0 || lstrcmpW(ParentText, L"HubSlotXzpScene") == 0)
				Dashboard_Calls::SetFill(hObj, Color2, Color1, INI::Gradient);

			else if (lstrcmpW(ObjID, L"figSearchBox") == 0 || lstrcmpW(ParentText, L"DescriptionGrid") == 0 || lstrcmpW(ParentText, L"HubSlotXzpScene") == 0 ||
				lstrcmpW(ParentText, L"Back") == 0 || lstrcmpW(ParentText, L"Color") == 0 || lstrcmpW(ParentText, L"_SceneHost") == 0
				|| lstrcmpW(ObjID, L"labBackground") == 0 || lstrcmpW(ObjID, L"HeaderShader") == 0 || lstrcmpW(ObjID, L"gameList") == 0 || lstrcmpW(ObjID, L"titleList") == 0
				|| lstrcmpW(ObjID, L"achievementList") == 0 || lstrcmpW(ObjID, L"SlotContainerButton") == 0 || lstrcmpW(ObjID, L"HeaderBackground") == 0 || lstrcmpW(ObjID, L"graphic_metapane") == 0
				|| lstrcmpW(ObjID, L"HeaderShader") == 0 || lstrcmpW(ObjID, L"graphic_memsilver") == 0 || lstrcmpW(ObjID, L"gamercard") == 0
				|| lstrcmpW(ObjID, L"gamercardsmbkg") == 0 || lstrcmpW(ObjID, L"gamercardbg") == 0 || lstrcmpW(ObjID, L"graphic_metapane") == 0 || lstrcmpW(ObjID, L"headerPanel") == 0
				|| lstrcmpW(ObjID, L"figGraphicMeta") == 0 || lstrcmpW(ObjID, L"BackgroundPanel") == 0 || lstrcmpW(ObjID, L"XeLive_BackgroundUnderline") == 0 || lstrcmpW(ObjID, L"XeLive_InfoBoxBorderLeft") == 0 || lstrcmpW(ObjID, L"XeLive_InfoBoxBorderBottom") == 0
				|| lstrcmpW(ObjID, L"ShaderFigure") == 0 || lstrcmpW(ObjID, L"baseMetaPane") == 0)
				if (true)
					Dashboard_Calls::SetFill(hObj, Color2, Color1, INI::Gradient);

			return ERROR_SUCCESS;
		}

		return E_FAIL;
	}

	VOID ResolveFunctions() {
		Dashboard_Calls::XuiElementGetId = (HRESULT(*)(HXUIOBJ, LPCWSTR*))(DWORD)Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10176);
		Dashboard_Calls::XuiElementGetChildById = (HRESULT(*)(HXUIOBJ, LPCWSTR, HXUIOBJ*))(DWORD)Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10177);

		Dashboard_Calls::XuiElementAddChild = (HRESULT(*)(HXUIOBJ, HXUIOBJ))(DWORD)Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10155);
		Dashboard_Calls::XuiElementGetParent = (HRESULT(*)(HXUIOBJ, HXUIOBJ*))(DWORD)Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10157);

		Dashboard_Calls::XuiFigureSetFill = (HRESULT(*)(HXUIOBJ, XUI_FILL_TYPE, DWORD, XUIGradientStop*, int, float, const D3DXVECTOR2*, const D3DXVECTOR2*))(DWORD)Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10210);

		Dashboard_Calls::XuiCreateObject = (HRESULT(*)(LPCWSTR, HXUIOBJ *))(DWORD)Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10028);
		Dashboard_Calls::XuiElementSetBounds = (HRESULT(*)(HXUIOBJ, float, float))(DWORD)Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10165);
		Dashboard_Calls::XuiElementSetPosition = (HRESULT(*)(HXUIOBJ, const D3DXVECTOR3*))(DWORD)Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10167);

		Dashboard_Calls::XUIElementPropVal_SetString = (HRESULT(*)(Dashboard_Structures::XUIElementPropVal*, LPCWSTR))(DWORD)Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10094);
		Dashboard_Calls::XuiObjectGetProperty = (HRESULT(*)(HXUIOBJ, DWORD, DWORD, Dashboard_Structures::XUIElementPropVal*))(DWORD)Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10047);
		Dashboard_Calls::XuiObjectSetProperty = (HRESULT(*)(HXUIOBJ, DWORD, DWORD, const Dashboard_Structures::XUIElementPropVal *))(DWORD)Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10048);
		Dashboard_Calls::XuiObjectGetPropertyId = (HRESULT(*)(HXUIOBJ, LPCWSTR, DWORD *))(DWORD)Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10044);
	}
}