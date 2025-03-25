#include "api_memory.h"

#include "api_context.h"

namespace june
{

ApiMemory::ApiMemory(ApiContext* context, JuneApiMemoryDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

ApiContext* ApiMemory::getContext() const
{
    return m_context;
}

} // namespace june