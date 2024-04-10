#pragma once

#include "../Include.h"

class BaseObject
{
private:
public:
	BaseObject(const std::uint64_t& Address = 0x00)
	{
		if (Address <= 0x00)
		{
			this->Address = 0x00;
			return;
		}

		this->Address = Address;
	}

	std::uint64_t Address;
};

#include "Storage/Storage.hpp"

#include "Offset/Offset.hpp"

#include "UnityEngine/UnityEngine.h"

#include "Classes/Classes.hpp"

#include "World/World.hpp"
#include "Screen/Screen.hpp"