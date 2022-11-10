#include "../../submenu.h"

class VisualInformationMenu : public Submenu {
public:
	static VisualInformationMenu* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate();

	VisualInformationMenu() : Submenu() { }
	~VisualInformationMenu();
};

namespace VisualInformationMenuVars
{
	struct Vars {
		bool m_targetDetails;
		bool m_screenResolution;
		bool m_fps;
		bool m_ping;
		bool m_map;
		bool m_gameMode;
		bool m_gameHost;
		bool m_natType;
		bool m_clientCount;

		char m_displayOptions[10][50];
		int m_displayIndex;
	};
}