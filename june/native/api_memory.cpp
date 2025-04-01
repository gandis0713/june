#include "api_memory.h"

namespace june
{

ApiMemory::ApiMemory(ApiContext* context, JuneApiMemoryDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

SharedMemory* ApiMemory::getSharedMemory() const
{
    return reinterpret_cast<ApiMemory*>(m_descriptor.sharedMemory)->getSharedMemory();
}

void ApiMemory::connect(ApiMemory* srcMemory)
{
    this->addInput(srcMemory);
    srcMemory->addOutput(this);
}

void ApiMemory::addInput(ApiMemory* memory)
{
    m_inputs.insert(memory);
}

void ApiMemory::removeInput(ApiMemory* memory)
{
    m_inputs.erase(memory);
}

void ApiMemory::removeInputAll()
{
    m_inputs.clear();
}

void ApiMemory::addOutput(ApiMemory* memory)
{
    m_outputs.insert(memory);
}

void ApiMemory::removeOutput(ApiMemory* memory)
{
    m_outputs.erase(memory);
}

void ApiMemory::removeOutputAll()
{
    m_outputs.clear();
}

} // namespace june