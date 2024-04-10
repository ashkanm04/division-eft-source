#pragma once

#include "../Misc.hpp"

namespace EFT
{
	namespace Misc
	{
		class CDebugCamera
		{
		private:
			void UpdateOffset()
			{
				while (true)
				{
					Sleep(15);

					if (GetAsyncKeyState(CONFIG_GET(int, DebugCameraKey)) & 0x1)
					{
						CONFIG_GET(bool, DebugCamera) = !CONFIG_GET(bool, DebugCamera);
					}

					if (!CONFIG_GET(bool, DebugCamera))
					{
						this->Offset->Set(Math::Vector3());
						continue;
					}

					SDK::Player LocalPlayer = EFT::Cache::Instance->LocalPlayer->Data->Get();

					if (!LocalPlayer.Address)
					{
						continue;
					}

					SDK::MovementContext MovementContext = LocalPlayer.GetMovementContext();

					if (!MovementContext.Address)
					{
						continue;
					}
					
					const float Speed = GetAsyncKeyState(VK_SHIFT) ? 0.30f : 0.10f;
					Math::Vector3 Buffer = Math::Vector3();

					if (GetAsyncKeyState('W'))
					{
						Buffer.z += Speed;
					}

					if (GetAsyncKeyState('S'))
					{
						Buffer.z -= Speed;
					}

					if (GetAsyncKeyState('A'))
					{
						Buffer.x -= Speed;
					}

					if (GetAsyncKeyState('D'))
					{
						Buffer.x += Speed;
					}

					if (GetAsyncKeyState(VK_SPACE))
					{
						Buffer.y += Speed;
					}

					this->Offset->Set
					(
						this->Offset->Get() +
						Math::Vector4::QuanternionMult(Math::Vector3(Buffer.x, 0.00f, Buffer.z), MovementContext.GetRotation()) +
						Math::Vector3(0.00f, Buffer.y, 0.00f)
					);
				}
			}

			void UpdatePosition()
			{
				Math::Vector3 Position = Math::Vector3();

				while (true)
				{
					if (!CONFIG_GET(bool, DebugCamera))
					{
						Position = Math::Vector3();
						Sleep(1);
						continue;
					}

					UnityEngine::Camera Camera = EFT::Cache::Instance->Camera->Data->Get();

					if (!Camera.Address)
					{
						continue;
					}

					UnityEngine::GameObject GameObject = Camera.GetGameObject();

					if (!GameObject.Address)
					{
						continue;
					}

					UnityEngine::VisualState VisualState = GameObject.GetVisualState();

					if (!VisualState.Address)
					{
						continue;
					}

					if (!Position.IsValid())
					{
						Position = VisualState.GetPosition();
					}

					VisualState.SetPosition(Position + this->Offset->Get());
				}
			}
		public:
			Mutex::Object<Math::Vector3>* Offset = new Mutex::Object<Math::Vector3>();

			void Run()
			{
				std::thread([&]() { this->UpdatePosition(); }).detach();
				std::thread([&]() { this->UpdateOffset(); }).detach();
			}
		};

		inline EFT::Misc::CDebugCamera* DebugCamera = new EFT::Misc::CDebugCamera();
	}
}