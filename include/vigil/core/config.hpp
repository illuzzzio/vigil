#ifndef VIGIL_CORE_CONFIG_HPP
#define VIGIL_CORE_CONFIG_HPP

#include <string>
#include <optional>

namespace vigil::core {

enum class GraphicsAPI {
    Vulkan,
    DirectX12,
    Auto
};

struct Config {
    bool frame_interpolation_enabled{true};
    GraphicsAPI graphics_api{GraphicsAPI::Vulkan};
    bool diagnostic_mode{false};
    std::string logging_level{"info"};
    bool frame_capture_enabled{false};
    std::string frame_capture_path{"logs/captures/"};
    bool performance_monitoring_enabled{true};
    int target_fps{60};

    bool validate(std::string& error_message) const;
    bool load_from_file(const std::string& filepath);
    bool save_to_file(const std::string& filepath) const;
};

} // namespace vigil::core

#endif // VIGIL_CORE_CONFIG_HPP
