#include "vulkan_buffer_memory.h"

#include "vulkan_buffer.h"
#include "vulkan_context.h"

namespace june
{

VulkanBufferMemory* VulkanBufferMemory::create(VulkanContext* context, JuneBufferMemoryDescriptor const* descriptor)
{
    return new VulkanBufferMemory(context, descriptor);
}

VulkanBufferMemory::VulkanBufferMemory(VulkanContext* context, JuneBufferMemoryDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

Buffer* VulkanBufferMemory::createBuffer(JuneBufferDescriptor const* descriptor)
{
    return VulkanBuffer::create(this, descriptor);
}

Context* VulkanBufferMemory::getContext() const
{
    return m_context;
}

} // namespace june