#pragma once

#include "UnityEngine.h"

namespace UnityEngine
{
	class GameObject : public BaseObject
	{
	private:
	public:
		GameObject(const std::uint64_t& Address = 0x00) : BaseObject(Address) {}

		template<typename Type>
		static Type Find(const std::string& Name)
		{
			const std::uint64_t Manager = Driver->Read<std::uint64_t>(Storage->UnityPlayer + Offset::GameObjectManager);

			if (!Manager)
			{
				return Type();
			}

			std::uint64_t Current = Driver->Read<std::uint64_t>(Manager + 0x28);
			std::uint64_t Last = Driver->Read<std::uint64_t>(Manager + 0x20);

			while (Current && Driver->Read<std::uint64_t>(Current + 0x10) != Driver->Read<std::uint64_t>(Last + 0x10))
			{
				UnityEngine::GameObject Entry = UnityEngine::GameObject(Driver->Read<std::uint64_t>(Current + 0x10));

				if (!Entry.Address)
				{
					Current = Driver->Read<std::uint64_t>(Current + 0x8);
					continue;
				}

				const std::string ObjectName = Entry.GetObjectName();

				if (!ObjectName.size())
				{
					Current = Driver->Read<std::uint64_t>(Current + 0x8);
					continue;
				}

				if (ObjectName.find(Name) != std::string::npos)
				{
					return Type(Driver->ReadChain<std::uint64_t>(Driver->Read<std::uint64_t>(Current + 0x10), {0x30, 0x18, 0x28}));
				}

				Current = Driver->Read<std::uint64_t>(Current + 0x8);
			}

			return Type();
		}

		UnityEngine::VisualState GetVisualState()
		{
			return UnityEngine::VisualState(Driver->ReadChain<std::uint64_t>(this->Address, { 0x30,0x8,0x38 }));
		}

		std::string GetObjectName()
		{
#ifdef DEVELOPMENT
			return Driver->ReadString(this->Address + 0x60);
#endif

#ifndef DEVELOPMENT
			return Driver->ReadString(Driver->Read<std::uint64_t>(this->Address + 0x60));
#endif
		}
	};
}