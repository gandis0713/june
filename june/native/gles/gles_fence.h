#pragma once

#include "june/june.h"
#include "june/native/fence.h"

#include "gles_api.h"

#include <mutex>
#include <unordered_map>

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

public:
    EGLSyncKHR getEGLSyncKHR() const;
    int getFenceFd() const;

protected:
    void updated(Fence* fence) override;

private:
    mutable std::mutex m_mutex;

    std::unordered_map<Fence*, EGLSyncKHR> m_waitSync{};
    EGLSyncKHR m_signalSync{ EGL_NO_SYNC_KHR };
};

} // namespace june