#include "vigil/core/logger.hpp"
#include <cassert>
#include <iostream>

void run_logger_tests() {
    std::cout << "  - Running logger tests...\n";
    auto& logger = vigil::core::Logger::instance();

    logger.init(vigil::core::LogLevel::Debug, "");
    assert(logger.get_level() == vigil::core::LogLevel::Debug);

    logger.set_level(vigil::core::LogLevel::Error);
    assert(logger.get_level() == vigil::core::LogLevel::Error);

    // Test logging calls do not throw
    logger.error("Test error log message: {}", 42);
    logger.warn("Test warning log message");
    logger.info("Test info log message");
    logger.debug("Test debug log message");

    logger.set_level(vigil::core::LogLevel::Info);
}
