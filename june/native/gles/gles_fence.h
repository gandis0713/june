#pragma once

#include "june/june.h"
#include "june/native/fence.h"

#include "gles_api.h"

#include <mutex>

namespace june
{

class GLESApiContext;
class GLESFence : public Fence
{
public:
    static GLESFence* create(GLESApiContext* context, JuneFenceDescriptor const* descriptor);

public:
    GLESFence() = delete;
    GLESFence(GLESApiContext* context, JuneFenceDescriptor const* descriptor);
    ~GLESFence() override = default;

    GLESFence(const GLESFence&) = delete;
    GLESFence& operator=(const GLESFence&) = delete;

public:
    void begin() override;
    void end() override;
    void wait() override;

private:
    EGLSyncKHR m_sync{ EGL_NO_SYNC_KHR };
    std::mutex m_mutex;
};

} // namespace june