#pragma once

#include "Classes.hpp"

namespace SDK
{
	class DizSkinningSkeleton : public UnityEngine::Object
	{
	private:
	public:
		DizSkinningSkeleton(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}

		UnityEngine::List GetTransformList()
		{
			return UnityEngine::List(Driver->Read<std::uint64_t>(this->Address + Offset::Diz_Skinning_Skeleton::_values));
		}
	};
}