#pragma once

#include "Classes.hpp"

namespace SDK
{
	class HealthController : public UnityEngine::Object
	{
	private:
	public:
		HealthController(const std::uint64_t& Address = 0x00) : UnityEngine::Object(Address) {}

		float GetHealth()
		{
			const auto health_body = Driver->Read<uintptr_t>(this->Address + 0x90);

			const auto body_controller = Driver->Read<uintptr_t>(health_body + 0x18);

			for (int i = 0; i <= 6; i++)
			{
				const auto body_part = Driver->Read<uintptr_t>(body_controller + 0x30 + (i * 0x18));
				const auto health_container = Driver->Read<uintptr_t>(body_part + 0x10);

				const auto health_max = Driver->Read<float>(health_container + 0x14);
				auto health = Driver->Read<float>(health_container + 0x10);

				float hp = 0.f;
				hp += health;
				health = hp / health_max * 100;

				return health;
			}

			return 0.f;
		}

		bool IsAlive()
		{
			return Driver->Read<bool>(this->Address + 0xd4);
		}
		int GetETagStatus()
		{
			return Driver->Read<bool>(this->Address + 0xf0);//etag status
		}
	};
}