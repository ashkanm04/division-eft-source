#pragma once

#include "CR3.h"

#include "../Device/Device.h"
#include "../Definitions.h"
#include "../Process/Process.h"

std::uint64_t KMU::CR3::VirtualToPhysical(const std::uint64_t& DirectoryTable, const std::uint64_t& Address)
{
    DIR_TABLE_BASE  DirectoryTableBase = { 0 };
    VIRTUAL_ADDRESS VirtualAddress = { 0 };
    PDPTE_LARGE     PDPTELarge = { 0 };
    PDE_LARGE       PDELarge = { 0 };
    PML4E           PML4E = { 0 };
    PDPTE           PDPTE = { 0 };
    PDE             PDE = { 0 };
    PTE             PTE = { 0 };

    DirectoryTableBase.Address = DirectoryTable;
    VirtualAddress.Address = Address;

    /* This calculation results in the PML4E address */
    if (!KMU::Device->Read((DirectoryTableBase.PhysicalAddress << PAGE_4KB_SHIFT) + (VirtualAddress.Pml4Index * 8), &PML4E, sizeof(PML4E)))
    {
        std::printf("ye313131s\n");
        return 0x00;
    }

    if (!PML4E.Present)
    {
        return 0x00;
    }

    /* This calculation results in the PDPTE address */
    if (!KMU::Device->Read((PML4E.PhysicalAddress << PAGE_4KB_SHIFT) + (VirtualAddress.PdptIndex * 8), &PDPTE, sizeof(PDPTE)))
    {
        return 0x00;
    }

    if (!PDPTE.Present)
    {
        return 0x00;
    }

    if (IS_LARGE_PAGE(PDPTE.Address))
    {
        PDPTELarge.Address = PDPTE.Address;

        return (PDPTELarge.PhysicalAddress << PAGE_1GB_SHIFT) + PAGE_1GB_OFFSET(Address);
    }

    /* This calculation results in the PDE address */
    if (!KMU::Device->Read((PDPTE.PhysicalAddress << PAGE_4KB_SHIFT) + (VirtualAddress.PdIndex * 8), &PDE, sizeof(PDE)))
    {
        return 0x00;
    }

    if (!PDE.Present)
    {
        return 0x00;
    }

    if (IS_LARGE_PAGE(PDE.Address))
    {
        PDELarge.Address = PDE.Address;

        return (PDELarge.PhysicalAddress << PAGE_2MB_SHIFT) + PAGE_2MB_OFFSET(Address);
    }

    /* This calculation results in the PTE address */
    if (!KMU::Device->Read((PDE.PhysicalAddress << PAGE_4KB_SHIFT) + (VirtualAddress.PtIndex * 8), &PTE, sizeof(PTE)))
    {
        return 0x00;
    }

    if (!PTE.Present)
    {
        return 0x00;
    }

    return (PTE.PhysicalAddress << PAGE_4KB_SHIFT) + VirtualAddress.PageIndex;
}

KMU::CR3::CR3(const std::uint64_t& Address)
{
	this->Address = Address;
}


std::uint64_t KMU::CR3::System()
{
    std::uint64_t Result = 0x00;

    for (std::uint64_t Index = 0; Index < 100; Index++)
    {
        BYTE Buffer[0x1000];

        if (!KMU::Device->Read(0x1000 + (Index * 0x1000), Buffer, sizeof(Buffer)))
            continue;

        if (0x00000001000600E9 ^ (0xffffffffffff00ff & *(std::uint64_t*)(Buffer)))
            continue;

        if (0xfffff80000000000 ^ (0xfffff80000000000 & *(std::uint64_t*)(Buffer + 0x70)))
            continue;

        if (0xffffff0000000fff & *(std::uint64_t*)(Buffer + 0xA0))
            continue;

        Result = *(std::uint64_t*)(Buffer + 0xA0);

        break;
    }

    return Result;
}

bool KMU::CR3::Write(const std::uint64_t& Address, void* Buffer, const std::size_t& Size)
{
    if (!Address || !Buffer || !Size)
    {
        return false;
    }

    std::size_t Offset = 0x00;
    std::size_t Value = Size;

    while (Value)
    {
        const std::uint64_t Physical = this->VirtualToPhysical(this->Address, Offset + Address);

        if (!Physical)
        {
            return false;
        }

        const std::uint64_t Bytes = min(PAGE_SIZE - (Physical & 0xFFF), Value);

        if (!KMU::Device->Write(Physical, (void*)((std::uint64_t)(Buffer)+Offset), Bytes))
        {
            return false;
        }

        Offset += Bytes;
        Value -= Bytes;
    }

    return true;
}

bool KMU::CR3::Read(const std::uint64_t& Address, void* Buffer, const std::size_t& Size)
{
    if (!Address || !Buffer || !Size)
    {
        return false;
    }

    std::size_t Offset = 0x00;
    std::size_t Value = Size;

    while (Value)
    {
        const std::uint64_t Physical = this->VirtualToPhysical(this->Address, Offset + Address);

        if (!Physical)
        {
            return false;
        }

        const std::uint64_t Bytes = min(PAGE_SIZE - (Physical & 0xFFF), Value);

        if (!KMU::Device->Read(Physical, (void*)((std::uint64_t)(Buffer)+Offset), Bytes))
        {
            return false;
        }

        Offset += Bytes;
        Value -= Bytes;
    }

    return true;
}