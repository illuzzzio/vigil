#include "vigil/core/application.hpp"
#include "vigil/core/logger.hpp"
#include "vigil/diagnostics/system_info.hpp"
#include "vigil/graphics/vulkan_context.hpp"
#include <iostream>
#include <algorithm>

namespace vigil::core {

Application::Application(int argc, char* argv[]) : argc_(argc) {
    for (int i = 0; i < argc; ++i) {
        args_.emplace_back(argv[i]);
    }
}

void Application::print_help() const {
    std::cout << "Vigil — Windows Game-Frame Interpolation & Performance Engineering v" << VERSION << "\n\n"
              << "Usage: vigil [options]\n\n"
              << "Options:\n"
              << "  --help           Display this help message and exit\n"
              << "  --version        Display version information and exit\n"
              << "  --diagnostics    Run comprehensive system and hardware diagnostics\n"
              << "  --vulkan-info    Report Vulkan SDK availability and GPU capabilities\n";
}

void Application::print_version() const {
    std::cout << "Vigil version " << VERSION << "\n"
              << "C++20 / Vulkan Experimental Foundation\n";
}

void Application::run_diagnostics_mode() const {
    auto report = diagnostics::SystemInfo::gather_report();
    diagnostics::SystemInfo::print_report(report);
}

void Application::run_vulkan_info_mode() const {
    std::cout << "Vigil Vulkan Capability Report:\n";
    bool loader = graphics::VulkanContext::is_loader_available();
    std::cout << "  - Vulkan Loader Available: " << (loader ? "YES" : "NO") << "\n";

    graphics::VulkanContext ctx;
    if (ctx.initialize()) {
        std::cout << "  - Vulkan Instance Initialized: YES\n";
        auto devices = ctx.get_physical_devices();
        std::cout << "  - Physical Devices Found: " << devices.size() << "\n";
        for (size_t i = 0; i < devices.size(); ++i) {
            std::cout << "    [" << i << "] " << devices[i].device_name 
                      << " (Driver: " << devices[i].driver_version << ")\n";
        }
    } else {
        std::cout << "  - Vulkan Instance Initialized: NO (SDK or runtime not active)\n";
    }
}

int Application::run() {
    // Initialize logger with default info level and log file output
    Logger::instance().init(LogLevel::Info, "logs/vigil.log");

    // Check configuration
    std::string err_msg;
    if (!config_.validate(err_msg)) {
        Logger::instance().warn("Configuration validation warning: {}", err_msg);
    }

    // Handle CLI arguments
    if (argc_ > 1) {
        std::string cmd = args_[1];
        if (cmd == "--help") {
            print_help();
            return 0;
        } else if (cmd == "--version") {
            print_version();
            return 0;
        } else if (cmd == "--diagnostics") {
            run_diagnostics_mode();
            return 0;
        } else if (cmd == "--vulkan-info") {
            run_vulkan_info_mode();
            return 0;
        } else {
            std::cerr << "Error: Unknown option '" << cmd << "'.\n"
                      << "Use 'vigil --help' for usage information.\n";
            Logger::instance().error("Unknown command line option: {}", cmd);
            return 1;
        }
    }

    // Default execution flow
    Logger::instance().info("Starting Vigil v{} Foundation...", VERSION);
    
    auto report = diagnostics::SystemInfo::gather_report();
    Logger::instance().info("System: {} | CPU Arch: {} | Cores: {} | RAM: {} MB Total / {} MB Available",
        report.os_version, report.cpu_architecture, report.logical_processors,
        report.total_memory_mb, report.available_memory_mb);

    Logger::instance().info("Vulkan Status: {}", report.vulkan_available ? "Available (" + report.vulkan_device_name + ")" : "Not Available");
    Logger::instance().info("Vigil core initialization successful. Exiting cleanly.");

    return 0;
}

} // namespace vigil::core
