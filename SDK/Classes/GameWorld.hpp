#pragma once

#include "Classes.hpp"

namespace SDK
{
	class GameWorld : public UnityEngine::Object
	{
	private:
	public:
		GameWorld(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) { }

		UnityEngine::Dictionary GetAllObservedPlayers()
		{
			return UnityEngine::Dictionary(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_GameWorld::allObservedPlayersByID));
		}

		UnityEngine::List GetRegisteredPlayers()
		{
			return UnityEngine::List(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_GameWorld::RegisteredPlayers));
		}

		SDK::Player GetLocalPlayer()
		{
			return SDK::Player(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_GameWorld::MainPlayer));
		}
	};
}