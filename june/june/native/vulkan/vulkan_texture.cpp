#include "vulkan_texture.h"

#include "vulkan_api_context.h"
#include "vulkan_shared_memory.h"
#include "vulkan_texture.h"

namespace june
{

VulkanTexture* VulkanTexture::create(VulkanApiContext* context, JuneTextureDescriptor const* descriptor)
{
    return new VulkanTexture(context, descriptor);
}

VulkanTexture::VulkanTexture(VulkanApiContext* context, JuneTextureDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

Fence* VulkanTexture::createFence(JuneFenceDescriptor const* descriptor)
{
    return nullptr;
}

void* VulkanTexture::getVkImage() const
{
    return nullptr;
}

ApiContext* VulkanTexture::getApiContext() const
{
    return m_context;
}

} // namespace june