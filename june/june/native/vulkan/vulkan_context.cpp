#include "vulkan_context.h"

#include "june/common/assert.h"
#include "vulkan_buffer_memory.h"
#include "vulkan_texture_memory.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

// memory
const char kExtensionNameKhrExternalMemory[] = "VK_KHR_external_memory";

namespace june
{

Context* VulkanContext::create(Instance* instance, JuneVulkanApiContextDescriptor const* descriptor)
{
    return new VulkanContext(instance, descriptor);
}

VulkanContext::VulkanContext(Instance* instance, JuneVulkanApiContextDescriptor const* descriptor)
    : m_instance(instance)
    , m_vkInstance(static_cast<VkInstance>(descriptor->vkInstance))
    , m_vkPhysicalDevice(static_cast<VkPhysicalDevice>(descriptor->vkPhysicalDevice))
    , m_vkDevice(static_cast<VkDevice>(descriptor->vkDevice))
{
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

    gatherInfo();

    spdlog::info("Required Vulkan API Version in Application: {}.{}.{}",
                 VK_API_VERSION_MAJOR(m_info.apiVersion),
                 VK_API_VERSION_MINOR(m_info.apiVersion),
                 VK_API_VERSION_PATCH(m_info.apiVersion));

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

    const VulkanInstanceKnobs& instanceKnobs = static_cast<const VulkanInstanceKnobs&>(m_info);
    if (!vkAPI.loadInstanceProcs(m_vkInstance, instanceKnobs))
    {
        throw std::runtime_error(fmt::format("Failed to load instance prosc."));
    }
}

VulkanContext::~VulkanContext()
{
    // do not destroy instance for vulkan.
}

BufferMemory* VulkanContext::createBufferMemory(JuneBufferMemoryDescriptor const* descriptor)
{
    return VulkanBufferMemory::create(this, descriptor);
}

TextureMemory* VulkanContext::createTextureMemory(JuneTextureMemoryDescriptor const* descriptor)
{
    return VulkanTextureMemory::create(this, descriptor);
}

Instance* VulkanContext::getInstance() const
{
    return m_instance;
}

JuneApiType VulkanContext::getApiType() const
{
    return JuneApiType::JUNE_VULKAN;
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

const VulkanInfo& VulkanContext::getInfo() const
{
    return m_info;
}

void VulkanContext::gatherInfo()
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

        m_info.layerProperties.resize(instanceLayerCount);
        result = vkAPI.EnumerateInstanceLayerProperties(&instanceLayerCount, m_info.layerProperties.data());
        if (result != VK_SUCCESS)
        {
            spdlog::error("Failed to enumerate instance layer properties. {}", static_cast<int32_t>(result));
            return;
        }

        for (const auto& layerProperty : m_info.layerProperties)
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

        m_info.extensionProperties.resize(instanceExtensionCount);
        result = vkAPI.EnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, m_info.extensionProperties.data());
        if (result != VK_SUCCESS)
        {
            spdlog::error("Failed to enumerate instance extension properties.");
            return;
        }

        for (const auto& extensionProperty : m_info.extensionProperties)
        {
            // TODO: set instance knobs for extension
            spdlog::info("Instance Extension Name: {}, SpecVersion: {}", extensionProperty.extensionName, extensionProperty.specVersion);

#if defined(__ANDROID__) || defined(ANDROID) || defined(__linux__) || defined(WIN32)
            if (strncmp(extensionProperty.extensionName, kExtensionNameKhrExternalMemory, VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                m_info.externalMemory = true;
            }
#endif

#if VK_HEADER_VERSION >= 216
            if (strncmp(extensionProperty.extensionName, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME, VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                m_info.portabilityEnum = true;
            }
#endif
        }
    }
}

bool VulkanContext::checkInstanceExtensionSupport(const std::vector<const char*> requiredInstanceExtensions)
{
    for (const auto& requiredInstanceExtension : requiredInstanceExtensions)
    {
        bool extensionFound = false;
        for (const auto& availableInstanceExtension : m_info.extensionProperties)
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

#if defined(__ANDROID__) || defined(ANDROID) || defined(__linux__) || defined(WIN32)
    requiredInstanceExtensions.push_back(kExtensionNameKhrExternalMemory);
#endif

#if VK_HEADER_VERSION >= 216
    if (m_info.portabilityEnum)
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
        for (const auto& availableInstanceLayer : m_info.layerProperties)
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

} // namespace june
