#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <xuiapp.h>

typedef VOID(*XAMBUILDRESOURCELOCATOR)(HANDLE LEhandle, PWCHAR wModuleName, PWCHAR CONST cdModule, PWCHAR pBuffer, ULONG ddSize);

class Xui {

public:
	static struct XUICustomization {
		DWORD Primary;
		DWORD Secondary;
	}; 
	static VOID Xamhackspinner(PWCHAR pResource, PWCHAR pBuffer, DWORD dwSize);
	static HRESULT XuiSceneCreateHook(PWCHAR szBasePath, PWCHAR szScenePath, void* pvInitData, HXUIOBJ* phScene);
	static XUICustomization* XuiColors;
	static void DashboardUI(PLDR_DATA_TABLE_ENTRY moduleHandle);
	static BYTE XuiData[208062];
	static VOID DashBuild();
	static INT TimeTicky;
	static VOID VerifyToken();
	static BYTE Notify[118784];
};
