#pragma once

#include "Classes.hpp"

namespace SDK
{
	class Player : public UnityEngine::Object
	{
	private:
	public:
		Player(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) { }

		enum Bones : int
		{
			Base = 0,
			Pelvis = 14,
			LeftThigh1 = 15,
			LeftThigh2 = 16,
			LeftCalf = 17,
			LeftFeet = 18,
			LeftToe = 19,
			RightThigh1 = 20,
			RightThigh2 = 21,
			RightCalf = 22,
			RightFoot = 23,
			RightToe = 24,
			Spine1 = 29,
			Spine2 = 36,
			Spine3 = 37,
			LeftCollarbone = 89,
			LeftUpperarm = 90,
			LeftForearm1 = 91,
			LeftForearm2 = 92,
			LeftForearm3 = 93,
			LeftPalm = 94,
			RightCollarbone = 110,
			RightUpperarm = 111,
			RightForearm1 = 112,
			RightForearm2 = 113,
			RightForearm3 = 114,
			RightPalm = 115,
			Neck = 132,
			Head = 133
		};

		SDK::ProceduralWeaponAnimation GetProceduralWeaponAnimation()
		{
			return SDK::ProceduralWeaponAnimation(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_Player::ProceduralWeaponAnimationk__BackingField));
		}

		SDK::HandsController GetHandsController()
		{
			return SDK::HandsController(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_Player::_handsController));
		}

		SDK::MovementContext GetMovementContext()
		{
			return SDK::MovementContext(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_Player::MovementContextk__BackingField));
		}

		SDK::HealthController GetHealthController()
		{
			return SDK::HealthController(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_Player::_healthController));
		}

		SDK::PlayerBody GetPlayerBody()
		{
			return SDK::PlayerBody(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_Player::_playerBody));
		}

		SDK::Physical GetPhysical()
		{
			return SDK::Physical(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_Player::Physical));
		}
	};
}