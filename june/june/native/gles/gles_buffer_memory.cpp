#include "gles_shared_memory.h"

#include "gles_buffer.h"
#include "gles_context.h"

namespace june
{

GLESBufferMemory* GLESBufferMemory::create(GLESContext* context, JuneBufferMemoryDescriptor const* descriptor)
{
    return new GLESBufferMemory(context, descriptor);
}

GLESBufferMemory::GLESBufferMemory(GLESContext* context, JuneBufferMemoryDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

Buffer* GLESBufferMemory::createBuffer(JuneBufferDescriptor const* descriptor)
{
    return GLESBuffer::create(this, descriptor);
}

Context* GLESBufferMemory::getContext() const
{
    return m_context;
}

} // namespace june