#include "gles_fence.h"

#include "gles_api_context.h"

namespace june
{

GLESFence* GLESFence::create(GLESApiContext* context, JuneFenceDescriptor const* descriptor)
{
    return new GLESFence(context, descriptor);
}

GLESFence::GLESFence(GLESApiContext* context, JuneFenceDescriptor const* descriptor)
    : Fence(context, descriptor)
{
}

} // namespace june