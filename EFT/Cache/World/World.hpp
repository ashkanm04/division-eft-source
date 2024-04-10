#pragma once

#include "../Cache.hpp"

#include "Object.hpp"

namespace EFT
{
	namespace Cache
	{
		class CWorld
		{
		private:
			void Update()
			{
				while (true)
				{
					Sleep(5);

					UnityEngine::Camera Camera = EFT::Cache::Instance->Camera->Data->Get();

					if (!Camera.Address)
					{
						continue;
					}

					SDK::GameWorld GameWorld = EFT::Cache::Instance->GameWorld->Data->Get();

					if (!GameWorld.Address)
					{
						continue;
					}

					this->PlayerList->Cache();
				}
			}
		public:
			WorldObject<SDK::ObservedPlayerView, World::Player>* PlayerList = new WorldObject<SDK::ObservedPlayerView, World::Player>(EFT::Cache::Entity->PlayerList->Data);

			void Run()
			{
				std::thread([&]() { this->Update(); }).detach();
			}
		};

		inline EFT::Cache::CWorld* World = new EFT::Cache::CWorld();
	}
}