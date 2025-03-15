#include "vulkan_context.h"

#include "june/common/assert.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

// surface
const char kExtensionNameKhrSurface[] = "VK_KHR_surface";
const char kExtensionNameMvkMacosSurface[] = "VK_MVK_macos_surface";
const char kExtensionNameExtMetalSurface[] = "VK_EXT_metal_surface";
const char kExtensionNameKhrWin32Surface[] = "VK_KHR_win32_surface";
const char kExtensionNameKhrAndroidSurface[] = "VK_KHR_android_surface";
const char kExtensionNameKhrXcbSurface[] = "VK_KHR_xcb_surface";
// const char kExtensionNameKhrWaylandSurface[] = "VK_KHR_wayland_surface";
// const char kExtensionNameKhrXlibSurface[] = "VK_KHR_xlib_surface";

// swapchain
const char kExtensionNameKhrSwapchain[] = "VK_KHR_swapchain";

namespace june
{

Context* VulkanContext::create(Instance* instance, JuneVulkanApiContextDescriptor const* descriptor)
{
    return new VulkanContext(instance, descriptor);
}

VulkanContext::VulkanContext(Instance* instance, JuneVulkanApiContextDescriptor const* descriptor)
    : m_instance(instance)
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

Instance* VulkanContext::getInstance() const
{
    return m_instance;
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

            if (strncmp(extensionProperty.extensionName, kExtensionNameKhrSurface, VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                m_info.surface = true;
            }

            if (strncmp(extensionProperty.extensionName, kExtensionNameKhrAndroidSurface, VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                m_info.androidSurface = true;
            }
            if (strncmp(extensionProperty.extensionName, kExtensionNameExtMetalSurface, VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                m_info.metalSurface = true;
            }
            if (strncmp(extensionProperty.extensionName, kExtensionNameMvkMacosSurface, VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                m_info.macosSurface = true;
            }
            if (strncmp(extensionProperty.extensionName, kExtensionNameKhrWin32Surface, VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                m_info.win32Surface = true;
            }
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

    requiredInstanceExtensions.push_back(kExtensionNameKhrSurface);

#if defined(__ANDROID__) || defined(ANDROID)
    requiredInstanceExtensions.push_back(kExtensionNameKhrAndroidSurface);
#elif defined(__linux__)
    requiredInstanceExtensions.push_back(kExtensionNameKhrXcbSurface);
#elif defined(_WIN32)
    requiredInstanceExtensions.push_back(kExtensionNameKhrWin32Surface);
#elif defined(__APPLE__)
#if defined(VK_USE_PLATFORM_MACOS_MVK)
    requiredInstanceExtensions.push_back(kExtensionNameMvkMacosSurface);
#elif defined(VK_USE_PLATFORM_METAL_EXT)
    requiredInstanceExtensions.push_back(kExtensionNameExtMetalSurface);
#endif
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
