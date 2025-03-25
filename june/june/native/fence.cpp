#include "fence.h"

#include "api_context.h"

namespace june
{

Fence::Fence(ApiContext* context, JuneFenceDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

} // namespace june