#include "vulkan_context.h"

#include "june/common/assert.h"
#include "june/native/shared_memory.h"
#if __has_include("vulkan_ahardwarebuffer_vkimage.h")
#include "vulkan_ahardwarebuffer_vkimage.h"
#endif

#include "vulkan_fence.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

// memory
const char kExtensionNameKhrExternalMemory[] = "VK_KHR_external_memory";

namespace june
{

ApiContext* VulkanContext::create(Instance* instance, JuneApiContextDescriptor const* descriptor)
{
    return new VulkanContext(instance, descriptor);
}

VulkanContext::VulkanContext(Instance* instance, JuneApiContextDescriptor const* descriptor)
    : ApiContext(instance, descriptor)
{
    // It assumes that the descriptor is valid and has been validated before this point.
    auto vulkanDescriptor = reinterpret_cast<JuneVulkanContextDescriptor const*>(descriptor->nextInChain);

    m_vkInstance = static_cast<VkInstance>(vulkanDescriptor->vkInstance);
    m_vkPhysicalDevice = static_cast<VkPhysicalDevice>(vulkanDescriptor->vkPhysicalDevice);
    m_vkDevice = static_cast<VkDevice>(vulkanDescriptor->vkDevice);

#if defined(__ANDROID__) || defined(ANDROID)
    const char vulkanLibraryName[] = "libvulkan.so";
#elif defined(__linux__)
    const char vulkanLibraryName[] = "libvulkan.so.1";
#elif defined(__APPLE__)
    const char vulkanLibraryName[] = "libvulkan.dylib";
#elif defined(WIN32)
    const char vulkanLibraryName[] = "vulkan-1.dll";
#endif

    if (!m_vulkanLib.open(vulkanLibraryName))
    {
        throw std::runtime_error(fmt::format("Failed to open vulkan library: {}", vulkanLibraryName));
    }

    if (!vkAPI.loadInstanceProcs(&m_vulkanLib))
    {
        throw std::runtime_error(fmt::format("Failed to load instance prosc in vulkan library: {}", vulkanLibraryName));
    }

    gatherInstanceInfo();

    spdlog::info("Required Vulkan API Version in Application: {}.{}.{}",
                 VK_API_VERSION_MAJOR(m_instanceInfo.apiVersion),
                 VK_API_VERSION_MINOR(m_instanceInfo.apiVersion),
                 VK_API_VERSION_PATCH(m_instanceInfo.apiVersion));

    const std::vector<const char*> requiredInstanceLayers = getRequiredInstanceLayers();
    if (!checkInstanceLayerSupport(requiredInstanceLayers))
    {
        throw std::runtime_error("instance layers requested, but not available!");
    }

    const std::vector<const char*> requiredInstanceExtensions = getRequiredInstanceExtensions();
    if (!checkInstanceExtensionSupport(requiredInstanceExtensions))
    {
        throw std::runtime_error("instance extensions requested, but not available!");
    }

    const VulkanInstanceKnobs& instanceKnobs = static_cast<const VulkanInstanceKnobs&>(m_instanceInfo);
    if (!vkAPI.loadInstanceProcs(m_vkInstance, instanceKnobs))
    {
        throw std::runtime_error(fmt::format("Failed to load instance prosc."));
    }

    gatherPhysicalDeviceInfo();

    if (!vkAPI.loadDeviceProcs(m_vkDevice, static_cast<const VulkanDeviceKnobs&>(m_physicalDeviceInfo)))
    {
        throw std::runtime_error(fmt::format("Failed to load device procs."));
    }
}

VulkanContext::~VulkanContext()
{
    // do not destroy instance for vulkan.
    // do not destroy device for vulkan.

    if (m_vulkanLib.isValid())
    {
        m_vulkanLib.close();
    }
}

void VulkanContext::createResource(JuneResourceDescriptor const* descriptor)
{
    auto sharedMemory = reinterpret_cast<SharedMemory*>(descriptor->sharedMemory);
    auto rawMemory = sharedMemory->getRawMemory();

    switch (rawMemory->getType())
    {
#if defined(__ANDROID__) || defined(ANDROID)
    case RawMemoryType::kAHardwareBuffer: {
        VulkanAHardwareBufferVkImage::create(this, descriptor);
        return;
    }
#endif
    default:
        break;
    }
}

Fence* VulkanContext::createFence(JuneFenceDescriptor const* descriptor)
{
    return VulkanFence::create(this, descriptor);
}

void VulkanContext::beginMemoryAccess(JuneApiContextBeginMemoryAccessDescriptor const* descriptor)
{
}

void VulkanContext::endMemoryAccess(JuneApiContextEndMemoryAccessDescriptor const* descriptor)
{
}

JuneApiType VulkanContext::getApiType() const
{
    return JuneApiType_Vulkan;
}

VkInstance VulkanContext::getVkInstance() const
{
    return m_vkInstance;
}

VkPhysicalDevice VulkanContext::getVkPhysicalDevice() const
{
    return m_vkPhysicalDevice;
}

VkDevice VulkanContext::getVkDevice() const
{
    return m_vkDevice;
}

const VulkanInstanceInfo& VulkanContext::getInstanceInfo() const
{
    return m_instanceInfo;
}

const VulkanPhysicalDeviceInfo& VulkanContext::getPhysicalDeviceInfo() const
{
    return m_physicalDeviceInfo;
}

void VulkanContext::gatherInstanceInfo()
{
    uint32_t apiVersion = 0u;
    if (vkAPI.EnumerateInstanceVersion != nullptr)
    {
        vkAPI.EnumerateInstanceVersion(&apiVersion);
    }

    spdlog::info("Vulkan Loader API Version: {}.{}.{}",
                 VK_API_VERSION_MAJOR(apiVersion),
                 VK_API_VERSION_MINOR(apiVersion),
                 VK_API_VERSION_PATCH(apiVersion));

    // Gather instance layer properties.
    {
        uint32_t instanceLayerCount = 0;
        VkResult result = vkAPI.EnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
        if (result != VK_SUCCESS && result != VK_INCOMPLETE)
        {
            spdlog::error("Failed to get instance layer properties count. {}", static_cast<int32_t>(result));
            return;
        }

        m_instanceInfo.layerProperties.resize(instanceLayerCount);
        result = vkAPI.EnumerateInstanceLayerProperties(&instanceLayerCount, m_instanceInfo.layerProperties.data());
        if (result != VK_SUCCESS)
        {
            spdlog::error("Failed to enumerate instance layer properties. {}", static_cast<int32_t>(result));
            return;
        }

        for (const auto& layerProperty : m_instanceInfo.layerProperties)
        {
            // TODO: set instance knobs for layer
            spdlog::info("Instance Layer Name: {}", layerProperty.layerName);
        }
    }

    // Gather instance extension properties.
    {
        uint32_t instanceExtensionCount = 0;
        VkResult result = vkAPI.EnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr);
        if (result != VK_SUCCESS && result != VK_INCOMPLETE)
        {
            spdlog::error("Failed to get instance extension properties count.");
            return;
        }

        m_instanceInfo.extensionProperties.resize(instanceExtensionCount);
        result = vkAPI.EnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, m_instanceInfo.extensionProperties.data());
        if (result != VK_SUCCESS)
        {
            spdlog::error("Failed to enumerate instance extension properties.");
            return;
        }

        for (const auto& extensionProperty : m_instanceInfo.extensionProperties)
        {
            // TODO: set instance knobs for extension
            spdlog::info("Instance Extension Name: {}, SpecVersion: {}", extensionProperty.extensionName, extensionProperty.specVersion);

#if VK_HEADER_VERSION >= 216
            if (strncmp(extensionProperty.extensionName, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME, VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                m_instanceInfo.portabilityEnum = true;
            }
#endif
        }
    }
}

void VulkanContext::gatherPhysicalDeviceInfo()
{
    // Gather physical device properties and features.
    vkAPI.GetPhysicalDeviceProperties(m_vkPhysicalDevice, &m_physicalDeviceInfo.physicalDeviceProperties);

    spdlog::info("Vulkan Device API Version: {}.{}.{}",
                 VK_API_VERSION_MAJOR(m_physicalDeviceInfo.physicalDeviceProperties.apiVersion),
                 VK_API_VERSION_MINOR(m_physicalDeviceInfo.physicalDeviceProperties.apiVersion),
                 VK_API_VERSION_PATCH(m_physicalDeviceInfo.physicalDeviceProperties.apiVersion));

    // currently only support AAA.BBB.CCC.
    // TODO: support AAA.BBB.CCC.DDD for NVIDIA and AAA.BBB for intel windows
    spdlog::info("Vulkan Device Instance Version: {}.{}.{}",
                 VK_API_VERSION_MAJOR(m_physicalDeviceInfo.physicalDeviceProperties.driverVersion),
                 VK_API_VERSION_MINOR(m_physicalDeviceInfo.physicalDeviceProperties.driverVersion),
                 VK_API_VERSION_PATCH(m_physicalDeviceInfo.physicalDeviceProperties.driverVersion));

    spdlog::info("Physical Device Id: {}", static_cast<uint32_t>(m_physicalDeviceInfo.physicalDeviceProperties.deviceID));
    spdlog::info("Physical Device Name: {}", m_physicalDeviceInfo.physicalDeviceProperties.deviceName);
    spdlog::info("Physical Device Type: {}", static_cast<uint32_t>(m_physicalDeviceInfo.physicalDeviceProperties.deviceType));
    spdlog::info("Physical Device Vender ID: {}", static_cast<uint32_t>(m_physicalDeviceInfo.physicalDeviceProperties.vendorID));

    vkAPI.GetPhysicalDeviceFeatures(m_vkPhysicalDevice, &m_physicalDeviceInfo.physicalDeviceFeatures);

    // Gather device memory properties.
    {
        VkPhysicalDeviceMemoryProperties memoryProperties{};
        vkAPI.GetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &memoryProperties);

        m_physicalDeviceInfo.memoryTypes.assign(memoryProperties.memoryTypes, memoryProperties.memoryTypes + memoryProperties.memoryTypeCount);
        m_physicalDeviceInfo.memoryHeaps.assign(memoryProperties.memoryHeaps, memoryProperties.memoryHeaps + memoryProperties.memoryHeapCount);

        for (const auto& memoryType : m_physicalDeviceInfo.memoryTypes)
        {
            spdlog::info("Heap index: {}, property flags: {}", memoryType.heapIndex, static_cast<uint32_t>(memoryType.propertyFlags));
        }

        for (const auto& memoryHeap : m_physicalDeviceInfo.memoryHeaps)
        {
            spdlog::info("Heap size: {}, flags: {}", memoryHeap.size, static_cast<uint32_t>(memoryHeap.flags));
        }
    }

    // Gather queue Family Properties.
    {
        uint32_t queueFamilyCount = 0;
        vkAPI.GetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueFamilyCount, nullptr);

        m_physicalDeviceInfo.queueFamilyProperties.resize(queueFamilyCount);
        vkAPI.GetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueFamilyCount, m_physicalDeviceInfo.queueFamilyProperties.data());

        for (const auto& queueFamilyProperty : m_physicalDeviceInfo.queueFamilyProperties)
        {
            spdlog::info("queue flags: {}, queue count: {}", static_cast<uint32_t>(queueFamilyProperty.queueFlags), queueFamilyProperty.queueCount);
        }
    }

    // Gather device layer properties.
    {
        uint32_t deviceLayerCount = 0;
        VkResult result = vkAPI.EnumerateDeviceLayerProperties(m_vkPhysicalDevice, &deviceLayerCount, nullptr);
        if (result != VK_SUCCESS && result != VK_INCOMPLETE)
        {
            throw std::runtime_error(fmt::format("Failure EnumerateDeviceLayerProperties to get count. Error: {}", static_cast<int32_t>(result)));
        }

        m_physicalDeviceInfo.layerProperties.resize(deviceLayerCount);
        result = vkAPI.EnumerateDeviceLayerProperties(m_vkPhysicalDevice, &deviceLayerCount, m_physicalDeviceInfo.layerProperties.data());
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error(fmt::format("Failure EnumerateDeviceLayerProperties. Error: {}", static_cast<int32_t>(result)));
        }

        for (const auto& layerProperty : m_physicalDeviceInfo.layerProperties)
        {
            spdlog::info("Device Layer Name: {}", layerProperty.layerName);
        }
    }

    // Gather device extension properties.
    {
        uint32_t deviceExtensionCount = 0;
        VkResult result = vkAPI.EnumerateDeviceExtensionProperties(m_vkPhysicalDevice, nullptr, &deviceExtensionCount, nullptr);
        if (result != VK_SUCCESS && result != VK_INCOMPLETE)
        {
            throw std::runtime_error(fmt::format("Failure EnumerateDeviceExtensionProperties to get count. Error: {}", static_cast<int32_t>(result)));
        }

        m_physicalDeviceInfo.extensionProperties.resize(deviceExtensionCount);
        result = vkAPI.EnumerateDeviceExtensionProperties(m_vkPhysicalDevice, nullptr, &deviceExtensionCount, m_physicalDeviceInfo.extensionProperties.data());
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error(fmt::format("Failure EnumerateDeviceExtensionProperties. Error: {}", static_cast<int32_t>(result)));
        }

        for (const auto& extensionProperty : m_physicalDeviceInfo.extensionProperties)
        {
            spdlog::info("Device Extention Name: {}", extensionProperty.extensionName);

            // TODO: define "VK_KHR_portability_subset"
            if (strncmp(extensionProperty.extensionName, "VK_KHR_portability_subset", VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                m_physicalDeviceInfo.portabilitySubset = true;
            }
        }
    }
}

bool VulkanContext::checkInstanceExtensionSupport(const std::vector<const char*> requiredInstanceExtensions)
{
    for (const auto& requiredInstanceExtension : requiredInstanceExtensions)
    {
        bool extensionFound = false;
        for (const auto& availableInstanceExtension : m_instanceInfo.extensionProperties)
        {
            if (strcmp(requiredInstanceExtension, availableInstanceExtension.extensionName) == 0)
            {
                extensionFound = true;
                break;
            }
        }

        if (!extensionFound)
        {
            spdlog::error("Instance extension is not found. {}", requiredInstanceExtension);
            return false;
        }
    }

    return true;
}

const std::vector<const char*> VulkanContext::getRequiredInstanceExtensions()
{
    std::vector<const char*> requiredInstanceExtensions{};

#if VK_HEADER_VERSION >= 216
    if (m_instanceInfo.portabilityEnum)
    {
        requiredInstanceExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    }
#endif

    spdlog::info("Required Instance extensions :");
    for (const auto& extension : requiredInstanceExtensions)
    {
        spdlog::info("{}{}", '\t', extension);
    }

    return requiredInstanceExtensions;
}

bool VulkanContext::checkInstanceLayerSupport(const std::vector<const char*> requiredInstanceLayers)
{
    for (const auto& requiredInstanceLayer : requiredInstanceLayers)
    {
        bool layerFound = false;
        for (const auto& availableInstanceLayer : m_instanceInfo.layerProperties)
        {
            if (strcmp(requiredInstanceLayer, availableInstanceLayer.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

const std::vector<const char*> VulkanContext::getRequiredInstanceLayers()
{
    std::vector<const char*> requiredInstanceLayers{};

    spdlog::info("Required Instance layers :");
    for (const auto& layer : requiredInstanceLayers)
    {
        spdlog::info("{}{}", '\t', layer);
    }

    return requiredInstanceLayers;
}

int findMemoryTypeIndex(const VulkanPhysicalDeviceInfo& info, VkMemoryPropertyFlags flags)
{
    int memoryTypeIndex = -1;
    for (int i = 0; i < info.memoryTypes.size(); ++i)
    {
        const auto& memoryType = info.memoryTypes[i];
        if ((memoryType.propertyFlags & flags) == flags)
        {
            memoryTypeIndex = i;
            break;
        }
    }

    return memoryTypeIndex;
}

} // namespace june
