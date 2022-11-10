#include "stdafx.h"
#include "visualESP.h"
#include "visualESPSettings.h"

using namespace VisualESPMenuVars;

namespace VisualESPMenuVars {
	Vars vars;

	ScrollStruct<int> espSnaplinePositions[] = {
		"Top", 0,
		"Middle", 1,
		"Bottom", 2
	};

	ScrollStruct<int> espBoxType[] = {
		"2D", 0,
		"3D", 1,
		"Pyramid", 2,
		"Filled", 3,
		"Corner", 4
	};

	Positioning GetPositioning(int client) {
		Positioning pos;
		pos.m_head = Native::Engine::GetTagPosition(client, "j_head");
		pos.m_feet = Native::Engine::GetTagPosition(client, "j_ankle_ri");
		pos.m_helmet = Native::Engine::GetTagPosition(client, "j_helmet");
		pos.m_origin = Native::Custom::GetCEntity()[client].m_origin;
		return pos;
	}

	void DrawESPLine(D3DXVECTOR3 center, float x1, float y1, float z1, float x2, float y2, float z2, Color color) {
		D3DXVECTOR3 pointPos1 = D3DXVECTOR3(center.x + x1, center.y + y1, center.z + z1);
		D3DXVECTOR3 pointPos2 = D3DXVECTOR3(center.x + x2, center.y + y2, center.z + z2);
		Vector2 xy1, xy2;

		if (Utils::WorldToScreen(pointPos1, &xy1.x, &xy1.y) && Utils::WorldToScreen(pointPos2, &xy2.x, &xy2.y))
			GetRenderer()->DrawLine(xy1.x, xy1.y, xy2.x, xy2.y, 1, color, GetRenderer()->m_whiteMaterial);
	}

	void DrawBoneOnPlayer(int client, char*startBone, char*endBone, Color color) {
		float SX, SY, EX, EY;
		D3DXVECTOR3 Start = Native::Engine::GetTagPosition(client, startBone);
		D3DXVECTOR3 Finish = Native::Engine::GetTagPosition(client, endBone);
		if (Utils::WorldToScreen(Start, &SX, &SY) && Utils::WorldToScreen(Finish, &EX, &EY))
			GetRenderer()->DrawLine(SX, SY, EX, EY, 1, color, GetRenderer()->m_whiteMaterial);
	}

	void NamePatch() {
		*(int*)AuthData::g_addresses[AuthData::PATCH_VisualESPName] = vars.m_name ? 0x4e800020 : 0x7D8802A6;
	}

	void _2DCallback(CgArray* cg_t, CEntity entity, ClientInfo clientInfo) {
		auto position = GetPositioning(entity.m_clientNumber);
		bool isFriend = Utils::IsFriend(entity.m_clientNumber);

		bool shouldRender = (vars.m_friendlies && isFriend) || (vars.m_enemies && !isFriend);
		if (shouldRender) {
			Color color = Color(0xFFFFFFFF);
			if (vars.m_friendlies && isFriend)
				color = vars.m_2dFriendlies;
			else if (vars.m_enemies && !isFriend)
				color = vars.m_2dEnemies;

			if (entity.m_clientNumber == vars.m_aimbotTarget) {
				color = vars.m_2dAimbotTarget;
			}

			D3DXVECTOR3 helmet, origin;
			if (Utils::WorldToScreen(position.m_helmet, &helmet.x, &helmet.y) && Utils::WorldToScreen(position.m_origin, &origin.x, &origin.y)) {
				float width = fabs((helmet.y - origin.y) / 4);

				GetRenderer()->DrawLine(origin.x - width, origin.y, origin.x + width, origin.y, 1, color, GetRenderer()->m_whiteMaterial);
				GetRenderer()->DrawLine(origin.x - width, helmet.y, origin.x + width, helmet.y, 1, color, GetRenderer()->m_whiteMaterial);
				GetRenderer()->DrawLine(origin.x - width, origin.y, origin.x - width, helmet.y, 1, color, GetRenderer()->m_whiteMaterial);
				GetRenderer()->DrawLine(origin.x + width, helmet.y, origin.x + width, origin.y, 1, color, GetRenderer()->m_whiteMaterial);
			}
		}
	}

	void _CornerCallback(CgArray* cg_t, CEntity entity, ClientInfo clientInfo) {
		auto position = GetPositioning(entity.m_clientNumber);
		bool isFriend = Utils::IsFriend(entity.m_clientNumber);

		bool shouldRender = (vars.m_friendlies && isFriend) || (vars.m_enemies && !isFriend);
		if (shouldRender) {
			Color color = Color(0xFFFFFFFF);
			if (vars.m_friendlies && isFriend)
				color = vars.m_cornerFriendlies;
			else if (vars.m_enemies && !isFriend)
				color = vars.m_cornerEnemies;

			if (entity.m_clientNumber == vars.m_aimbotTarget) {
				color = vars.m_cornerAimbotTarget;
			}

			D3DXVECTOR3 origin, head;
			position.m_origin.z -= 5.f;
			if (Utils::WorldToScreen(position.m_origin, &origin.x, &origin.y) && Utils::WorldToScreen(position.m_head, &head.x, &head.y)) {
				float height = origin.y - head.y;
				float width = entity.m_flags & 8 ? height / 0.9f : height / 1.95f;

				GetRenderer()->DrawRect((origin.x - (width / 2)), (origin.y - height), (width / 4), 1, eJustify::JustifyLeft, color);
				GetRenderer()->DrawRect((origin.x - (width / 2)), origin.y, (width / 4), 1, eJustify::JustifyLeft, color);
				GetRenderer()->DrawRect((origin.x - (width / 2)), (origin.y - height), 1, (width / 4), eJustify::JustifyLeft, color);
				GetRenderer()->DrawRect((origin.x + (width / 2)), (origin.y - height), 1, (width / 4), eJustify::JustifyLeft, color);

				GetRenderer()->DrawRect((origin.x + ((width / 2) - (width / 4))), (origin.y - height), (width / 4), 1, eJustify::JustifyLeft, color);
				GetRenderer()->DrawRect((origin.x + ((width / 2) - (width / 4))), origin.y, (width / 4) + 1, 1, eJustify::JustifyLeft, color);
				GetRenderer()->DrawRect((origin.x - (width / 2)), (origin.y - (width / 4)), 1, (width / 4), eJustify::JustifyLeft, color);
				GetRenderer()->DrawRect((origin.x + (width / 2)), (origin.y - (width / 4)), 1, (width / 4), eJustify::JustifyLeft, color);
			}
		}
	}

	void _FilledCallback(CgArray* cg_t, CEntity entity, ClientInfo clientInfo) {
		auto position = GetPositioning(entity.m_clientNumber);
		bool isFriend = Utils::IsFriend(entity.m_clientNumber);

		bool shouldRender = (vars.m_friendlies && isFriend) || (vars.m_enemies && !isFriend);
		if (shouldRender) {
			Color color = Color(0xFFFFFFFF);
			if (vars.m_friendlies && isFriend)
				color = vars.m_filledFriendlies;
			else if (vars.m_enemies && !isFriend)
				color = vars.m_filledEnemies;

			if (entity.m_clientNumber == vars.m_aimbotTarget) {
				color = vars.m_filledAimbotTarget;
			}

			D3DXVECTOR3 helmet, origin;
			if (Utils::WorldToScreen(position.m_helmet, &helmet.x, &helmet.y) && Utils::WorldToScreen(position.m_origin, &origin.x, &origin.y)) {
				float width = fabs((helmet.y - origin.y) / 4);

				GetRenderer()->DrawRect(origin.x - width, helmet.y, width * 2 - 1, origin.y - helmet.y, eJustify::JustifyLeft, color.WithOpacity(80));
				GetRenderer()->DrawLine(origin.x - width, origin.y, origin.x + width, origin.y, 1, color, GetRenderer()->m_whiteMaterial);
				GetRenderer()->DrawLine(origin.x - width, helmet.y, origin.x + width, helmet.y, 1, color, GetRenderer()->m_whiteMaterial);
				GetRenderer()->DrawLine(origin.x - width, origin.y, origin.x - width, helmet.y, 1, color, GetRenderer()->m_whiteMaterial);
				GetRenderer()->DrawLine(origin.x + width, helmet.y, origin.x + width, origin.y, 1, color, GetRenderer()->m_whiteMaterial);
			}
		}
	}

	void _3DCallback(CgArray* cg_t, CEntity entity, ClientInfo clientInfo) {
		bool isFriend = Utils::IsFriend(entity.m_clientNumber);
		float height = 60.f;
		float gap = 20.f;

		bool shouldRender = (vars.m_friendlies && isFriend) || (vars.m_enemies && !isFriend);
		if (shouldRender) {
			Color color = Color(0xFFFFFFFF);
			if (vars.m_friendlies && isFriend)
				color = vars.m_3dFriendlies;
			else if (vars.m_enemies && !isFriend)
				color = vars.m_3dEnemies;

			if (entity.m_clientNumber == vars.m_aimbotTarget) {
				color = vars.m_3dAimbotTarget;
			}

			DrawESPLine(entity.m_origin, -gap, -gap, 0, gap, -gap, 0, color);
			DrawESPLine(entity.m_origin, -gap, -gap, 0, -gap, gap, 0, color);
			DrawESPLine(entity.m_origin, gap, gap, 0, gap, -gap, 0, color);
			DrawESPLine(entity.m_origin, gap, gap, 0, -gap, gap, 0, color);
			DrawESPLine(entity.m_origin, -gap, -gap, 0, -gap, -gap, height, color);
			DrawESPLine(entity.m_origin, -gap, gap, 0, -gap, gap, height, color);
			DrawESPLine(entity.m_origin, gap, -gap, 0, gap, -gap, height, color);
			DrawESPLine(entity.m_origin, gap, gap, 0, gap, gap, height, color);
			DrawESPLine(entity.m_origin, -gap, -gap, height, gap, -gap, height, color);
			DrawESPLine(entity.m_origin, -gap, -gap, height, -gap, gap, height, color);
			DrawESPLine(entity.m_origin, gap, gap, height, gap, -gap, height, color);
			DrawESPLine(entity.m_origin, gap, gap, height, -gap, gap, height, color);
		}
	}

	void _PyramidCallback(CgArray* cg_t, CEntity entity, ClientInfo clientInfo) {
		bool isFriend = Utils::IsFriend(entity.m_clientNumber);
		float height = 60.f;
		float gap = 20.f;

		bool shouldRender = (vars.m_friendlies && isFriend) || (vars.m_enemies && !isFriend);
		if (shouldRender) {
			Color color = Color(0xFFFFFFFF);
			if (vars.m_friendlies && isFriend)
				color = vars.m_pyramidFriendlies;
			else if (vars.m_enemies && !isFriend)
				color = vars.m_pyramidEnemies;

			if (entity.m_clientNumber == vars.m_aimbotTarget) {
				color = vars.m_pyramidAimbotTarget;
			}

			DrawESPLine(entity.m_origin, -gap, -gap, 0, gap, -gap, 0, color);
			DrawESPLine(entity.m_origin, -gap, -gap, 0, -gap, gap, 0, color);
			DrawESPLine(entity.m_origin, gap, gap, 0, gap, -gap, 0, color);
			DrawESPLine(entity.m_origin, gap, gap, 0, -gap, gap, 0, color);
			DrawESPLine(entity.m_origin, -gap, -gap, 0, 0, 0, height, color);
			DrawESPLine(entity.m_origin, -gap, gap, 0, 0, 0, height, color);
			DrawESPLine(entity.m_origin, gap, -gap, 0, 0, 0, height, color);
			DrawESPLine(entity.m_origin, gap, gap, 0, 0, 0, height, color);
		}
	}

	void _SnaplineCallback(CgArray* cg_t, CEntity entity, ClientInfo clientInfo) {
		Vector2 screenPosition;
		bool isFriend = Utils::IsFriend(entity.m_clientNumber);
		bool shouldRender = (vars.m_friendlies && isFriend) || (vars.m_enemies && !isFriend);

		if (shouldRender) {
			if (Utils::WorldToScreen(Native::Engine::GetTagPosition(entity.m_clientNumber, "j_mainroot"), &screenPosition.x, &screenPosition.y)) {
				Color color = Color(0xFFFFFFFF);
				if (VisualESPMenuVars::vars.m_aimbotTarget == entity.m_clientNumber)
					color = vars.m_snaplineAimbotTarget;
				else if (vars.m_friendlies && isFriend)
					color = vars.m_snaplineFriendlies;
				else if (vars.m_enemies && !isFriend)
					color = vars.m_snaplineEnemies;

				float timesAmount = 0.0f;
				switch (vars.m_snaplineSelection) {
				case 0: timesAmount = 0.0f; break;
				case 1: timesAmount = 0.5f; break;
				case 2: timesAmount = 1.0f; break;
				}

				GetRenderer()->DrawLine(Native::Custom::GetCG()->m_refdef.m_width * 0.5f, Native::Custom::GetCG()->m_refdef.m_height * timesAmount, screenPosition.x, screenPosition.y, 1.25f, color, GetRenderer()->m_whiteMaterial);
			}
		}
	}

	void _BoneCallback(CgArray* cg_t, CEntity entity, ClientInfo clientInfo) {
		bool isFriend = Utils::IsFriend(entity.m_clientNumber);

		bool shouldRender = (vars.m_friendlies && isFriend) || (vars.m_enemies && !isFriend);
		if (shouldRender) {
			Color color = Color(0xFFFFFFFF);
			if (vars.m_friendlies && isFriend)
				color = vars.m_bonesFriendlies;
			else if (vars.m_enemies && !isFriend)
				color = vars.m_bonesEnemies;

			if (entity.m_clientNumber == vars.m_aimbotTarget) {
				color = vars.m_bonesAimbotTarget;
			}

			DrawBoneOnPlayer(entity.m_clientNumber, "j_knee_le", "pelvis", color);
			DrawBoneOnPlayer(entity.m_clientNumber, "j_elbow_ri", "j_wrist_ri", color);
			DrawBoneOnPlayer(entity.m_clientNumber, "j_neck", "j_elbow_ri", color);
			DrawBoneOnPlayer(entity.m_clientNumber, "j_ankle_ri", "j_knee_ri", color);
			DrawBoneOnPlayer(entity.m_clientNumber, "j_elbow_le", "j_wrist_le", color);
			DrawBoneOnPlayer(entity.m_clientNumber, "j_ankle_le", "j_knee_le", color);
			DrawBoneOnPlayer(entity.m_clientNumber, "j_knee_ri", "pelvis", color);
			DrawBoneOnPlayer(entity.m_clientNumber, "pelvis", "j_neck", color);
			DrawBoneOnPlayer(entity.m_clientNumber, "j_neck", "j_elbow_le", color);
			DrawBoneOnPlayer(entity.m_clientNumber, "j_neck", "j_head", color);
			DrawBoneOnPlayer(entity.m_clientNumber, "j_head", "j_helmet", color);
		}
	}

	void _NameCallback(CgArray* cg_t, CEntity entity, ClientInfo clientInfo) {
		Vector2  head;
		auto pos = GetPositioning(entity.m_clientNumber);
		bool isFriend = Utils::IsFriend(entity.m_clientNumber);
		bool shouldRender = (vars.m_friendlies && isFriend) || (vars.m_enemies && !isFriend);

		if (shouldRender && Utils::WorldToScreen(pos.m_head, &head.x, &head.y)) {
			Color color = Color(0xFFFFFFFF);
			Color color2 = Color(0, 0, 0, 150);
			if (vars.m_friendlies && isFriend) {
				color = vars.m_nameTextFriendlies;
				color2 = vars.m_nameBoxFriendlies;
			}
			else if (vars.m_enemies && !isFriend) {
				color = vars.m_nameTextEnemies;
				color2 = vars.m_nameBoxEnemies;
			}

			GetRenderer()->DrawRect(GetRenderer()->CalculateXValueWithAlign(head.x - 2, clientInfo.m_name, GetRenderer()->Font.normalFont, 0.30f, eJustify::JustifyCenter), head.y - 15, (Native::UI::R_TextWidth(clientInfo.m_name, GetRenderer()->Font.normalFont) * 0.30f + 2), 10, eJustify::JustifyLeft, color2);
			GetRenderer()->DrawString(clientInfo.m_name, GetRenderer()->CalculateXValueWithAlign(head.x, clientInfo.m_name, GetRenderer()->Font.normalFont, 0.30f, eJustify::JustifyCenter), head.y - 5, GetRenderer()->Font.normalFont, 0.30f, color, false, false, eJustify::JustifyLeft);
		}
	}

	void _WeaponShaderCallback(CgArray* cg_t, CEntity entity, ClientInfo clientInfo) {
		bool isFriend = Utils::IsFriend(entity.m_clientNumber);

		Vector2 screenPosition;
		auto pos = GetPositioning(entity.m_clientNumber);

		bool shouldRender = (vars.m_friendlies && isFriend) || (vars.m_enemies && !isFriend);
		if (shouldRender) {
			if (Utils::WorldToScreen(pos.m_origin, &screenPosition.x, &screenPosition.y)) {
				Color color = Color(0xFFFFFFFF);
				if (vars.m_friendlies && isFriend)
					color = vars.m_weaponShadersFriendlies;
				else if (vars.m_enemies && !isFriend)
					color = vars.m_weaponShadersEnemies;

				if (entity.m_clientNumber == vars.m_aimbotTarget) {
					color = vars.m_weaponShadersAimbotTarget;
				}

				auto weaponid = entity.m_weaponIndex;
				auto shader = Utils::GetWeaponShader(weaponid);
				if (shader) {
					Native::UI::CG_DrawRotatedPicPhysical(0x827322C0, screenPosition.x - 15, screenPosition.y, 30, 15, 0, color.GetFloatPtr(), shader);
				}
			}
		}
	}

	void _WeaponNameCallback(CgArray* cg_t, CEntity entity, ClientInfo clientInfo) {
		bool isFriend = Utils::IsFriend(entity.m_clientNumber);

		Vector2 screenPosition;
		auto pos = GetPositioning(entity.m_clientNumber);

		bool shouldRender = (vars.m_friendlies && isFriend) || (vars.m_enemies && !isFriend);
		if (shouldRender) {
			if (Utils::WorldToScreen(pos.m_origin, &screenPosition.x, &screenPosition.y)) {
				Color color = Color(0xFFFFFFFF);
				if (vars.m_friendlies && isFriend)
					color = vars.m_weaponNamesFriendlies;
				else if (vars.m_enemies && !isFriend)
					color = vars.m_weaponNamesEnemies;
			}
		}
	}
}

void VisualESPMenu::Init() {
	SetParentSubmenu<VisualsMenu>();
	SetName("Visual ESP");

	addOption(SubmenuOption("Settings")
		.addSubmenu<VisualESPSettingsMenu>()
		.addTooltip("Edit settings for the ESP"));

	addOption(ToggleOption("Draw Friendlies")
		.addToggle(vars.m_friendlies).canBeSaved()
		.addTooltip("Draw the enabled esp options on friendlies"));

	addOption(ToggleOption("Draw Enemies")
		.addToggle(vars.m_enemies).canBeSaved()
		.addTooltip("Draw the enabled esp options on enemies"));

	addOption(ScrollOption<int>(TOGGLE, "Draw Box")
		.addScroll(vars.m_boxType, 0, NUMOF(espBoxType) - 1, espBoxType)
		.addToggle(vars.m_box)
		.addTooltip("Draw box"));

	addOption(ToggleOption("Draw Names")
		.addToggle(vars.m_name).canBeSaved()
		.addFunction(NamePatch)
		.addTooltip("Draw the clients name above their head"));

	addOption(ScrollOption<int>(TOGGLE, "Draw Snaplines")
		.addToggle(vars.m_snapline)
		.addScroll(vars.m_snaplineSelection, 0, NUMOF(espSnaplinePositions) - 1, espSnaplinePositions)
		.addTooltip("Draw a line from a dynamic screen position to the player"));

	addOption(ToggleOption("Draw Bones")
		.addToggle(vars.m_bones).canBeSaved()
		.addTooltip("Draw a skeleton around the player"));

	addOption(ToggleOption("Draw Weapon Shaders")
		.addToggle(vars.m_weaponShaders).canBeSaved()
		.addTooltip("Draw weapon shaders"));
}

void VisualESPMenu::InitVars() {
	vars.m_snaplineEnemies = Color(0xc73737FF);
	vars.m_snaplineFriendlies = Color(0x37c775FF);
	vars.m_snaplineAimbotTarget = Color(255, 255, 0);

	vars.m_bonesFriendlies = Color(0xFFFFFFFF);
	vars.m_bonesEnemies = Color(0xFFFFFFFF);
	vars.m_bonesAimbotTarget = Color(255, 255, 0);

	vars.m_weaponShadersFriendlies = Color(0x37c775FF);
	vars.m_weaponShadersEnemies = Color(0xc73737FF);
	vars.m_weaponShadersAimbotTarget = Color(255, 255, 0);

	vars.m_cornerFriendlies = Color(0x37c775FF);
	vars.m_cornerEnemies = Color(0xc73737FF);
	vars.m_cornerAimbotTarget = Color(255, 255, 0);

	vars.m_nameBoxFriendlies = Color(0, 0, 0, 150);
	vars.m_nameBoxEnemies = Color(0, 0, 0, 150);

	vars.m_nameTextEnemies = Color(0xFFFFFFFF);
	vars.m_nameTextFriendlies = Color(0xFFFFFFFF);

	vars.m_2dFriendlies = Color(0x37c775FF);
	vars.m_2dEnemies = Color(0xc73737FF);
	vars.m_2dAimbotTarget = Color(255, 255, 0);
	
	vars.m_3dFriendlies = Color(0x37c775FF);
	vars.m_3dEnemies = Color(0xc73737FF);
	vars.m_3dAimbotTarget = Color(255, 255, 0);

	vars.m_filledFriendlies = Color(0x37c775FF);
	vars.m_filledEnemies = Color(0xc73737FF);
	vars.m_filledAimbotTarget = Color(255, 255, 0);

	vars.m_pyramidFriendlies = Color(0x37c775FF);
	vars.m_pyramidEnemies = Color(0xc73737FF);
	vars.m_pyramidAimbotTarget = Color(255, 255, 0);
}

/*Update once when submenu is opened*/
void VisualESPMenu::UpdateOnce() {}

/*Update while submenu is active*/
void VisualESPMenu::Update() {}

/*Background update*/
void VisualESPMenu::FeatureUpdate() {
	if (vars.m_name) {
		Utils::ClientCallback(true, _NameCallback);
	}

	if (vars.m_box) {
		switch (vars.m_boxType) {
		case 0:
			Utils::ClientCallback(true, _2DCallback);
			break;
		case 1:
			Utils::ClientCallback(true, _3DCallback);
			break;
		case 2:
			Utils::ClientCallback(true, _PyramidCallback);
			break;
		case 3:
			Utils::ClientCallback(true, _FilledCallback);
			break;
		case 4:
			Utils::ClientCallback(true, _CornerCallback);
			break;
		}
	}

	if (vars.m_snapline) {
		Utils::ClientCallback(true, _SnaplineCallback);
	}

	if (vars.m_bones) {
		Utils::ClientCallback(true, _BoneCallback);
	}

	if (vars.m_weaponShaders) {
		Utils::ClientCallback(true, _WeaponShaderCallback);
	}

	if (vars.m_weaponNames) {
		Utils::ClientCallback(true, _WeaponNameCallback);
	}
};

/*Singleton*/
VisualESPMenu* _instance;
VisualESPMenu* VisualESPMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VisualESPMenu();
		_instance->Init();
		_instance->InitVars();
		GetSubmenuHandler()->AddSubmenu(_instance);
	}
	return _instance;
}
VisualESPMenu::~VisualESPMenu() { delete _instance; }