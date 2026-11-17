#include <iostream>
#include <cassert>

// Forward declarations of test suites
void run_config_tests();
void run_logger_tests();
void run_cli_tests();
void run_application_tests();
void run_vulkan_tests();

int main() {
    std::cout << "Running Vigil Test Suite...\n";
    
    run_config_tests();
    run_logger_tests();
    run_cli_tests();
    run_application_tests();
    run_vulkan_tests();

    std::cout << "All Vigil tests passed successfully!\n";
    return 0;
}
