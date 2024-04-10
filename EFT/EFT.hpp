#pragma once

#include "../Include.h"

#include "Cache/Cache.hpp"
#include "Combat/Combat.hpp"
#include "Misc/Misc.hpp"

namespace EFT
{
	void Run()
	{
		EFT::Combat::Run();
		EFT::Cache::Run();
		EFT::Misc::Run();
	}
}