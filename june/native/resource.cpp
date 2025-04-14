#include "resource.h"

#include "fence.h"

namespace june
{

Resource::Resource(ApiContext* context, JuneResourceDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

void Resource::connect(Resource* inputResource)
{
    this->addInput(inputResource);
    inputResource->addOutput(this);

    m_fence->connect(inputResource->getFence());
}

void Resource::addInput(Resource* memory)
{
    m_inputs.insert(memory);
}

void Resource::removeInput(Resource* memory)
{
    m_inputs.erase(memory);
}

void Resource::removeInputAll()
{
    m_inputs.clear();
}

void Resource::addOutput(Resource* memory)
{
    m_outputs.insert(memory);
    m_signal.connect(&Resource::slot, memory);
}

void Resource::removeOutput(Resource* memory)
{
    m_outputs.erase(memory);
    m_signal.disconnect(&Resource::slot, memory);
}

void Resource::removeOutputAll()
{
    for (auto& output : m_outputs)
    {
        m_signal.disconnect(&Resource::slot, output);
    }
    m_outputs.clear();
}

SharedMemory* Resource::getSharedMemory() const
{
    return reinterpret_cast<SharedMemory*>(m_descriptor.sharedMemory);
}

Fence* Resource::getFence() const
{
    return m_fence;
}

void Resource::signal()
{
    m_signal(this);
}

void Resource::slot(Resource* memory)
{
    m_accessMutex.unlock();
}

} // namespace june