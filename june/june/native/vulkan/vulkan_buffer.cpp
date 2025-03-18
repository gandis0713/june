#include "vulkan_buffer.h"

#include "vulkan_buffer.h"
#include "vulkan_context.h"
#include "vulkan_shared_memory.h"

namespace june
{

VulkanBuffer* VulkanBuffer::create(VulkanContext* context, JuneBufferDescriptor const* descriptor)
{
    return new VulkanBuffer(context, descriptor);
}

VulkanBuffer::VulkanBuffer(VulkanContext* context, JuneBufferDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

Fence* VulkanBuffer::createFence(JuneFenceDescriptor const* descriptor)
{
    return nullptr;
}

Context* VulkanBuffer::getContext() const
{
    return m_context;
}

} // namespace june