#include "../submenu.h"

class VisualsMenu : public Submenu {
public:
	static VisualsMenu* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate();

	VisualsMenu() : Submenu() { }
	~VisualsMenu();
};

namespace VisualsMenuVars
{
	struct Vars {
		bool m_health;
		bool m_worldEntities;
		bool m_proximityWarning;
		bool m_visibleWarning;
		bool m_indicators;

		vector<const char*> m_queue;

		Color m_healthColor;
		Color m_healthTextColor;
	};

	extern Vars vars;
}