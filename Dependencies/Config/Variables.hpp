#pragma once

#include "Config.hpp"

#define CONFIG_GET(Type, Name) Config::Get<Type>(Config::Variables->Name)

namespace Config
{
	struct VariablesList
	{
		// Aim
		DEF_VAR(bool, AimEnabled, false);
		DEF_VAR(int, AimKey, VK_XBUTTON2);
		DEF_VAR(int, AimFOV, 75);
		DEF_VAR(int, AimBone, 0);
		DEF_VAR(bool, AimSilent, false);

		DEF_VAR(bool, Skeleton, false);
		DEF_VAR(bool, Distance, false);
		DEF_VAR(bool, Nickname, false);
		DEF_VAR(bool, Box, false);

		// Misc
		DEF_VAR(bool, DebugCamera, false);
		DEF_VAR(int, DebugCameraKey, 'H');
		DEF_VAR(bool, NoRecoil, false);
		DEF_VAR(bool, NoSway, false);
		DEF_VAR(bool, HealthStatus, true);
		// Visual
		DEF_VAR(bool, Crosshair, false);
		DEF_VAR(int, MiscCrosshairType, 2);

		DEF_VAR(bool, CheatInformation, true);
		DEF_VAR(bool, OverlayVSync, false);
	};

	inline VariablesList* Variables = new VariablesList();
}