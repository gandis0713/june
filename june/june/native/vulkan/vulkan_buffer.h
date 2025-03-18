#pragma once

#include "june/june.h"
#include "june/native/buffer.h"

namespace june
{

class VulkanApiContext;
class VulkanBuffer : public Buffer
{
public:
    static VulkanBuffer* create(VulkanApiContext* context, JuneBufferDescriptor const* descriptor);

public:
    VulkanBuffer() = delete;
    VulkanBuffer(VulkanApiContext* context, JuneBufferDescriptor const* descriptor);
    ~VulkanBuffer() override = default;

    VulkanBuffer(const VulkanBuffer&) = delete;
    VulkanBuffer& operator=(const VulkanBuffer&) = delete;

public: // June API
    Fence* createFence(JuneFenceDescriptor const* descriptor) override;
    void* getVkBuffer() const override;

public:
    ApiContext* getApiContext() const override;

private:
    VulkanApiContext* m_context;
    const JuneBufferDescriptor m_descriptor;
};

} // namespace june