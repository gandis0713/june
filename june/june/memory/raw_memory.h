#pragma once

#include <stdint.h>

namespace june
{

enum class RawMemoryType
{
    kNone = 0,
    kAHardwareBuffer,
    kDmaBuf,
};

struct RawMemoryDescriptor
{
    RawMemoryType type{ RawMemoryType::kNone };
    uint32_t width{ 0 };
    uint32_t height{ 0 };
    uint32_t layers{ 0 };
    bool hasOwnership{ false };
};

class SharedMemory;
class RawMemory
{
public:
    static RawMemoryType getDefaultMemoryType();

public:
    RawMemory() = delete;
    virtual ~RawMemory() = default;

public:
    RawMemoryType getType() const;

protected:
    RawMemory(SharedMemory* sharedMemory, const RawMemoryDescriptor& descriptor);

protected:
    SharedMemory* m_sharedMemory{ nullptr };
    const RawMemoryDescriptor m_descriptor;
};

} // namespace june