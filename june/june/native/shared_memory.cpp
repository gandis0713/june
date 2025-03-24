#include "shared_memory.h"

#include "api_context.h"

namespace june
{

SharedMemory::SharedMemory(ApiContext* context, std::unique_ptr<RawMemory> rawMemory, JuneSharedMemoryDescriptor const* descriptor)
    : m_context(context)
    , m_rawMemory(std::move(rawMemory))
    , m_descriptor(*descriptor)
{
}

ApiContext* SharedMemory::getContext() const
{
    return m_context;
}

size_t SharedMemory::getSize() const
{
    return m_descriptor.width * m_descriptor.height * m_descriptor.layers;
}

} // namespace june