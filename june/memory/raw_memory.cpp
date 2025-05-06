#include "raw_memory.h"

#include "june/native/shared_memory.h"

namespace june
{

RawMemory::RawMemory(const RawMemoryDescriptor& descriptor)
    : m_descriptor(descriptor)
{
}

RawMemoryType RawMemory::getType() const
{
    return m_descriptor.type;
}

bool RawMemory::hasOwnership() const
{
    return m_descriptor.hasOwnership;
}

} // namespace june