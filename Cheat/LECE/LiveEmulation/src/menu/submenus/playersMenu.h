#pragma once
#include "../submenu.h"

class PlayersMenu : public Submenu {
public:
	static PlayersMenu* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate();

	PlayersMenu() : Submenu() { }
	~PlayersMenu();
};

namespace PlayersMenuVars
{
	struct UserData {
		int m_id;
		const char* m_name;

		UserData(int id, const char* name)
			: m_id(id), m_name(name)
		{ }
	};

	struct Vars {
		bool m_needsToSort;
		vector<UserData> players;
	};

	extern Vars vars;
}