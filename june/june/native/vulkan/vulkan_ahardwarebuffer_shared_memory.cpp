#include "vulkan_ahardwarebuffer_shared_memory.h"

#include "vulkan_api_context.h"

namespace june
{

VulkanAHardwareBufferSharedMemory* VulkanAHardwareBufferSharedMemory::create(VulkanApiContext* context, JuneSharedMemoryDescriptor const* descriptor)
{
    return new VulkanAHardwareBufferSharedMemory(context, descriptor);
}

VulkanAHardwareBufferSharedMemory* VulkanAHardwareBufferSharedMemory::create(VulkanApiContext* context, JuneSharedMemoryDescriptor const* descriptor, JuneSharedMemoryAHardwareBufferDescriptor const* ahbDescriptor)
{
    return new VulkanAHardwareBufferSharedMemory(context, descriptor, ahbDescriptor);
}

VulkanAHardwareBufferSharedMemory::VulkanAHardwareBufferSharedMemory(VulkanApiContext* context, JuneSharedMemoryDescriptor const* descriptor)
    : SharedMemory(context, descriptor)
{
    AHardwareBuffer_Desc desc = {
        .width = m_descriptor.width,
        .height = m_descriptor.height,
        .layers = m_descriptor.layers,
        .format = AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM,
        .usage = AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE | AHARDWAREBUFFER_USAGE_GPU_COLOR_OUTPUT,
    };

    AHardwareBuffer_allocate(&desc, &m_ahbDescriptor.aHardwareBuffer);
}

VulkanAHardwareBufferSharedMemory::VulkanAHardwareBufferSharedMemory(VulkanApiContext* context, JuneSharedMemoryDescriptor const* descriptor, JuneSharedMemoryAHardwareBufferDescriptor const* ahbDescriptor)
    : SharedMemory(context, descriptor)
    , m_ahbDescriptor(*ahbDescriptor)
{
    m_aHardwareBuffer = m_ahbDescriptor.aHardwareBuffer;
}

void VulkanAHardwareBufferSharedMemory::beginAccess(JuneBeginAccessDescriptor const* descriptor)
{
    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
        case JuneSType_BeginAccessVulkanTexture: {
            JuneBeginAccessVulkanTextureDescriptor const* vulkanTextureDescriptor = reinterpret_cast<JuneBeginAccessVulkanTextureDescriptor const*>(current);
        }
        break;
        case JuneSType_BeginAccessVulkanBuffer: {
            JuneBeginAccessVulkanBufferDescriptor const* vulkanBufferDescriptor = reinterpret_cast<JuneBeginAccessVulkanBufferDescriptor const*>(current);
        }
        break;
        default:
            return VulkanSharedMemory::create(this, descriptor);
        }

        current = current->next;
    }

    return nullptr;
}

void VulkanAHardwareBufferSharedMemory::endAccess(JuneEndAccessDescriptor const* descriptor)
{
}

AHardwareBuffer* VulkanAHardwareBufferSharedMemory::getAHardwareBuffer() const
{
    return m_ahbDescriptor.aHardwareBuffer;
}

} // namespace june