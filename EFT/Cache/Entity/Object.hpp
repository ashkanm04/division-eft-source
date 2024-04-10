#pragma once

#include "Entity.hpp"

namespace EFT
{
	namespace Cache
	{
		template<typename Type>
		class EntityObject
		{
		private:
			std::mutex Mutex;
		public:
			void Clear()
			{
				this->Mutex.lock();
				this->Data->Clear();
				this->Mutex.unlock();
			}

			void Set(std::vector<Type> Value)
			{
				this->Mutex.lock();
				this->Data->Set(Value);
				this->Mutex.unlock();
			}

			Mutex::List<Type>* Data = new Mutex::List<Type>();
		};
	}
}