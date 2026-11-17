#include "vigil/core/config.hpp"
#include <cassert>
#include <iostream>

void run_config_tests() {
    std::cout << "  - Running config tests...\n";
    vigil::core::Config config;
    
    // Default values validation
    std::string err;
    assert(config.validate(err) == true);
    assert(config.target_fps == 60);
    assert(config.logging_level == "info");

    // Invalid target FPS
    config.target_fps = -10;
    assert(config.validate(err) == false);
    config.target_fps = 60;

    // Invalid logging level
    config.logging_level = "invalid_level";
    assert(config.validate(err) == false);
    config.logging_level = "info";

    // File save and load roundtrip
    std::string test_file = "test_config.ini";
    assert(config.save_to_file(test_file) == true);

    vigil::core::Config loaded_config;
    assert(loaded_config.load_from_file(test_file) == true);
    assert(loaded_config.target_fps == 60);
    assert(loaded_config.logging_level == "info");

    // Clean up test file
    std::remove(test_file.c_str());
}
