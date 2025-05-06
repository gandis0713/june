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
    static GLESFence* create(GLESContext* context, JuneFenceCreateDescriptor const* descriptor);

public:
    GLESFence() = delete;
    GLESFence(GLESContext* context, JuneFenceCreateDescriptor const* descriptor);
    ~GLESFence() override = default;

    GLESFence(const GLESFence&) = delete;
    GLESFence& operator=(const GLESFence&) = delete;

public: // June API
    void reset(JuneFenceResetDescriptor const* descriptor) override;
    void exportFence(JuneFenceExportDescriptor const* descriptor) override;

public:
    void refresh() override;
    int getFd() const override;

private:
    mutable std::mutex m_mutex;

    EGLSyncKHR m_signalSync{ EGL_NO_SYNC_KHR };
};

} // namespace june