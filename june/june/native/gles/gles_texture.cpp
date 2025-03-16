#include "gles_texture.h"
#include "gles_context.h"

namespace june
{

GLESTexture* GLESTexture::create(GLESContext* context, JuneTextureDescriptor const* descriptor)
{
    return new GLESTexture(context, descriptor);
}

GLESTexture::GLESTexture(GLESContext* context, JuneTextureDescriptor const* descriptor)
    : m_context(context)
    , m_descriptor(*descriptor)
{
}

Context* GLESTexture::getContext() const
{
    return m_context;
}

} // namespace june