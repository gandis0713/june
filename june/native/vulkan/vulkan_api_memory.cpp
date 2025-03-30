#include "vulkan_api_memory.h"

#include "vulkan_api_context.h"

namespace june
{

VulkanApiMemory::VulkanApiMemory(VulkanApiContext* context, JuneApiMemoryDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

} // namespace june