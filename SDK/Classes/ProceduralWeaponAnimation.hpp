#pragma once

#include "Classes.hpp"

namespace SDK
{
	class ProceduralWeaponAnimation : public UnityEngine::Object
	{
	private:
	public:
		ProceduralWeaponAnimation(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}

		SDK::FirearmController GetFirearmController()
		{
			return SDK::FirearmController(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_Animations_ProceduralWeaponAnimation::_firearmAnimationData));
		}

		SDK::ShotEffector GetShotEffector()
		{
			return SDK::ShotEffector(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_Animations_ProceduralWeaponAnimation::Shootingg));
		}

		SDK::WalkEffector GetWalkEffector()
		{
			return SDK::WalkEffector(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_Animations_ProceduralWeaponAnimation::Walk));
		}

		SDK::BreathEffector GetBreathEffector()
		{
			return SDK::BreathEffector(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_Animations_ProceduralWeaponAnimation::Breath));
		}

		void SetMask(std::uint32_t Value)
		{
			Driver->Write<std::uint32_t>(this->Address + Offset::EFT_Animations_ProceduralWeaponAnimation::Mask, Value);
		}
	};
}