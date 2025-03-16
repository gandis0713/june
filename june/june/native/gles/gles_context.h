#pragma once

#include "june/common/cast.h"
#include "june/common/dylib.h"
#include "june/native/context.h"

#include "june/june.h"

#include <memory>
#include <vector>

namespace june
{

class Instance;
class GLESContext : public Context
{
public:
    static Context* create(Instance* instance, JuneGLESApiContextDescriptor const* descriptor);

public:
    GLESContext() = delete;
    ~GLESContext() override;

    GLESContext(const GLESContext&) = delete;
    GLESContext& operator=(const GLESContext&) = delete;

public: // June APi
    BufferMemory* createBufferMemory(JuneBufferMemoryDescriptor const* descriptor) override;
    TextureMemory* createTextureMemory(JuneTextureMemoryDescriptor const* descriptor) override;

public:
    Instance* getInstance() const override;
    JuneApiType getApiType() const override;

public: // gles
public:
    // TODO: GLES API

private:
    Instance* m_instance = nullptr;

private:
    void* m_eglContext = nullptr;
    void* m_eglDisplay = nullptr;
    // TODO: EGL context
    // TODO: EGL display

    DyLib m_glesLib{};

private:
    GLESContext(Instance* instance, JuneGLESApiContextDescriptor const* descriptor);
};

} // namespace june