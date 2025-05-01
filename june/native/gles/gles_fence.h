#pragma once

#include "june/june.h"
#include "june/native/fence.h"

#include "gles_api.h"

#include <mutex>
#include <unordered_map>

namespace june
{

class GLESContext;
class GLESFence : public Fence
{
public:
    static GLESFence* create(GLESContext* context, JuneFenceDescriptor const* descriptor);

public:
    GLESFence() = delete;
    GLESFence(GLESContext* context, JuneFenceDescriptor const* descriptor);
    ~GLESFence() override = default;

    GLESFence(const GLESFence&) = delete;
    GLESFence& operator=(const GLESFence&) = delete;

public:
    void refresh() override;

public:
    EGLSyncKHR getEGLSyncKHR() const;
    int getFd() const;

private:
    void waitEGLSyncKHR();
    void createEGLSyncKHR();
    void createFd();

private:
    mutable std::mutex m_mutex;

    EGLSyncKHR m_signalSync{ EGL_NO_SYNC_KHR };
    int m_signalFd{ -1 };
};

} // namespace june