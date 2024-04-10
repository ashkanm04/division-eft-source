#pragma once

#include "Classes.hpp"

namespace SDK
{
	class Physical : public UnityEngine::Object
	{
	private:
	public:
		Physical(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}

		SDK::StaminaParameters GetHandsStamina()
		{
			return SDK::StaminaParameters(Driver->Read<std::uint64_t>(this->Address + Offset::__GClass0675::HandsStamina));
		}

		SDK::StaminaParameters GetStamina()
		{
			return SDK::StaminaParameters(Driver->Read<std::uint64_t>(this->Address + Offset::__GClass0675::Stamina));
		}

		float GetHandsCapacity()
		{
			return Driver->Read<float>(this->Address + Offset::__GClass0675::HandsRestoreRate);
		}
	};
}