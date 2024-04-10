#pragma once

#include <cstdint>
#include <cstdio>

namespace KMU
{
	class CR3
	{
	private:
	public:	
		CR3(const std::uint64_t& Address = 0x00);
		static std::uint64_t VirtualToPhysical(const std::uint64_t& DirectoryTable, const std::uint64_t& Address);
		static std::uint64_t System();

		bool Write(const std::uint64_t& Address, void* Buffer, const std::size_t& Size);
		bool Read(const std::uint64_t& Address, void* Buffer, const std::size_t& Size);
		
		template<typename Type>
		bool Write(const std::uint64_t& Address, const Type& Value)
		{
			return this->Write(Address, (void*)(&Value), sizeof(Type));
		}

		template<typename Type>
		Type Read(const std::uint64_t& Address)
		{
			Type Data = Type();

			this->Read(Address, (void*)(&Data), sizeof(Type));

			return Data;
		}

		std::uint64_t Address;
	};
}