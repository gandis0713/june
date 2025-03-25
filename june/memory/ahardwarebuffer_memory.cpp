#include "ahardwarebuffer_memory.h"

#include "june/native/api_context.h"

namespace june
{

std::unique_ptr<AHardwareBufferMemory> AHardwareBufferMemory::create(const RawMemoryDescriptor& descriptor)
{
    return std::unique_ptr<AHardwareBufferMemory>(new AHardwareBufferMemory(descriptor));
}

std::unique_ptr<AHardwareBufferMemory> AHardwareBufferMemory::create(const RawMemoryDescriptor& descriptor,
                                                                     const AHardwareBufferMemoryDescriptor& ahbDescriptor)
{
    return std::unique_ptr<AHardwareBufferMemory>(new AHardwareBufferMemory(descriptor, ahbDescriptor));
}

AHardwareBufferMemory::AHardwareBufferMemory(const RawMemoryDescriptor& descriptor)
    : RawMemory(descriptor)
    , m_ahbDescriptor(AHardwareBufferMemoryDescriptor{})
{
    AHardwareBuffer_Desc desc = {
        .width = m_descriptor.width,
        .height = m_descriptor.height,
        .layers = m_descriptor.layers,
        .format = AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM,
        .usage = AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE | AHARDWAREBUFFER_USAGE_GPU_COLOR_OUTPUT,
    };

    AHardwareBuffer_allocate(&desc, &m_ahbDescriptor.aHardwareBuffer);
}

AHardwareBufferMemory::AHardwareBufferMemory(const RawMemoryDescriptor& descriptor,
                                             const AHardwareBufferMemoryDescriptor& ahbDescriptor)
    : RawMemory(descriptor)
    , m_ahbDescriptor(ahbDescriptor)
{
}

AHardwareBufferMemory::~AHardwareBufferMemory()
{
    if (m_descriptor.hasOwnership)
        AHardwareBuffer_release(m_ahbDescriptor.aHardwareBuffer);
}

AHardwareBuffer* AHardwareBufferMemory::getAHardwareBuffer() const
{
    return m_ahbDescriptor.aHardwareBuffer;
}

} // namespace june