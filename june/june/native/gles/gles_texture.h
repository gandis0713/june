#pragma once

#include "june/june.h"
#include "june/native/texture.h"

namespace june
{

class GLESContext;
class GLESTextureMemory;
class GLESTexture : public Texture
{
public:
    static GLESTexture* create(GLESTextureMemory* memory, JuneResourceDescriptor const* descriptor);

public:
    GLESTexture() = delete;
    GLESTexture(GLESTextureMemory* memory, JuneResourceDescriptor const* descriptor);
    ~GLESTexture() override = default;

    GLESTexture(const GLESTexture&) = delete;
    GLESTexture& operator=(const GLESTexture&) = delete;

public: // June API
public:
    Context* getContext() const override;
    TextureMemory* getMemory() const override;

private:
    GLESTextureMemory* m_memory;
    const JuneResourceDescriptor m_descriptor;
};

} // namespace june