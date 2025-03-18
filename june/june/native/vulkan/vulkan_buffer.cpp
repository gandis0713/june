#include "vulkan_buffer.h"

#include "vulkan_buffer.h"
#include "vulkan_context.h"
#include "vulkan_shared_memory.h"

namespace june
{

VulkanBuffer* VulkanBuffer::create(VulkanSharedMemory* memory, JuneBufferDescriptor const* descriptor)
{
    return new VulkanBuffer(memory, descriptor);
}

VulkanBuffer::VulkanBuffer(VulkanSharedMemory* memory, JuneBufferDescriptor const* descriptor)
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

SharedMemory* VulkanBuffer::getMemory() const
{
    return m_memory;
}

} // namespace june