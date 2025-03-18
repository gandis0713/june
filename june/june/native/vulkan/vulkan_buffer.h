#pragma once

#include "june/june.h"
#include "june/native/buffer.h"

namespace june
{

class VulkanContext;
class VulkanSharedMemory;
class VulkanBuffer : public Buffer
{
public:
    static VulkanBuffer* create(VulkanSharedMemory* memory, JuneBufferDescriptor const* descriptor);

public:
    VulkanBuffer() = delete;
    VulkanBuffer(VulkanSharedMemory* memory, JuneBufferDescriptor const* descriptor);
    ~VulkanBuffer() override = default;

    VulkanBuffer(const VulkanBuffer&) = delete;
    VulkanBuffer& operator=(const VulkanBuffer&) = delete;

public: // June API
    Fence* createFence(JuneFenceDescriptor const* descriptor) override;

public:
    Context* getContext() const override;
    SharedMemory* getMemory() const override;

private:
    VulkanSharedMemory* m_memory;
    const JuneBufferDescriptor m_descriptor;
};

} // namespace june