#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#include "Source/Include.h"
#include "Source/Dependencies/Window/Callback/Menu.hpp"
#include "Source/Dependencies/Window/Callback/Draw.hpp"
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

void APIENTRY EntryPoint(PVOID Address)
{
    AUTH_ENTRYPOINT

    // Initialize Authentication
    Storage->Info = Auth->Initialize(Address);

    // Store Global Variables
    Storage->ScreenSize = Math::Vector2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

#ifdef DEVELOPMENT
    const bool Status = Driver->Load
    (
        "EscapeFromTarkov.exe",
        0x6271E720,
    
        "WinIo",
        0x80102040,
        0x80102044
    );
    
    if (!Status)
    {
        std::printf("Failed\n");
        return;
    }
#endif

    // Get Module(s)
    std::vector<std::string> ModuleVec = {};
    ModuleVec.push_back(_("UnityPlayer.dll"));
    ModuleVec.push_back(_("mono-2.0-bdwgc.dll"));
    std::vector<std::uint64_t> Modules = Auth->SetupBypass(ModuleVec);
    ModuleVec.clear();

#ifdef DEVELOPMENT
    Storage->UnityPlayer = Driver->GetModuleBase("UnityPlayer.dll");
#endif

#ifndef DEVELOPMENT
    Driver->Safe = true;

    Storage->UnityPlayer = Modules[0];
    Storage->MonoDll = Modules[1];
#endif

    if (!Exception::Setup())
    {
        Auth->Log(_("Failed to setup exceptions"));
        return;
    }

    if (!Config::Setup())
    {
        Auth->Log(_("Failed to setup configs"));
        return;
    }

    srand(time(nullptr));

    EFT::Run();

    // Setup Overlay
    Window->Setup();
    Window->Render(FindWindowA(_("UnityWndClass"), _("EscapeFromTarkov")), Menu::Callback, Draw::Callback);
}

void FakeEntryPoint()
{
#ifdef DEVELOPMENT
    EntryPoint(0);
#endif
}

int main()
{
    EntryPoint(0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread((LPSECURITY_ATTRIBUTES)NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(FakeEntryPoint), (LPVOID)NULL, NULL, (LPDWORD)NULL);
    return TRUE;
}

ForceFunctionToBeLinked ForceLink({ EntryPoint });

#ifdef DEVELOPMENT
Authentication* Auth = new Authentication();
//DriverCommunication* Driver = new DriverCommunication();
#endif