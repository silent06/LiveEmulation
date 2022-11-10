#pragma once
#include "menu/submenu.h"

class SettingsColorRainbow : public Submenu {
public:
	static SettingsColorRainbow* GetInstance();
	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	SettingsColorRainbow() : Submenu() {}
	~SettingsColorRainbow();
};

namespace SettingsColorRainbowVars
{
	struct Vars {
		Rainbow m_rainbow;
		bool m_enable;
		vector<pair<RGBA*, bool*>> m_colors;

		bool m_ui[22];
		bool m_indicators[7];
		bool m_reticle;
		bool m_radar[9];
		bool m_visuals[2];
	};

	extern Vars vars;
}