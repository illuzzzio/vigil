#ifndef VIGIL_DIAGNOSTICS_SYSTEM_INFO_HPP
#define VIGIL_DIAGNOSTICS_SYSTEM_INFO_HPP

#include <string>

namespace vigil::diagnostics {

struct SystemReport {
    std::string os_version;
    std::string cpu_architecture;
    unsigned int logical_processors{0};
    unsigned long long total_memory_mb{0};
    unsigned long long available_memory_mb{0};
    bool vulkan_available{false};
    std::string vulkan_device_name;
    std::string vulkan_driver_version;
};

class SystemInfo {
public:
    static SystemReport gather_report();
    static void print_report(const SystemReport& report);
};

} // namespace vigil::diagnostics

#endif // VIGIL_DIAGNOSTICS_SYSTEM_INFO_HPP
