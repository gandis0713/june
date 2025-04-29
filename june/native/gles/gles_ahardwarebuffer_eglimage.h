#pragma once

#include "gles_api.h"
#include "june/june.h"

namespace june
{

class GLESApiContext;
class GLESAHardwareBufferEGLImage
{
public:
    static int create(GLESApiContext* context, JuneResourceDescriptor const* descriptor);
};

} // namespace june