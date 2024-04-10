#pragma once

#include "../Combat.hpp"

namespace EFT
{
	namespace Combat
	{
		class CAimbot
		{
		private:
			void Update()
			{
				while (true)
				{
					Sleep(1);

					SDK::Player LocalPlayer = EFT::Cache::Instance->LocalPlayer->Data->Get();

					if (!LocalPlayer.Address)
					{
						continue;
					}

					std::pair<Math::Vector2, bool> Angles = EFT::Cache::Target->Angles->Get();

					if (!Angles.second)
					{
						continue;
					}

					SDK::MovementContext MovementContext = LocalPlayer.GetMovementContext();

					if (!MovementContext.Address)
					{
						continue;
					}

					if (GetAsyncKeyState(CONFIG_GET(int, AimKey)) && CONFIG_GET(bool, AimEnabled))
					{
						MovementContext.SetViewAngles(Angles.first);
					}
				}
			}
		public:
			void Run()
			{
				std::thread([&]() { this->Update(); }).detach();
			}
		};

		inline EFT::Combat::CAimbot* Aimbot = new EFT::Combat::CAimbot();
	}
}