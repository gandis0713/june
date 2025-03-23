#include "ahardwarebuffer_memory.h"

#include "june/native/api_context.h"

namespace june
{

AHardwareBufferMemory* AHardwareBufferMemory::create(SharedMemory* sharedMemory,
                                                     const RawMemoryDescriptor& descriptor)
{
    return new AHardwareBufferMemory(sharedMemory, descriptor);
}

AHardwareBufferMemory* AHardwareBufferMemory::create(SharedMemory* sharedMemory,
                                                     const RawMemoryDescriptor& descriptor,
                                                     const AHardwareBufferMemoryDescriptor& ahbDescriptor)
{
    return new AHardwareBufferMemory(sharedMemory, descriptor, ahbDescriptor);
}

AHardwareBufferMemory::AHardwareBufferMemory(SharedMemory* sharedMemory,
                                             const RawMemoryDescriptor& descriptor)
    : RawMemory(sharedMemory, descriptor)
    , m_ahbDescriptor(nullptr)
{
    AHardwareBufferMemory_Desc desc = {
        .width = m_descriptor.width,
        .height = m_descriptor.height,
        .layers = m_descriptor.layers,
        .format = AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM,
        .usage = AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE | AHARDWAREBUFFER_USAGE_GPU_COLOR_OUTPUT,
    };

    AHardwareBufferMemory_allocate(&desc, &m_ahbDescriptor.aHardwareBuffer);
}

AHardwareBufferMemory::AHardwareBufferMemory(SharedMemory* sharedMemory,
                                             const RawMemoryDescriptor& descriptor,
                                             const AHardwareBufferMemoryDescriptor& ahbDescriptor)
    : RawMemory(sharedMemory, descriptor)
    , m_ahbDescriptor(*ahbDescriptor)
{
    m_aHardwareBuffer = m_ahbDescriptor.aHardwareBuffer;
}

AHardwareBufferMemory::~AHardwareBufferMemory()
{
    if (m_descriptor.hasOwnership)
        AHardwareBufferMemory_free(&m_ahbDescriptor.aHardwareBuffer);
}

AHardwareBufferMemory* AHardwareBufferMemory::getAHardwareBuffer() const
{
    return m_ahbDescriptor.aHardwareBuffer;
}

} // namespace june