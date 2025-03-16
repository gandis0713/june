#pragma once

#include "june/common/cast.h"
#include "june/common/dylib.h"
#include "june/native/context.h"

#include "june/june.h"

#include "vulkan_api.h"

#include <memory>
#include <vector>

namespace june
{

struct VulkanInfo : VulkanInstanceKnobs
{
    std::vector<VkLayerProperties> layerProperties;
    std::vector<VkExtensionProperties> extensionProperties;
};

class Instance;
class VulkanContext : public Context
{
public:
    static Context* create(Instance* instance, JuneVulkanApiContextDescriptor const* descriptor);

public:
    VulkanContext() = delete;
    ~VulkanContext() override;

    VulkanContext(const VulkanContext&) = delete;
    VulkanContext& operator=(const VulkanContext&) = delete;

public:
    Instance* getInstance() const override;

public: // vulkan
    VkInstance getVkInstance() const;
    VkPhysicalDevice getVkPhysicalDevice() const;
    VkDevice getVkDevice() const;

    const VulkanInfo& getInfo() const;

public:
    VulkanAPI vkAPI{};

private:
    void gatherInfo();
    bool checkInstanceExtensionSupport(const std::vector<const char*> requiredInstanceExtensions);
    const std::vector<const char*> getRequiredInstanceExtensions();
    bool checkInstanceLayerSupport(const std::vector<const char*> requiredInstanceLayers);
    const std::vector<const char*> getRequiredInstanceLayers();

private:
    Instance* m_instance = nullptr;

private:
    VkInstance m_vkInstance = VK_NULL_HANDLE;
    VkPhysicalDevice m_vkPhysicalDevice = VK_NULL_HANDLE;
    VkDevice m_vkDevice = VK_NULL_HANDLE;

    DyLib m_vulkanLib{};
    VulkanInfo m_info{};

private:
    VulkanContext(Instance* instance, JuneVulkanApiContextDescriptor const* descriptor);
};

} // namespace june
