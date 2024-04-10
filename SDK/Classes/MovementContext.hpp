#pragma once

#include "../SDK.h"

namespace SDK
{
	class MovementContext : public UnityEngine::Object
	{
	private:
	public:
		MovementContext(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}
		
		void SetFrozen(bool Value)
		{
			if (Value)
			{

			}
			else
			{
				
			}
		}

		void SetViewAngles(Math::Vector2 Value)
		{
			Driver->Write<Math::Vector2>(this->Address + Offset::EFT_MovementContext::_rotation, Value);
		}

		Math::Vector4 GetRotation()
		{
			Math::Vector2 ViewAngles = this->GetViewAngles();
			return Math::Vector4::ToQuanternion(Math::Vector3(ViewAngles.y, ViewAngles.x, 0.00f));
		}

		Math::Vector2 GetViewAngles()
		{
			return Driver->Read<Math::Vector2>(this->Address + Offset::EFT_MovementContext::_rotation);
		}
	};
}