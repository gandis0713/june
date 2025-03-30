#include "ahardwarebuffer_memory.h"

#include "june/native/api_context.h"

namespace june
{

std::unique_ptr<AHardwareBufferMemory> AHardwareBufferMemory::create(const RawMemoryDescriptor& descriptor,
                                                                     const AHardwareBufferMemoryDescriptor& ahbDescriptor)
{
    if (ahbDescriptor.aHardwareBuffer)
        return std::unique_ptr<AHardwareBufferMemory>(new AHardwareBufferMemory(descriptor, ahbDescriptor.aHardwareBuffer));
    else
        return std::unique_ptr<AHardwareBufferMemory>(new AHardwareBufferMemory(descriptor, ahbDescriptor.aHardwareBufferDesc));
}

AHardwareBufferMemory::AHardwareBufferMemory(const RawMemoryDescriptor& descriptor,
                                             AHardwareBuffer* ahb)
    : RawMemory(descriptor)
    , m_ahb(ahb)
{
    AHardwareBuffer_describe(m_ahb, &m_desc);
}

AHardwareBufferMemory::AHardwareBufferMemory(const RawMemoryDescriptor& descriptor,
                                             const AHardwareBuffer_Desc& ahbDescriptor)
    : RawMemory(descriptor)
    , m_desc(ahbDescriptor)
{
    AHardwareBuffer_allocate(&m_desc, &m_ahb);
}

AHardwareBufferMemory::~AHardwareBufferMemory()
{
    if (m_descriptor.hasOwnership)
        AHardwareBuffer_release(m_ahb);
}

AHardwareBuffer* AHardwareBufferMemory::getAHardwareBuffer() const
{
    return m_ahb;
}

} // namespace june