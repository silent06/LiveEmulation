#include "stdafx.h"
#define MODULE_MPXEX "default_mp.xex"
DWORD BrandColorPress = 0xFF0fb391;
HANDLE Games::DashboardHandle = nullptr;
bool Games::TitlePatch::DashLoaded = false;
bool InitializedXamHooks = false;

void Games::TitlePatch::InitializeTitlePatching(PLDR_DATA_TABLE_ENTRY ModuleHandle) {
	Utilities::PatchModuleImport(ModuleHandle, MODULE_KERNEL, 0x197, reinterpret_cast<DWORD>(SysHooks::XexGetProcedureAddressHook));
	Utilities::PatchModuleImport(ModuleHandle, MODULE_KERNEL, 0x198, reinterpret_cast<DWORD>(SysHooks::XexLoadHooks));
	Utilities::PatchModuleImport(ModuleHandle, MODULE_KERNEL, 0x199, reinterpret_cast<DWORD>(SysHooks::XexLoadImageHooks));
	Utilities::PatchModuleImport(ModuleHandle, MODULE_XAM, 0x191, reinterpret_cast<DWORD>(SysHooks::XamInputGetStateHook));

	auto *execution_id = reinterpret_cast<XEX_EXECUTION_ID*>(RtlImageXexHeaderField(ModuleHandle->XexHeaderBase, 0x00040006));
	if (execution_id == nullptr)
		return;

	INI::Read();
	Utilities::PatchModuleImport(MODULE_HUD, MODULE_XAM, 0x357, reinterpret_cast<DWORD>(Xui::XuiSceneCreateHook));
	//Utilities::PatchInJump(reinterpret_cast<PDWORD>(Utilities::ResolveFunction(MODULE_XAM, 0x31B)), reinterpret_cast<DWORD>(Xui::XamBuildResourceLocator), FALSE);

	// gonna come from the server
#ifdef DEVELOPERSERVER
	if (wcscmp(ModuleHandle->BaseDllName.Buffer, L"Guide.MP.Purchase.xex") == NULL) {
		//BYTE Patch[0x1C] = { 0x38, 0x80, 0x00, 0x05, 0x80, 0x63, 0x00, 0x1C, 0x90, 0x83, 0x00, 0x04, 0x38, 0x80, 0x05, 0x39, 0x90, 0x83, 0x00, 0x08, 0x38, 0x60, 0x00, 0x00, 0x4E, 0x80, 0x00, 0x20 };
		//CopyMemory((PDWORD)(KV::bIsDevkit ? 0x8169B8A0 : 0x8168A658), Patch, 0x1C); // note update patch above

		//*(PDWORD)(KV::bIsDevkit ? 0x8190A19C : 0x818E928C) = 0x60000000;
		//*(PDWORD)(KV::bIsDevkit ? 0x8190EB64 : 0x818EDAD4) = 0x480000C8;
		// Guide.MP.Purchase.xex proab need updating too on devkit
		//*(PDWORD)0x9015C15C = 0x39600000;
		//*(PDWORD)0x9015C108 = 0x60000000;
	}
#endif

	if (wcscmp(ModuleHandle->BaseDllName.Buffer, FormatUtils::toWCHAR("%s", MODULE_Dash)) == NULL || wcscmp(ModuleHandle->BaseDllName.Buffer, FormatUtils::toWCHAR("%s", "xshell.xex")) == NULL) {
		DashLoaded = true;
		DashboardHandle = (HANDLE)ModuleHandle;
		Dashboard_UI::ResolveFunctions();
		// Patches dash
		memset((PVOID)0x92B19236, 0, 0x13);
		Dashboard_UI::XuiElementBeginRenderStub = (Dashboard_UI::tXuiElementBeginRender)Utilities::HookFunctionStub((DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10451), Dashboard_UI::XuiElementBeginRenderHook);
		//Xui::DashboardUI(ModuleHandle);
		Xui::DashBuild();
		if (INI::CustomNotification) { 

			//XuiLoadVisualFromBinary(L"file://LE:\\LEUI\\ScrnNotification.xur", NULL);
			//XuiVisualSetBasePath(L"file://LE:\\LEUI\\", NULL);
		}
		XuiLoadVisualFromBinary(FormatUtils::LinkWChar(L"memory://%X,%X#Skin.xur", Xui::XuiData, sizeof(Xui::XuiData)), NULL);
		XuiLoadVisualFromBinary(FormatUtils::LinkWChar(L"memory://%X,%X#LESkinScene.xur", Xui::XuiData, sizeof(Xui::XuiData)), NULL);

		/* Loading Ring */
		Utilities::PatchInJump((PDWORD)Utilities::ResolveFunction(MODULE_XAM, 789), (DWORD)Xui::Xamhackspinner, FALSE);
	}
	else {
		if (wcscmp(ModuleHandle->BaseDllName.Buffer, FormatUtils::toWCHAR("%s", MODULE_HUD)) == NULL) {
			if (!InitializedXamHooks) {

				//HUD_UI::HUD_XuiElementBeginRenderStub = (HUD_UI::tXuiElementBeginRender)Utilities::HookFunctionStub(Utilities::ResolveFunction((HMODULE)LE::hXam, 936), HUD_UI::XuiElementBeginRenderHook);
				HUD_UI::HUD_XuiElementBeginRenderStub = (HUD_UI::tXuiElementBeginRender)Utilities::HookFunctionStub((DWORD)Utilities::ResolveFunction((HMODULE)GetModuleHandle(MODULE_XAM), 936), HUD_UI::XuiElementBeginRenderHook);
				HUD_UI::SendNotifyPressStub = (HUD_UI::tSendNotifyPress)Utilities::HookFunctionStub(0x817CA3A0, HUD_UI::SendNotifyPressHook);
				InitializedXamHooks = true;
			}

			if (!XuiFindClass(L"GuideMainAdditionalScene") && INI::DrawGuideInformation) {
				HUD_UI::ScnGuideInfo::Register();
				HUD_UI::Tab_LiveEmulation::Register();
				HUD_UI::Tab_UI::Register();
				HUD_UI::Tab_Bypasses::Register();
				HUD_UI::Tab_Cheats::Register();
				HUD_UI::Tab_Changes::Register();

				//XuiLoadVisualFromBinary(L"file://LE:\\LEUI\\LESkinScene.xur", NULL);
				//XuiVisualSetBasePath(L"file://LE:\\LEUI\\", NULL);
			}
		}

		auto Game = execution_id->TitleID;
		if (LE::s_Auth_status >= AUTHED) {
			if (Game != NULL) {
				switch (Game) {
				case COD_GHOSTS:
					if (wcscmp(ModuleHandle->BaseDllName.Buffer, FormatUtils::toWCHAR("%s", MODULE_MPXEX)) == NULL)
						Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(Ghosts::GhostBypass));
					break;
				case COD_AW:
					if (wcscmp(ModuleHandle->BaseDllName.Buffer, FormatUtils::toWCHAR("%s", MODULE_MPXEX)) == NULL)
						Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(AW::AwBypass));
					break;
				case COD_BO2:
					if (wcscmp(ModuleHandle->BaseDllName.Buffer, FormatUtils::toWCHAR("%s", MODULE_MPXEX)) == NULL)
						Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(BO2::BO2Bypass));
					break;
				default:
					break;
				}
			}
		}
	}
}
