#pragma once
#include "menu/submenu.h"

class VisualReticleSettingsMenu : public Submenu {
public:
	static VisualReticleSettingsMenu* GetInstance();
	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VisualReticleSettingsMenu() : Submenu() {}
	~VisualReticleSettingsMenu();
};

namespace VisualReticleSettingsMenuVars
{
	struct Vars {
	};

	extern Vars vars;
}