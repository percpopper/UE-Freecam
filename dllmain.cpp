#include "UnrealEngine.h"
#include "PatternScan.h"
#include "MinHook/MinHook.h"

// https://docs.unrealengine.com/4.27/en-US/API/Runtime/Engine/Engine/ULocalPlayer/GetViewPoint/
void(*OGetViewPoint)(ULocalPlayer*, FMinimalViewInfo*, EStereoscopicPass) = nullptr;
void GetViewPoint(ULocalPlayer* thisLocalPlayer, FMinimalViewInfo* OutViewInfo, EStereoscopicPass StereoPass)
{
    // Call original function setting OutViewInfo (Before modifying it)
    OGetViewPoint(thisLocalPlayer, OutViewInfo, StereoPass);

    static bool GetCameraLocation = true;
    static FVector NewLocation;

    if (GetCameraLocation) {
        // Set our new location to the original spot once
        NewLocation = OutViewInfo->Location;
        GetCameraLocation = false;
    }

    if (GetAsyncKeyState('W') & 1) NewLocation.X += 75.f;
    else if (GetAsyncKeyState('S') & 1) NewLocation.X -= 75.f;
    else if (GetAsyncKeyState('A') & 1) NewLocation.Y += 75.f;
    else if (GetAsyncKeyState('D') & 1) NewLocation.Y -= 75.f;
    else if (GetAsyncKeyState(VK_SPACE) & 1) NewLocation.Z += 75.f;
    else if (GetAsyncKeyState(VK_SHIFT) & 1) NewLocation.Z -= 75.f;

    // Modify OutViewInfo
    OutViewInfo->Location = NewLocation;
    OutViewInfo->FOV = 150.f;

    // Start cheat from here if you want
    APlayerController* PlayerController = thisLocalPlayer->PlayerController;
    if (!PlayerController) return;

    APawn* AcknowledgedPawn = PlayerController->AcknowledgedPawn;
    if (!AcknowledgedPawn) return;

}   

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call != DLL_PROCESS_ATTACH) return FALSE;

    std::uintptr_t GetViewPointAddress = PatternScan((uintptr_t)GetModuleHandleA(NULL), "4C 8B DC 49 89 5B 20 55 56 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 41 0F");
    if (!GetViewPointAddress) return FALSE;

    if (MH_Initialize() != MH_STATUS::MH_OK) return FALSE;

    if (MH_CreateHook(reinterpret_cast<LPVOID>(GetViewPointAddress), GetViewPoint, reinterpret_cast<LPVOID*>(&OGetViewPoint)) != MH_STATUS::MH_OK) return FALSE;

    if (MH_EnableHook(reinterpret_cast<LPVOID>(GetViewPointAddress)) != MH_STATUS::MH_OK) return FALSE;

    return TRUE;
}