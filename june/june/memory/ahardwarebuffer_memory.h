#pragma once

#include "june/june.h"
#include "raw_memory.h"

#include <android/hardware_buffer.h>

#include <memory>

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
    static std::unique_ptr<AHardwareBufferMemory> create(const RawMemoryDescriptor& descriptor);
    static std::unique_ptr<AHardwareBufferMemory> create(const RawMemoryDescriptor& descriptor,
                                                         const AHardwareBufferMemoryDescriptor& ahbDescriptor);

public:
    AHardwareBufferMemory() = delete;
    AHardwareBufferMemory(const RawMemoryDescriptor& descriptor);
    AHardwareBufferMemory(const RawMemoryDescriptor& descriptor,
                          const AHardwareBufferMemoryDescriptor& ahbDescriptor);
    ~AHardwareBufferMemory() override;

    AHardwareBufferMemory(const AHardwareBufferMemory&) = delete;
    AHardwareBufferMemory& operator=(const AHardwareBufferMemory&) = delete;

public:
    AHardwareBuffer* getAHardwareBuffer() const;

private:
    AHardwareBufferMemoryDescriptor m_ahbDescriptor;
};

} // namespace june