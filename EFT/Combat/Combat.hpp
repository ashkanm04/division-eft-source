#pragma once

#include "../EFT.hpp"

#include "Aimbot/Aimbot.hpp"

namespace EFT
{
	namespace Combat
	{
		void Run()
		{
			EFT::Combat::Aimbot->Run();
		}
	}
}