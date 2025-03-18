#include "vulkan_fence.h"

#include "vulkan_buffer.h"
#include "vulkan_texture.h"

namespace june
{

VulkanFence* VulkanFence::create(VulkanBuffer* buffer, JuneFenceDescriptor const* descriptor)
{
    return new VulkanFence(buffer, descriptor);
}

VulkanFence* VulkanFence::create(VulkanTexture* texture, JuneFenceDescriptor const* descriptor)
{
    return new VulkanFence(texture, descriptor);
}

VulkanFence::VulkanFence(VulkanBuffer* buffer, JuneFenceDescriptor const* descriptor)
    : m_buffer(buffer)
    , m_descriptor(*descriptor)
{
}

VulkanFence::VulkanFence(VulkanTexture* texture, JuneFenceDescriptor const* descriptor)
    : m_texture(texture)
    , m_descriptor(*descriptor)
{
}

Buffer* VulkanFence::getBuffer() const
{
    return m_buffer;
}

Texture* VulkanFence::getTexture() const
{
    return m_texture;
}

} // namespace june