#pragma once
#include "menu/submenu.h"

class VisualRadarSettingsMenu : public Submenu {
public:
	static VisualRadarSettingsMenu* GetInstance();
	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VisualRadarSettingsMenu() : Submenu() {}
	~VisualRadarSettingsMenu();
};

namespace VisualRadarSettingsMenuVars
{
	struct Vars {
		Color m_map;
		Color m_selfIcon;
		Color m_outline;
		Color m_friendlyIcon;
		Color m_enemyIcon;
		Color m_turrets;
		Color m_missiles;
		Color m_helicopters;
		Color m_crates;
	};

	extern Vars vars;
}