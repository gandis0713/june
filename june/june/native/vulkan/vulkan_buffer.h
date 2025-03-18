#pragma once

#include "june/june.h"
#include "june/native/buffer.h"

namespace june
{

class VulkanContext;
class VulkanBuffer : public Buffer
{
public:
    static VulkanBuffer* create(VulkanContext* context, JuneBufferDescriptor const* descriptor);

public:
    VulkanBuffer() = delete;
    VulkanBuffer(VulkanContext* context, JuneBufferDescriptor const* descriptor);
    ~VulkanBuffer() override = default;

    VulkanBuffer(const VulkanBuffer&) = delete;
    VulkanBuffer& operator=(const VulkanBuffer&) = delete;

public: // June API
    Fence* createFence(JuneFenceDescriptor const* descriptor) override;

public:
    Context* getContext() const override;

private:
    VulkanContext* m_context;
    const JuneBufferDescriptor m_descriptor;
};

} // namespace june