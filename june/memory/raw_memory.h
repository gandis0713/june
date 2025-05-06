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
    bool hasOwnership{ false };
};

class Resource;
class RawMemory
{
public:
    RawMemory() = delete;
    virtual ~RawMemory() = default;

public:
    RawMemoryType getType() const;
    bool hasOwnership() const;

protected:
    RawMemory(const RawMemoryDescriptor& descriptor);

protected:
    const RawMemoryDescriptor m_descriptor;
};

} // namespace june