#include "../../submenu.h"

class MainChangeNameMenu : public Submenu {
public:
	static MainChangeNameMenu* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MainChangeNameMenu() : Submenu() {}
	~MainChangeNameMenu();
};

namespace MainChangeNameMenuVars
{
	struct Vars {
		bool m_rainbowName;
		int m_nameColor;
		char* m_name;
		char* m_defaultName;
		char m_precompiled[0x20];
	};

	extern Vars vars;
}