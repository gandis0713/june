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
typedef struct JuneSharedMemory_T* JuneSharedMemory; // Opaque handle for a sharing memory
typedef struct JuneApiContext_T* JuneApiContext;     // Opaque handle for an API context
typedef struct JuneApiMemory_T* JuneApiMemory;       // Opaque handle for a API(OpenGL ES, Vulkan, OpenCL and so on) memory
typedef struct JuneResource_T* JuneResource;         // Opaque handle for a resource with api memory
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
    JuneStype_EGLImageResourceDescriptor = 0x00000007,
    JuneSType_VkImageResourceDescriptor = 0x00000008,
    JuneSType_VkBufferResourceDescriptor = 0x00000009,
    JuneSType_BeginAccessVulkanBuffer = 0x0000000A,
    JuneSType_BeginAccessVulkanTexture = 0x0000000B,
} JuneSType;

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

typedef struct JuneResourceMemoeyEGLImageDescriptor
{
    JuneChainedStruct chain;
    void* eglImage;
} JuneResourceMemoeyEGLDescriptor;

typedef struct JuneSharedMemoryAHardwareBufferDescriptor
{
    JuneChainedStruct chain;
    void* aHardwareBuffer;     // TODO: not allow use this field
    void* aHardwareBufferDesc; // AHardwareBuffer_Desc
} JuneSharedMemoryAhardwareBufferDescriptor;

typedef struct JuneSharedMemoryDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    JuneSharedMemoryUsage usage;
    uint32_t width;
    uint32_t height;
    uint32_t layers;
} JuneSharedMemoryDescriptor;

typedef struct JuneApiMemoryDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    JuneSharedMemory sharedMemory;
} JuneApiMemoryDescriptor;

// can be chained with JuneResourceDescriptor
typedef struct JuneResourceVkBufferDescriptor
{
    JuneChainedStruct chain;
    void* vkBufferCreateInfo;
} JuneResourceVkBufferDescriptor;

// can be chained with JuneResourceDescriptor
typedef struct JuneResourceCLMemDescriptor
{
    JuneChainedStruct chain;
    // TODO: add more fields if needed
} JuneResourceCLMemDescriptor;

// can be chained with JuneResourceDescriptor
typedef struct JuneResourceVkImageDescriptor
{
    JuneChainedStruct chain;
    void* vkImageCreateInfo;
} JuneResourceVkImageDescriptor;

typedef struct JuneResourceEGLImageDescriptor
{
    JuneChainedStruct chain;
    // TODO: add more fields if needed
} JuneResourceEGLImageDescriptor;

typedef struct JuneResourceDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
} JuneResourceDescriptor;

typedef struct JuneFenceDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    JuneApiMemory apiMemory;
} JuneFenceDescriptor;

typedef struct JuneApiMemoryBeginAccessEGLImageDescriptor
{
    JuneChainedStruct chain;
} JuneApiMemoryBeginAccessEGLImageDescriptor;

typedef struct JuneApiMemoryBeginAccessVkImageDescriptor
{
    JuneChainedStruct chain;
    void* vkSubmitInfo;
    uint32_t oldLayout;
    uint32_t newLayout;
} JuneApiMemoryBeginAccessVkImageDescriptor;

typedef struct JuneApiMemoryBeginAccessVkBufferDescriptor
{
    JuneChainedStruct chain;
    void* vkSubmitInfo;
} JuneApiMemoryBeginAccessVkBufferDescriptor;

typedef struct JuneApiMemoryBeginAccessDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    JuneFence* fences;
    uint32_t fenceCount;
} JuneApiMemoryBeginAccessDescriptor;

typedef struct JuneApiMemoryEndAccessVkImageDescriptor
{
    JuneChainedStruct chain;
    void* vkSubmitInfo;
    uint32_t oldLayout;
    uint32_t newLayout;
} JuneApiMemoryEndAccessVkImageDescriptor;

typedef struct JuneApiMemoryEndAccessDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    JuneFence* fences;
    uint32_t fenceCount;
} JuneApiMemoryEndAccessDescriptor;

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void (*JuneProc)(void);
    typedef JuneProc (*JuneProcGetProcAddress)(StringView name);
    typedef JuneInstance (*JuneProcCreateInstance)(JuneInstanceDescriptor const* desc);

    // for Instance
    typedef JuneSharedMemory (*JuneProcInstanceCreateSharedMemory)(JuneInstance instance, JuneSharedMemoryDescriptor const* descriptor);
    typedef JuneApiContext (*JuneProcInstanceCreateApiContext)(JuneInstance instance, JuneApiContextDescriptor const* desc);
    typedef void (*JuneProcInstanceDestroy)(JuneInstance instance);

    // for Api Context
    typedef JuneApiMemory (*JuneProcApiContextCreateApiMemory)(JuneApiContext context, JuneApiMemoryDescriptor const* descriptor);
    typedef JuneFence (*JuneProcApiContextCreateFence)(JuneApiContext context, JuneFenceDescriptor const* descriptor);
    typedef void (*JuneProcApiContextDestroy)(JuneApiContext context);

    // for Shared Memory
    typedef void (*JuneProcSharedMemoryDestroy)(JuneSharedMemory memory);

    // for Api Memory
    typedef void (*JuneProcApiMemoryBeginAccess)(JuneApiMemory memory, JuneApiMemoryBeginAccessDescriptor const* descriptor);
    typedef void (*JuneProcApiMemoryEndAccess)(JuneApiMemory memory, JuneApiMemoryEndAccessDescriptor const* descriptor);
    typedef void (*JuneProcApiMemoryConnect)(JuneApiMemory srcMemory, JuneApiMemory dstMemory);
    typedef void* (*JuneProcApiMemoryCreateResource)(JuneApiMemory memory, JuneResourceDescriptor const* descriptor);
    typedef void (*JuneProcApiMemoryDestroy)(JuneApiMemory memory);

    // for Fence
    typedef void (*JuneProcFenceDestroy)(JuneFence fence);

#if !defined(JUNE_SKIP_DECLARATIONS)

    JUNE_EXPORT JuneProc juneGetProcAddress(StringView name);
    JUNE_EXPORT JuneInstance juneCreateInstance(JuneInstanceDescriptor const* desc);

    JUNE_EXPORT JuneSharedMemory juneInstanceCreateSharedMemory(JuneInstance instance, JuneSharedMemoryDescriptor const* descriptor);
    JUNE_EXPORT JuneApiContext juneInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc);
    JUNE_EXPORT void juneInstanceDestroy(JuneInstance instance);

    JUNE_EXPORT JuneApiMemory juneApiContextCreateApiMemory(JuneApiContext context, JuneApiMemoryDescriptor const* descriptor);
    JUNE_EXPORT JuneFence juneApiContextCreateFence(JuneApiContext context, JuneFenceDescriptor const* descriptor);
    JUNE_EXPORT void juneApiContextDestroy(JuneApiContext context);

    JUNE_EXPORT void juneApiMemoryBeginAccess(JuneApiMemory memory, JuneApiMemoryBeginAccessDescriptor const* descriptor);
    JUNE_EXPORT void juneApiMemoryEndAccess(JuneApiMemory memory, JuneApiMemoryEndAccessDescriptor const* descriptor);
    JUNE_EXPORT void juneApiMemoryConnect(JuneApiMemory srcMemory, JuneApiMemory dstMemory);
    JUNE_EXPORT void* juneApiMemoryCreateResource(JuneApiMemory memory, JuneResourceDescriptor const* descriptor);
    JUNE_EXPORT void juneApiMemoryDestroy(JuneApiMemory memory);

    JUNE_EXPORT void juneFenceDestroy(JuneFence fence);

#endif // !defined(JUNE_SKIP_DECLARATIONS)

#ifdef __cplusplus
} // extern "C"
#endif

#endif // JUNE_H_