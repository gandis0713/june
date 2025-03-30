#pragma once

#include "june/june.h"
#include "june/native/gles/gles_api_memory.h"

#include "gles_api.h"

namespace june
{

class GLESApiContext;
class GLESAHardwareBufferApiMemory : public GLESApiMemory
{
public:
    static GLESAHardwareBufferApiMemory* create(GLESApiContext* context, JuneApiMemoryDescriptor const* descriptor);

public:
    GLESAHardwareBufferApiMemory() = delete;
    GLESAHardwareBufferApiMemory(GLESApiContext* context, JuneApiMemoryDescriptor const* descriptor);
    ~GLESAHardwareBufferApiMemory() override = default;

    GLESAHardwareBufferApiMemory(const GLESAHardwareBufferApiMemory&) = delete;
    GLESAHardwareBufferApiMemory& operator=(const GLESAHardwareBufferApiMemory&) = delete;

public: // June API
    void beginAccess(JuneApiMemoryBeginAccessDescriptor const* descriptor) override;
    void endAccess(JuneApiMemoryEndAccessDescriptor const* descriptor) override;
    void* createResource(JuneResourceDescriptor const* descriptor) override;

private:
    int32_t initialize();

private:
    EGLClientBuffer m_clientBuffer{ nullptr };
};

} // namespace june