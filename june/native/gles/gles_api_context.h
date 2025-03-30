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
class GLESApiContext : public ApiContext
{
public:
    static ApiContext* create(Instance* instance, JuneGLESApiContextDescriptor const* descriptor);

public:
    GLESApiContext() = delete;
    ~GLESApiContext() override;

    GLESApiContext(const GLESApiContext&) = delete;
    GLESApiContext& operator=(const GLESApiContext&) = delete;

public: // June API
    ApiMemory* createApiMemory(JuneApiMemoryDescriptor const* descriptor) override;
    Fence* createFence(JuneFenceDescriptor const* descriptor) override;

public:
    Instance* getInstance() const override;
    JuneApiType getApiType() const override;

public: // gles
public:
    GLESAPI eglAPI{};

private:
private:
    Instance* m_instance = nullptr;

private:
    DyLib m_glesLib{};

private:
    GLESApiContext(Instance* instance, JuneGLESApiContextDescriptor const* descriptor);
};

} // namespace june
