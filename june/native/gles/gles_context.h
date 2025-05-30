#pragma once

#include "june/common/cast.h"
#include "june/common/dylib.h"
#include "june/native/api_context.h"

#include "june/june.h"

#include "gles_api.h"

#include <memory>
#include <vector>

namespace june
{

class Instance;
class GLESContext : public ApiContext
{
public:
    static ApiContext* create(Instance* instance, JuneApiContextDescriptor const* descriptor);

public:
    GLESContext() = delete;
    ~GLESContext() override;

    GLESContext(const GLESContext&) = delete;
    GLESContext& operator=(const GLESContext&) = delete;

public: // June API
    void createResource(JuneResourceCreateDescriptor const* descriptor) override;
    void exportFence(JuneFenceExportDescriptor const* descriptor) override;

public:
    JuneApiType getApiType() const override;

public: // gles
    EGLContext getEGLContext() const;
    EGLDisplay getEGLDisplay() const;

public:
    GLESAPI eglAPI{};

private:
    EGLContext m_eglContext{ nullptr };
    EGLDisplay m_eglDisplay{ nullptr };
    DyLib m_glesLib{};

private:
    GLESContext(Instance* instance, JuneApiContextDescriptor const* descriptor);
};

} // namespace june
