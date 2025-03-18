#include "vulkan_buffer.h"

#include "vulkan_buffer.h"
#include "vulkan_buffer_memory.h"
#include "vulkan_context.h"

namespace june
{

VulkanBuffer* VulkanBuffer::create(VulkanBufferMemory* memory, JuneBufferDescriptor const* descriptor)
{
    return new VulkanBuffer(memory, descriptor);
}

VulkanBuffer::VulkanBuffer(VulkanBufferMemory* memory, JuneBufferDescriptor const* descriptor)
    : m_memory(memory)
    , m_descriptor(*descriptor)
{
}

Fence* VulkanBuffer::createFence(JuneFenceDescriptor const* descriptor)
{
    return nullptr;
}

Context* VulkanBuffer::getContext() const
{
    return m_memory->getContext();
}

BufferMemory* VulkanBuffer::getMemory() const
{
    return m_memory;
}

} // namespace june