#include "vulkan_shared_memory.h"

#include "vulkan_buffer.h"
#include "vulkan_context.h"
#include "vulkan_texture.h"

namespace june
{

VulkanSharedMemory* VulkanSharedMemory::create(VulkanContext* context, JuneSharedMemoryDescriptor const* descriptor)
{
    return new VulkanSharedMemory(context, descriptor);
}

VulkanSharedMemory::VulkanSharedMemory(VulkanContext* context, JuneSharedMemoryDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

Buffer* VulkanSharedMemory::createBuffer(JuneBufferDescriptor const* descriptor)
{
    return VulkanBuffer::create(this, descriptor);
}

Texture* VulkanSharedMemory::createTexture(JuneTextureDescriptor const* descriptor)
{
    return VulkanTexture::create(this, descriptor);
}

void VulkanSharedMemory::beginAccess(JuneBeginAccessDescriptor const* descriptor)
{
}

void VulkanSharedMemory::endAccess(JuneEndAccessDescriptor const* descriptor)
{
}

Context* VulkanSharedMemory::getContext() const
{
    return m_context;
}

} // namespace june