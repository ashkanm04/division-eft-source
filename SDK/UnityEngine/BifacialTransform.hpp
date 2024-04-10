#pragma once

#include "../SDK.h"

namespace UnityEngine
{
	class BifacialTransform : public BaseObject
	{
	private:
	public:
		BifacialTransform(const std::uint64_t& Address = 0x00) : BaseObject(Address) {}

		UnityEngine::Transform GetTransform()
		{
			return UnityEngine::Transform(Driver->Read<std::uint64_t>(this->Address + 0x10));
		}
	};
}