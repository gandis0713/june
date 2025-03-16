#pragma once

#include "june/june.h"
#include "june/native/texture.h"

namespace june
{

class VulkanContext;
class VulkanTextureMemory;
class VulkanTexture : public Texture
{
public:
    static VulkanTexture* create(VulkanTextureMemory* memory, JuneTextureDescriptor const* descriptor);

public:
    VulkanTexture() = delete;
    VulkanTexture(VulkanTextureMemory* memory, JuneTextureDescriptor const* descriptor);
    ~VulkanTexture() override = default;

    VulkanTexture(const VulkanTexture&) = delete;
    VulkanTexture& operator=(const VulkanTexture&) = delete;

public: // June API
public:
    Context* getContext() const override;
    TextureMemory* getMemory() const override;

private:
    VulkanTextureMemory* m_memory;
    const JuneTextureDescriptor m_descriptor;
};

} // namespace june