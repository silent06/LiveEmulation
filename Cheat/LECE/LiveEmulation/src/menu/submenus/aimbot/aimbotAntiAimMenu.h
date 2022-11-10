#pragma once
#include "../../submenu.h"

class AimbotAntiAimMenu : public Submenu {
public:
	static AimbotAntiAimMenu* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	AimbotAntiAimMenu() : Submenu() {}
	~AimbotAntiAimMenu();
};

namespace AimbotAntiAimMenuVars {
	struct Vars {
		bool m_fakeLag;
		int m_fakeLagAmount;

		int m_xSelection;
		bool m_x;

		int m_ySelection;
		bool m_y;

		float m_spinXSpeed;
		float m_spinYSpeed;
	};

	extern Vars vars;
}