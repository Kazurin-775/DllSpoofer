#ifdef _MSC_VER
#include <shellscalingapi.h>
#else // _MSC_VER

#include <Windows.h>

// MinGW does not provide shellscalingapi.h, so we manually
// put the definitions we need here.

typedef enum PROCESS_DPI_AWARENESS {
    PROCESS_DPI_UNAWARE = 0,
    PROCESS_SYSTEM_DPI_AWARE = 1,
    PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

STDAPI SetProcessDpiAwareness(
    _In_ PROCESS_DPI_AWARENESS value);

#endif // _MSC_VER

void InstallHook()
{
    // put the code you want to run when the DLL loads here...
    SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
}
