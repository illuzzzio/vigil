#ifndef VIGIL_PLATFORM_WINDOWS_PLATFORM_HPP
#define VIGIL_PLATFORM_WINDOWS_PLATFORM_HPP

#include <string>

namespace vigil::platform {

class WindowsPlatform {
public:
    static std::string get_windows_version_string();
    static std::string get_cpu_architecture();
    static unsigned int get_logical_processor_count();
    static unsigned long long get_total_physical_memory_mb();
    static unsigned long long get_available_physical_memory_mb();
};

} // namespace vigil::platform

#endif // VIGIL_PLATFORM_WINDOWS_PLATFORM_HPP
