cmake_minimum_required(VERSION 3.25)
project(Vigil VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable testing
enable_testing()

# Options
option(VIGIL_BUILD_TESTS "Build Vigil unit tests" ON)
option(VIGIL_ENABLE_VULKAN "Enable Vulkan support if SDK is found" ON)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /wx /permissive-)
else()
    add_compile_options(-Wall -Wextra -Werror -Wno-unused-parameter -Wno-unknown-pragmas)
endif()

# Find Vulkan package QUIETLY as required
find_package(Vulkan QUIET)

if(Vulkan_FOUND AND VIGIL_ENABLE_VULKAN)
    message(STATUS "Vulkan SDK found: ${Vulkan_VERSION}")
    set(VIGIL_HAS_VULKAN TRUE)
else()
    message(STATUS "Vulkan SDK not found or disabled. Building core without Vulkan GPU context support.")
    set(VIGIL_HAS_VULKAN FALSE)
endif()

# Core Library
add_library(vigil_core
    src/core/application.cpp
    src/core/logger.cpp
    src/core/config.cpp
    src/platform/windows_platform.cpp
    src/diagnostics/system_info.cpp
)

if(VIGIL_HAS_VULKAN)
    target_sources(vigil_core PRIVATE src/graphics/vulkan_context.cpp)
    target_link_libraries(vigil_core PUBLIC Vulkan::Vulkan)
    target_compile_definitions(vigil_core PUBLIC VIGIL_HAS_VULKAN=1)
else()
    target_compile_definitions(vigil_core PUBLIC VIGIL_HAS_VULKAN=0)
endif()

target_include_directories(vigil_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(vigil_core PRIVATE)

# Executable
add_executable(vigil
    src/main.cpp
)

target_link_libraries(vigil PRIVATE vigil_core)

# Tests
if(VIGIL_BUILD_TESTS)
    add_executable(vigil_tests
        tests/test_main.cpp
        tests/test_config.cpp
        tests/test_logger.cpp
        tests/test_cli.cpp
        tests/test_application.cpp
        tests/test_vulkan.cpp
    )
    target_link_libraries(vigil_tests PRIVATE vigil_core)
    
    include(CTest)
    add_test(NAME VigilUnitTests COMMAND vigil_tests)
endif()

# Installation rules
include(GNUInstallDirs)
install(TARGETS vigil vigil_core
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
