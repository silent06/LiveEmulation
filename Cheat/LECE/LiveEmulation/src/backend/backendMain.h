#pragma once

class Backend {
public:
	static handle_s m_hookEvents[HANDLE_COUNT];
	static Vector2 m_radarXY;
	static bool m_canRenderRadar;
	static DWORD m_myModuleBase;
	static DWORD m_myModuleSize;
	static UIContext* m_uiContext;

	static void Start();
	static bool ApplyHooks();
	static bool ApplyPatches();
	static void Cleanup();
	static void Reset() { m_loaded = false; };
	static bool IsLoaded() { return m_loaded; };
	static void ForceLoad() { m_loaded = true; };
private:
	static bool m_loaded;
};