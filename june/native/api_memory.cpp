#include "api_memory.h"

#include "fence.h"

namespace june
{

ApiMemory::ApiMemory(ApiContext* context, JuneApiMemoryDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

void ApiMemory::connect(ApiMemory* inputMemory)
{
    this->addInput(inputMemory);
    inputMemory->addOutput(this);

    m_fence->connect(inputMemory->getFence());
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
    m_signal.connect(&ApiMemory::slot, memory);
}

void ApiMemory::removeOutput(ApiMemory* memory)
{
    m_outputs.erase(memory);
    m_signal.disconnect(&ApiMemory::slot, memory);
}

void ApiMemory::removeOutputAll()
{
    for (auto& output : m_outputs)
    {
        m_signal.disconnect(&ApiMemory::slot, output);
    }
    m_outputs.clear();
}

SharedMemory* ApiMemory::getSharedMemory() const
{
    return reinterpret_cast<SharedMemory*>(m_descriptor.sharedMemory);
}

Fence* ApiMemory::getFence() const
{
    return m_fence;
}

void ApiMemory::signal()
{
    m_signal(this);
}

void ApiMemory::slot(ApiMemory* memory)
{
    m_accessMutex.unlock();
}

} // namespace june