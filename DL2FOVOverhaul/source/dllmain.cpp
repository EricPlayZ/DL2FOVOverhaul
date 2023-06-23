#include <Windows.h>

static HANDLE hMainThread{};

extern DWORD64 WINAPI MainThread(HMODULE hModule);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD64 ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);
        hMainThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, NULL);
        if (!hMainThread)
            return FALSE;
        break;
    }
    case DLL_PROCESS_DETACH:
        FreeLibraryAndExitThread(hModule, 0);
        if (hMainThread)
            CloseHandle(hMainThread);
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

