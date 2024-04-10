#pragma once

#include "Classes.hpp"

namespace SDK
{
	class WalkEffector : public UnityEngine::Object
	{
	private:
	public:
		WalkEffector(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}

		void SetIsWalking(bool Value)
		{
			Driver->Write<bool>(this->Address + Offset::__WalkEffector::_isWalking, Value);
		}

		void SetIntensity(float Value)
		{
			Driver->Write<float>(this->Address + Offset::__WalkEffector::Intensity, Value);
		}

		void SetStepFrequency(float Value)
		{
			Driver->Write<float>(this->Address + Offset::__WalkEffector::StepFrequency, Value);
		}

		void SetOverweight(float Value)
		{
			Driver->Write<float>(this->Address + Offset::__WalkEffector::Overweight, Value);
		}
	};
}