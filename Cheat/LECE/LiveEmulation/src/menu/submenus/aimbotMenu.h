#pragma once
#include "../submenu.h"

class AimbotMenu : public Submenu{
public:
	static AimbotMenu* GetInstance();

	void Init() override;
	void InitVars() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate();

	AimbotMenu() : Submenu() { }
	~AimbotMenu();
};

namespace AimbotMenuVars
{
	struct Vars {
		bool m_required;
		bool m_autoShoot;
		bool m_autowall;
		bool m_autoAim;
		bool m_shieldHandler;
		bool m_showTargetViaEsp;
		int m_bone;
		int m_aimType;
		int m_targetType;
		bool m_fmj;
		bool m_prediction;
		float m_predictionDelta;
		int m_crouchEventSelection;
		bool m_crouchEvent;
		int m_crouchDelay;

		bool m_canShoot;
		bool m_canAim;

		D3DXVECTOR3 m_silentAngle;
		D3DXVECTOR3 m_toAngle;

		const char* m_usingTag;
		bool m_hittable;
		bool m_visible;
	};

	int GetTarget();
	bool IsTargetVisible(float fraction);
	void FixMovement(UserCommand* pCmd, float CurAngle, float OldAngle, float fOldForward, float fOldSidemove);
	void SilentAim(UserCommand* oldCommand, D3DXVECTOR3 toAngle);
	void SnapAim(D3DXVECTOR3 toAngle);
	float GetBoneFraction(int client, const char* aimtag);

	extern Vars vars;
	extern ScrollStruct<int> m_crouchEvent[4];
}