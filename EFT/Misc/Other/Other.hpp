#pragma once

#include "../Misc.hpp"

namespace EFT
{
	namespace Misc
	{
		class COther
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

					SDK::Physical Physical = LocalPlayer.GetPhysical();

					if (!Physical.Address)
					{
						continue;
					}

					SDK::StaminaParameters HandsStamina = Physical.GetHandsStamina();

					if (!HandsStamina.Address)
					{
						continue;
					}

					SDK::ProceduralWeaponAnimation ProceduralWeaponAnimation = LocalPlayer.GetProceduralWeaponAnimation();

					if (!ProceduralWeaponAnimation.Address)
					{
						continue;
					}

					SDK::ShotEffector ShotEffector = ProceduralWeaponAnimation.GetShotEffector();

					if (!ShotEffector.Address)
					{
						continue;
					}

					SDK::BreathEffector BreathEffector = ProceduralWeaponAnimation.GetBreathEffector();

					if (!BreathEffector.Address)
					{
						continue;
					}

					if (CONFIG_GET(bool, NoRecoil))
					{
						ShotEffector.SetRecoilStrength(Math::Vector2());
					}

					if (CONFIG_GET(bool, NoSway))
					{
						ProceduralWeaponAnimation.SetMask(16);

						BreathEffector.SetStiffUntil(FLT_MAX);
						BreathEffector.SetOverweight(-1.00f);
						BreathEffector.SetIntensity(0.00f);
						BreathEffector.SetTremorOn(false);
						HandsStamina.SetCurrent(100.00f);
					}
				}
			}
		public:
			void Run()
			{
				std::thread([&]() { this->Update(); }).detach();
			}
		};

		inline EFT::Misc::COther* Other = new EFT::Misc::COther();
	}
}