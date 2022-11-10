#include "../../submenu.h"

class VisualWorldEntities : public Submenu {
public:
	static VisualWorldEntities* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate();

	VisualWorldEntities() : Submenu() {}
	~VisualWorldEntities();
};

namespace VisualWorldEntitiesMenuVars {
	struct Vars {
		bool m_scav;
		bool m_weapons;
		bool m_throwable;
	};

	extern Vars vars;
}