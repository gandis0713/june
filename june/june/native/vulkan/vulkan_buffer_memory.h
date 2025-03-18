#pragma once

#include "june/june.h"
#include "june/native/buffer_memory.h"

namespace june
{

class VulkanContext;
class VulkanBufferMemory : public BufferMemory
{
public:
    static VulkanBufferMemory* create(VulkanContext* context, JuneBufferMemoryDescriptor const* descriptor);

public:
    VulkanBufferMemory() = delete;
    VulkanBufferMemory(VulkanContext* context, JuneBufferMemoryDescriptor const* descriptor);
    ~VulkanBufferMemory() override = default;

    VulkanBufferMemory(const VulkanBufferMemory&) = delete;
    VulkanBufferMemory& operator=(const VulkanBufferMemory&) = delete;

public: // June API
    Buffer* createBuffer(JuneBufferDescriptor const* descriptor) override;
    void beginAccess(JuneBeginBufferAccessDescriptor const* descriptor) override;
    void endAccess(JuneEndBufferAccessDescriptor const* descriptor) override;

public:
    Context* getContext() const override;

private:
    VulkanContext* m_context;
    const JuneBufferMemoryDescriptor m_descriptor;
};

} // namespace june