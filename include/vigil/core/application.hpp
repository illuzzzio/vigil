#ifndef VIGIL_CORE_APPLICATION_HPP
#define VIGIL_CORE_APPLICATION_HPP

#include "vigil/core/config.hpp"
#include <string>
#include <vector>

namespace vigil::core {

class Application {
public:
    Application(int argc, char* argv[]);
    ~Application() = default;

    int run();

    static constexpr const char* VERSION = "0.1.0";

private:
    void print_help() const;
    void print_version() const;
    void run_diagnostics_mode() const;
    void run_vulkan_info_mode() const;

    int argc_;
    std::vector<std::string> args_;
    Config config_;
};

} // namespace vigil::core

#endif // VIGIL_CORE_APPLICATION_HPP
