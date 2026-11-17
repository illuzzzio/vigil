#ifndef VIGIL_GRAPHICS_VULKAN_CONTEXT_HPP
#define VIGIL_GRAPHICS_VULKAN_CONTEXT_HPP

#include <string>
#include <vector>

namespace vigil::graphics {

struct VulkanDeviceInfo {
    std::string device_name;
    std::string driver_version;
    std::string api_version;
    std::string device_type;
};

class VulkanContext {
public:
    VulkanContext();
    ~VulkanContext();

    bool initialize();
    bool is_initialized() const { return initialized_; }
    
    std::vector<VulkanDeviceInfo> get_physical_devices() const;
    const VulkanDeviceInfo* get_primary_device() const;

    static bool is_loader_available();

private:
    bool initialized_{false};
    void* vk_instance_{nullptr}; // Opaque VkInstance handle
    std::vector<VulkanDeviceInfo> devices_;
};

} // namespace vigil::graphics

#endif // VIGIL_GRAPHICS_VULKAN_CONTEXT_HPP
