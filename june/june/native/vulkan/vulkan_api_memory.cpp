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

void VulkanApiMemory::beginAccess(JuneBeginAccessDescriptor const* descriptor)
{
    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        current = current->next;
    }
}

void VulkanApiMemory::endAccess(JuneEndAccessDescriptor const* descriptor)
{
}

} // namespace june