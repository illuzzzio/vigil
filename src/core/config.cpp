#include "vigil/core/config.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace vigil::core {

bool Config::validate(std::string& error_message) const {
    if (target_fps <= 0 || target_fps > 1000) {
        error_message = "Target FPS must be between 1 and 1000.";
        return false;
    }
    if (logging_level != "debug" && logging_level != "info" && 
        logging_level != "warning" && logging_level != "error") {
        error_message = "Invalid logging level. Must be 'debug', 'info', 'warning', or 'error'.";
        return false;
    }
    if (frame_capture_path.empty()) {
        error_message = "Frame capture path cannot be empty.";
        return false;
    }
    return true;
}

bool Config::load_from_file(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Simple key=value parser
        auto delimiter_pos = line.find('=');
        if (delimiter_pos == std::string::npos) continue;

        std::string key = line.substr(0, delimiter_pos);
        std::string value = line.substr(delimiter_pos + 1);

        // Trim whitespace (basic)
        key.erase(key.find_last_not_of(" \n\r\t") + 1);
        key.erase(0, key.find_first_not_of(" \n\r\t"));
        value.erase(value.find_last_not_of(" \n\r\t") + 1);
        value.erase(0, value.find_first_not_of(" \n\r\t"));

        if (key == "frame_interpolation_enabled") {
            frame_interpolation_enabled = (value == "true" || value == "1");
        } else if (key == "graphics_api") {
            if (value == "Vulkan") graphics_api = GraphicsAPI::Vulkan;
            else if (value == "DirectX12") graphics_api = GraphicsAPI::DirectX12;
            else graphics_api = GraphicsAPI::Auto;
        } else if (key == "diagnostic_mode") {
            diagnostic_mode = (value == "true" || value == "1");
        } else if (key == "logging_level") {
            logging_level = value;
        } else if (key == "frame_capture_enabled") {
            frame_capture_enabled = (value == "true" || value == "1");
        } else if (key == "frame_capture_path") {
            frame_capture_path = value;
        } else if (key == "performance_monitoring_enabled") {
            performance_monitoring_enabled = (value == "true" || value == "1");
        } else if (key == "target_fps") {
            try {
                target_fps = std::stoi(value);
            } catch (...) {}
        }
    }
    return true;
}

bool Config::save_to_file(const std::string& filepath) const {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return false;
    }

    file << "frame_interpolation_enabled=" << (frame_interpolation_enabled ? "true" : "false") << "\n";
    file << "graphics_api=";
    switch (graphics_api) {
        case GraphicsAPI::Vulkan: file << "Vulkan"; break;
        case GraphicsAPI::DirectX12: file << "DirectX12"; break;
        case GraphicsAPI::Auto: file << "Auto"; break;
    }
    file << "\n";
    file << "diagnostic_mode=" << (diagnostic_mode ? "true" : "false") << "\n";
    file << "logging_level=" << logging_level << "\n";
    file << "frame_capture_enabled=" << (frame_capture_enabled ? "true" : "false") << "\n";
    file << "frame_capture_path=" << frame_capture_path << "\n";
    file << "performance_monitoring_enabled=" << (performance_monitoring_enabled ? "true" : "false") << "\n";
    file << "target_fps=" << target_fps << "\n";

    return true;
}

} // namespace vigil::core
