#include "june/june.h"

namespace june
{

extern JuneProc procGetProcAddress(char const* procName);
extern JuneInstance procCreateInstance(JuneInstanceDescriptor const* desc);
extern JuneApiContext procCreateApiContext(JuneInstance innstance, JuneApiContextDescriptor const* desc);
extern void procDestroyInstance(JuneInstance instance);
extern void procDestroyApiContext(JuneApiContext context);
extern JuneBuffer procCreateBuffer(JuneApiContext context, JuneBufferDescriptor const* descriptor);
extern JuneTexture procCreateTexture(JuneApiContext context, JuneTextureDescriptor const* descriptor);
extern void procDestroyBuffer(JuneBuffer buffer);
extern void procDestroyTexture(JuneTexture texture);

} // namespace june

extern "C"
{
    using namespace june;

    JUNE_EXPORT JuneProc juneGetProcAddress(char const* procName)
    {
        return procGetProcAddress(procName);
    }

    JUNE_EXPORT JuneInstance juneCreateInstance(JuneInstanceDescriptor const* desc)
    {
        return procCreateInstance(desc);
    }

    JUNE_EXPORT JuneApiContext juneCreateApiContext(JuneInstance innstance, JuneApiContextDescriptor const* desc)
    {
        return procCreateApiContext(innstance, desc);
    }

    JUNE_EXPORT void juneDestroyInstance(JuneInstance instance)
    {
        return procDestroyInstance(instance);
    }

    JUNE_EXPORT void juneDestroyApiContext(JuneApiContext context)
    {
        return procDestroyApiContext(context);
    }

    JUNE_EXPORT JuneBuffer juneCreateBuffer(JuneApiContext context, JuneBufferDescriptor const* descriptor)
    {
        return procCreateBuffer(context, descriptor);
    }

    JUNE_EXPORT JuneTexture juneCreateTexture(JuneApiContext context, JuneTextureDescriptor const* descriptor)
    {
        return procCreateTexture(context, descriptor);
    }

    JUNE_EXPORT void juneDestroyBuffer(JuneBuffer buffer)
    {
        return procDestroyBuffer(buffer);
    }

    JUNE_EXPORT void juneDestroyTexture(JuneTexture texture)
    {
        return procDestroyTexture(texture);
    }
}