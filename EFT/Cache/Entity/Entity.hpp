#pragma once

#include "../Cache.hpp"

#include "Object.hpp"


namespace EFT
{
	namespace Cache
	{
		class CEntity
		{
		private:
			void Clear()
			{
				this->PlayerList->Clear();
			}

			void Update()
			{
				while (true)
				{
					Sleep(100);

					SDK::GameWorld GameWorld = EFT::Cache::Instance->GameWorld->Data->Get();

					if (!GameWorld.Address)
					{
						this->Clear();
						continue;
					}

					UnityEngine::List RegisteredPlayers = GameWorld.GetRegisteredPlayers();

					if (!RegisteredPlayers.Address)
					{
						this->Clear();
						continue;
					}

					const std::uint64_t Start = RegisteredPlayers.GetStart();
					const std::uint32_t Size = RegisteredPlayers.GetSize();

					std::vector<SDK::ObservedPlayerView> PlayerList;
					for (std::uint32_t Index = 0; Index < Size; Index++)
					{
						SDK::ObservedPlayerView Player = RegisteredPlayers.GetClass<SDK::ObservedPlayerView>(Start, Index);

						if (!Player.Address)
						{
							continue;
						}

						PlayerList.push_back(Player);
					}
					this->PlayerList->Set(PlayerList);
				}
			}
		public:
			EntityObject<SDK::ObservedPlayerView>* PlayerList = new EntityObject<SDK::ObservedPlayerView>();

			void Run()
			{
				std::thread([&]() { this->Update(); }).detach();
			}
		};

		inline EFT::Cache::CEntity* Entity = new EFT::Cache::CEntity();
	}
}