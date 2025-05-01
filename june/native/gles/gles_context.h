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
    void createResource(JuneResourceDescriptor const* descriptor) override;
    Fence* createFence(JuneFenceDescriptor const* descriptor) override;
    void beginMemoryAccess(JuneApiContextBeginMemoryAccessDescriptor const* descriptor) override;
    void endMemoryAccess(JuneApiContextEndMemoryAccessDescriptor const* descriptor) override;

public:
    JuneApiType getApiType() const override;

public: // gles
    EGLContext getEGLContext() const;
    EGLDisplay getEGLDisplay() const;
    EGLSyncKHR createEGLSyncKHR(const int fd);

public:
    GLESAPI eglAPI{};

private:
    EGLContext m_context{ nullptr };
    EGLDisplay m_display{ nullptr };
    DyLib m_glesLib{};

private:
    GLESContext(Instance* instance, JuneApiContextDescriptor const* descriptor);
};

} // namespace june
