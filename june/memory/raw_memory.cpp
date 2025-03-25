#include "raw_memory.h"

#include "june/native/shared_memory.h"

namespace june
{

RawMemoryType RawMemory::getDefaultMemoryType()
{
#if defined(__ANDROID__) || defined(ANDROID)
    return RawMemoryType::kAHardwareBuffer;
#elif defined(__linux__)
    return RawMemoryType::kDmaBuf;
#endif
    return RawMemoryType::kNone;
}

RawMemory::RawMemory(const RawMemoryDescriptor& descriptor)
    : m_descriptor(descriptor)
{
}

RawMemoryType RawMemory::getType() const
{
    return m_descriptor.type;
}

} // namespace june