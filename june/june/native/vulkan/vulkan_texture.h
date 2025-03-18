#pragma once

#include "june/june.h"
#include "june/native/texture.h"

namespace june
{

class VulkanApiContext;
class VulkanTexture : public Texture
{
public:
    static VulkanTexture* create(VulkanApiContext* context, JuneTextureDescriptor const* descriptor);

public:
    VulkanTexture() = delete;
    VulkanTexture(VulkanApiContext* context, JuneTextureDescriptor const* descriptor);
    ~VulkanTexture() override = default;

    VulkanTexture(const VulkanTexture&) = delete;
    VulkanTexture& operator=(const VulkanTexture&) = delete;

public: // June API
    Fence* createFence(JuneFenceDescriptor const* descriptor) override;
    void* getVkImage() const override;

public:
    ApiContext* getApiContext() const override;

private:
    VulkanApiContext* m_context;
    const JuneTextureDescriptor m_descriptor;
};

} // namespace june