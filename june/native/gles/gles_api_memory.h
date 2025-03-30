#pragma once

#include "june/june.h"
#include "june/native/api_memory.h"

namespace june
{

class GLESApiContext;
class GLESApiMemory : public ApiMemory
{
public:
    static GLESApiMemory* create(GLESApiContext* context, JuneApiMemoryDescriptor const* descriptor);

public:
    GLESApiMemory() = delete;
    GLESApiMemory(GLESApiContext* context, JuneApiMemoryDescriptor const* descriptor);
    ~GLESApiMemory() override = default;

    GLESApiMemory(const GLESApiMemory&) = delete;
    GLESApiMemory& operator=(const GLESApiMemory&) = delete;

public: // June API
    void beginAccess(JuneApiMemoryBeginAccessDescriptor const* descriptor) override;
    void endAccess(JuneApiMemoryEndAccessDescriptor const* descriptor) override;
    void* createResource(JuneResourceDescriptor const* descriptor) override;
};

} // namespace june