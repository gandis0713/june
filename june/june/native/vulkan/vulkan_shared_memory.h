#pragma once

#include "june/june.h"
#include "june/native/shared_memory.h"

namespace june
{

class VulkanApiContext;
class Buffer;
class VulkanSharedMemory : public SharedMemory
{
public:
    static VulkanSharedMemory* create(VulkanApiContext* context, JuneSharedMemoryDescriptor const* descriptor);

public:
    VulkanSharedMemory() = delete;
    VulkanSharedMemory(VulkanApiContext* context, JuneSharedMemoryDescriptor const* descriptor);
    ~VulkanSharedMemory() override = default;

    VulkanSharedMemory(const VulkanSharedMemory&) = delete;
    VulkanSharedMemory& operator=(const VulkanSharedMemory&) = delete;

public: // June API
    void beginAccess(JuneBeginAccessDescriptor const* descriptor) override;
    void endAccess(JuneEndAccessDescriptor const* descriptor) override;
};

} // namespace june