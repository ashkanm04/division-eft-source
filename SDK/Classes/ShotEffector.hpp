#pragma once

#include "Classes.hpp"

namespace SDK
{
	class ShotEffector : public UnityEngine::Object
	{
	private:
	public:
		ShotEffector(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}

		void SetRecoilStrength(Math::Vector2 Value)
		{
			Driver->Write<Math::Vector2>(this->Address + 0x48, Value);
		}
	};
}