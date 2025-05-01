#pragma once

#include "gles_api.h"
#include "june/june.h"

namespace june
{

class GLESContext;
class GLESAHardwareBufferEGLImage
{
public:
    static int create(GLESContext* context, JuneResourceDescriptor const* descriptor);
};

} // namespace june