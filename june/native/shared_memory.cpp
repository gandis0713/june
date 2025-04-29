#include "shared_memory.h"

#include "instance.h"
#if defined(__ANDROID__) || defined(ANDROID)
#include "june/memory/ahardwarebuffer_memory.h"
#endif

namespace june
{

SharedMemory* SharedMemory::create(Instance* instance, JuneSharedMemoryDescriptor const* descriptor)
{
    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
#if defined(__ANDROID__) || defined(ANDROID)
        case JuneSType_AHardwareBufferSharedMemory: {
            JuneSharedMemoryAHardwareBufferDescriptor const* ahbDescriptor = reinterpret_cast<JuneSharedMemoryAHardwareBufferDescriptor const*>(current);

            RawMemoryDescriptor rawMemoryDescriptor;
            rawMemoryDescriptor.type = RawMemoryType::kAHardwareBuffer;
            rawMemoryDescriptor.width = descriptor->width;
            rawMemoryDescriptor.height = descriptor->height;
            rawMemoryDescriptor.layers = descriptor->layers;

            AHardwareBufferMemoryDescriptor ahbMemoryDescriptor;
            ahbMemoryDescriptor.aHardwareBuffer = static_cast<AHardwareBuffer*>(ahbDescriptor->aHardwareBuffer);
            ahbMemoryDescriptor.aHardwareBufferDesc = *static_cast<AHardwareBuffer_Desc*>(ahbDescriptor->aHardwareBufferDesc);

            std::unique_ptr<RawMemory> rawMemory = AHardwareBufferMemory::create(rawMemoryDescriptor, ahbMemoryDescriptor);
            return new SharedMemory(instance, std::move(rawMemory), descriptor);
        }
        break;
#endif
        default:
            break;
        }

        current = current->next;
    }

    return new SharedMemory(instance, nullptr, descriptor);
}

SharedMemory::SharedMemory(Instance* instance, std::unique_ptr<RawMemory> rawMemory, JuneSharedMemoryDescriptor const* descriptor)
    : m_instance(instance)
    , m_rawMemory(std::move(rawMemory))
    , m_descriptor(*descriptor)
{
}

void SharedMemory::beginAccess(JuneSharedMemoryBeginAccessDescriptor const* descriptor)
{
    // Implementation for beginning access to the shared memory
}

void SharedMemory::endAccess(JuneSharedMemoryEndAccessDescriptor const* descriptor)
{
    // Implementation for ending access to the shared memory
}

Instance* SharedMemory::getInstance() const
{
    return m_instance;
}

size_t SharedMemory::getSize() const
{
    return m_descriptor.width * m_descriptor.height * m_descriptor.layers;
}

RawMemory* SharedMemory::getRawMemory() const
{
    return m_rawMemory.get();
}

void SharedMemory::attach(Resource* resource)
{
    m_attachedApiResources.push_back(resource);
}

} // namespace june