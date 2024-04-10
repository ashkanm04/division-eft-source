#pragma once

#include "Classes.hpp"

namespace SDK
{
	class HandsController : public UnityEngine::Object
	{
	private:
	public:
		HandsController(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}

		UnityEngine::BifacialTransform GetFireport()
		{
			uintptr_t hands = Driver->Read<uint64_t>(this->Address + Offset::EFT_Player::_handsController);
			return UnityEngine::BifacialTransform(Driver->Read<std::uint64_t>(hands + Offset::__Player_AbstractHandsController::WeaponRootk__BackingField));
		}
	};
}