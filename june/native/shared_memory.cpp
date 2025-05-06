#include "shared_memory.h"

#include "instance.h"
#if defined(__ANDROID__) || defined(ANDROID)
#include "june/memory/ahardwarebuffer_memory.h"
#endif
#include "api_context.h"

#include <spdlog/spdlog.h>

namespace june
{

SharedMemory* SharedMemory::import(Instance* instance, JuneSharedMemoryImportDescriptor const* descriptor)
{
    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
#if defined(__ANDROID__) || defined(ANDROID)
        case JuneSType_SharedMemoryAHardwareBufferImportDescriptor: {
            JuneSharedMemoryAHardwareBufferImportDescriptor const* ahbDescriptor = reinterpret_cast<JuneSharedMemoryAHardwareBufferImportDescriptor const*>(current);

            AHardwareBufferImportDescriptor ahbMemoryDescriptor;
            ahbMemoryDescriptor.aHardwareBuffer = static_cast<AHardwareBuffer*>(ahbDescriptor->aHardwareBuffer);

            std::unique_ptr<RawMemory> rawMemory = AHardwareBufferMemory::import(ahbMemoryDescriptor);
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

SharedMemory* SharedMemory::create(Instance* instance, JuneSharedMemoryCreateDescriptor const* descriptor)
{
    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
#if defined(__ANDROID__) || defined(ANDROID)
        case JuneSType_SharedMemoryAHardwareBufferCreateDescriptor: {
            JuneSharedMemoryAHardwareBufferCreateDescriptor const* ahbDescriptor = reinterpret_cast<JuneSharedMemoryAHardwareBufferCreateDescriptor const*>(current);

            AHardwareBufferCreateDescriptor ahbMemoryDescriptor;
            ahbMemoryDescriptor.aHardwareBufferDesc = static_cast<AHardwareBuffer_Desc*>(ahbDescriptor->aHardwareBufferDesc);

            std::unique_ptr<RawMemory> rawMemory = AHardwareBufferMemory::create(ahbMemoryDescriptor);
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

SharedMemory::SharedMemory(Instance* instance, std::unique_ptr<RawMemory> rawMemory, JuneSharedMemoryImportDescriptor const* descriptor)
    : Object(std::string(descriptor->label.data, descriptor->label.length))
    , m_instance(instance)
    , m_rawMemory(std::move(rawMemory))
{
}

SharedMemory::SharedMemory(Instance* instance, std::unique_ptr<RawMemory> rawMemory, JuneSharedMemoryCreateDescriptor const* descriptor)
    : Object(std::string(descriptor->label.data, descriptor->label.length))
    , m_instance(instance)
    , m_rawMemory(std::move(rawMemory))
{
}

Instance* SharedMemory::getInstance() const
{
    return m_instance;
}

size_t SharedMemory::getSize() const
{
    return 0; // TODO: from raw memory
}

RawMemory* SharedMemory::getRawMemory() const
{
    return m_rawMemory.get();
}

} // namespace june