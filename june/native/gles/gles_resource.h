#pragma once

#include "june/june.h"
#include "june/native/resource.h"

namespace june
{

class GLESApiContext;
class GLESResource : public Resource
{
public:
    GLESResource() = delete;
    ~GLESResource() override = default;

    GLESResource(const GLESResource&) = delete;
    GLESResource& operator=(const GLESResource&) = delete;

protected:
    GLESResource(GLESApiContext* context, JuneResourceDescriptor const* descriptor);
};

} // namespace june