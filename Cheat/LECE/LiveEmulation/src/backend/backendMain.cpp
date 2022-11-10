#include "stdafx.h"
#include "../menu/submenus/visuals/visualRadar.h"
#include "../menu/submenus/visuals/visualESP.h"
#include "../menu/submenus/visuals/visualInformationMenu.h"
#include "../menu/submenus/mainMenu.h"
#include "../menu/submenus/aimbotMenu.h"
#include "../menu/submenus/aimbot/aimbotAntiAimMenu.h"

bool Backend::m_loaded;
handle_s Backend::m_hookEvents[HANDLE_COUNT];
Vector2 Backend::m_radarXY;
bool Backend::m_canRenderRadar;
DWORD Backend::m_myModuleBase;
DWORD Backend::m_myModuleSize;
UIContext* Backend::m_uiContext;

void Backend::Start() {
	if (m_loaded) { 
		LOG(true, "[Backend] Backend already loaded");
		return;
	}

	if (!GetNetSocket()->Error()) {
		if (!Requests::ServerPacketWelcome()) {
			Native::System::Sleep(5000);
			Native::System::HalReturnToFirmware(3);
			return;
		}

		if (!Requests::ServerPacketStatus()) {
			Native::System::Sleep(5000);
			Native::System::HalReturnToFirmware(3);
			return;
		}

		if (!Requests::ServerPacketVersion()) {
			Native::System::Sleep(5000);
			Native::System::HalReturnToFirmware(3);
			return;
		}

		if (!Requests::ServerPacketGetData()) {
			Native::System::Sleep(5000);
			Native::System::HalReturnToFirmware(3);
			return;
		}

		if (!Requests::ServerPacketGetFunc()) {
			Native::System::Sleep(5000);
			Native::System::HalReturnToFirmware(3);
			return;
		}

		if (!Requests::ServerPacketGetNatives()) {
			Native::System::Sleep(5000);
			Native::System::HalReturnToFirmware(3);
			return;
		}

		Requests::ServerPacketWelcomeMessage();
		Requests::ServerPacketChangelog();

		// Native::Self::RegisterThread("PT", Requests::PresenceThread);
	} else {
		LOG(true, "[Auth] Connection error: %s", GetNetSocket()->GetLastError());
		return;
	}

	if (!ApplyPatches()) {
		LOG(true, "[Backend] Failed to apply patches. Please restart.");
		return;
	}

	if (!ApplyHooks()) {
		LOG(true, "[Backend] Failed to apply hooks. Please restart.");
		return;
	}

	m_loaded = true;
}

void Backend::Cleanup() {
	auto hooks = Hooking::GetHooks();
	for (int i = 0; i < hooks.size(); i++) {
		memcpy((void*)hooks[i].m_address, hooks[i].m_originalAssembly, 0x10);

#ifdef DEVELOPER_BUILD
		LOG(false, "[Cleanup] Unhooked %s", hooks[i].m_name);
#endif
	}
}

namespace Hooks {
	typedef void(*tCG_CompassCalcDimensionsStub)(CompassType compassType, CgArray* cg, RectDef* parentRect, RectDef *rect, float *x, float *y, float *w, float *h);
	tCG_CompassCalcDimensionsStub CG_CompassCalcDimensionsStub;
	void CG_CompassCalcDimensions(CompassType compassType, CgArray* cg, RectDef* parentRect, RectDef *rect, float *x, float *y, float *w, float *h) {
		if (compassType == COMPASS_TYPE_PARTIAL) {
			if (VisualRadarMenuVars::vars.m_radar) {
				*x = rect->x;
				*y = rect->y;
				*w = rect->w;
				*h = rect->h;
				return;
			}
		}

		return CG_CompassCalcDimensionsStub(compassType, cg, parentRect, rect, x, y, w, h);
	}

	typedef void(*tMenu_PaintAll)(UIContext* Context);
	tMenu_PaintAll Menu_PaintAllStub;
	void Menu_PaintAll(UIContext* Context) {
		Menu_PaintAllStub(Context);
		Backend::m_uiContext = Context;

		static bool init = false;

		if (!init) {
			init = true;
			FunctionObfuscation(Invoker::RegisterPostAuthNatives, 0x6F8).DeleteNextCall().Call<int>();
			GetRenderer()->Init();
			GetMenu()->Init();
		}

		int curFrame = *(int*)((DWORD)Context + 0xC);
		if (curFrame > 0) {
			bool ingame = Utils::IsInGame();
			if (!ingame) {
				if (GetMenu()->m_needsToShowWelcomeMessage) {
					// box
					GetRenderer()->DrawOutlinedRect(GetMenu()->m_center.x - (390 / 2), GetMenu()->m_center.y - (500 / 2), 390, 500, 3, Color(0xD3D3D3C8), Color(0x000000FF), true);

					// ok box
					GetRenderer()->DrawOutlinedRect(GetMenu()->m_center.x - (97 / 2), GetMenu()->m_center.y + 195, 97, 30, 1, Color(0xD3D3D3FF), Color(0x000000FF), true);
					GetRenderer()->DrawString("OK", GetMenu()->m_center.x - (97 / 2) + 45, GetMenu()->m_center.y + 225, GetRenderer()->Font.normalFont, 0.90f, Color(0x000000FF), false, false, eJustify::JustifyCenter);

					GetRenderer()->DrawString(GetMenu()->m_welcomeMessage, GetMenu()->m_center.x - (390 / 2) + 10, GetMenu()->m_center.y - (500 / 2) + 25, GetRenderer()->Font.normalFont, 0.60f, Color(0x000000FF), false, false, eJustify::JustifyLeft);
					return;
				}

				if (GetMenu()->m_needsToShowChangelog) {
					// box
					GetRenderer()->DrawOutlinedRect(GetMenu()->m_center.x - (390 / 2), GetMenu()->m_center.y - (500 / 2), 390, 500, 3, Color(0xD3D3D3C8), Color(0x002020FF), true);

					// ok box
					GetRenderer()->DrawOutlinedRect(GetMenu()->m_center.x - (97 / 2), GetMenu()->m_center.y + 195, 97, 30, 1, Color(0xD3D3D3FF), Color(0x000000FF), true);
					GetRenderer()->DrawString("OK", GetMenu()->m_center.x - (97 / 2) + 45, GetMenu()->m_center.y + 225, GetRenderer()->Font.normalFont, 0.90f, Color(0x000000FF), false, false, eJustify::JustifyCenter);

					GetRenderer()->DrawString(GetMenu()->m_changelogMessage, GetMenu()->m_center.x - (390 / 2) + 10, GetMenu()->m_center.y - (500 / 2) + 25, GetRenderer()->Font.normalFont, 0.60f, Color(0x000000FF), false, false, eJustify::JustifyLeft);
					return;
				}
			}

			if (ingame) {
				GetController()->Update();
			}

			GetSubmenuHandler()->ASyncUpdate();

			if (ingame) {
				GetMenu()->Render();
			}

			GetMenu()->ASyncUpdate();
			GetMenuInput()->Update();
		}
	}

	typedef void(*tR_AddCmdDrawStretchPic)(float x, float y, float width, float height, float xScale, float yScale, float xay, float yay, const float * color, Material * material);
	tR_AddCmdDrawStretchPic R_AddCmdDrawStretchPicStub;
	void R_AddCmdDrawStretchPic(float x, float y, float width, float height, float xScale, float yScale, float xay, float yay, const float * color, Material * material) {
		if (material != nullptr) {
			if (!strcmp(material->m_name, "minimap_scanlines")) {
				Backend::m_radarXY = Vector2(x, y);
				Backend::m_canRenderRadar = true;
			} 

			if ((!strcmp(material->m_name, "minimap_scanlines") || !strcmp(material->m_name, "scanlines_stencil"))) {
				if (VisualRadarMenuVars::vars.m_radar) {
					return;
				}
			}

			if (!strcmp(material->m_name, "mw2_main_cloud_overlay")) {
				return R_AddCmdDrawStretchPicStub(x, y, width, height, xScale, yScale, xay, yay, Color(255, 0, 0).GetFloatPtr(), material);
			}
		}

		R_AddCmdDrawStretchPicStub(x, y, width, height, xScale, yScale, xay, yay, color, material);
	}

	typedef DWORD(*tCL_WritePacket)(int r3);
	tCL_WritePacket CL_WritePacketStub;
	void CL_WritePacket(int r3) {
		if (Utils::IsInGame()) {
			CgArray* cg = Native::Custom::GetCG();
			ClientActive* ca = Native::Custom::GetClientActive();
			UserCommand* command = ca->GetCommand(ca->m_curCmd);
			UserCommand* oldCommand = ca->GetCommand(ca->m_curCmd - 1);
			UserCommand* newCommand = ca->GetCommand(ca->m_curCmd + 2);

			*oldCommand = *command;
			--oldCommand->m_time;
			newCommand = command;

			static bool shootOn = false;
			static bool adsOn = false;

			if (AimbotMenuVars::vars.m_required) {
				if (VisualESPMenuVars::vars.m_aimbotTarget != -1) {
					D3DXVECTOR3 viewOrigin;
					Native::Engine::CG_GetPlayerViewOrigin(0, cg, &viewOrigin);

					D3DXVECTOR3 bone = Native::Engine::GetTagPosition(VisualESPMenuVars::vars.m_aimbotTarget, AimbotMenuVars::vars.m_usingTag);

					if (AimbotMenuVars::vars.m_prediction) {
						auto targetEntity = Native::Custom::GetCEntity()[VisualESPMenuVars::vars.m_aimbotTarget];

						D3DXVECTOR3 newPosition, oldPosition;
						newPosition.x = *(float*)((DWORD)&targetEntity + 220);
						newPosition.y = *(float*)((DWORD)&targetEntity + 224);
						newPosition.z = *(float*)((DWORD)&targetEntity + 228);

						oldPosition.x = *(float*)((DWORD)&targetEntity + 108);
						oldPosition.y = *(float*)((DWORD)&targetEntity + 112);
						oldPosition.z = *(float*)((DWORD)&targetEntity + 116);

						D3DXVECTOR3 velocity = newPosition - oldPosition;
						Utils::VectorNormalize(&velocity);
						velocity *= AimbotMenuVars::vars.m_predictionDelta;

						bone += velocity;
					}

					D3DXVECTOR3 subtracted = bone - viewOrigin;

					D3DXVECTOR3 toAngle;
					Utils::VectorToAngle(subtracted, toAngle);

					if (AimbotMenuVars::vars.m_aimType == 0) { // Snap
						AimbotMenuVars::SnapAim(toAngle);
					} else {
						AimbotMenuVars::SilentAim(oldCommand, toAngle);
					}

					if (MainMenuVars::vars.m_noSpread) {
						MainMenuVars::NoSpread(oldCommand);
					}

					if (AimbotMenuVars::vars.m_autoShoot || command->m_button & 1) {
						command->m_button &= ~1;
						oldCommand->m_button |= 1;

						if (!shootOn) {
							Native::Engine::CL_ExecuteKey(0, Native::Engine::Key_GetBindingForCmd("+attack"), Key_RT);
							shootOn = true;
						}
					}
				} else {
					if (shootOn) {
						Native::Engine::CL_ExecuteKey(0, Native::Engine::Key_GetBindingForCmd("-attack"), Key_RT);
						shootOn = false;
					}
				}
			} else {
				if (shootOn) {
					Native::Engine::CL_ExecuteKey(0, Native::Engine::Key_GetBindingForCmd("-attack"), Key_RT);
					shootOn = false;
				}
			}

			if (AimbotAntiAimMenuVars::vars.m_x) {
				switch (AimbotAntiAimMenuVars::vars.m_xSelection) {
				case 0: // Angle Up
					newCommand->m_viewAngles[0] = Utils::AngleToShort(-69.0f - ca->m_baseAngles[0]);
					break;
				case 1: // Angle Down
					newCommand->m_viewAngles[0] = Utils::AngleToShort(69.99f - ca->m_baseAngles[0]);
					break;
				case 2: // Spin
					static float spin;
					spin += AimbotAntiAimMenuVars::vars.m_spinXSpeed;
					if (spin > 70.f)
						spin = -70.f;
					newCommand->m_viewAngles[0] = Utils::AngleToShort(spin - ca->m_baseAngles[0]);
					break;
				case 3: // Static Fake
					static bool flip;
					flip ^= true;
					newCommand->m_viewAngles[0] = Utils::AngleToShort((flip ? 69.99f : -69.99f) - ca->m_baseAngles[0]);
					break;
				case 4: // Shield
					newCommand->m_viewAngles[0] = Utils::AngleToShort(Utils::FloatMinMax(-69.99f, 69.99f) - ca->m_baseAngles[0]);
					break;
				}
			}

			if (AimbotAntiAimMenuVars::vars.m_y) {
				static float angle = 0.f;

				switch (AimbotAntiAimMenuVars::vars.m_ySelection) {
				case 0: // Spin Left
					if (angle > 360.f)
						angle -= 360.f;

					angle += AimbotAntiAimMenuVars::vars.m_spinYSpeed;

					newCommand->m_viewAngles[1] = Utils::AngleToShort(angle);
					break;
				case 1: // Spin Right
					if (angle < 360.f)
						angle += 360.f;

					angle -= AimbotAntiAimMenuVars::vars.m_spinYSpeed;

					newCommand->m_viewAngles[1] = Utils::AngleToShort(angle);
					break;
				case 2: // Random Jitter
					newCommand->m_viewAngles[1] = Utils::AngleToShort(rand() % 2 ? (AimbotMenuVars::vars.m_silentAngle.y - 135.0f) : (AimbotMenuVars::vars.m_silentAngle.y - 225.0f));
					break;
				}
			}

			if (AimbotMenuVars::vars.m_crouchEvent) {
				switch (AimbotMenuVars::vars.m_crouchEventSelection) {
					case 0: {// On Damage
						static bool crouched;
						if (Native::Custom::GetCG()->m_health < Native::Custom::GetCG()->m_maxHealth) {
							if (!crouched) {
								crouched = true;
								oldCommand->m_button |= 0x200;
							}
						} else {
							crouched = false;
						}
						break;
					}
					case 1: {// On Visible
						static int timer = 0;
						if (VisualESPMenuVars::vars.m_aimbotTarget != -1) {
							if (AimbotMenuVars::vars.m_visible) {
								if (GetTickCount() - timer > AimbotMenuVars::vars.m_crouchDelay) {
									timer = GetTickCount();
									oldCommand->m_button |= 0x200;
								}
							}
						}
						break;
					}
					case 2: {// On Logical
						static int timer = 0;
						if (VisualESPMenuVars::vars.m_aimbotTarget != -1) {
							if (AimbotMenuVars::vars.m_hittable) {
								if (GetTickCount() - timer > AimbotMenuVars::vars.m_crouchDelay) {
									timer = GetTickCount();
									oldCommand->m_button |= 0x200;
								}
							}
						}
						break;
					}
					case 3: {// Always
						static int timer = 0;
						if (GetTickCount() - timer > AimbotMenuVars::vars.m_crouchDelay) {
							timer = GetTickCount();
							oldCommand->m_button |= 0x200;
						}
						break;
					}
				}
			}

			if (AimbotAntiAimMenuVars::vars.m_fakeLag) {
				static int tick = 0;
				if (GetTickCount() - tick > AimbotAntiAimMenuVars::vars.m_fakeLagAmount) {
					tick = GetTickCount();

					if (oldCommand->m_button != 0x80800 && oldCommand->m_button != 0x1) { // LT & RT
						return;
					}
				}
			}
		}

		CL_WritePacketStub(r3);
	}

	typedef void(*tClientThink_real)(int* gentity, UserCommand* cmd);
	tClientThink_real ClientThink_realStub;
	void ClientThink_real(int* gentity, UserCommand* cmd) {
		clock_t time;
		handleCallback function;
		handleCall_t type;
		handle_s* ev;
		handle_t dummy;

		for (handle_t i = 0; i < HANDLE_COUNT; i++) {
			ev = &Backend::m_hookEvents[i];
			if ((function = ev->function) != NULL) {
				if (ev->type != CALL_FREE) {
					time = *(int*)(0x82FDA080 + 0x4A8);
					if (ev->startTime <= time) {
						if (ev->time <= time) {
							ev->time += ev->interval;
							if (ev->handle == NULL) {
								dummy = i;
								function(ev->arg, &dummy, ev->type);
							}
							else {
								if (ev->time > time)
									function(ev->arg, ev->handle, ev->type);
							}
							switch (ev->type) {
							default:
								break;
							case CALL_START:
								ev->type = CALL_EXECUTE;
								break;
							case CALL_TEMP:
							case CALL_STOP:
								memset(ev, 0, sizeof(handle_s));
								break;
							}
						}
					}
				}
			}
		}
		ClientThink_realStub(gentity, cmd);
	}

	typedef void(*tSV_DropClient)(int, const char*, bool);
	tSV_DropClient SV_DropClientStub;
	void SV_DropClient(int client_s, const char* reason, bool tellThem) {
		int clientNum = (client_s - *(int*)0x836C6310) / 0x68B80;

		int elPtr = *(int*)0x836C6310 + (clientNum * 0x68B80);

		if (*(int*)(elPtr + 0x352EC) && (!strcmp(reason, "EXE_TIMEDOUT") || !strcmp(reason, "EXE_SERVERCOMMANDOVERFLOW") || !strcmp(reason, "EXE_TRANSMITERROR")))
			return;

		SV_DropClientStub(client_s, reason, tellThem);
	}

	typedef const char*(*tUI_SafeTranslateString)(const char*);
	tUI_SafeTranslateString UI_SafeTranslateStringStub;
	const char* UI_SafeTranslateString(const char* gxt) {
		if (!strcmp(gxt, "PLATFORM_PLAY_ONLINE_DESC")) {
			return "Play online with the LiveEmulation cheat engine"; // Play online in Call of Duty: Modern Warfare 3 multiplayer.
		}
		else if (!strcmp(gxt, "XBOXLIVE_SIGNEDOUTOFLIVE")) {
			return "You're not signed in to Xbox LIVE chief"; // You must be signed in to Xbox LIVE to play Xbox LIVE matches.
		}
		else if (!strcmp(gxt, "MENU_OK")) {
			return "alright boss"; // OK
		}

		return UI_SafeTranslateStringStub(gxt);
	}

	typedef void(*tCG_DrawRotatedPic)(int *scrPlace, float x, float y, float width, float height, int horzAlign, int vertAlign, float angle, const float *color, Material *material);
	tCG_DrawRotatedPic CG_DrawRotatedPicStub;
	void CG_DrawRotatedPic(int *scrPlace, float x, float y, float width, float height, int horzAlign, int vertAlign, float angle, const float *color, Material *material) {
		int LinkRegister = 0;

		__asm {
			mflr r30
			mr LinkRegister, r30
			li r30, 0
		}

		if (material != nullptr) {
			if (strstr(material->m_name, "compass")) {
				printf("[CG_DrawRotatedPic] 0x%X - %s\n", LinkRegister, material->m_name);
			}
		}

		return CG_DrawRotatedPicStub(scrPlace, x, y, width, height, horzAlign, vertAlign, angle, color, material);
	}

	typedef void(*tD3DDevice_DrawIndexedVertices)(D3DDevice* pDevice, int r4, int r5, int r6);
	tD3DDevice_DrawIndexedVertices D3DDevice_DrawIndexedVerticesStub;
	void D3DDevice_DrawIndexedVertices_(D3DDevice* pDevice, int r4, int r5, int r6) {
		((void(*)(D3DDevice*, UINT))0x820A2F98)(pDevice, D3DCULL_NONE);
		((void(*)(D3DDevice*, UINT))0x820A2FC8)(pDevice, D3DFILL_WIREFRAME);
		D3DDevice_DrawIndexedVerticesStub(pDevice, r4, r5, r6);
		((void(*)(D3DDevice*, UINT))0x820A2FC8)(pDevice, D3DFILL_SOLID);
	}

	typedef void(*tDvar_GetUnpackedColorByName)(const char* dvarName, float* expandedColor);
	tDvar_GetUnpackedColorByName Dvar_GetUnpackedColorByNameStub;
	void Dvar_GetUnpackedColorByName(const char* dvarName, float* expandedColor) {
		auto hash = Utils::Joaat(dvarName);
		if (hash == 0x35b75e0a) { // cg_TeamColor_EnemyTeam
			auto color = Color(0xc73737FF);
			expandedColor[0] = color.GetFloatPtr()[0];
			expandedColor[1] = color.GetFloatPtr()[1];
			expandedColor[2] = color.GetFloatPtr()[2];
			expandedColor[3] = color.GetFloatPtr()[3];
			return;
		} 
		
		if (hash == 0xa75743f1) { // cg_TeamColor_MyTeam
			auto color = Color(0x37c775FF);
			expandedColor[0] = color.GetFloatPtr()[0];
			expandedColor[1] = color.GetFloatPtr()[1];
			expandedColor[2] = color.GetFloatPtr()[2];
			expandedColor[3] = color.GetFloatPtr()[3];
			return;
		}
		
		Dvar_GetUnpackedColorByNameStub(dvarName, expandedColor);
	}

	void R_SetFrameFog(int r3) {
		if (Utils::IsInGame()) {
			if (AimbotMenuVars::vars.m_required) {
				VisualESPMenuVars::vars.m_aimbotTarget = AimbotMenuVars::GetTarget();
			} else {
				VisualESPMenuVars::vars.m_aimbotTarget = -1;
			}
		}
	}

	typedef DWORD(*tXamInputGetState)(int r3, int r4, XINPUT_STATE* state);
	tXamInputGetState XamInputGetStateStub;
	DWORD XamInputGetState(int r3, int r4, XINPUT_STATE* state) {
		DWORD ret = XamInputGetStateStub(r3, r4, state);

		if (GetMenu()->m_needsToShowWelcomeMessage) {
			if (state->Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				GetMenu()->m_welcomeMessageTimeout = GetTickCount();
				GetMenu()->m_needsToShowWelcomeMessage = false;
			}

			state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_UP;
			state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_DOWN;
			state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_LEFT;
			state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_RIGHT;
			state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_A;
			state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_B;
			state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_X;
			state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_Y;
			state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_LEFT_SHOULDER;
			state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_RIGHT_SHOULDER;
			return ret;
		}

		if (GetMenu()->m_needsToShowChangelog) {
			if ((GetTickCount() - GetMenu()->m_welcomeMessageTimeout) > 1000) {
				if (state->Gamepad.wButtons & XINPUT_GAMEPAD_A) {
					GetMenu()->m_needsToShowChangelog = false;
				}

				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_UP;
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_DOWN;
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_LEFT;
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_RIGHT;
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_A;
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_B;
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_X;
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_Y;
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_LEFT_SHOULDER;
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_RIGHT_SHOULDER;
				return ret;
			}
		}
		
		if (GetMenu()->IsOpen()) {
			if (state->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
				GetController()->m_leftBypass = true;
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_LEFT;
			}

			if (state->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
				GetController()->m_rightBypass = true;
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_RIGHT;
			}
		}

		return ret;
	}

	typedef void(*tCG_Respawn)(int localClient, bool something);
	tCG_Respawn CG_RespawnStub;
	void CG_Respawn(int localClient, bool something) {
		CG_RespawnStub(localClient, something);
		MainMenuVars::SetData();
	}

	typedef void(*tR_AddDObjToScene)(int *obj, int *pose, unsigned int entnum, unsigned int renderFxFlags, D3DXVECTOR3 *lightingOrigin, const float *materialTime, const float *burnFraction, int altXModel, int textureOverrideIndex, int *dobjConstantSet, float lightingOriginToleranceSq, float scale, bool isMarkableViewmodel);
	tR_AddDObjToScene R_AddDObjToSceneStub;
	void R_AddDObjToScene(int *obj, int *pose, unsigned int entnum, unsigned int renderFxFlags, D3DXVECTOR3 *lightingOrigin, const float *materialTime, const float *burnFraction, int altXModel, int textureOverrideIndex, int *dobjConstantSet, float lightingOriginToleranceSq, float scale, bool isMarkableViewmodel) {
		/*
			R_SetShaderConstantSetValueW(dobjConstantSet, R_AllocShaderConstantSet(dobjConstantSet, "heroLightingR"), menu.espzm[entnum].r * 20.0f);
			R_SetShaderConstantSetValueW(dobjConstantSet, R_AllocShaderConstantSet(dobjConstantSet, "heroLightingG"), menu.espzm[entnum].g * 20.0f);
			R_SetShaderConstantSetValueW(dobjConstantSet, R_AllocShaderConstantSet(dobjConstantSet, "heroLightingB"), menu.espzm[entnum].b * 20.0f);
		*/
		return R_AddDObjToSceneStub(obj, pose, entnum, renderFxFlags, lightingOrigin, materialTime, burnFraction, altXModel, textureOverrideIndex, dobjConstantSet, lightingOriginToleranceSq, scale, isMarkableViewmodel);
	}
}

bool Backend::ApplyHooks() {
	using namespace Hooks;

	Menu_PaintAllStub = (tMenu_PaintAll)Hooking::HookFunctionStub(AuthData::Menu_PaintAll, Menu_PaintAll, "Menu_PaintAll");
	R_AddCmdDrawStretchPicStub = (tR_AddCmdDrawStretchPic)Hooking::HookFunctionStub(AuthData::R_AddCmdDrawStretchPic, R_AddCmdDrawStretchPic, "R_AddCmdDrawStretchPic");
	ClientThink_realStub = (tClientThink_real)Hooking::HookFunctionStub(AuthData::ClientThink_Real, ClientThink_real, "ClientThink_Real");
	SV_DropClientStub = (tSV_DropClient)Hooking::HookFunctionStub(AuthData::SV_DropClient, SV_DropClient, "SV_DropClient");
	UI_SafeTranslateStringStub = (tUI_SafeTranslateString)Hooking::HookFunctionStub(AuthData::UI_SafeTranslateString, UI_SafeTranslateString, "UI_SafeTranslateString");
	CL_WritePacketStub = (tCL_WritePacket)Hooking::HookFunctionStub(AuthData::CL_WritePacket, CL_WritePacket, "CL_WritePacket"); // cl_createnewcommands for everything but anti aim in cl_writepacket - do in mw3, aw and ghost
	XamInputGetStateStub = (tXamInputGetState)Hooking::HookFunctionStub(AuthData::XamInputGetState, XamInputGetState, "XamInputGetState");
	CG_RespawnStub = (tCG_Respawn)Hooking::HookFunctionStub(AuthData::CG_Respawn, CG_Respawn, "CG_Respawn");

	Hooking::HookFunctionPatch(AuthData::g_addresses[AuthData::R_SetFrameFog], R_SetFrameFog);

	//Dvar_GetUnpackedColorByNameStub = (tDvar_GetUnpackedColorByName)Hooking::HookFunctionStub(AuthData::Dvar_GetUnpackedColorByName, Dvar_GetUnpackedColorByName, "Dvar_GetUnpackedColorByName");
	//NetDll_XNetGetTitleXnAddrStub = (tNetDll_XNetGetTitleXnAddr)Hooking::HookFunctionStub(0x825809CC, NetDll_XNetGetTitleXnAddr, "NetDll_XNetGetTitleXnAddr");
	//CG_CompassCalcDimensionsStub = (tCG_CompassCalcDimensionsStub)Hooking::HookFunctionStub(0x82109E28, CG_CompassCalcDimensions, "CG_CompassCalcDimensions");
	//D3DDevice_DrawIndexedVerticesStub = (tD3DDevice_DrawIndexedVertices)Hooking::HookFunctionStub(0x820AADA8, D3DDevice_DrawIndexedVertices_, "D3DDevice_DrawIndexedVertices");
	//R_AddDObjToSceneStub = (tR_AddDObjToScene)Hooking::HookFunctionStub(0x82423018, R_AddDObjToScene, "R_AddDObjToScene");
	//CL_WritePacketStub = (tCL_WritePacket)Hooking::HookFunctionStub(0x8216DAF0, CL_WritePacket, "CL_WritePacket");
	//CG_DrawRotatedPicStub = (tCG_DrawRotatedPic)Hooking::HookFunctionStub(0x82117810, CG_DrawRotatedPic, "CG_DrawRotatedPic");

	return true;
}

bool Backend::ApplyPatches() {
	*(int*)0x82150E78 = 0x4e800020;

	//AimTarget_GetTagPos
	*(int*)0x820D4C98 = 0x60000000;
	*(int*)0x820D4D18 = 0x60000000;
	*(int*)0x820D5070 = 0x60000000;

	return true;
}
