#include <Windows.h>
#include "dllname.h"

#define PROXY extern "C" __declspec(naked) void
#ifdef _MSC_VER
// https://docs.microsoft.com/en-us/cpp/assembler/inline/using-operators-in-asm-blocks?view=vs-2019
#define FUNC(name, index) PROXY hook_##name() { __asm jmp (DLLFUNCS[index * TYPE DllFunc].func) }
#else // _MSC_VER
#define FUNC(name, index) PROXY hook_##name() { __asm("jmp *%0" : : "m"(DLLFUNCS[index].func)); }
#endif // _MSC_VER

struct DllFunc {
    FARPROC func;
    const char *name;
    int ord;
    
    void locate(HMODULE hDll) {
        if(name)
            func = GetProcAddress(hDll, name);
        else
            func = GetProcAddress(hDll, MAKEINTRESOURCEA(ord));
    }
};

#include "func_defs.inc"

void InstallHook();

static HMODULE hDll = nullptr;

static inline bool LoadOrigDll()
{
    wchar_t dllPath[MAX_PATH];
    if (GetSystemDirectoryW(dllPath, MAX_PATH)) {
        wcscat_s(dllPath, L"\\" DLLNAME ".dll");
        hDll = LoadLibraryW(dllPath);
        if (hDll) {
            for(size_t i = 0; i < NUM_FUNCS; i++)
                DLLFUNCS[i].locate(hDll);
            return true;
        }
    }
    return false;
}

bool WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
    if(fdwReason == DLL_PROCESS_ATTACH) {
        InstallHook();
        return LoadOrigDll();
    } else if(fdwReason == DLL_PROCESS_DETACH) {
        FreeLibrary(hDll);
        hDll = nullptr;
        for(size_t i = 0; i < NUM_FUNCS; i++)
            DLLFUNCS[i].func = nullptr;
    }
    return true;
}
