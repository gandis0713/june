#include "shared_memory.h"

#include "instance.h"
#if defined(__ANDROID__) || defined(ANDROID)
#include "june/memory/ahardwarebuffer_memory.h"
#endif
#include "api_context.h"

#include <spdlog/spdlog.h>

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

void SharedMemory::lock(ApiContext* apiContext)
{
    spdlog::error("try lock: {:#x}", reinterpret_cast<std::uintptr_t>(apiContext));
    m_mutex.lock();
    spdlog::error("locked: {:#x}", reinterpret_cast<std::uintptr_t>(apiContext));
    m_ownerApiContext = apiContext;
}

void SharedMemory::unlock(ApiContext* apiContext)
{
    spdlog::error("try unlock: {:#x}", reinterpret_cast<std::uintptr_t>(apiContext));
    if (m_ownerApiContext == apiContext)
    {
        m_mutex.unlock();
        spdlog::error("unlocked: {:#x}", reinterpret_cast<std::uintptr_t>(apiContext));
    }
}

void SharedMemory::attach(ApiContext* apiContext)
{
    m_attachedApiContext.insert(apiContext);
}

void SharedMemory::detach(ApiContext* apiContext)
{
    m_attachedApiContext.erase(apiContext);
}

} // namespace june