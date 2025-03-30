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
    AHardwareBuffer_Desc aHardwareBufferDesc{};
};

class ApiContext;
class Buffer;
class AHardwareBufferMemory : public RawMemory
{
public:
    static std::unique_ptr<AHardwareBufferMemory> create(const RawMemoryDescriptor& descriptor,
                                                         const AHardwareBufferMemoryDescriptor& ahbDescriptor);

public:
    AHardwareBufferMemory() = delete;
    ~AHardwareBufferMemory() override;

    AHardwareBufferMemory(const AHardwareBufferMemory&) = delete;
    AHardwareBufferMemory& operator=(const AHardwareBufferMemory&) = delete;

public:
    AHardwareBuffer* getAHardwareBuffer() const;

private:
    AHardwareBuffer* m_ahb{ nullptr };
    AHardwareBuffer_Desc m_desc{};

private:
    AHardwareBufferMemory(const RawMemoryDescriptor& descriptor,
                          AHardwareBuffer* ahb);
    AHardwareBufferMemory(const RawMemoryDescriptor& descriptor,
                          const AHardwareBuffer_Desc& ahbDescriptor);
};

} // namespace june