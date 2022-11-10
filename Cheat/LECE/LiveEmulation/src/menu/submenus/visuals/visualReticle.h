#pragma once
#include "../../submenu.h"

class VisualReticleMenu : public Submenu {
public:
	static VisualReticleMenu* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VisualReticleMenu() : Submenu() {}
	~VisualReticleMenu();
};

namespace VisualReticleMenuVars
{
	struct Vars {
		bool m_disableGameReticle;
		int m_scale;
		int m_rotation;
		Color m_color;
		bool m_render;
		bool m_rotate;
		int m_rotateSpeed;
		int m_rotateIncrement;

		bool m_onceFlag;
	};

	extern Vars vars;
}