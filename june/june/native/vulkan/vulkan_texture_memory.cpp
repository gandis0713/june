#include "vulkan_texture_memory.h"

#include "vulkan_context.h"
#include "vulkan_texture.h"

namespace june
{

VulkanTextureMemory* VulkanTextureMemory::create(VulkanContext* context, JuneTextureMemoryDescriptor const* descriptor)
{
    return new VulkanTextureMemory(context, descriptor);
}

VulkanTextureMemory::VulkanTextureMemory(VulkanContext* context, JuneTextureMemoryDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

Texture* VulkanTextureMemory::createTexture(JuneTextureDescriptor const* descriptor)
{
    return VulkanTexture::create(this, descriptor);
}

void VulkanTextureMemory::beginAccess(JuneBeginTextureAccessDescriptor const* descriptor)
{
}

void VulkanTextureMemory::endAccess(JuneEndTextureAccessDescriptor const* descriptor)
{
}

Context* VulkanTextureMemory::getContext() const
{
    return m_context;
}

} // namespace june