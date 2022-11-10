#pragma once
#include "../submenu.h"

class MainMenu : public Submenu {
public:
	static MainMenu* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate();

	MainMenu() : Submenu() { }
	~MainMenu();
};

namespace MainMenuVars
{
	struct Vars {
		bool m_noRecoil;
		bool m_noFlinch;
		bool m_noSway;
		bool m_noSpread;
		bool m_noFlashbangScreenEffect;
		bool m_noConcussionScreenEffect;
		bool m_laser;
		bool m_spoofIP;
		bool m_constantRadar;
		bool m_hearEveryone;
		float m_fov;
		int m_class;
		bool m_thirdPerson;
		float m_thirdPersonRange;
		bool m_needsToCrouch;
		int m_changeTeam;
		bool m_onceInGameFlag;
		bool m_autoUninfect;
	};

	extern Vars vars;

	void SetData();
	void NoSpread(UserCommand* oldCommand);
}