#pragma once

#include "UnityEngine.h"

namespace UnityEngine
{
	class Camera : public BaseObject
	{
	private:
	public:
		Camera(const std::uint64_t& Address = 0x00) : BaseObject(Address) {}

		static UnityEngine::Camera Find(const std::string& Name)
		{
			const std::uint64_t Manager = Driver->Read<std::uint64_t>(Storage->UnityPlayer + Offset::AllCameras);

			if (!Manager)
			{
				return UnityEngine::Camera();
			}

			const std::uint64_t Start = Driver->Read<std::uint64_t>(Manager + 0x00);
			const std::uint32_t Size = Driver->Read<std::uint32_t>(Manager + 0x10);

			for (std::uint32_t Index = 0; Index < Size; Index++)
			{
				UnityEngine::Camera Entry = UnityEngine::Camera(Driver->Read<std::uint64_t>(Start + (Index * 0x8)));

				if (!Entry.Address || !Entry.IsActive())
				{
					continue;
				}

				UnityEngine::GameObject GameObject = Entry.GetGameObject();

				if (!GameObject.Address)
				{
					continue;
				}

				const std::string ObjectName = GameObject.GetObjectName();

				if (ObjectName.find(Name) != std::string::npos)
				{
					return Entry;
				}
			}

			return UnityEngine::Camera();
		}

		static bool WorldToScreen(const Math::Vector3& World, Math::Vector2* Screen, UnityEngine::Matrix ViewMatrix)
		{
			 Math::Vector3 TransposeVector{ ViewMatrix._14, ViewMatrix._24, ViewMatrix._34 };
			 Math::Vector3 RightVector{ ViewMatrix._11, ViewMatrix._21, ViewMatrix._31 };
			 Math::Vector3 UpVector{ ViewMatrix._12, ViewMatrix._22, ViewMatrix._32 };

			float W = TransposeVector.Dot(World) + ViewMatrix._44;
			float Y = UpVector.Dot(World) + ViewMatrix._42;
			float X = RightVector.Dot(World) + ViewMatrix._41;

			Math::Vector2 Final = Math::Vector2((Storage->ScreenSize.x / 2.00f) * (1 + X / W), (Storage->ScreenSize.y / 2.00f) * (1 - Y / W));

			if (!Final.IsValid() || W < 0.098f)
				return false;

			Screen->x = Final.x;
			Screen->y = Final.y;

			return true;
		}

		UnityEngine::GameObject GetGameObject()
		{
			return UnityEngine::GameObject(Driver->Read<std::uint64_t>(this->Address + 0x30));
		}

		UnityEngine::Matrix GetViewMatrix()
		{
			return Driver->Read<UnityEngine::Matrix>(this->Address + 0x2E4);
		}

		Math::Vector3 GetPosition()
		{
			return Driver->Read<Math::Vector3>(this->Address + 0x42C);
		}

		bool IsActive()
		{
			return Driver->Read<bool>(this->Address + 0x39);
		}
	};
}