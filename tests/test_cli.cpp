#include "vigil/core/application.hpp"
#include <cassert>
#include <iostream>

void run_cli_tests() {
    std::cout << "  - Running CLI argument handling tests...\n";

    // Test help command execution
    char* argv_help[] = { (char*)"vigil", (char*)"--help" };
    vigil::core::Application app_help(2, argv_help);
    assert(app_help.run() == 0);

    // Test version command execution
    char* argv_ver[] = { (char*)"vigil", (char*)"--version" };
    vigil::core::Application app_ver(2, argv_ver);
    assert(app_ver.run() == 0);

    // Test diagnostics command execution
    char* argv_diag[] = { (char*)"vigil", (char*)"--diagnostics" };
    vigil::core::Application app_diag(2, argv_diag);
    assert(app_diag.run() == 0);

    // Test vulkan-info command execution
    char* argv_vk[] = { (char*)"vigil", (char*)"--vulkan-info" };
    vigil::core::Application app_vk(2, argv_vk);
    assert(app_vk.run() == 0);

    // Test unknown command
    char* argv_bad[] = { (char*)"vigil", (char*)"--unknown-flag" };
    vigil::core::Application app_bad(2, argv_bad);
    assert(app_bad.run() == 1);
}
