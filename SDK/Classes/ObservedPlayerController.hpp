#pragma once

#include "Classes.hpp"

namespace SDK
{
	class ObservedPlayerController : public UnityEngine::Object
	{
	private:
	public:
		ObservedPlayerController(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}

		SDK::HealthController GetHealthController()
		{
			return SDK::HealthController(Driver->Read<std::uint64_t>(this->Address + Offset::__GClass1D77::gClass0887_0xF0));
		}
	};
}