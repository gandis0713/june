#pragma once

#include "june/june.h"
#include "raw_memory.h"

#include <android/hardware_buffer.h>

namespace june
{

struct AHardwareBufferMemoryDescriptor
{
    AHardwareBuffer* aHardwareBuffer{ nullptr };
};

class ApiContext;
class Buffer;
class AHardwareBufferMemory : public RawMemory
{
public:
    static AHardwareBufferMemory* create(SharedMemory* sharedMemory,
                                         const RawMemoryDescriptor& descriptor);
    static AHardwareBufferMemory* create(SharedMemory* sharedMemory,
                                         const RawMemoryDescriptor& descriptor,
                                         const AHardwareBufferMemoryDescriptor& ahbDescriptor);

public:
    AHardwareBufferMemory() = delete;
    AHardwareBufferMemory(SharedMemory* sharedMemory,
                          const RawMemoryDescriptor& descriptor);
    AHardwareBufferMemory(SharedMemory* sharedMemory,
                          const RawMemoryDescriptor& descriptor,
                          const AHardwareBufferMemoryDescriptor& ahbDescriptor);
    ~AHardwareBufferMemory() override;

    AHardwareBufferMemory(const AHardwareBufferMemory&) = delete;
    AHardwareBufferMemory& operator=(const AHardwareBufferMemory&) = delete;

public:
    AHardwareBufferMemory* getAHardwareBuffer() const;

private:
    AHardwareBufferMemory* m_aHardwareBuffer{ nullptr };
};

} // namespace june