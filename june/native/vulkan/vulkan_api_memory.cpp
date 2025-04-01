#include "vulkan_api_memory.h"

#include "vulkan_api_context.h"

namespace june
{

VulkanApiMemory::VulkanApiMemory(VulkanApiContext* context, JuneApiMemoryDescriptor const* descriptor)
    : ApiMemory(context, descriptor)
{
}

} // namespace june