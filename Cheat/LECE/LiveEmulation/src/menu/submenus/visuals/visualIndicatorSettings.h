#include "../../submenu.h"

class VisualIndicatorSettings : public Submenu {
public:
	static VisualIndicatorSettings* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate();

	VisualIndicatorSettings() : Submenu() {}
	~VisualIndicatorSettings();
};

namespace VisualIndicatorSettingsMenuVars {
	struct Vars {
		float cg_hudGrenadeIconOffset;
		float cg_hudGrenadeIconWidth;
		float cg_hudGrenadeIconHeight;
		float cg_hudGrenadePointerWidth;
		float cg_hudGrenadePointerHeight;

		Vector2 m_playerPointerSize;

		Color m_friendlyPlayerPointerColor;
		Color m_enemyPlayerPointerColor;
		Color m_selectedPlayerTargetPointerColor;

		Color m_weaponPointerColor;
		Color m_weaponIconColor;

		Color m_scavengerPointerColor;

		Color m_throwablePointerColor;
	};

	extern Vars vars;
}