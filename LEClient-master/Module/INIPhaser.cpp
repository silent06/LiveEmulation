#include "stdafx.h"

bool INI::EnableBlockXblDns;
bool INI::MachineIDSpoofing;
bool INI::CustomNotification;
bool INI::LEImage;
bool INI::DrawGuideInformation;
bool INI::Gradient;
bool INI::GradientBG;
bool INI::GradientBtns;
bool INI::ColorFade;
bool INI::GhostBypass;
bool INI::AwBypass;
bool INI::BO2Bypass;
bool INI::B03Bypass;
CSimpleIniA ini;

void INI::Create() {

	BYTE INIData[0x47F] = {
	0xEF, 0xBB, 0xBF, 0x23, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F,
	0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0D, 0x0A, 0x23, 0x20, 0x20, 0x20,
	0x20, 0x2F, 0x20, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x28, 0x5F, 0x29, 0x20,
	0x5F, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20,
	0x20, 0x20, 0x20, 0x2F, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, 0x20, 0x20,
	0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20,
	0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x2F,
	0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x20, 0x20, 0x2F, 0x20,
	0x2F, 0x5F, 0x20, 0x20, 0x20, 0x28, 0x5F, 0x29, 0x20, 0x20, 0x5F, 0x5F,
	0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x0D,
	0x0A, 0x23, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x20, 0x20, 0x20,
	0x2F, 0x20, 0x2F, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x2F, 0x20, 0x2F, 0x20,
	0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x20, 0x2F, 0x20, 0x5F, 0x5F, 0x2F,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x5F, 0x5F, 0x20, 0x60, 0x5F,
	0x5F, 0x20, 0x5C, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x2F, 0x20,
	0x20, 0x2F, 0x20, 0x2F, 0x20, 0x20, 0x2F, 0x20, 0x5F, 0x5F, 0x20, 0x60,
	0x2F, 0x20, 0x2F, 0x20, 0x5F, 0x5F, 0x2F, 0x20, 0x20, 0x2F, 0x20, 0x2F,
	0x20, 0x20, 0x2F, 0x20, 0x5F, 0x5F, 0x20, 0x5C, 0x20, 0x20, 0x2F, 0x20,
	0x5F, 0x5F, 0x20, 0x5C, 0x0D, 0x0A, 0x23, 0x20, 0x20, 0x2F, 0x20, 0x2F,
	0x5F, 0x5F, 0x5F, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x20, 0x7C, 0x20, 0x7C,
	0x2F, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x20, 0x5F, 0x5F, 0x2F, 0x20, 0x2F,
	0x20, 0x2F, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x2F,
	0x20, 0x2F, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x2F, 0x2F, 0x20, 0x2F, 0x5F,
	0x2F, 0x20, 0x2F, 0x20, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x20, 0x2F, 0x20,
	0x2F, 0x5F, 0x2F, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x2F, 0x5F, 0x20, 0x20,
	0x20, 0x2F, 0x20, 0x2F, 0x20, 0x20, 0x2F, 0x20, 0x2F, 0x5F, 0x2F, 0x20,
	0x2F, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x2F, 0x0D, 0x0A, 0x23,
	0x20, 0x2F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, 0x2F, 0x5F, 0x2F, 0x20,
	0x20, 0x20, 0x7C, 0x5F, 0x5F, 0x5F, 0x2F, 0x20, 0x20, 0x5C, 0x5F, 0x5F,
	0x5F, 0x2F, 0x20, 0x2F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, 0x20, 0x20,
	0x20, 0x2F, 0x5F, 0x2F, 0x20, 0x2F, 0x5F, 0x2F, 0x20, 0x2F, 0x5F, 0x2F,
	0x20, 0x5C, 0x5F, 0x5F, 0x2C, 0x5F, 0x2F, 0x20, 0x20, 0x2F, 0x5F, 0x2F,
	0x20, 0x20, 0x20, 0x5C, 0x5F, 0x5F, 0x2C, 0x5F, 0x2F, 0x20, 0x20, 0x5C,
	0x5F, 0x5F, 0x2F, 0x20, 0x20, 0x2F, 0x5F, 0x2F, 0x20, 0x20, 0x20, 0x5C,
	0x5F, 0x5F, 0x5F, 0x5F, 0x2F, 0x20, 0x2F, 0x5F, 0x2F, 0x20, 0x2F, 0x5F,
	0x2F, 0x20, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0D, 0x0A, 0x23, 0x20, 0x4F, 0x70,
	0x74, 0x69, 0x6F, 0x6E, 0x73, 0x20, 0x61, 0x72, 0x65, 0x20, 0x74, 0x6F,
	0x67, 0x67, 0x6C, 0x65, 0x64, 0x20, 0x75, 0x73, 0x69, 0x6E, 0x67, 0x20,
	0x74, 0x72, 0x75, 0x65, 0x20, 0x3D, 0x20, 0x4F, 0x6E, 0x2C, 0x20, 0x66,
	0x61, 0x6C, 0x73, 0x65, 0x20, 0x3D, 0x20, 0x4F, 0x66, 0x66, 0x0D, 0x0A,
	0x0D, 0x0A, 0x5B, 0x4C, 0x69, 0x76, 0x65, 0x45, 0x6E, 0x76, 0x69, 0x72,
	0x6F, 0x6E, 0x6D, 0x65, 0x6E, 0x74, 0x5D, 0x0D, 0x0A, 0x45, 0x6E, 0x61,
	0x62, 0x6C, 0x65, 0x5F, 0x42, 0x6C, 0x6F, 0x63, 0x6B, 0x58, 0x62, 0x6C,
	0x44, 0x6E, 0x73, 0x20, 0x3D, 0x20, 0x66, 0x61, 0x6C, 0x73, 0x65, 0x0D,
	0x0A, 0x0D, 0x0A, 0x5B, 0x55, 0x49, 0x5D, 0x0D, 0x0A, 0x45, 0x6E, 0x61,
	0x62, 0x6C, 0x65, 0x5F, 0x44, 0x61, 0x73, 0x68, 0x55, 0x69, 0x4D, 0x6F,
	0x64, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x73, 0x20,
	0x3D, 0x20, 0x66, 0x61, 0x6C, 0x73, 0x65, 0x0D, 0x0A, 0x45, 0x6E, 0x61,
	0x62, 0x6C, 0x65, 0x5F, 0x48, 0x75, 0x64, 0x43, 0x6F, 0x6C, 0x6F, 0x72,
	0x4D, 0x6F, 0x64, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E,
	0x73, 0x20, 0x3D, 0x20, 0x66, 0x61, 0x6C, 0x73, 0x65, 0x0D, 0x0A, 0x45,
	0x6E, 0x61, 0x62, 0x6C, 0x65, 0x5F, 0x44, 0x72, 0x61, 0x77, 0x47, 0x75,
	0x69, 0x64, 0x65, 0x49, 0x6E, 0x66, 0x6F, 0x72, 0x6D, 0x61, 0x74, 0x69,
	0x6F, 0x6E, 0x20, 0x3D, 0x20, 0x74, 0x72, 0x75, 0x65, 0x0D, 0x0A, 0x45,
	0x6E, 0x61, 0x62, 0x6C, 0x65, 0x5F, 0x43, 0x75, 0x73, 0x74, 0x6F, 0x6D,
	0x4E, 0x6F, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E,
	0x20, 0x3D, 0x20, 0x74, 0x72, 0x75, 0x65, 0x0D, 0x0A, 0x0D, 0x0A, 0x5B,
	0x42, 0x79, 0x70, 0x61, 0x73, 0x73, 0x5D, 0x0D, 0x0A, 0x45, 0x6E, 0x61,
	0x62, 0x6C, 0x65, 0x5F, 0x4D, 0x61, 0x63, 0x68, 0x69, 0x6E, 0x65, 0x49,
	0x44, 0x53, 0x70, 0x6F, 0x6F, 0x66, 0x20, 0x3D, 0x20, 0x74, 0x72, 0x75,
	0x65, 0x0D, 0x0A, 0x45, 0x6E, 0x61, 0x62, 0x6C, 0x65, 0x5F, 0x47, 0x68,
	0x6F, 0x73, 0x74, 0x73, 0x5F, 0x42, 0x79, 0x70, 0x61, 0x73, 0x73, 0x20,
	0x3D, 0x20, 0x74, 0x72, 0x75, 0x65, 0x0D, 0x0A, 0x45, 0x6E, 0x61, 0x62,
	0x6C, 0x65, 0x5F, 0x41, 0x57, 0x5F, 0x42, 0x79, 0x70, 0x61, 0x73, 0x73,
	0x20, 0x3D, 0x20, 0x74, 0x72, 0x75, 0x65, 0x0D, 0x0A, 0x45, 0x6E, 0x61,
	0x62, 0x6C, 0x65, 0x5F, 0x42, 0x4F, 0x32, 0x5F, 0x42, 0x79, 0x70, 0x61,
	0x73, 0x73, 0x20, 0x3D, 0x20, 0x74, 0x72, 0x75, 0x65, 0x0D, 0x0A, 0x45,
	0x6E, 0x61, 0x62, 0x6C, 0x65, 0x5F, 0x42, 0x4F, 0x33, 0x5F, 0x42, 0x79,
	0x70, 0x61, 0x73, 0x73, 0x20, 0x3D, 0x20, 0x74, 0x72, 0x75, 0x65
	};

	Utilities::CWriteFile(INI_Path, INIData, sizeof(INIData));
	Read();
}

void INI::Init() {
	ini.SetUnicode();
	Utilities::FileExists(INI_Path) ? Read() : Create();
}

void INI::UpdateSettings() {
	ini.SetboolValue("LiveEnvironment", "Enable_BlockXblDns", EnableBlockXblDns);

	ini.SetboolValue("UI", "Enable_DrawGuideInformation", DrawGuideInformation);
	ini.SetboolValue("UI", "Enable_CustomNotification", CustomNotification);
	ini.SetboolValue("UI", "Enable_LEImage", LEImage);
	ini.SetboolValue("UI", "Enable_Gradient", Gradient);
	ini.SetboolValue("UI", "Enable_GradientBG", GradientBG);
	ini.SetboolValue("UI", "Enable_GradientBtns", GradientBtns);
	ini.SetboolValue("UI", "Enable_ColorFade", ColorFade);

	ini.SetboolValue("Bypass", "Enable_MachineIDSpoof", MachineIDSpoofing);
	ini.SetboolValue("Bypass", "Enable_Ghosts_Bypass", GhostBypass);
	ini.SetboolValue("Bypass", "Enable_AW_Bypass", AwBypass);
	ini.SetboolValue("Bypass", "Enable_BO2_Bypass", BO2Bypass);
	ini.SetboolValue("Bypass", "Enable_BO3_Bypass", B03Bypass);

	ini.SaveFile(INI_Path);
}


void INI::Read() {
	ini.LoadFile(INI_Path);

	EnableBlockXblDns = ini.GetboolValue("LiveEnvironment", "Enable_BlockXblDns");

	DrawGuideInformation = ini.GetboolValue("UI", "Enable_DrawGuideInformation");
	CustomNotification = ini.GetboolValue("UI", "Enable_CustomNotification");
	LEImage = ini.GetboolValue("UI", "Enable_LEImage");
	Gradient = ini.GetboolValue("UI", "Enable_Gradient");
	GradientBG = ini.GetboolValue("UI", "Enable_GradientBG");
	GradientBtns = ini.GetboolValue("UI", "Enable_GradientBtns");
	ColorFade = ini.GetboolValue("UI", "Enable_ColorFade");

	MachineIDSpoofing = ini.GetboolValue("Bypass", "Enable_MachineIDSpoof");
	GhostBypass = ini.GetboolValue("Bypass", "Enable_Ghosts_Bypass");
	AwBypass = ini.GetboolValue("Bypass", "Enable_AW_Bypass");
	BO2Bypass = ini.GetboolValue("Bypass", "Enable_BO2_Bypass");
	B03Bypass = ini.GetboolValue("Bypass", "Enable_BO3_Bypass");
}