#pragma once

#include "Classes.hpp"

namespace SDK
{
	class FirearmController : public UnityEngine::Object
	{
	private:
	public:
		FirearmController(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}

		
	};
}