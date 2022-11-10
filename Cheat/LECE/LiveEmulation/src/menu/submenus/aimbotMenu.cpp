#include "stdafx.h"
#include "aimbotMenu.h"
#include "visuals/visualESP.h"
#include "aimbot/aimbotAntiAimMenu.h"
#include <unordered_map>

using namespace AimbotMenuVars;

namespace AimbotMenuVars {
	Vars vars;

	struct FractionSorter {
		const char* m_tag;
		float m_fraction;

		FractionSorter() {
			m_fraction = 0.0f;
		}

		FractionSorter(const char* tag, float fraction)
			: m_tag(tag), m_fraction(fraction) {
		}
	};

	ScrollStruct<const char*> m_boneNames[] = {
		"Auto Bone", "",
		"", "j_mainroot"
		"", "j_coatfront_le"
		"", "j_coatfront_ri"
		"", "j_coatrear_le"
		"", "j_coatrear_ri"
		"", "j_hip_le"
		"", "j_hip_ri"
		"", "j_spinelower"
		"", "j_hiptwist_le"
		"", "j_hiptwist_ri"
		"", "j_knee_le"
		"", "j_knee_ri"
		"", "j_shorts_le"
		"", "j_shorts_lift_le"
		"", "j_shorts_lift_ri"
		"", "j_shorts_ri"
		"", "j_spineupper"
		"", "j_ankle_le"
		"", "j_ankle_ri"
		"", "j_knee_bulge_le"
		"", "j_knee_bulge_ri"
		"", "j_spine4"
		"", "j_ball_le"
		"", "j_ball_ri"
		"", "j_clavicle_le"
		"", "j_clavicle_ri"
		"", "j_collar"
		"", "j_neck"
		"", "j_shoulderraise_le"
		"", "j_shoulderraise_ri"
		"", "j_head"
		"", "j_shoulder_le"
		"", "j_shoulder_ri"
		"", "j_brow_le"
		"", "j_brow_ri"
		"", "j_cheek_le"
		"", "j_cheek_ri"
		"", "j_elbow_bulge_le"
		"", "j_elbow_bulge_ri"
		"", "j_elbow_le"
		"", "j_elbow_ri"
		"", "j_eye_lid_bot_le"
		"", "j_eye_lid_bot_ri"
		"", "j_eye_lid_top_le"
		"", "j_eye_lid_top_ri"
		"", "j_eyeball_le"
		"", "j_eyeball_ri"
		"", "j_head_end"
		"", "j_jaw"
		"", "j_levator_le"
		"", "j_levator_ri"
		"", "j_lip_top_le"
		"", "j_lip_top_ri"
		"", "j_mouth_le"
		"", "j_mouth_ri"
		"", "j_shouldertwist_le"
		"", "j_shouldertwist_ri"
		"", "j_chin_skinroll"
		"", "j_helmet"
		"", "j_lip_bot_le"
		"", "j_lip_bot_ri"
		"", "j_wrist_le"
		"", "j_wrist_ri"
		"", "j_wristtwist_le"
		"", "j_wristtwist_ri"
		"", "j_gun"
		"", "j_index_le_1"
		"", "j_index_ri_1"
		"", "j_mid_le_1"
		"", "j_mid_ri_1"
		"", "j_pinky_le_1"
		"", "j_pinky_ri_1"
		"", "j_ring_le_1"
		"", "j_ring_ri_1"
		"", "j_thumb_le_1"
		"", "j_thumb_ri_1"
		"", "j_index_le_2"
		"", "j_index_ri_2"
		"", "j_mid_le_2"
		"", "j_mid_ri_2"
		"", "j_pinky_le_2"
		"", "j_pinky_ri_2"
		"", "j_ring_le_2"
		"", "j_ring_ri_2"
		"", "j_thumb_le_2"
		"", "j_thumb_ri_2"
		"", "j_index_le_3"
		"", "j_index_ri_3"
		"", "j_mid_le_3"
		"", "j_mid_ri_3"
		"", "j_pinky_le_3"
		"", "j_pinky_ri_3"
		"", "j_ring_le_3"
		"", "j_ring_ri_3"
		"", "j_thumb_le_3"
		"", "j_thumb_ri_3"
		"", "j_legextra_le"
		"", "j_legextra_ri"
		"", "j_pants_hang"
		"", "j_spine1"
		"", "j_back_equip_le"
		"", "j_back_equip_ri"
		"", "j_gasmask"
		"", "j_legextra_le_end"
		"", "j_legextra_ri_end"
		"", "j_spade"
		"", "j_spine2"
		"", "j_back_equip_end_le"
		"", "j_back_equip_end_le1"
		"", "j_gasmask_end"
		"", "j_pack1"
		"", "j_pack2"
		"", "j_pack3"
		"", "j_pack4"
		"", "j_pack5"
		"", "j_pack6"
		"", "j_spade_end"
		"", "j_spine3"
		"", "j_toeend_le"
		"", "j_toeend_ri"
	};
	ScrollStruct<int> m_aimType[2] = {
		"Snap", 0,
		"Silent", 1
	};
	ScrollStruct<int> m_targetType[2] = {
		"Visible", 0,
		"Logical", 1
	};
	ScrollStruct<int> m_crouchEvent[] = {
		"On Damage", 0,
		"On Visible", 1,
		"On Logical", 2,
		"Always", 3
	};

	bool SortCompareFractions(FractionSorter& first, FractionSorter& second) {
		return first.m_fraction > second.m_fraction;
	}

	bool IsClientUsingShield(int client) {
		auto entity = Native::Custom::GetCEntity()[client];
		int weaponId = entity.m_weaponIndex;
		int secondaryWeaponId = *(BYTE*)((int)&entity + 0x12B);

		return weaponId == 3 || weaponId == 4 || secondaryWeaponId == 3 || secondaryWeaponId == 4;
	}

	float GetBoneFraction(int client, const char* aimtag) {
		Trace t;
		D3DXVECTOR3 visibleVec;

		auto entity = Native::Custom::GetCEntity()[client];

		int dobj = Native::Engine::Com_GetClientDObj(entity.m_handle, 0);
		if (!dobj) return 0.f;

		if (Native::Engine::CG_DObjGetWorldTagPos(&entity, dobj, Native::Engine::SL_GetString(aimtag), &visibleVec)) {
			Native::Engine::CG_Trace(&t, &Native::Custom::GetCG()->m_refdef.m_origin, &visibleVec, Native::Custom::GetCG()->m_clientNum);
			return t.m_fraction;
		}

		return 0.f;
	}

	bool IsTargetVisible(float fraction) {
		auto min = vars.m_fmj ? 0.85f : 0.94f;
		return fraction >= min;
	}

	bool CanKillTarget(int client, const char* tag) {
		D3DXVECTOR3 end = Native::Engine::GetTagPosition(client, tag);

		BulletFireParams bulletFireParams;
		bulletFireParams.m_maxEntityNumber = 2046;
		bulletFireParams.m_ignoreEntIndex = Native::Custom::GetCG()->m_clientNum;
		bulletFireParams.m_damageMultiplier = 1.0f;
		bulletFireParams.m_methodOfDeath = 1;
		bulletFireParams.m_originalStart = Native::Custom::GetCG()->m_refdef.m_origin;
		bulletFireParams.m_start = Native::Custom::GetCG()->m_refdef.m_origin;
		bulletFireParams.m_end = end;

		D3DXVECTOR3 endDir;
		Utils::VectorSubtract(&end, &Native::Custom::GetCG()->m_refdef.m_origin, &endDir);
		Utils::VectorNormalize(&endDir);
		bulletFireParams.m_dir = endDir;

		BulletTraceResults bulletTraceResult;
		bool hit = Native::Engine::BulletTrace(0, &bulletFireParams, &Native::Custom::GetCEntity()[Native::Custom::GetCG()->m_clientNum], &bulletTraceResult, 0);
		if (!hit) {
			return false;
		}

		int hitEntity = Native::Engine::Trace_GetEntityHitId(&bulletTraceResult.m_trace);
		if (hitEntity == client) {
			return true;
		}

		*(BYTE*)(0x82160228 + 3) = 0xFF;
		*(long long*)0x8215F9C0 = 0x386000014E800020;
		*(long long*)0x821FDF18 = 0x386000014E800020;
		*(long long*)0x82159738 = 0x386000014E800020;

		Native::Engine::FireBulletPenetrate(0, &bulletFireParams, Native::Custom::GetCEntity()[Native::Custom::GetCG()->m_clientNum].m_weaponIndex, -1, &Native::Custom::GetCEntity()[Native::Custom::GetCG()->m_clientNum], -1, Native::Custom::GetCG()->m_refdef.m_origin, false);

		*(long long*)0x8215F9C0 = 0x7D8802A6483F8901;
		*(long long*)0x821FDF18 = 0x7D8802A64835A3A9;
		*(long long*)0x82159738 = 0x7D8802A69181FFF8;

		return bulletFireParams.m_ignoreEntIndex == client || bulletTraceResult.m_trace.m_fraction >= (vars.m_fmj ? 0.85f : 0.94f);
	}

	FractionSorter GetHittableBone(int client) {
		static FractionSorter highestFraction;

		bool shield = IsClientUsingShield(client);

		if (IsTargetVisible(GetBoneFraction(client, "j_head"))) return FractionSorter("j_head", 1.0f);

		if (shield) {
			// and knees, and ball
			if (IsTargetVisible(GetBoneFraction(client, "j_ankle_ri"))) return FractionSorter("j_ankle_ri", 1.0f);
			if (IsTargetVisible(GetBoneFraction(client, "j_ankle_le"))) return FractionSorter("j_ankle_le", 1.0f);
		}

		for (int i = 1; i < NUMOF(m_boneNames); i++) {
			float thisFraction = GetBoneFraction(client, m_boneNames[i].m_result);

			if (highestFraction.m_fraction < thisFraction) {
				highestFraction.m_fraction = thisFraction;
				highestFraction.m_tag = m_boneNames[i].m_result;
			}
		}

		return highestFraction;
	}

	int GetTarget() {
		float closestDistance = FLT_MAX;
		int closestPlayer = -1;

		auto cg_t = Native::Custom::GetCG();
		for (int i = 0; i < Native::Custom::GetServerInfo()->m_maxClients; i++) {
			if (i == cg_t->m_clientNum) continue;

			auto entity = Native::Custom::GetCEntity()[i];
			if (!Utils::IsFriend(i) && Utils::IsAlive(i)) {
				const char* tag = m_boneNames[vars.m_bone].m_result;

				vars.m_usingTag = tag;

				if (vars.m_shieldHandler || vars.m_bone == 0) {
					auto hittable = GetHittableBone(i);

					vars.m_usingTag = hittable.m_tag;
					vars.m_visible = IsTargetVisible(hittable.m_fraction);
				} else {
					vars.m_visible = IsTargetVisible(GetBoneFraction(entity.m_clientNumber, tag)); 
				}

				if (vars.m_autowall) {
					vars.m_hittable = CanKillTarget(i, tag);
				}

				switch (vars.m_targetType) {
				case 0: // Visible
					if (!vars.m_visible) {
						continue;
					}
					break;
				case 1: // Logical
					if (!vars.m_hittable) {
						if (!vars.m_visible) {
							continue;
						}
					}
					break;
				}

				float distance = Utils::GetDistance(Native::Custom::GetCEntity()[cg_t->m_clientNum].m_origin, entity.m_origin);
				if (distance < closestDistance) {
					closestDistance = distance;
					closestPlayer = i;
				}
			}
		}

		return closestPlayer;
	}

	void FixMovement(UserCommand* pCmd, float CurAngle, float OldAngle, float fOldForward, float fOldSidemove) {
		float deltaView = CurAngle - OldAngle;
		int forward = (int)(cos(deltaView * (3.14159265358979323846264338327950288f / 180.0f)) * fOldForward + cos((deltaView + 90.f) * (3.14159265358979323846264338327950288f / 180.0f)) * fOldSidemove);
		int right = (int)(sin(deltaView * (3.14159265358979323846264338327950288f / 180.0f)) * fOldForward + sin((deltaView + 90.f) * (3.14159265358979323846264338327950288f / 180.0f)) * fOldSidemove);

		if (forward < -128) forward = -128;
		else if (forward > 127) forward = 127;
		if (right < -128) right = -128;
		else if (right > 127) right = 127;

		pCmd->m_forwardmove = (char)forward;
		pCmd->m_rightmove = (char)right;
	}

	void SilentAim(UserCommand* oldCommand, D3DXVECTOR3 toAngle) {
		float oldAngle = Utils::ShortToAngle(oldCommand->m_viewAngles[1]);
		oldCommand->m_viewAngles[0] = Utils::AngleToShort(toAngle.x - Native::Custom::GetClientActive()->m_baseAngles[0]);
		oldCommand->m_viewAngles[1] = Utils::AngleToShort(toAngle.y - Native::Custom::GetClientActive()->m_baseAngles[1]);
		FixMovement(oldCommand, Utils::ShortToAngle(oldCommand->m_viewAngles[1]), oldAngle, oldCommand->m_forwardmove, oldCommand->m_rightmove);
	}

	void SnapAim(D3DXVECTOR3 toAngle) {
		Native::Custom::GetClientActive()->m_viewAngles[0] = toAngle.x - Native::Custom::GetClientActive()->m_baseAngles[0];
		Native::Custom::GetClientActive()->m_viewAngles[1] = toAngle.y - Native::Custom::GetClientActive()->m_baseAngles[1];
	}
}

void AimbotMenu::Init() {
	SetName("LiveEmulation Cheat Engine", "Aimbot");

	addOption(SubmenuOption("Anti Aim")
		.addSubmenu<AimbotAntiAimMenu>()
		.addTooltip("Anti aim"));

	addOption(ToggleOption("Aimbot Required")
		.addToggle(vars.m_required).canBeSaved()
		.addTooltip("Enable this to enable the aimbot options below"));

	addOption(ScrollOption<int>(SCROLL, "Target Type")
		.addScroll(vars.m_targetType, 0, NUMOF(m_targetType) - 1, m_targetType)
		.addTooltip("Target type\nVisible: Any visible client\nLogical: Any killable client"));

	addOption(ScrollOption<int>(SCROLL, "Aimbot Type")
		.addScroll(vars.m_aimType, 0, NUMOF(m_aimType) - 1, m_aimType)
		.addTooltip("Aim type\nSnap: Lock on and off\nSilent: Invisible aiming"));

	addOption(ScrollOption<const char*>(SCROLL, "Aim Tag")
		.addScroll(vars.m_bone, 0, NUMOF(m_boneNames) - 1, m_boneNames)
		.addTooltip("Select the bone to check"));

	addOption(ToggleOption("Auto Wall")
		.addToggle(vars.m_autowall).canBeSaved()
		.addTooltip("Auto shoot your weapon"));

	addOption(ToggleOption("Auto Shoot")
		.addToggle(vars.m_autoShoot).canBeSaved()
		.addTooltip("Auto shoot your weapon"));

	addOption(ToggleOption("Auto Aim")
		.addToggle(vars.m_autoAim).canBeSaved()
		.addTooltip("Auto aim your weapon"));

	addOption(NumberOption<float>(TOGGLE, "Position Prediction")
		.addToggle(vars.m_prediction)
		.addNumber(vars.m_predictionDelta, "%.1f", 0.1f).addMin(0.01f).addMax(3.0f)
		.addTooltip("Predict the players position. Edit the delta time to use when calculating the velocity"));

	addOption(ToggleOption("Shield Handler")
		.addToggle(vars.m_shieldHandler).canBeSaved()
		.addTooltip("Automatically change the bone internally if the client is using a shield (or has a shield on back)"));

	addOption(ToggleOption("Full Metal Jacket")
		.addToggle(vars.m_fmj).canBeSaved()
		.addTooltip("Increase your chance of penetrating walls"));

	addOption(ToggleOption("Show Target via ESP")
		.addToggle(vars.m_showTargetViaEsp).canBeSaved()
		.addTooltip("Show the client through the esp"));
}

void AimbotMenu::InitVars() {
	vars.m_crouchDelay = 300;
	vars.m_predictionDelta = 0.2f;
}

/*Update once when submenu is opened*/
void AimbotMenu::UpdateOnce() {}

/*Update while submenu is active*/
void AimbotMenu::Update() {}

/*Background update*/
void AimbotMenu::FeatureUpdate() {};

/*Singleton*/
AimbotMenu* _instance;
AimbotMenu* AimbotMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new AimbotMenu();
		_instance->InitVars();
		_instance->Init();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
AimbotMenu::~AimbotMenu() { delete _instance; }