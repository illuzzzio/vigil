#include "vigil/diagnostics/system_info.hpp"
#include "vigil/platform/windows_platform.hpp"
#include "vigil/graphics/vulkan_context.hpp"
#include <iostream>

namespace vigil::diagnostics {

SystemReport SystemInfo::gather_report() {
    SystemReport report;
    report.os_version = platform::WindowsPlatform::get_windows_version_string();
    report.cpu_architecture = platform::WindowsPlatform::get_cpu_architecture();
    report.logical_processors = platform::WindowsPlatform::get_logical_processor_count();
    report.total_memory_mb = platform::WindowsPlatform::get_total_physical_memory_mb();
    report.available_memory_mb = platform::WindowsPlatform::get_available_physical_memory_mb();

    graphics::VulkanContext vk_ctx;
    report.vulkan_available = vk_ctx.initialize();
    if (report.vulkan_available) {
        auto primary = vk_ctx.get_primary_device();
        if (primary) {
            report.vulkan_device_name = primary->device_name;
            report.vulkan_driver_version = primary->driver_version;
        } else {
            report.vulkan_device_name = "Unknown Vulkan Device";
        }
    } else {
        report.vulkan_device_name = "N/A (Vulkan Unavailable)";
    }

    return report;
}

void SystemInfo::print_report(const SystemReport& report) {
    std::cout << "========================================\n"
              << "       VIGIL SYSTEM DIAGNOSTICS         \n"
              << "========================================\n"
              << "Operating System:    " << report.os_version << "\n"
              << "CPU Architecture:    " << report.cpu_architecture << "\n"
              << "Logical Processors:  " << report.logical_processors << "\n"
              << "Total Physical RAM:  " << report.total_memory_mb << " MB\n"
              << "Available RAM:       " << report.available_memory_mb << " MB\n"
              << "Vulkan Status:       " << (report.vulkan_available ? "Available" : "Unavailable") << "\n"
              << "Primary GPU:         " << report.vulkan_device_name << "\n"
              << "Driver Version:      " << report.vulkan_driver_version << "\n"
              << "========================================\n";
}

} // namespace vigil::diagnostics
