#pragma once

#include "../Cache.hpp"

namespace EFT
{
	namespace Cache
	{
		class CTarget
		{
		private:
			void UpdateEntity()
			{
				while (true)
				{
					Sleep(50);
					
					UnityEngine::Camera Camera = EFT::Cache::Instance->Camera->Data->Get();
					float Current = FLT_MAX;

					if (!Camera.Address)
					{
						this->Entity->Set(SDK::ObservedPlayerView());
						continue;
					}

					UnityEngine::Matrix ViewMatrix = Camera.GetViewMatrix();
					SDK::ObservedPlayerView Entity;
					
					for (std::size_t Index = 0; Index < EFT::Cache::Entity->PlayerList->Data->Size(); Index++)
					{
						SDK::ObservedPlayerView Player = EFT::Cache::Entity->PlayerList->Data->Get(Index);

						if (!Player.Address)
						{
							continue;
						}

						SDK::PlayerBody PlayerBody = Player.GetPlayerBody();

						if (!PlayerBody.Address)
						{
							continue;
						}

						SDK::DizSkinningSkeleton DizSkinningSkeleton = PlayerBody.GetDizSkinningSkeleton();

						if (!DizSkinningSkeleton.Address)
						{
							continue;
						}

						UnityEngine::List TransformList = DizSkinningSkeleton.GetTransformList();

						if (!TransformList.Address)
						{
							continue;
						}

						const std::uint64_t Start = TransformList.GetStart();

						if (!Start)
						{
							continue;
						}

						UnityEngine::Transform Transform = TransformList.GetClass<UnityEngine::Transform>(Start, CONFIG_GET(int, AimBone) == 0 ? SDK::Player::Bones::Head : SDK::Player::Bones::Spine2);

						if (!Transform.Address)
						{
							continue;
						}

						Math::Vector3 World = Transform.GetPosition();

						if (!World.IsValid())
						{
							continue;
						}

						Math::Vector2 Screen = Math::Vector2();
						if (!UnityEngine::Camera::WorldToScreen(World, &Screen, ViewMatrix))
						{
							continue;
						}

						const float Distance = Screen.Distance(Storage->ScreenSize / 2.00f);

						if (Distance < Current && CONFIG_GET(int, AimFOV) > Distance)
						{
							Current = Distance;
							Entity = Player;
						}
					}

					this->Entity->Set(Entity);
				}
			}

			void UpdateTarget()
			{
				while (true)
				{
					Sleep(1);

					std::pair<Math::Vector2, bool> Result = std::pair<Math::Vector2, bool>();
					UnityEngine::Camera Camera = EFT::Cache::Instance->Camera->Data->Get();

					if (!Camera.Address)
					{
						this->Angles->Set({ Math::Vector2(), false });
						continue;
					}

					Math::Vector3 CameraPosition = Camera.GetPosition();

					if (!CameraPosition.IsValid())
					{
						this->Angles->Set({ Math::Vector2(), false });
						continue;
					}

					SDK::Player LocalPlayer = EFT::Cache::Instance->LocalPlayer->Data->Get();

					if (!LocalPlayer.Address)
					{
						this->Angles->Set({ Math::Vector2(), false });
						continue;
					}
					
					SDK::HandsController HandsController = LocalPlayer.GetHandsController();

					if (!HandsController.Address)
					{
						this->Angles->Set({ Math::Vector2(), false });
						continue;
					}

					UnityEngine::Transform Fireport = HandsController.GetFireport().GetTransform();

					if (!Fireport.Address)
					{
						this->Angles->Set({ Math::Vector2(), false });
						continue;
					}

					Math::Vector3 FireportPosition = Fireport.GetPosition();

					if (!FireportPosition.IsValid())
					{
						this->Angles->Set({ Math::Vector2(), false });
						continue;
					}

					SDK::ObservedPlayerView Entity = this->Entity->Get();

					if (!Entity.Address)
					{
						this->Angles->Set({ Math::Vector2(), false });
						continue;
					}

					SDK::PlayerBody PlayerBody = Entity.GetPlayerBody();

					if (!PlayerBody.Address)
					{
						this->Angles->Set({ Math::Vector2(), false });
						continue;
					}

					SDK::DizSkinningSkeleton DizSkinningSkeleton = PlayerBody.GetDizSkinningSkeleton();

					if (!DizSkinningSkeleton.Address)
					{
						this->Angles->Set({ Math::Vector2(), false });
						continue;
					}

					UnityEngine::List TransformList = DizSkinningSkeleton.GetTransformList();

					if (!TransformList.Address)
					{
						this->Angles->Set({ Math::Vector2(), false });
						continue;
					}

					const std::uint64_t Start = TransformList.GetStart();

					if (!Start)
					{
						this->Angles->Set({ Math::Vector2(), false });
						continue;
					}

					UnityEngine::Transform Transform = TransformList.GetClass<UnityEngine::Transform>(Start, CONFIG_GET(int, AimBone) == 0 ? SDK::Player::Bones::Head : SDK::Player::Bones::Spine2);

					if (!Transform.Address)
					{
						this->Angles->Set({ Math::Vector2(), false });
						continue;
					}

					Math::Vector3 World = Transform.GetPosition();

					if (!World.IsValid())
					{
						this->Angles->Set({ Math::Vector2(), false });
						continue;
					}

					Result.first = Math::CalculateAngles(CameraPosition, World);
					Result.second = true;

					this->Angles->Set(Result);
				}
			}

			Mutex::Object<SDK::ObservedPlayerView>* Entity = new Mutex::Object<SDK::ObservedPlayerView>();
		public:
			Mutex::Object<std::pair<Math::Vector2, bool>>* Angles = new Mutex::Object<std::pair<Math::Vector2, bool>>();

			void Run()
			{
				std::thread([&]() { this->UpdateEntity(); }).detach();
				std::thread([&]() { this->UpdateTarget(); }).detach();
			}
		};

		inline EFT::Cache::CTarget* Target = new EFT::Cache::CTarget();
	}
}