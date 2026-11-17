#include "vigil/graphics/vulkan_context.hpp"
#ifndef VIGIL_CORE_LOGGER_HPP
#define VIGIL_CORE_LOGGER_HPP
#include "vigil/core/logger.hpp"
#endif

#if VIGIL_HAS_VULKAN
#include <vulkan/vulkan.h>
#endif

namespace vigil::graphics {

VulkanContext::VulkanContext() = default;

VulkanContext::~VulkanContext() {
#if VIGIL_HAS_VULKAN
    if (vk_instance_) {
        vkDestroyInstance(static_cast<VkInstance>(vk_instance_), nullptr);
        vk_instance_ = nullptr;
    }
#endif
}

bool VulkanContext::is_loader_available() {
#if VIGIL_HAS_VULKAN
    // If compiled with Vulkan and headers are present, we test loading or basic check
    uint32_t instance_version = 0;
    if (vkEnumerateInstanceVersion) {
        VkResult res = vkEnumerateInstanceVersion(&instance_version);
        return (res == VK_SUCCESS);
    }
    return true;
#else
    return false;
#endif
}

bool VulkanContext::initialize() {
#if VIGIL_HAS_VULKAN
    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Vigil Game-Frame Interpolation Engine";
    app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    app_info.pEngineName = "Vigil";
    app_info.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    app_info.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    // Enable validation layers in debug if available
#ifdef _DEBUG
    const char* validation_layer = "VK_LAYER_KHRONOS_validation";
    uint32_t layer_count = 0;
    if (vkEnumerateInstanceLayerProperties(&layer_count, nullptr) == VK_SUCCESS && layer_count > 0) {
        std::vector<VkLayerProperties> available_layers(layer_count);
        if (vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data()) == VK_SUCCESS) {
            for (const auto& layer : available_layers) {
                if (strcmp(validation_layer, layer.layerName) == 0) {
                    create_info.enabledLayerCount = 1;
                    create_info.ppEnabledLayerNames = &validation_layer;
                    break;
                }
            }
        }
    }
#endif

    VkInstance instance = nullptr;
    VkResult result = vkCreateInstance(&create_info, nullptr, &instance);
    if (result != VK_SUCCESS) {
        return false;
    }

    vk_instance_ = static_cast<void*>(instance);
    initialized_ = true;

    // Enumerate physical devices
    uint32_t device_count = 0;
    if (vkEnumeratePhysicalDevices(instance, &device_count, nullptr) == VK_SUCCESS && device_count > 0) {
        std::vector<VkPhysicalDevice> vk_devices(device_count);
        if (vkEnumeratePhysicalDevices(instance, &device_count, vk_devices.data()) == VK_SUCCESS) {
            for (auto pd : vk_devices) {
                VkPhysicalDeviceProperties props;
                vkGetPhysicalDeviceProperties(pd, &props);

                VulkanDeviceInfo info;
                info.device_name = props.deviceName;
                
                uint32_t driver_ver = props.driverVersion;
                info.driver_version = std::to_string(driver_ver);
                info.api_version = std::to_string(VK_VERSION_MAJOR(props.apiVersion)) + "." +
                                   std::to_string(VK_VERSION_MINOR(props.apiVersion));
                
                switch (props.deviceType) {
                    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: info.device_type = "Discrete GPU"; break;
                    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: info.device_type = "Integrated GPU"; break;
                    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: info.device_type = "Virtual GPU"; break;
                    case VK_PHYSICAL_DEVICE_TYPE_CPU: info.device_type = "CPU"; break;
                    default: info.device_type = "Other"; break;
                }

                devices_.push_back(info);
            }
        }
    }

    return true;
#else
    return false;
#endif
}

std::vector<VulkanDeviceInfo> VulkanContext::get_physical_devices() const {
    return devices_;
}

const VulkanDeviceInfo* VulkanContext::get_primary_device() const {
    if (devices_.empty()) return nullptr;
    return &devices_[0];
}

} // namespace vigil::graphics
