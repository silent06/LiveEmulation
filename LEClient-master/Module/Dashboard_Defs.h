#pragma once
#include "stdafx.h"

namespace Dashboard_Structures {
	struct XUIElementPropVal {
		XUI_ELEMENT_PROPERTY_TYPE type;
		union {
			BOOL bVal;
			int nVal;
			unsigned int uVal;
			float fVal;
			WCHAR *szVal;
			D3DVECTOR vecVal;
			XUIQuaternion quatVal;
			IXUIPropObj *pObj;
			struct {
				void *pvCustomData;
				XUICustomPropHandler *pHandler;
			} CustomVal;
		};

#ifdef __cplusplus
		XUIElementPropVal() {
			((void(*)(...))(Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10079)))(this);
		}
		~XUIElementPropVal() {
			((void(*)(...))(Utilities::ResolveFunction((HMODULE)Games::DashboardHandle, 10080)))(this);
		}
#endif
	};

	struct XUIElementPropDef {
		DWORD dwFlags;
		DWORD dwId;
		DWORD dwOffset;
		DWORD dwExtra;
		LPCWSTR szPropName;
		XUI_ELEMENT_PROPERTY_TYPE Type;
		const XUIElementPropVal *pDefaultVal;
		XUICustomPropHandler *pCustomHandler;
		PFN_GETPROP pfnGetProp;
		PFN_SETPROP pfnSetProp;
		PFN_GETPROPDEF pfnGetPropDef;
		PFN_GETCOUNT pfnGetCount;

#ifndef _XBOX
		LPCWSTR szPropertyEditor;
#endif
	};

	struct XUIClass {
		DWORD cbSize;
		LPCWSTR szClassName;
		LPCWSTR szBaseClassName;
		LPCWSTR szClassDetails;
		XUIObjectMethods Methods;
		const XUIElementPropDef *pPropDefs;
		DWORD dwPropDefCount;
		BOOL bExtensionClass;
	};
}

namespace Dashboard_Calls {
	extern HRESULT(*XuiElementGetId)(HXUIOBJ hObj, LPCWSTR *pszId);
	extern HRESULT(*XuiElementGetChildById)(HXUIOBJ hObj, LPCWSTR szId, HXUIOBJ *phChild);

	extern HRESULT(*XuiElementAddChild)(HXUIOBJ hObj, HXUIOBJ hChild);
	extern HRESULT(*XuiElementGetParent)(HXUIOBJ hObj, HXUIOBJ* phParent);

	extern HRESULT(*XuiFigureSetFill)(HXUIOBJ hObj, XUI_FILL_TYPE nFillType, DWORD dwFillColor, XUIGradientStop *pStops, int nNumStops, float fGradientAngle, const D3DXVECTOR2 *pvScale, const D3DXVECTOR2 *pvTrans);

	extern HRESULT(*XuiCreateObject)(LPCWSTR szClassName, HXUIOBJ *phObj);
	extern HRESULT(*XuiElementSetBounds)(HXUIOBJ hObj, float fWidth, float fHeight);
	extern HRESULT(*XuiElementSetPosition)(HXUIOBJ hObj, const D3DXVECTOR3 *pvPos);

	extern HRESULT(*XUIElementPropVal_SetString)(Dashboard_Structures::XUIElementPropVal* pThis, LPCWSTR val);
	extern HRESULT(*XuiObjectGetProperty)(HXUIOBJ hObj, DWORD dwPropId, DWORD dwIndex, Dashboard_Structures::XUIElementPropVal *pVal);
	extern HRESULT(*XuiObjectSetProperty)(HXUIOBJ hObj, DWORD dwPropId, DWORD dwIndex, const Dashboard_Structures::XUIElementPropVal *pVal);
	extern HRESULT(*XuiObjectGetPropertyId)(HXUIOBJ hObj, LPCWSTR szPropName, DWORD *pdwId);

	extern HRESULT SetFill(HXUIOBJ hObj, DWORD Color1, DWORD Color2, BOOL Gradient);

	extern HRESULT SetProperty(HXUIOBJ Obj, LPCWSTR szPropName, Dashboard_Structures::XUIElementPropVal* propVal);

	extern HRESULT GetProperty(HXUIOBJ Obj, LPCWSTR szPropName, Dashboard_Structures::XUIElementPropVal* propVal);
}