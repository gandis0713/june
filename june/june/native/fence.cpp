#include "fence.h"

namespace june
{

Fence::Fence(JuneFenceDescriptor const* descriptor)
    : m_descriptor(*descriptor)
{
}

} // namespace june