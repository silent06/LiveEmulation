#pragma once
#include "menu/submenu.h"

class VisualESPSettingsMenu : public Submenu {
public:
	static VisualESPSettingsMenu* GetInstance();
	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VisualESPSettingsMenu() : Submenu() {}
	~VisualESPSettingsMenu();
};

namespace VisualESPSettingsMenuVars
{
	struct Vars {

	};

	extern Vars vars;
}