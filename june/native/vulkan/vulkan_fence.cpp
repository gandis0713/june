#include "vulkan_fence.h"

#include "vulkan_api_context.h"

namespace june
{

VulkanFence* VulkanFence::create(VulkanApiContext* context, JuneFenceDescriptor const* descriptor)
{
    return new VulkanFence(context, descriptor);
}

VulkanFence::VulkanFence(VulkanApiContext* context, JuneFenceDescriptor const* descriptor)
    : Fence(context, descriptor)
{
}

void VulkanFence::begin()
{
}

void VulkanFence::end()
{
}

void VulkanFence::wait()
{
}

} // namespace june