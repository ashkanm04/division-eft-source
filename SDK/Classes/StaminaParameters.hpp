#pragma once

#include "Classes.hpp"

namespace SDK
{
	class StaminaParameters : public UnityEngine::Object
	{
	private:
	public:
		StaminaParameters(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}

		float GetCurrent()
		{
			return Driver->Read<float>(this->Address + Offset::__GClass0673::Current);
		}

		void SetCurrent(float Value)
		{
			Driver->Write<float>(this->Address + Offset::__GClass0673::Current, Value);
		}
	};
}