#include "vulkan_texture.h"
#include "vulkan_context.h"

namespace june
{

VulkanTexture* VulkanTexture::create(VulkanContext* context, JuneTextureDescriptor const* descriptor)
{
    return new VulkanTexture(context, descriptor);
}

VulkanTexture::VulkanTexture(VulkanContext* context, JuneTextureDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

Context* VulkanTexture::getContext() const
{
    return m_context;
}

} // namespace june