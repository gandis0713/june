#include "vulkan_ahardwarebuffer_resource.h"

#include "june/memory/ahardwarebuffer_memory.h"
#include "june/native/shared_memory.h"
#include "vulkan_api.h"
#include "vulkan_api_context.h"
#include "vulkan_fence.h"

#include <spdlog/spdlog.h>

namespace june
{

VulkanAHardwareBufferResource* VulkanAHardwareBufferResource::create(VulkanApiContext* context, JuneResourceDescriptor const* descriptor)
{
    auto apiResource = new VulkanAHardwareBufferResource(context, descriptor);
    apiResource->initialize();

    return apiResource;
}

VulkanAHardwareBufferResource::VulkanAHardwareBufferResource(VulkanApiContext* context, JuneResourceDescriptor const* descriptor)
    : VulkanResource(context, descriptor)
{
}

void VulkanAHardwareBufferResource::beginAccess(JuneResourceBeginAccessDescriptor const* descriptor)
{
    m_accessMutex.lock();

    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        current = current->next;
    }

    m_fence->begin();
}

void VulkanAHardwareBufferResource::endAccess(JuneResourceEndAccessDescriptor const* descriptor)
{
    m_fence->end();

    signal();
}

void* VulkanAHardwareBufferResource::getResource(JuneGetResourceDescriptor const* descriptor)
{
    return static_cast<void*>(m_image);
}

int32_t VulkanAHardwareBufferResource::initialize()
{
    createResource();

    auto vulkanApiContext = reinterpret_cast<VulkanApiContext*>(m_context);
    JuneFenceDescriptor fenceDescriptor{};
    m_fence = VulkanFence::create(vulkanApiContext, &fenceDescriptor);

    return 0;
}

bool VulkanAHardwareBufferResource::createResource()
{
    const JuneChainedStruct* current = m_descriptor.nextInChain;
    while (current)
    {
        switch (current->sType)
        {
        case JuneSType_VkImageResourceDescriptor: {
            auto sharedMemory = reinterpret_cast<SharedMemory*>(m_descriptor.sharedMemory);
            auto ahbMemory = static_cast<AHardwareBufferMemory*>(sharedMemory->getRawMemory());

            auto vulkanApiContext = reinterpret_cast<VulkanApiContext*>(m_context);
            const auto& vkAPI = vulkanApiContext->vkAPI;

            VkDevice device = vulkanApiContext->getVkDevice();

            VkExternalMemoryImageCreateInfo externalMemoryImageCreateInfo{};
            externalMemoryImageCreateInfo.sType = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO;
            externalMemoryImageCreateInfo.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_ANDROID_HARDWARE_BUFFER_BIT_ANDROID;

            const auto* imageDesc = reinterpret_cast<const JuneResourceVkImageDescriptor*>(current);
            VkImageCreateInfo* imageInfo = reinterpret_cast<VkImageCreateInfo*>(imageDesc->vkImageCreateInfo);
            imageInfo->pNext = &externalMemoryImageCreateInfo;

            VkResult result = vkAPI.CreateImage(device, imageInfo, nullptr, &m_image);
            if (result != VK_SUCCESS)
            {
                spdlog::error("Failed to create Vulkan image: {}", static_cast<uint32_t>(result));
                return false;
            }

            VkAndroidHardwareBufferFormatPropertiesANDROID formatProps = {};
            formatProps.sType = VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_FORMAT_PROPERTIES_ANDROID;
            formatProps.pNext = nullptr;

            VkAndroidHardwareBufferPropertiesANDROID bufferProps = {};
            bufferProps.sType = VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_PROPERTIES_ANDROID;
            bufferProps.pNext = &formatProps;

            AHardwareBuffer* hardwareBuffer = ahbMemory->getAHardwareBuffer();

            result = vkAPI.GetAndroidHardwareBufferPropertiesANDROID(device, hardwareBuffer, &bufferProps);
            if (result != VK_SUCCESS)
            {
                spdlog::error("Failed to get AHardwareBuffer properties: {}", static_cast<uint32_t>(result));
                return false;
            }

            spdlog::trace("AHardwareBuffer format properties: format = {}, externalFormat = {}, formatFeatures = {}, "
                          "suggestedYcbcrModel = {}, suggestedYcbcrRange = {}, "
                          "suggestedXChromaOffset = {}, suggestedYChromaOffset = {}",
                          static_cast<uint32_t>(formatProps.format), static_cast<uint64_t>(formatProps.externalFormat),
                          static_cast<uint32_t>(formatProps.formatFeatures),
                          static_cast<uint32_t>(formatProps.suggestedYcbcrModel), static_cast<uint32_t>(formatProps.suggestedYcbcrRange),
                          static_cast<uint32_t>(formatProps.suggestedXChromaOffset), static_cast<uint32_t>(formatProps.suggestedYChromaOffset));

            spdlog::trace("AHardwareBuffer properties: allocationSize = {}, memoryTypeBits = {}",
                          bufferProps.allocationSize, bufferProps.memoryTypeBits);

            auto info = vulkanApiContext->getPhysicalDeviceInfo();

            uint32_t memoryTypeIndex = std::numeric_limits<uint32_t>::max();
            for (uint32_t i = 0; i < info.memoryTypes.size(); i++)
            {
                if ((bufferProps.memoryTypeBits & (1 << i)) &&
                    (info.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
                {
                    memoryTypeIndex = i;
                    break;
                }
            }

            if (memoryTypeIndex == std::numeric_limits<uint32_t>::max())
            {
                spdlog::error("Failed to find suitable memory type for AHardwareBuffer");
                return false;
            }
            spdlog::trace("Memory type index: {}", memoryTypeIndex);

            VkMemoryDedicatedAllocateInfo dedicatedAllocateInfo;
            dedicatedAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO;
            dedicatedAllocateInfo.pNext = nullptr;
            dedicatedAllocateInfo.buffer = VK_NULL_HANDLE;
            dedicatedAllocateInfo.image = m_image;

            VkImportAndroidHardwareBufferInfoANDROID importAHBInfo = {};
            importAHBInfo.sType = VK_STRUCTURE_TYPE_IMPORT_ANDROID_HARDWARE_BUFFER_INFO_ANDROID;
            importAHBInfo.buffer = hardwareBuffer;
            importAHBInfo.pNext = &dedicatedAllocateInfo;

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = bufferProps.allocationSize;
            allocInfo.memoryTypeIndex = static_cast<uint32_t>(memoryTypeIndex);
            allocInfo.pNext = &importAHBInfo;

            VkDeviceMemory deviceMemory;
            result = vkAPI.AllocateMemory(device, &allocInfo, nullptr, &deviceMemory);
            if (result != VK_SUCCESS)
            {
                spdlog::error("Failed to allocate Vulkan memory: {}", static_cast<uint32_t>(result));
                return false;
            }

            result = vkAPI.BindImageMemory(device, m_image, deviceMemory, 0);
            if (result != VK_SUCCESS)
            {
                spdlog::error("Failed to bind Vulkan image memory: {}", static_cast<uint32_t>(result));
                return false;
            }

            VkMemoryRequirements memRequirements{};
            vkAPI.GetImageMemoryRequirements(device, m_image, &memRequirements);
            spdlog::trace("Image memory requirements: size = {}, alignment = {}, memoryTypeBits = {}",
                          static_cast<uint64_t>(memRequirements.size), static_cast<uint64_t>(memRequirements.alignment), memRequirements.memoryTypeBits);

            if (bufferProps.allocationSize != memRequirements.size)
            {
                spdlog::error("AHardwareBuffer allocation size is difference with image size, [ahardwarebuffer size: {}], [image size: {}]",
                              bufferProps.allocationSize, memRequirements.size);
            }

            return m_image;
        }
        break;
        default:
            spdlog::error("Unsupported resource type: {}", static_cast<uint32_t>(current->sType));
            return false;
        }

        current = current->next;
    }

    return m_image != VK_NULL_HANDLE;
}

} // namespace june