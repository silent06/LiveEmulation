#pragma once
#include "../../submenu.h"

class MainChangeNamePresetMenu : public Submenu {
public:
	static MainChangeNamePresetMenu* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MainChangeNamePresetMenu() : Submenu() {}
	~MainChangeNamePresetMenu();
};

namespace MainChangeNamePresetMenuVars
{
	
}