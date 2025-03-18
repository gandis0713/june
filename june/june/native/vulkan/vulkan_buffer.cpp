#include "vulkan_buffer.h"

#include "vulkan_api_context.h"
#include "vulkan_buffer.h"
#include "vulkan_shared_memory.h"

namespace june
{

VulkanBuffer* VulkanBuffer::create(VulkanApiContext* context, JuneBufferDescriptor const* descriptor)
{
    return new VulkanBuffer(context, descriptor);
}

VulkanBuffer::VulkanBuffer(VulkanApiContext* context, JuneBufferDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

Fence* VulkanBuffer::createFence(JuneFenceDescriptor const* descriptor)
{
    return nullptr;
}

void* VulkanBuffer::getVkBuffer() const
{
    return nullptr;
}

ApiContext* VulkanBuffer::getApiContext() const
{
    return m_context;
}

} // namespace june