#pragma once

#include "UnityEngine.h"

namespace UnityEngine
{
	class Object : public BaseObject
	{
	private:
	public:
		Object(const std::uint64_t& Address = 0x00) : BaseObject(Address) {}

		std::string GetTypeName()
		{
			return Driver->ReadString(Driver->Read<std::uint64_t>(this->Address + 0x00) + 0x10);
		}

		UnityEngine::GameObject GetGameObject()
		{
			return UnityEngine::GameObject(Driver->ReadChain<std::uint64_t>(this->Address, { 0x10, 0x30 }));
		}
	};
}