#include "stdafx.h"

namespace Dashboard_Calls {
	HRESULT(*XuiElementGetId)(HXUIOBJ hObj, LPCWSTR *pszId);
	HRESULT(*XuiElementGetChildById)(HXUIOBJ hObj, LPCWSTR szId, HXUIOBJ *phChild);

	HRESULT(*XuiElementAddChild)(HXUIOBJ hObj, HXUIOBJ hChild);
	HRESULT(*XuiElementGetParent)(HXUIOBJ hObj, HXUIOBJ* phParent);

	HRESULT(*XuiFigureSetFill)(HXUIOBJ hObj, XUI_FILL_TYPE nFillType, DWORD dwFillColor, XUIGradientStop *pStops, int nNumStops, float fGradientAngle, const D3DXVECTOR2 *pvScale, const D3DXVECTOR2 *pvTrans);

	HRESULT(*XuiCreateObject)(LPCWSTR szClassName, HXUIOBJ *phObj);
	HRESULT(*XuiElementSetBounds)(HXUIOBJ hObj, float fWidth, float fHeight);
	HRESULT(*XuiElementSetPosition)(HXUIOBJ hObj, const D3DXVECTOR3 *pvPos);

	HRESULT(*XUIElementPropVal_SetString)(Dashboard_Structures::XUIElementPropVal* pThis, LPCWSTR val);
	HRESULT(*XuiObjectGetProperty)(HXUIOBJ hObj, DWORD dwPropId, DWORD dwIndex, Dashboard_Structures::XUIElementPropVal *pVal);
	HRESULT(*XuiObjectSetProperty)(HXUIOBJ hObj, DWORD dwPropId, DWORD dwIndex, const Dashboard_Structures::XUIElementPropVal *pVal);
	HRESULT(*XuiObjectGetPropertyId)(HXUIOBJ hObj, LPCWSTR szPropName, DWORD *pdwId);

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

		return Dashboard_Calls::XuiFigureSetFill(hObj, fillType, Color1, gs, 2, 0, &pvScalingFactor, &pTrnas);
	}

	HRESULT SetProperty(HXUIOBJ Obj, LPCWSTR szPropName, Dashboard_Structures::XUIElementPropVal* propVal) {
		HRESULT result = E_FAIL;

		DWORD propID = 0;
		if (NT_SUCCESS(result = XuiObjectGetPropertyId(Obj, szPropName, &propID)))
			result = XuiObjectSetProperty(Obj, propID, 0, propVal);

		return result;
	}

	HRESULT GetProperty(HXUIOBJ Obj, LPCWSTR szPropName, Dashboard_Structures::XUIElementPropVal* propVal) {
		HRESULT result = E_FAIL;

		DWORD propID = 0;
		if (NT_SUCCESS(result = XuiObjectGetPropertyId(Obj, szPropName, &propID)))
			result = XuiObjectGetProperty(Obj, propID, 0, propVal);

		return result;
	}
}