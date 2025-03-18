#include "vulkan_shared_memory.h"

#include "vulkan_context.h"

namespace june
{

VulkanSharedMemory* VulkanSharedMemory::create(VulkanContext* context, JuneSharedMemoryDescriptor const* descriptor)
{
    return new VulkanSharedMemory(context, descriptor);
}

VulkanSharedMemory::VulkanSharedMemory(VulkanContext* context, JuneSharedMemoryDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

void VulkanSharedMemory::beginAccess(JuneBeginAccessDescriptor const* descriptor)
{
}

void VulkanSharedMemory::endAccess(JuneEndAccessDescriptor const* descriptor)
{
}

Context* VulkanSharedMemory::getContext() const
{
    return m_context;
}

} // namespace june