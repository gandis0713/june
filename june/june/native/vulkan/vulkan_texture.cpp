#include "vulkan_texture.h"

#include "vulkan_context.h"
#include "vulkan_texture.h"
#include "vulkan_texture_memory.h"

namespace june
{

VulkanTexture* VulkanTexture::create(VulkanTextureMemory* memory, JuneTextureDescriptor const* descriptor)
{
    return new VulkanTexture(memory, descriptor);
}

VulkanTexture::VulkanTexture(VulkanTextureMemory* memory, JuneTextureDescriptor const* descriptor)
    : m_memory(memory)
    , m_descriptor(*descriptor)
{
}

Context* VulkanTexture::getContext() const
{
    return m_memory->getContext();
}

TextureMemory* VulkanTexture::getMemory() const
{
    return m_memory;
}

} // namespace june