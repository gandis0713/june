#include "vulkan_shared_memory.h"

#include "vulkan_api_context.h"

namespace june
{

VulkanSharedMemory* VulkanSharedMemory::create(VulkanApiContext* context, JuneSharedMemoryDescriptor const* descriptor)
{
    return new VulkanSharedMemory(context, descriptor);
}

VulkanSharedMemory::VulkanSharedMemory(VulkanApiContext* context, JuneSharedMemoryDescriptor const* descriptor)
    : SharedMemory(context, descriptor)
{
}

void VulkanSharedMemory::beginAccess(JuneBeginAccessDescriptor const* descriptor)
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
            break;
        }

        current = current->next;
    }
}

void VulkanSharedMemory::endAccess(JuneEndAccessDescriptor const* descriptor)
{
}

} // namespace june