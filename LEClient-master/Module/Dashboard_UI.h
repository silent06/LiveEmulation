#pragma once
#include "stdafx.h"

extern DWORD Color1;
extern DWORD Color2;

namespace Dashboard_UI {
	typedef HRESULT(*tXuiElementBeginRender)(HXUIOBJ hObj, XUIMessageRender *pRenderData, XUIRenderStruct *pRenderStruct);
	extern tXuiElementBeginRender XuiElementBeginRenderStub;

	extern HRESULT XuiElementBeginRenderHook(HXUIOBJ hObj, XUIMessageRender *pRenderData, XUIRenderStruct *pRenderStruct);

	extern VOID ResolveFunctions();
}