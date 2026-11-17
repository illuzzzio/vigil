#include "vigil/platform/windows_platform.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sysinfoapi.h>

namespace vigil::platform {

std::string WindowsPlatform::get_windows_version_string() {
    // Using RtlGetVersion via ntdll or standard GetVersionEx wrapper / system info
    // For simplicity and safety without crashing, we query OS version via kernel32 / NT API or fallback
    typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
    HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
    if (hMod) {
        RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
        if (fxPtr) {
            RTL_OSVERSIONINFOW rovi = { 0 };
            rovi.dwOSVersionInfoSize = sizeof(rovi);
            if (fxPtr(&rovi) == 0) {
                return "Windows " + std::to_string(rovi.dwMajorVersion) + "." + 
                       std::to_string(rovi.dwMinorVersion) + " (Build " + 
                       std::to_string(rovi.dwBuildNumber) + ")";
            }
        }
    }
    return "Windows (Version Unknown)";
}

std::string WindowsPlatform::get_cpu_architecture() {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    switch (si.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64: return "x86_64 (AMD64)";
        case PROCESSOR_ARCHITECTURE_ARM64: return "ARM64";
        case PROCESSOR_ARCHITECTURE_INTEL: return "x86 (Intel)";
        default: return "Unknown Architecture";
    }
}

unsigned int WindowsPlatform::get_logical_processor_count() {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return static_cast<unsigned int>(si.dwNumberOfProcessors);
}

unsigned long long WindowsPlatform::get_total_physical_memory_mb() {
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status)) {
        return static_cast<unsigned long long>(status.ullTotalPhys / (1024 * 1024));
    }
    return 0;
}

unsigned long long WindowsPlatform::get_available_physical_memory_mb() {
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status)) {
        return static_cast<unsigned long long>(status.ullAvailPhys / (1024 * 1024));
    }
    return 0;
}

} // namespace vigil::platform
