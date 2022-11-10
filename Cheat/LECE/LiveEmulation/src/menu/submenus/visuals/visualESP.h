#pragma once
#include "../../submenu.h"

class VisualESPMenu : public Submenu {
public:
	static VisualESPMenu* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate();

	VisualESPMenu() : Submenu() { }
	~VisualESPMenu();
};

namespace VisualESPMenuVars
{
	struct Vars {
		int m_aimbotTarget;
		bool m_friendlies;
		bool m_enemies;

		bool m_box;
		int m_boxType;

		bool m_snapline;
		int m_snaplineSelection;
		Color m_snaplineFriendlies;
		Color m_snaplineEnemies;
		Color m_snaplineAimbotTarget;

		bool m_bones;
		Color m_bonesFriendlies;
		Color m_bonesEnemies;
		Color m_bonesAimbotTarget;

		Color m_2dFriendlies;
		Color m_2dEnemies;
		Color m_2dAimbotTarget;

		Color m_3dFriendlies;
		Color m_3dEnemies;
		Color m_3dAimbotTarget;

		Color m_pyramidFriendlies;
		Color m_pyramidEnemies;
		Color m_pyramidAimbotTarget;

		Color m_filledFriendlies;
		Color m_filledEnemies;
		Color m_filledAimbotTarget;

		bool m_name;
		Color m_nameTextFriendlies;
		Color m_nameTextEnemies;
		Color m_nameBoxFriendlies;
		Color m_nameBoxEnemies;

		Color m_cornerFriendlies;
		Color m_cornerEnemies;
		Color m_cornerAimbotTarget;

		bool m_weaponNames;
		Color m_weaponNamesFriendlies;
		Color m_weaponNamesEnemies;

		bool m_weaponShaders;
		Color m_weaponShadersFriendlies;
		Color m_weaponShadersEnemies;
		Color m_weaponShadersAimbotTarget;
	};

	extern Vars vars;

	struct Positioning {
		D3DXVECTOR3 m_feet;
		D3DXVECTOR3 m_head;
		D3DXVECTOR3 m_helmet;
		D3DXVECTOR3 m_origin;
	};
}