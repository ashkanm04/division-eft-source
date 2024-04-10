#pragma once

#include "Classes.hpp"

namespace SDK
{
	class ObservedPlayerView : public UnityEngine::Object
	{
	private:
	public:
		ObservedPlayerView(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}

		SDK::ObservedPlayerController GetObservedPlayerController()
		{
			return SDK::ObservedPlayerController(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_NextObservedPlayer_ObservedPlayerView::gClass1D77_0x80));//obfuscated :3
		}

		SDK::PlayerBody GetPlayerBody()
		{
			return SDK::PlayerBody(Driver->Read<std::uint64_t>(this->Address + Offset::EFT_NextObservedPlayer_ObservedPlayerView::playerBody_0x60));
		}
	
		std::string GetNickname()
		{
			return Driver->ReadStringIL2CPP(this->Address + Offset::EFT_NextObservedPlayer_ObservedPlayerView::stringg4);//obfuscated :3
		}

		bool IsScav()
		{
			std::uint32_t Side = Driver->Read<std::uint32_t>(this->Address + Offset::EFT_NextObservedPlayer_ObservedPlayerView::intt7);//obfuscated :3

			return Side == 4;
		}

		bool IsAI()
		{
			return Driver->Read<bool>(this->Address + Offset::EFT_NextObservedPlayer_ObservedPlayerView::booll11);//obfuscated :3
		}
	};
}