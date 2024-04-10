#pragma once

#include "../EFT.hpp"

#include "DebugCamera/DebugCamera.hpp"
#include "Other/Other.hpp"

namespace EFT
{
	namespace Misc
	{
		void Run()
		{
			EFT::Misc::DebugCamera->Run();
			EFT::Misc::Other->Run();
		}
	}
}