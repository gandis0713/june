#pragma once

#include "june/common/cast.h"
#include "june/common/dylib.h"
#include "june/native/api_context.h"

#include "june/june.h"

#include "vulkan_api.h"

#include <memory>
#include <vector>

namespace june
{

struct VulkanInstanceInfo : VulkanInstanceKnobs
{
    std::vector<VkLayerProperties> layerProperties;
    std::vector<VkExtensionProperties> extensionProperties;
};

struct VulkanPhysicalDeviceInfo : VulkanDeviceKnobs
{
    VkPhysicalDeviceFeatures physicalDeviceFeatures{};
    VkPhysicalDeviceProperties physicalDeviceProperties{};

    std::vector<VkQueueFamilyProperties> queueFamilyProperties{};

    std::vector<VkLayerProperties> layerProperties;
    std::vector<VkExtensionProperties> extensionProperties;

    std::vector<VkMemoryType> memoryTypes;
    std::vector<VkMemoryHeap> memoryHeaps;
};

class Instance;
class VulkanContext : public ApiContext
{
public:
    static ApiContext* create(Instance* instance, JuneApiContextDescriptor const* descriptor);

public:
    VulkanContext() = delete;
    ~VulkanContext() override;

    VulkanContext(const VulkanContext&) = delete;
    VulkanContext& operator=(const VulkanContext&) = delete;

public: // June APi
    void createResource(JuneResourceDescriptor const* descriptor) override;
    Fence* createFence(JuneFenceDescriptor const* descriptor) override;
    void beginMemoryAccess(JuneApiContextBeginMemoryAccessDescriptor const* descriptor) override;
    void endMemoryAccess(JuneApiContextEndMemoryAccessDescriptor const* descriptor) override;

public:
    JuneApiType getApiType() const override;

public: // vulkan
    VkInstance getVkInstance() const;
    VkPhysicalDevice getVkPhysicalDevice() const;
    VkDevice getVkDevice() const;

    const VulkanInstanceInfo& getInstanceInfo() const;
    const VulkanPhysicalDeviceInfo& getPhysicalDeviceInfo() const;

public:
    VulkanAPI vkAPI{};

private:
    void gatherInstanceInfo();
    void gatherPhysicalDeviceInfo();
    bool checkInstanceExtensionSupport(const std::vector<const char*> requiredInstanceExtensions);
    const std::vector<const char*> getRequiredInstanceExtensions();
    bool checkInstanceLayerSupport(const std::vector<const char*> requiredInstanceLayers);
    const std::vector<const char*> getRequiredInstanceLayers();

private:
    VkInstance m_vkInstance = VK_NULL_HANDLE;
    VkPhysicalDevice m_vkPhysicalDevice = VK_NULL_HANDLE;
    VkDevice m_vkDevice = VK_NULL_HANDLE;

    DyLib m_vulkanLib{};
    VulkanInstanceInfo m_instanceInfo{};
    VulkanPhysicalDeviceInfo m_physicalDeviceInfo{};

private:
    VulkanContext(Instance* instance, JuneApiContextDescriptor const* descriptor);
};

int findMemoryTypeIndex(const VulkanPhysicalDeviceInfo& info, VkMemoryPropertyFlags flags);

} // namespace june
