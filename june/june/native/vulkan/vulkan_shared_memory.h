#pragma once

#include "june/june.h"
#include "june/native/shared_memory.h"

namespace june
{

class VulkanContext;
class Buffer;
class VulkanSharedMemory : public SharedMemory
{
public:
    static VulkanSharedMemory* create(VulkanContext* context, JuneSharedMemoryDescriptor const* descriptor);

public:
    VulkanSharedMemory() = delete;
    VulkanSharedMemory(VulkanContext* context, JuneSharedMemoryDescriptor const* descriptor);
    ~VulkanSharedMemory() override = default;

    VulkanSharedMemory(const VulkanSharedMemory&) = delete;
    VulkanSharedMemory& operator=(const VulkanSharedMemory&) = delete;

public: // June API
    void beginAccess(JuneBeginAccessDescriptor const* descriptor) override;
    void endAccess(JuneEndAccessDescriptor const* descriptor) override;

public:
    Context* getContext() const override;

private:
    VulkanContext* m_context;
    const JuneSharedMemoryDescriptor m_descriptor;
};

} // namespace june