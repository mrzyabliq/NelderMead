// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

#ifdef _WIN32
    BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
        switch (ul_reason_for_call) {
            case DLL_PROCESS_ATTACH:
            case DLL_THREAD_ATTACH:
            case DLL_THREAD_DETACH:
            case DLL_PROCESS_DETACH:
                break;
        }
        return TRUE;
    }
#else
    // Linux-версия инициализации (вызывается при загрузке .so)
    __attribute__((constructor)) void lib_init() {
        // Ваша логика инициализации (если нужна)
    }

    __attribute__((destructor)) void lib_cleanup() {
        // Ваша логика деинициализации (если нужна)
    }
#endif

