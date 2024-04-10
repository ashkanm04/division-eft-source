#pragma once

#include "World.hpp"

namespace EFT
{
	namespace Cache
	{
		template<typename EntityType, typename WorldType>
		class WorldObject
		{
		private:
			Mutex::List<EntityType>* EntityList;
			std::mutex Mutex;
		public:
			WorldObject(Mutex::List<EntityType>* EntityList)
			{
				this->EntityList = EntityList;
			}

			void Cache()
			{
				this->Mutex.lock();
				std::vector<WorldType> Temp;
				for (std::size_t Index = 0; Index < this->EntityList->Size(); Index++)
				{
					Temp.push_back(WorldType(this->EntityList->Get(Index)));
				}
				this->Data->Set(Temp);
				this->Mutex.unlock();
			}

			Mutex::List<WorldType>* Data = new Mutex::List<WorldType>();
		};
	}
}