#pragma once

#include "UnityEngine.h"

namespace UnityEngine
{
	class List : public UnityEngine::Object
	{
	private:
	public:
		List(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}

		template<typename Type>
		Type GetClass(const std::uint64_t& Start, const std::uint32_t Index)
		{
			return Type(Driver->Read<std::uint64_t>(Start + (0x20 + (Index * sizeof(std::uint64_t)))));
		}

		template<typename Type>
		Type GetEntry(const std::uint64_t& Start, const std::uint32_t Index)
		{
			return Driver->Read<Type>(Start + (0x20 + (Index * sizeof(Type))));
		}

		std::uint64_t GetStart()
		{
			return Driver->Read<std::uint64_t>(this->Address + 0x10);
		}

		std::uint32_t GetSize()
		{
			return Driver->Read<std::uint32_t>(this->Address + 0x18);
		}
	};
}