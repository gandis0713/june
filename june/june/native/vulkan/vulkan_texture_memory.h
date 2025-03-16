#pragma once

#include "june/june.h"
#include "june/native/texture_memory.h"

namespace june
{

class VulkanContext;
class VulkanTextureMemory : public TextureMemory
{
public:
    static VulkanTextureMemory* create(VulkanContext* context, JuneTextureMemoryDescriptor const* descriptor);

public:
    VulkanTextureMemory() = delete;
    VulkanTextureMemory(VulkanContext* context, JuneTextureMemoryDescriptor const* descriptor);
    ~VulkanTextureMemory() override = default;

    VulkanTextureMemory(const VulkanTextureMemory&) = delete;
    VulkanTextureMemory& operator=(const VulkanTextureMemory&) = delete;

public: // June API
    Texture* createTexture(JuneTextureDescriptor const* descriptor) override;

public:
    Context* getContext() const override;

private:
    VulkanContext* m_context;
    const JuneTextureMemoryDescriptor m_descriptor;
};

} // namespace june