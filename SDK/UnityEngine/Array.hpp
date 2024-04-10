#pragma once

#include "UnityEngine.h"

namespace UnityEngine
{
	class Array : public BaseObject
	{
	private:
	public:
		Array(const std::uint64_t& Address = 0x00) : BaseObject(Address) {}

		template<typename Type>
		Type GetClass(const std::uint32_t& Index)
		{
			return Type(Driver->Read<std::uint64_t>(this->Address + (0x20 + (Index * sizeof(std::uint64_t)))));
		}

		template<typename Type>
		Type GetEntry(const std::uint32_t& Index)
		{
			return Driver->Read<Type>(this->Address + (0x20 + (Index * sizeof(Type))));
		}

		std::uint32_t GetSize()
		{
			return Driver->Read<std::uint32_t>(this->Address + 0x18);
		}
	};
}