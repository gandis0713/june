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

    JUNE_EXPORT JuneProc wgpuGetProcAddress(char const* procName)
    {
        return procGetProcAddress(procName);
    }

    JUNE_EXPORT JuneInstance JuneCreateInstance(JuneInstanceDescriptor const* desc)
    {
        return procCreateInstance(desc);
    }

    JUNE_EXPORT JuneApiContext JuneCreateApiContext(JuneInstance innstance, JuneApiContextDescriptor const* desc)
    {
        return procCreateApiContext(innstance, desc);
    }

    JUNE_EXPORT void JuneDestroyInstance(JuneInstance instance)
    {
        return procDestroyInstance(instance);
    }

    JUNE_EXPORT void JuneDestroyApiContext(JuneApiContext context)
    {
        return procDestroyApiContext(context);
    }

    JUNE_EXPORT JuneBuffer JuneCreateBuffer(JuneApiContext context, JuneBufferDescriptor const* descriptor)
    {
        return procCreateBuffer(context, descriptor);
    }

    JUNE_EXPORT JuneTexture JuneCreateTexture(JuneApiContext context, JuneTextureDescriptor const* descriptor)
    {
        return procCreateTexture(context, descriptor);
    }

    JUNE_EXPORT void JuneDestroyBuffer(JuneBuffer buffer)
    {
        return procDestroyBuffer(buffer);
    }

    JUNE_EXPORT void JuneDestroyTexture(JuneTexture texture)
    {
        return procDestroyTexture(texture);
    }
}