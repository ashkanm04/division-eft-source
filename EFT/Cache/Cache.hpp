#pragma once

#include "../EFT.hpp"

#include "Instance/Instance.hpp"
#include "Entity/Entity.hpp"
#include "Target/Target.hpp"
#include "World/World.hpp"

namespace EFT
{
	namespace Cache
	{
		void Run()
		{
			EFT::Cache::Instance->Run();
			EFT::Cache::Entity->Run();
			EFT::Cache::Target->Run();
			EFT::Cache::World->Run();
		}
	}
}