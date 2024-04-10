#pragma once

#include "UnityEngine.h"

namespace UnityEngine
{
	class VisualState : public BaseObject
	{
	private:
	public:
		VisualState(const std::uint64_t& Address = 0x00) : BaseObject(Address) {}

		Math::Vector3 GetPosition()
		{
			return Driver->Read<Math::Vector3>(this->Address + 0x90);
		}

		void SetPosition(Math::Vector3 Value)
		{
			Driver->Write<Math::Vector3>(this->Address + 0x90, Value);
		}
	};
}