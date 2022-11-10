#pragma once
#include "stdafx.h"

class INI {
public:

	// LiveEnvironment
	static bool EnableBlockXblDns;

	// cheats
	static bool MW3Engine;

	// ui
	static bool CustomNotification;
	static bool LEImage;
	static bool DrawGuideInformation;
	static bool Gradient;
	static bool GradientBG;
	static bool GradientBtns;
	static bool ColorFade;

	// bypass
	static bool GhostBypass;
	static bool AwBypass;
	static bool BO2Bypass;
	static bool B03Bypass;

	// console
	static bool MachineIDSpoofing;

	static void Init();
	static void Read();
	static void Create();
	static void UpdateSettings();
};
