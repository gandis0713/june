#include "vulkan_buffer.h"
#include "vulkan_context.h"

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

Context* VulkanBuffer::getContext() const
{
    return m_context;
}

} // namespace june