#pragma once

#include "june/june.h"
#include "june/native/fence.h"

namespace june
{

class VulkanBuffer;
class VulkanTexture;
class VulkanFence : public Fence
{
public:
    static VulkanFence* create(VulkanBuffer* buffer, JuneFenceDescriptor const* descriptor);
    static VulkanFence* create(VulkanTexture* texture, JuneFenceDescriptor const* descriptor);

public:
    VulkanFence() = delete;
    VulkanFence(VulkanBuffer* buffer, JuneFenceDescriptor const* descriptor);
    VulkanFence(VulkanTexture* texture, JuneFenceDescriptor const* descriptor);
    ~VulkanFence() override = default;

    VulkanFence(const VulkanFence&) = delete;
    VulkanFence& operator=(const VulkanFence&) = delete;

public: // June API
public:
    Buffer* getBuffer() const override;
    Texture* getTexture() const override;

private:
    VulkanBuffer* m_buffer;
    VulkanTexture* m_texture;
    const JuneFenceDescriptor m_descriptor;
};

} // namespace june