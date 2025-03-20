#pragma once

#include "june/june.h"
#include "june/native/shared_memory.h"

#include <android/hardware_buffer.h>

namespace june
{

class VulkanApiContext;
class Buffer;
class VulkanAHardwareBufferSharedMemory : public SharedMemory
{
public:
    static VulkanAHardwareBufferSharedMemory* create(VulkanApiContext* context, JuneSharedMemoryDescriptor const* descriptor);
    static VulkanAHardwareBufferSharedMemory* create(VulkanApiContext* context, JuneSharedMemoryDescriptor const* descriptor, JuneSharedMemoryAHardwareBufferDescriptor const* ahbDescriptor);

public:
    VulkanAHardwareBufferSharedMemory() = delete;
    VulkanAHardwareBufferSharedMemory(VulkanApiContext* context, JuneSharedMemoryDescriptor const* descriptor);
    VulkanAHardwareBufferSharedMemory(VulkanApiContext* context, JuneSharedMemoryDescriptor const* descriptor, JuneSharedMemoryAHardwareBufferDescriptor const* ahbDescriptor);
    ~VulkanAHardwareBufferSharedMemory() override = default;

    VulkanAHardwareBufferSharedMemory(const VulkanAHardwareBufferSharedMemory&) = delete;
    VulkanAHardwareBufferSharedMemory& operator=(const VulkanAHardwareBufferSharedMemory&) = delete;

public: // June API
    void beginAccess(JuneBeginAccessDescriptor const* descriptor) override;
    void endAccess(JuneEndAccessDescriptor const* descriptor) override;

public:
    AHardwareBuffer* getAHardwareBuffer() const;

private:
    AHardwareBuffer* m_aHardwareBuffer{ nullptr };
};

} // namespace june