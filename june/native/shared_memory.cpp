#include "shared_memory.h"

#include "instance.h"
#if defined(__ANDROID__) || defined(ANDROID)
#include "june/memory/ahardwarebuffer_memory.h"
#endif

namespace june
{

SharedMemory* SharedMemory::create(Instance* instance, JuneSharedMemoryDescriptor const* descriptor)
{
    RawMemoryDescriptor rawMemoryDescriptor;
    rawMemoryDescriptor.type = RawMemoryType::kAHardwareBuffer;
    rawMemoryDescriptor.width = descriptor->width;
    rawMemoryDescriptor.height = descriptor->height;
    rawMemoryDescriptor.layers = descriptor->layers;

    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
#if defined(__ANDROID__) || defined(ANDROID)
        case JuneSType_AHardwareBufferSharedMemory: {
            JuneSharedMemoryAHardwareBufferDescriptor const* ahbDescriptor = reinterpret_cast<JuneSharedMemoryAHardwareBufferDescriptor const*>(current);

            AHardwareBufferMemoryDescriptor ahbMemoryDescriptor;
            ahbMemoryDescriptor.aHardwareBuffer = static_cast<AHardwareBuffer*>(ahbDescriptor->aHardwareBuffer);

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

    auto defaultMemoryType = RawMemory::getDefaultMemoryType();
    switch (defaultMemoryType)
    {
#if defined(__ANDROID__) || defined(ANDROID)
    case RawMemoryType::kAHardwareBuffer: {
        std::unique_ptr<RawMemory> rawMemory = AHardwareBufferMemory::create(rawMemoryDescriptor);
        return new SharedMemory(instance, std::move(rawMemory), descriptor);
    }
    break;
#endif
    default:
        break;
    }

    return new SharedMemory(instance, nullptr, descriptor);
}

SharedMemory::SharedMemory(Instance* instance, std::unique_ptr<RawMemory> rawMemory, JuneSharedMemoryDescriptor const* descriptor)
    : m_instance(instance)
    , m_rawMemory(std::move(rawMemory))
    , m_descriptor(*descriptor)
{
}

Instance* SharedMemory::getInstance() const
{
    return m_instance;
}

size_t SharedMemory::getSize() const
{
    return m_descriptor.width * m_descriptor.height * m_descriptor.layers;
}

} // namespace june