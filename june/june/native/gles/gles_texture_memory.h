#pragma once

#include "june/june.h"
#include "june/native/texture_memory.h"

namespace june
{

class GLESContext;
class GLESTextureMemory : public TextureMemory
{
public:
    static GLESTextureMemory* create(GLESContext* context, JuneTextureMemoryDescriptor const* descriptor);

public:
    GLESTextureMemory() = delete;
    GLESTextureMemory(GLESContext* context, JuneTextureMemoryDescriptor const* descriptor);
    ~GLESTextureMemory() override = default;

    GLESTextureMemory(const GLESTextureMemory&) = delete;
    GLESTextureMemory& operator=(const GLESTextureMemory&) = delete;

public: // June API
    Texture* createTexture(JuneTextureDescriptor const* descriptor) override;

public:
    Context* getContext() const override;

private:
    GLESContext* m_context;
    const JuneTextureMemoryDescriptor m_descriptor;
};

} // namespace june