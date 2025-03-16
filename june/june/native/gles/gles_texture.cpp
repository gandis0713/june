#include "gles_texture.h"

#include "gles_context.h"
#include "gles_texture.h"
#include "gles_texture_memory.h"

namespace june
{

GLESTexture* GLESTexture::create(GLESTextureMemory* memory, JuneTextureDescriptor const* descriptor)
{
    return new GLESTexture(memory, descriptor);
}

GLESTexture::GLESTexture(GLESTextureMemory* memory, JuneTextureDescriptor const* descriptor)
    : m_memory(memory)
    , m_descriptor(*descriptor)
{
}

Context* GLESTexture::getContext() const
{
    return m_memory->getContext();
}

TextureMemory* GLESTexture::getMemory() const
{
    return m_memory;
}

} // namespace june