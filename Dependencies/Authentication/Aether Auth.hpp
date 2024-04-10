#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <list>

#define AUTH_ENTRYPOINT    { volatile std::uint32_t _ = 0xD4333DFA;              }
#define AUTH_THREAD(Magic) { if ((std::uint32_t)Magic != 0xD3333DFA) { return; } }
#define IsAddressValid(Address) (Address > 0x10000 && Address < 0x7FFFFFFFFFFF)

struct WebPlayerInfo
{
    std::string Name;
    float PositionX;
    float PositionZ;
    bool IsTeam;
};

#ifndef DEVELOPMENT

class Authentication
{
private:
public:
    class Info
    {
    private:
    public:
        std::string License;
        std::string Application;
        std::string Cheat;
        int Days;
        int Compensation;
        int SecondsRemaining;
        std::string RedeemedDate;
        std::string EndDate;
        int Status;
        std::vector<DWORD> Offsets;
    };

    Authentication::Info* Initialize(PVOID Address);
    std::vector<std::uint64_t> SetupBypass(std::vector<std::string> Modules);
    void Log(std::string Information);

    bool IsInjected(std::string SteamID);
    void WebRadar(int LocalPositionX, int LocalPositionZ, std::vector<WebPlayerInfo> Players);
    void SMS(std::string Phone, std::string Carrier, std::string Message);
};

class DriverCommunication
{
private:
public:
    std::uint64_t SectionBaseAddress = 0x00;
    bool Safe = false;

    bool Read(const std::uint64_t& Address, PVOID Buffer, SIZE_T Size);
    bool Write(const std::uint64_t& Address, PVOID Buffer, SIZE_T Size);

    std::string ReadString(const std::uint64_t& Address, SIZE_T Size = 128);
    std::string ReadStringIL2CPP(const std::uint64_t& Address);

    bool HideWindow(HWND Window, bool Hidden);
    DWORD ProcessId();

    template<typename Type>
    Type Read(const std::uint64_t& Address)
    {
        Type Buffer;

        if (!this->Read(Address, &Buffer, sizeof(Type)))
        {
            return Type();
        }

        return Buffer;
    }

    template<typename Type>
    void Write(const std::uint64_t& Address, Type Value)
    {
        this->Write(Address, &Value, sizeof(Value));
    }

    template<typename Type>
    Type ReadChain(const std::uint64_t& Address, std::vector<std::uint64_t> Chain)
    {
        std::uint64_t Current = Address;
        for (int i = 0; i < Chain.size() - 1; i++)
        {
            Current = this->Read<std::uint64_t>(Current + Chain[i]);
        }

        return this->Read<Type>(Current + Chain[Chain.size() - 1]);
    }
};

#else

inline DWORD Pid = 0x00;
inline HANDLE Handle = INVALID_HANDLE_VALUE;

class Authentication
{
private:
public:
    class Info
    {
    private:
    public:
        std::string License;
        std::string Application;
        std::string Cheat;
        int Days;
        int Compensation;
        int SecondsRemaining;
        std::string RedeemedDate;
        std::string EndDate;
        int Status;
        std::vector<DWORD> Offsets;
    };

    Authentication::Info* Initialize(PVOID Address)
    {
        Authentication::Info* Test = new Authentication::Info();
        Test->Application = "division";
        Test->Cheat = "eft cheat dev build";
        Test->SecondsRemaining = 1000 * 24 * 60 * 60;
        Test->Days = 9999;
        Test->License = "5I1T-BQCM-AU7I-0MV2";
        return Test;
    }

    DWORD GetProcessID(const char* procName)
    {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

        if (!Process32First(snapshot, &entry) == TRUE)
            return 0;

        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (strcmp(entry.szExeFile, procName) == 0)
                return (DWORD)entry.th32ProcessID;
        }

        return 0;
    }

    std::uint64_t GetModuleBase(std::string Name)
    {
        HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, Pid);
        if (Snapshot == INVALID_HANDLE_VALUE)
        {
            std::cout << "PID -> " << Pid << std::endl;
            return {};
        }

        MODULEENTRY32 ModEntry;
        ModEntry.dwSize = sizeof(MODULEENTRY32);

        if (!Module32First(Snapshot, &ModEntry))
            return {};

        do
        {
            if (strcmp(ModEntry.szModule, Name.c_str()) == 0)
            {
                CloseHandle(Snapshot);
                return (std::uint64_t)ModEntry.modBaseAddr;
            }
        } while (Module32Next(Snapshot, &ModEntry));

        CloseHandle(Snapshot);
        return 0x00;
    }

    std::vector<std::uint64_t> SetupBypass(std::vector<std::string> Modules)
    {
        Pid = GetProcessID("EscapeFromTarkov.exe");
        if (!Pid)
        {
            std::cout << "1\n";
            return {};
        }

        Handle = OpenProcess(PROCESS_ALL_ACCESS, false, Pid);
        if (!Handle)
        {
            return {};
        }

        std::vector<std::uint64_t> ModuleData = {};
        for (std::string Mod : Modules)
        {
            ModuleData.push_back(GetModuleBase(Mod));
        }

        return ModuleData;
    }

    void Log(std::string Information)
    {
        std::cout << Information << std::endl;
    }

    bool IsInjected(std::string SteamID)
    {
        return false;
    }
    void WebRadar(int LocalPositionX, int LocalPositionZ, std::vector<WebPlayerInfo> Players)
    {

    }
    void SMS(std::string Phone, std::string Carrier, std::string Message)
    {

    }
};
class DriverCommunication
{
private:
public:
    std::uint64_t SectionBaseAddress = 0x00;
    bool Safe = false;

    bool Read(const std::uint64_t& Address, PVOID Buffer, SIZE_T Size)
    {
        return ReadProcessMemory(Handle, (LPCVOID)Address, Buffer, Size, nullptr);
    }
    bool Write(const std::uint64_t& Address, PVOID Buffer, SIZE_T Size)
    {
        return WriteProcessMemory(Handle, (LPVOID)Address, Buffer, Size, nullptr);
    }

    std::string ReadString(const std::uint64_t& Address, SIZE_T Size = 128)
    {
        const std::uint64_t NewAddress = this->Read<std::uint64_t>(Address);

        if (!NewAddress)
        {
            return "";
        }

        // Array Of Characters To Read Into
        std::unique_ptr<char[]> Buffer(new char[Size]);

        // Reading The Characters Into The Array
        this->Read(NewAddress, Buffer.get(), Size);

        // Returning Array Of Characters As A String
        return Buffer.get();
    }
    std::string ReadStringIL2CPP(const std::uint64_t& Address)
    {
        if (!IsAddressValid(Address))
            return "";

        std::uint64_t NewAddress = this->Read<std::uint64_t>(Address);
        if (!NewAddress)
            return "";

        // Get String Length
        int Length = this->Read<int>(NewAddress + 0x10) * sizeof(wchar_t);
        if (!Length || Length > 1024)
            return "";

        // Array Of Characters To Read Into
        wchar_t* Buffer = (wchar_t*)malloc(Length);

        // Reading The Characters Into The Buffer
        this->Read(NewAddress + 0x14, Buffer, Length);

        // Convert From Unicode To Ansi
        std::wstring TempString = std::wstring(Buffer, Length / 2);
        std::string String = std::string(TempString.begin(), TempString.end());

        // Free The Buffer
        free(Buffer);

        return String;
    }

    bool HideWindow(HWND Window, bool Hidden)
    {
        return false;
    }
    DWORD ProcessId()
    {
        return Pid;
    }

    template<typename Type>
    Type Read(const std::uint64_t& Address)
    {
        Type Buffer;

        if (!this->Read(Address, &Buffer, sizeof(Type)))
        {
            return Type();
        }

        return Buffer;
    }

    template<typename Type>
    void Write(const std::uint64_t& Address, Type Value)
    {
        this->Write(Address, &Value, sizeof(Value));
    }

    template<typename Type>
    Type ReadChain(const std::uint64_t& Address, std::vector<std::uint64_t> Chain)
    {
        std::uint64_t Current = Address;
        for (int i = 0; i < Chain.size() - 1; i++)
        {
            Current = this->Read<std::uint64_t>(Current + Chain[i]);
        }

        return this->Read<Type>(Current + Chain[Chain.size() - 1]);
    }
};

#endif

extern Authentication* Auth;
#ifndef DEVELOPMENT
extern DriverCommunication* Driver;
#endif
struct ForceFunctionToBeLinked { volatile std::uint32_t _ = 0xD2333DFA; ForceFunctionToBeLinked(const std::vector<PVOID> p) { SetLastError(PtrToInt(true ? &p : 0)); } };