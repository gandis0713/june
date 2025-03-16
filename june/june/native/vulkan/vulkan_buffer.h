#pragma once

#include "june/june.h"
#include "june/native/buffer.h"

namespace june
{

class VulkanContext;
class VulkanBufferMemory;
class VulkanBuffer : public Buffer
{
public:
    static VulkanBuffer* create(VulkanBufferMemory* memory, JuneBufferDescriptor const* descriptor);

public:
    VulkanBuffer() = delete;
    VulkanBuffer(VulkanBufferMemory* memory, JuneBufferDescriptor const* descriptor);
    ~VulkanBuffer() override = default;

    VulkanBuffer(const VulkanBuffer&) = delete;
    VulkanBuffer& operator=(const VulkanBuffer&) = delete;

public: // June API
public:
    Context* getContext() const override;
    BufferMemory* getMemory() const override;

private:
    VulkanBufferMemory* m_memory;
    const JuneBufferDescriptor m_descriptor;
};

} // namespace june