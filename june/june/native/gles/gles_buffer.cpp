#include "gles_buffer.h"

#include "gles_buffer.h"
#include "gles_buffer_memory.h"
#include "gles_context.h"

namespace june
{

GLESBuffer* GLESBuffer::create(GLESBufferMemory* memory, JuneBufferDescriptor const* descriptor)
{
    return new GLESBuffer(memory, descriptor);
}

GLESBuffer::GLESBuffer(GLESBufferMemory* memory, JuneBufferDescriptor const* descriptor)
    : m_memory(memory)
    , m_descriptor(*descriptor)
{
}

Context* GLESBuffer::getContext() const
{
    return m_memory->getContext();
}

BufferMemory* GLESBuffer::getMemory() const
{
    return m_memory;
}

} // namespace june