#pragma once

#include "UnityEngine.h"

namespace UnityEngine
{
	class Dictionary : public UnityEngine::Object
	{
	private:
	public:
		// Constructor
		Dictionary(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) { }

		// Custom
		std::uint64_t GetStart()
		{
			return Driver->Read<std::uint32_t>(this->Address + 0x18);
		}

		std::uint32_t GetSize(std::uint64_t Start)
		{
			return Driver->Read<std::uint32_t>(Start + 0x18);
		}

		template<typename Type>
		void SetValue(std::uint64_t Start, const std::uint32_t& Index, const Type& Buffer)
		{
			return Driver->Write<Type>(Start + 0x18 * Index + 0x30, Buffer);
		}

		template<typename Type>
		void SetKey(std::uint64_t Start, const std::uint32_t& Index, const Type& Buffer)
		{
			return Driver->Write<Type>(Start + 0x18 * Index + 0x28, Buffer);
		}

		template<typename Type>
		Type GetValue(std::uint64_t Start, const std::uint32_t& Index)
		{
			return Driver->Read<Type>(Start + 0x18 * Index + 0x30);
		}

		template<typename Type>
		Type GetKey(std::uint64_t Start, const std::uint32_t& Index)
		{
			return Driver->Read<Type>(Start + 0x18 * Index + 0x28);
		}
	};
}