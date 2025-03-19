#ifndef JUNE_H_
#define JUNE_H_

#if defined(JUNE_SHARED_LIBRARY)
#if defined(_WIN32)
#if defined(JUNE_IMPLEMENTATION)
#define JUNE_EXPORT __declspec(dllexport)
#else
#define JUNE_EXPORT __declspec(dllimport)
#endif
#else // defined(_WIN32)
#if defined(JUNE_IMPLEMENTATION)
#define JUNE_EXPORT __attribute__((visibility("default")))
#else
#define JUNE_EXPORT
#endif
#endif // defined(_WIN32)
#else  // defined(JUNE_SHARED_LIBRARY)
#define JUNE_EXPORT
#endif // defined(JUNE_SHARED_LIBRARY)

#include <stddef.h>
#include <stdint.h>

typedef uint64_t JuneFlags;

typedef struct JuneInstance_T* JuneInstance;         // Opaque handle for an instance
typedef struct JuneApiContext_T* JuneApiContext;     // Opaque handle for an API context
typedef struct JuneSharedMemory_T* JuneSharedMemory; // Opaque handle for a shared memory
typedef struct JuneBuffer_T* JuneBuffer;             // Opaque handle for a buffer with shared memory
typedef struct JuneTexture_T* JuneTexture;           // Opaque handle for a texture with shared memory
typedef struct JuneFence_T* JuneFence;               // Opaque cross-API fence

struct JuneApiContextDescriptor;

typedef enum JuneApiType
{
    JuneApiType_Undefined = 0x00000000,
    JuneApiType_Vulkan = 0x00000001,
    JuneApiType_D3D11 = 0x00000002,
    JuneApiType_D3D12 = 0x00000003,
    JuneApiType_OpenGL = 0x00000004,
    JuneApiType_GLES = 0x00000005,
} JuneApiType;

typedef enum JuneSType
{
    JuneSType_VulkanApiContext = 0x00000000,
    JuneSType_D3D11ApiContext = 0x00000001,
    JuneSType_D3D12ApiContext = 0x00000002,
    JuneSType_OpenGLApiContext = 0x00000003,
    JuneSType_GLESApiContext = 0x00000004,
    JuneSType_EGLImageSharedMemory = 0x00000005,
    JuneSType_AHardwareBufferSharedMemory = 0x00000006,
} JuneSType;

typedef enum JuneTextureDimension
{
    JuneTextureDimension_Undefined = 0x00000000,
    JuneTextureDimension_1D = 0x00000001,
    JuneTextureDimension_2D = 0x00000002,
    JuneTextureDimension_3D = 0x00000003
} JuneTextureDimension;

typedef enum JuneTextureFormat
{
    JuneTextureFormat_Undefined = 0x00000000,
    JuneTextureFormat_RGBA8Unorm = 0x00000001,
    // TODO: Add more formats
} JuneTextureFormat;

typedef JuneFlags JuneSharedMemoryUsage;
static const JuneSharedMemoryUsage JuneSharedMemoryUsage_None = 0x0000000000000000;
static const JuneSharedMemoryUsage JuneSharedMemoryUsage_GPUSampledImage = 0x0000000000000001;
static const JuneSharedMemoryUsage JuneSharedMemoryUsage_GPUFramebuffer = 0x0000000000000002;
static const JuneSharedMemoryUsage JuneSharedMemoryUsage_GPUBuffer = 0x0000000000000004;
static const JuneSharedMemoryUsage JuneSharedMemoryUsage_CPURead = 0x0000000000000008;
static const JuneSharedMemoryUsage JuneSharedMemoryUsage_CPUWrite = 0x0000000000000010;

typedef struct JuneChainedStruct
{
    struct JuneChainedStruct const* next;
    JuneSType sType;
} JuneChainedStruct;

typedef struct StringView
{
    char const* data;
    size_t length;
} StringView;

typedef struct JuneExtent3D
{
    uint32_t width;
    uint32_t height;
    uint32_t depthOrArrayLayers;
} JuneExtent3D;

typedef struct JuneInstanceDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
} JuneInstanceDescriptor;

typedef struct JuneApiContextDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
} JuneApiContextDescriptor;

typedef struct JuneVulkanApiContextDescriptor
{
    JuneChainedStruct chain;
    void* vkInstance;
    void* vkPhysicalDevice;
    void* vkDevice;
} JuneVulkanApiContextDescriptor;

typedef struct JuneD3D12ApiContextDescriptor
{
    JuneChainedStruct chain;
    void* dxgiAdapter;
    void* d3d12Device;
} JuneD3D12ApiContextDescriptor;

typedef struct JuneD3D11ApiContextDescriptor
{
    JuneChainedStruct chain;
    void* dxgiAdapter;
    void* d3d11Device;
} JuneD3D11ApiContextDescriptor;

typedef struct JuneOpenGLApiContextDescriptor
{
    JuneChainedStruct chain;
    void* display;
    void* context;
} JuneOpenGLApiContextDescriptor;

typedef struct JuneGLESApiContextDescriptor
{
    JuneChainedStruct chain;
    void* display;
    void* context;
} JuneGLESApiContextDescriptor;

typedef struct JuneSharedMemoryDXGISharedHandleDescriptor
{
    JuneChainedStruct chain;
    void* handle; // ??
} JuneSharedMemoryDXGISharedHandleDescriptor;

typedef struct JuneTextureMemoeyEGLImageDescriptor
{
    JuneChainedStruct chain;
    void* eglImage;
} JuneTextureMemoeyEGLDescriptor;

typedef struct JuneSharedMemoryAHardwareBufferDescriptor
{
    JuneChainedStruct chain;
    void* aHardwareBuffer;
} JuneSharedMemoryAhardwareBufferDescriptor;

typedef struct JuneSharedMemoryDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    JuneSharedMemoryUsage usage;
} JuneSharedMemoryDescriptor;

typedef struct JuneBufferDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    JuneSharedMemory memory;
    JuneSharedMemoryUsage usage;
    size_t size;
} JuneBufferDescriptor;

typedef struct JuneTextureDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    JuneSharedMemory memory;
    JuneSharedMemoryUsage usage;
    JuneTextureDimension dimension;
    JuneExtent3D size;
    JuneTextureFormat format;
    uint32_t mipLevelCount;
    uint32_t sampleCount;
} JuneTextureDescriptor;

typedef struct JuneFenceDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
} JuneFenceDescriptor;

typedef struct JuneBeginAccessVulkanBufferDescriptor
{
    JuneChainedStruct chain;
    void* vkSubmitInfo;
} JuneBeginAccessVulkanDescriptor;

typedef struct JuneBeginAccessVulkanTextureDescriptor
{
    JuneChainedStruct chain;
    void* vkSubmitInfo;
    uint32_t oldLayout;
    uint32_t newLayout;
} JuneBeginAccessVulkanTextureDescriptor;

typedef struct JuneBeginAccessDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    JuneFence* fences;
    uint32_t fenceCount;
} JuneBeginAccessDescriptor;

typedef JuneBeginAccessDescriptor JuneEndAccessDescriptor;

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void (*JuneProc)(void);
    typedef JuneProc (*JuneProcGetProcAddress)(StringView name);
    typedef JuneInstance (*JuneProcCreateInstance)(JuneInstanceDescriptor const* desc);

    typedef JuneApiContext (*JuneProcInstanceCreateApiContext)(JuneInstance instance, JuneApiContextDescriptor const* desc);
    typedef void (*JuneProcInstanceDestroy)(JuneInstance instance);

    typedef JuneSharedMemory (*JuneProcApiContextCreateSharedMemory)(JuneApiContext context, JuneSharedMemoryDescriptor const* descriptor);
    typedef JuneBuffer (*JuneProcApiContextCreateBuffer)(JuneApiContext context, JuneBufferDescriptor const* descriptor);
    typedef JuneTexture (*JuneProcApiContextCreateTexture)(JuneApiContext context, JuneTextureDescriptor const* descriptor);
    typedef void (*JuneProcApiContextDestroy)(JuneApiContext context);

    typedef void (*JuneProcSharedMemoryBeginAccess)(JuneSharedMemory memory, JuneBeginAccessDescriptor const* descriptor);
    typedef void (*JuneProcSharedMemoryEndAccess)(JuneSharedMemory memory, JuneEndAccessDescriptor const* descriptor);
    typedef void (*JuneProcSharedMemoryDestroy)(JuneSharedMemory memory);

    typedef JuneFence (*JuneProcBufferCreateFence)(JuneBuffer buffer, JuneFenceDescriptor const* descriptor);
    typedef void* (*JuneProcBufferGetVkBuffer)(JuneBuffer buffer);
    typedef void (*JuneProcBufferDestroy)(JuneBuffer buffer);

    typedef JuneFence (*JuneProcTextureCreateFence)(JuneTexture texture, JuneFenceDescriptor const* descriptor);
    typedef void* (*JuneProcTextureGetVkImage)(JuneTexture texture);
    typedef void (*JuneProcTextureDestroy)(JuneTexture texture);

    typedef void (*JuneProcFenceDestroy)(JuneFence fence);

#if !defined(JUNE_SKIP_DECLARATIONS)

    JUNE_EXPORT JuneProc juneGetProcAddress(StringView name);
    JUNE_EXPORT JuneInstance juneCreateInstance(JuneInstanceDescriptor const* desc);

    JUNE_EXPORT JuneApiContext juneInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc);
    JUNE_EXPORT void juneInstanceDestroy(JuneInstance instance);

    JUNE_EXPORT JuneSharedMemory juneApiContextCreateSharedMemory(JuneApiContext context, JuneSharedMemoryDescriptor const* descriptor);
    JUNE_EXPORT JuneBuffer juneApiContextCreateBuffer(JuneApiContext context, JuneBufferDescriptor const* descriptor);
    JUNE_EXPORT JuneTexture juneApiContextCreateTexture(JuneApiContext context, JuneTextureDescriptor const* descriptor);
    JUNE_EXPORT void juneApiContextDestroy(JuneApiContext context);

    JUNE_EXPORT void juneSharedMemoryBeginAccess(JuneSharedMemory memory, JuneBeginAccessDescriptor const* descriptor);
    JUNE_EXPORT void juneSharedMemoryEndAccess(JuneSharedMemory memory, JuneEndAccessDescriptor const* descriptor);
    JUNE_EXPORT void juneSharedMemoryDestroy(JuneSharedMemory memory);

    JUNE_EXPORT JuneFence juneBufferCreateFence(JuneBuffer buffer, JuneFenceDescriptor const* descriptor);
    JUNE_EXPORT void juneBufferDestroy(JuneBuffer buffer);

    JUNE_EXPORT JuneFence juneTextureCreateFence(JuneTexture texture, JuneFenceDescriptor const* descriptor);
    JUNE_EXPORT void* juneTextureGetVkImage(JuneTexture texture);
    JUNE_EXPORT void juneTextureDestroy(JuneTexture texture);

    JUNE_EXPORT void juneFenceDestroy(JuneFence fence);

#endif // !defined(JUNE_SKIP_DECLARATIONS)

#ifdef __cplusplus
} // extern "C"
#endif

#endif // JUNE_H_