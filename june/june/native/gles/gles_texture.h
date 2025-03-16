#pragma once

#include "june/june.h"
#include "june/native/texture.h"

namespace june
{

class GLESContext;
class GLESTexture : public Texture
{
public:
    static GLESTexture* create(GLESContext* context, JuneTextureDescriptor const* descriptor);

public:
    GLESTexture() = delete;
    GLESTexture(GLESContext* context, JuneTextureDescriptor const* descriptor);
    ~GLESTexture() override = default;

    GLESTexture(const GLESTexture&) = delete;
    GLESTexture& operator=(const GLESTexture&) = delete;

public:
    // June API

public:
    Context* getContext() const override;

private:
    GLESContext* m_context;
    const JuneTextureDescriptor m_descriptor;
};

} // namespace june