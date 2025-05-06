#include "fence.h"

#include "api_context.h"

namespace june
{

Fence::Fence(ApiContext* context, JuneFenceCreateDescriptor const* descriptor)
    : Object(std::string(descriptor->label.data, descriptor->label.length))
    , m_context(context)
    , m_descriptor(*descriptor)
{
}

FenceType Fence::getType() const
{
    return m_type;
}

} // namespace june