#include "vulkan_api_memory.h"

#include "vulkan_api_context.h"

namespace june
{

VulkanApiMemory* VulkanApiMemory::create(VulkanApiContext* context, JuneApiMemoryDescriptor const* descriptor)
{
    return new VulkanApiMemory(context, descriptor);
}

VulkanApiMemory::VulkanApiMemory(VulkanApiContext* context, JuneApiMemoryDescriptor const* descriptor)
    : ApiMemory(context, descriptor)
{
}

void VulkanApiMemory::beginAccess(JuneApiMemoryBeginAccessDescriptor const* descriptor)
{
    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        current = current->next;
    }
}

void VulkanApiMemory::endAccess(JuneApiMemoryEndAccessDescriptor const* descriptor)
{
}

void* VulkanApiMemory::createResource(JuneResourceDescriptor const* descriptor)
{
    return nullptr;
}

} // namespace june