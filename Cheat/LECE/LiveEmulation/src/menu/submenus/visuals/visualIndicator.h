#include "../../submenu.h"

class VisualIndicator : public Submenu {
public:
	static VisualIndicator* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate();

	VisualIndicator() : Submenu() { }
	~VisualIndicator();
};

namespace VisualIndicatorMenuVars
{
	struct Vars {
		bool m_onceInGameFlag;
		bool m_player;
		bool m_weapons;
		bool m_scav;
		bool m_throwable;
	};
	void InitRadius();

	extern Vars vars;
}