#include "gles_buffer.h"
#include "gles_context.h"

namespace june
{

GLESBuffer* GLESBuffer::create(GLESContext* context, JuneBufferDescriptor const* descriptor)
{
    return new GLESBuffer(context, descriptor);
}

GLESBuffer::GLESBuffer(GLESContext* context, JuneBufferDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

Context* GLESBuffer::getContext() const
{
    return m_context;
}

} // namespace june