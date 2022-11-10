#pragma once
#include "../../submenu.h"

class VisualRadarMenu : public Submenu {
public:
	static VisualRadarMenu* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate();

	VisualRadarMenu() : Submenu() { }
	~VisualRadarMenu();
};

namespace VisualRadarMenuVars
{
	struct Vars {
		bool m_radar;
		bool m_outline;
		bool m_onceFlag;
	};

	extern Vars vars;
}
