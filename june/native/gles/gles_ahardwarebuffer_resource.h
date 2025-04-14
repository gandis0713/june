#pragma once

#include "june/june.h"
#include "june/native/gles/gles_resource.h"

#include "gles_api.h"

namespace june
{

class GLESApiContext;
class GLESAHardwareBufferResource : public GLESResource
{
public:
    static GLESAHardwareBufferResource* create(GLESApiContext* context, JuneResourceDescriptor const* descriptor);

public:
    GLESAHardwareBufferResource() = delete;
    GLESAHardwareBufferResource(GLESApiContext* context, JuneResourceDescriptor const* descriptor);
    ~GLESAHardwareBufferResource() override = default;

    GLESAHardwareBufferResource(const GLESAHardwareBufferResource&) = delete;
    GLESAHardwareBufferResource& operator=(const GLESAHardwareBufferResource&) = delete;

public: // June API
    void beginAccess(JuneResourceBeginAccessDescriptor const* descriptor) override;
    void endAccess(JuneResourceEndAccessDescriptor const* descriptor) override;
    void* getResource(JuneGetResourceDescriptor const* descriptor) override;

private:
    int32_t initialize();
    int32_t createEGLClientBuffer();
    int32_t createEGLImageKHR();

private:
    EGLClientBuffer m_clientBuffer{ nullptr };
    EGLImageKHR m_eglImage{ EGL_NO_IMAGE_KHR };
};

} // namespace june