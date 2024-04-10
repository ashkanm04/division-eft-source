#pragma once

#include "Classes.hpp"

namespace SDK
{
	class BreathEffector : public UnityEngine::Object
	{
	private:
	public:
		BreathEffector(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}

		void SetStiffUntil(float Value)
		{
			Driver->Write<float>(this->Address + Offset::EFT_Animations_BreathEffector::StiffUntill, Value);
		}

		void SetIntensity(float Value)
		{
			Driver->Write<float>(this->Address + Offset::EFT_Animations_BreathEffector::Intensity, Value);
		}

		void SetTremorOn(bool Value)
		{
			Driver->Write<bool>(this->Address + Offset::EFT_Animations_BreathEffector::TremorOn, Value);
		}

		void SetOverweight(float Value)
		{
			Driver->Write<float>(this->Address + Offset::EFT_Animations_BreathEffector::Overweightk__BackingField, Value);
		}
	};
}