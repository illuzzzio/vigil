#include "vigil/graphics/vulkan_context.hpp"
#include <cassert>
#include <iostream>

void run_vulkan_tests() {
    std::cout << "  - Running Vulkan availability handling tests...\n";

    bool loader_available = vigil::graphics::VulkanContext::is_loader_available();
    std::cout << "    [Test] Vulkan Loader Available: " << (loader_available ? "YES" : "NO") << "\n";

    vigil::graphics::VulkanContext ctx;
    bool init_success = ctx.initialize();
    if (init_success) {
        std::cout << "    [Test] Vulkan successfully initialized in test suite.\n";
        assert(ctx.is_initialized() == true);
        auto devices = ctx.get_physical_devices();
        std::cout << "    [Test] Physical devices count: " << devices.size() << "\n";
    } else {
        std::cout << "    [Test] Vulkan initialization skipped or unavailable (graceful handling verified).\n";
        assert(ctx.is_initialized() == false);
    }
}
