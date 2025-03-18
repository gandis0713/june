#include "vulkan_texture.h"

#include "vulkan_context.h"
#include "vulkan_shared_memory.h"
#include "vulkan_texture.h"

namespace june
{

VulkanTexture* VulkanTexture::create(VulkanSharedMemory* memory, JuneTextureDescriptor const* descriptor)
{
    return new VulkanTexture(memory, descriptor);
}

VulkanTexture::VulkanTexture(VulkanSharedMemory* memory, JuneTextureDescriptor const* descriptor)
    : m_memory(memory)
    , m_descriptor(*descriptor)
{
}

Fence* VulkanTexture::createFence(JuneFenceDescriptor const* descriptor)
{
    return nullptr;
}

Context* VulkanTexture::getContext() const
{
    return m_memory->getContext();
}

SharedMemory* VulkanTexture::getMemory() const
{
    return m_memory;
}

} // namespace june