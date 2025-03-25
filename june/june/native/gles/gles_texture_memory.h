#pragma once

#include "june/june.h"
#include "june/native/shared_memory.h"

namespace june
{

class GLESContext;
class GLESTextureMemory : public TextureMemory
{
public:
    static GLESTextureMemory* create(GLESContext* context, JuneResourceMemoryDescriptor const* descriptor);

public:
    GLESTextureMemory() = delete;
    GLESTextureMemory(GLESContext* context, JuneResourceMemoryDescriptor const* descriptor);
    ~GLESTextureMemory() override = default;

    GLESTextureMemory(const GLESTextureMemory&) = delete;
    GLESTextureMemory& operator=(const GLESTextureMemory&) = delete;

public: // June API
    Texture* createTexture(JuneResourceDescriptor const* descriptor) override;

public:
    Context* getContext() const override;

private:
    GLESContext* m_context;
    const JuneResourceMemoryDescriptor m_descriptor;
};

} // namespace june