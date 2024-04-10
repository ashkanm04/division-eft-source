#pragma once

#include "Instance.hpp"

#include <mutex>

struct Item {
	std::string bsgId;
	std::string shortName;
	double avg24hPrice;
	std::vector<std::string> tags;
};
struct ItemInfo {
	std::string shortName;
	double avg24hPrice;
	std::vector<std::string> tags;
	uintptr_t transform;
};

class CachedItemInfo
{
public:
	ItemInfo item;
	Math::Vector3 Pos;
};

std::unordered_map<std::string, Item>& getLootItems();
namespace EFT
{
	namespace Cache
	{
		template<typename Type>
		class InstanceObject
		{
		private:
			std::mutex Mutex;
		public:
			InstanceObject() { }

			void Update(const std::uint64_t& Address)
			{
				this->Mutex.lock();
				this->Data->Get().Address = Address;
				this->Mutex.unlock();
			}

			Mutex::Object<Type>* Data = new Mutex::Object<Type>();
		};
	}
}