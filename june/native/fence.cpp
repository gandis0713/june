#include "fence.h"

#include "api_context.h"

namespace june
{

Fence::Fence(ApiContext* context, JuneFenceDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

void Fence::connect(Fence* inputFence)
{
    this->addInput(inputFence);
    inputFence->addOutput(this);
}

void Fence::addInput(Fence* fence)
{
    m_inputs.insert(fence);
}

void Fence::removeInput(Fence* fence)
{
    m_inputs.erase(fence);
}

void Fence::removeInputAll()
{
    m_inputs.clear();
}

void Fence::addOutput(Fence* fence)
{
    m_outputs.insert(fence);
    m_signal.connect(&Fence::slot, fence);
}

void Fence::removeOutput(Fence* fence)
{
    m_outputs.erase(fence);
    m_signal.disconnect(&Fence::slot, fence);
}

void Fence::removeOutputAll()
{
    for (auto& output : m_outputs)
    {
        m_signal.disconnect(&Fence::slot, output);
    }
    m_outputs.clear();
}

FenceType Fence::getType() const
{
    return m_type;
}

void Fence::signal()
{
    m_signal(this);
}

void Fence::slot(Fence* fence)
{
    updated(fence);
}

} // namespace june