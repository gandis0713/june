#pragma once

#include "june/june.h"
#include "june/native/texture.h"

namespace june
{

class VulkanContext;
class VulkanSharedMemory;
class VulkanTexture : public Texture
{
public:
    static VulkanTexture* create(VulkanSharedMemory* memory, JuneTextureDescriptor const* descriptor);

public:
    VulkanTexture() = delete;
    VulkanTexture(VulkanSharedMemory* memory, JuneTextureDescriptor const* descriptor);
    ~VulkanTexture() override = default;

    VulkanTexture(const VulkanTexture&) = delete;
    VulkanTexture& operator=(const VulkanTexture&) = delete;

public: // June API
    Fence* createFence(JuneFenceDescriptor const* descriptor) override;

public:
    Context* getContext() const override;
    SharedMemory* getMemory() const override;

private:
    VulkanSharedMemory* m_memory;
    const JuneTextureDescriptor m_descriptor;
};

} // namespace june