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
}

void VulkanSharedMemory::endAccess(JuneEndAccessDescriptor const* descriptor)
{
}

} // namespace june