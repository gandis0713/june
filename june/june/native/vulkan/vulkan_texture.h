#pragma once

#include "june/june.h"
#include "june/native/texture.h"

namespace june
{

class VulkanContext;
class VulkanTexture : public Texture
{
public:
    static VulkanTexture* create(VulkanContext* context, JuneTextureDescriptor const* descriptor);

public:
    VulkanTexture() = delete;
    VulkanTexture(VulkanContext* context, JuneTextureDescriptor const* descriptor);
    ~VulkanTexture() override = default;

    VulkanTexture(const VulkanTexture&) = delete;
    VulkanTexture& operator=(const VulkanTexture&) = delete;

public:
    // June API

public:
    Context* getContext() const override;

private:
    VulkanContext* m_context;
    const JuneTextureDescriptor m_descriptor;
};

} // namespace june