#include "gles_shared_memory.h"

#include "gles_context.h"
#include "gles_texture.h"

namespace june
{

GLESTextureMemory* GLESTextureMemory::create(GLESContext* context, JuneTextureMemoryDescriptor const* descriptor)
{
    return new GLESTextureMemory(context, descriptor);
}

GLESTextureMemory::GLESTextureMemory(GLESContext* context, JuneTextureMemoryDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

Texture* GLESTextureMemory::createTexture(JuneTextureDescriptor const* descriptor)
{
    return GLESTexture::create(this, descriptor);
}

Context* GLESTextureMemory::getContext() const
{
    return m_context;
}

} // namespace june