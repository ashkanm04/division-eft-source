#pragma once

#include "Classes.hpp"

namespace SDK
{
	class PlayerBody : public UnityEngine::Object
	{
	private:
	public:
		PlayerBody(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}

		SDK::DizSkinningSkeleton GetDizSkinningSkeleton()
		{
			return SDK::DizSkinningSkeleton(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_PlayerBody::intt1));
		}
	};
}