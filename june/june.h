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
typedef struct JuneFence_T* JuneFence;               // Opaque cross-API fence

typedef enum JuneApiType
{
    JuneApiType_Undefined = 0x00000000,
    JuneApiType_Vulkan = 0x00000001,
    JuneApiType_D3D11 = 0x00000002,
    JuneApiType_D3D12 = 0x00000003,
    JuneApiType_OpenGL = 0x00000004,
    JuneApiType_GLES = 0x00000005,
    JuneApiType_CPU = 0x00000006,
} JuneApiType;

typedef enum JuneSType
{
    JuneSType_VulkanContext = 0x00000000,
    JuneSType_D3D11ApiContext = 0x00000001,
    JuneSType_D3D12ApiContext = 0x00000002,
    JuneSType_OpenGLApiContext = 0x00000003,
    JuneSType_GLESContext = 0x00000004,
    JuneSType_CPUContext = 0x00000005,

    JuneSType_EGLImageSharedMemory = 0x00000010,
    JuneSType_AHardwareBufferSharedMemory = 0x00000011,

    JuneSType_EGLImageResourceDescriptor = 0x00000020,
    JuneSType_VkImageResourceDescriptor = 0x00000021,
    JuneSType_VkBufferResourceDescriptor = 0x00000022,

    JuneSType_BeginAccessVkBuffer = 0x00000030,
    JuneSType_BeginAccessVkImage = 0x00000031,

    JuneSType_SharedMemoryExportedEGLSyncKHRSyncObject = 0x000000040,
    JuneSType_SharedMemoryExportedVkSemaphoreSyncObject = 0x000000041,
    JuneSType_SharedMemoryExportedFDSyncObject = 0x000000042,
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
    struct JuneChainedStruct* next;
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

// can be chained with JuneApiContextDescriptor
typedef struct JuneCPUContextDescriptor
{
    JuneChainedStruct chain;
} JuneCPUContextDescriptor;

// can be chained with JuneApiContextDescriptor
typedef struct JuneVulkanContextDescriptor
{
    JuneChainedStruct chain;
    void* vkInstance;
    void* vkPhysicalDevice;
    void* vkDevice;
} JuneVulkanContextDescriptor;

// can be chained with JuneApiContextDescriptor
typedef struct JuneD3D12ApiContextDescriptor
{
    JuneChainedStruct chain;
    void* dxgiAdapter;
    void* d3d12Device;
} JuneD3D12ApiContextDescriptor;

// can be chained with JuneApiContextDescriptor
typedef struct JuneD3D11ApiContextDescriptor
{
    JuneChainedStruct chain;
    void* dxgiAdapter;
    void* d3d11Device;
} JuneD3D11ApiContextDescriptor;

// can be chained with JuneApiContextDescriptor
typedef struct JuneOpenGLApiContextDescriptor
{
    JuneChainedStruct chain;
    void* display;
    void* context;
} JuneOpenGLApiContextDescriptor;

// can be chained with JuneApiContextDescriptor
typedef struct JuneGLESContextDescriptor
{
    JuneChainedStruct chain;
    void* display;
    void* context;
} JuneGLESContextDescriptor;

typedef struct JuneApiContextDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
} JuneApiContextDescriptor;

typedef struct JuneSharedMemoryDXGISharedHandleDescriptor
{
    JuneChainedStruct chain;
    void* handle;
} JuneSharedMemoryDXGISharedHandleDescriptor;

typedef struct JuneResourceMemoeyEGLImageDescriptor
{
    JuneChainedStruct chain;
    void* eglImage;
} JuneResourceMemoeyEGLDescriptor;

typedef struct JuneSharedMemoryAHardwareBufferDescriptor
{
    JuneChainedStruct chain;
    void* aHardwareBuffer;     // AHardwareBuffer
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

typedef struct JuneSharedMemorySyncInfo
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    JuneFence* fences;
    uint32_t fenceCount;
} JuneSharedMemorySyncInfo;

// can be chained with JuneSharedMemoryExportedSyncObject
typedef struct JuneSharedMemoryExportedFDSyncObject
{
    JuneChainedStruct chain;
    void* fds; // file descriptors
    uint32_t fdCount;
} JuneSharedMemoryExportedFDSyncObject;

// can be chained with JuneSharedMemoryExportedSyncObject
typedef struct JuneSharedMemoryExportedEGLSyncKHRSyncObject
{
    JuneChainedStruct chain;
    void* eglSyncs; // EGLSyncKHR
    uint32_t eglSyncCount;
} JuneSharedMemoryExportedEGLSyncKHRSyncObject;

// can be chained with JuneSharedMemoryExportedSyncObject
typedef struct JuneSharedMemoryExportedVkSemaphoreSyncObject
{
    JuneChainedStruct chain;
    void* vkSemaphores; // VkSemaphore
    uint32_t vkSemaphoreCount;
} JuneSharedMemoryExportedVkSemaphoreSyncObject;

typedef struct JuneSharedMemoryExportedSyncObject
{
    JuneChainedStruct* nextInChain;
} JuneSharedMemoryExportedSyncObject;

typedef struct JuneApiContextBeginMemoryAccessDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    JuneSharedMemory sharedMemory;
    const JuneSharedMemorySyncInfo* waitSyncInfo;
    JuneSharedMemoryExportedSyncObject* exportedSyncObject;

} JuneApiContextBeginMemoryAccessDescriptor;

typedef struct JuneApiContextEndMemoryAccessDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    JuneSharedMemory sharedMemory;
    const JuneSharedMemorySyncInfo* signalSyncInfo;
    JuneSharedMemoryExportedSyncObject* exportedSyncObject;
} JuneApiContextEndMemoryAccessDescriptor;

typedef struct JuneResourceVkBufferCreateInfo
{
    JuneChainedStruct const* nextInChain;
    const void* vkBufferCreateInfo; // VkBufferCreateInfo
} JuneResourceVkBufferCreateInfo;

typedef struct JuneResourceVkBufferResultInfo
{
    JuneChainedStruct const* nextInChain;
    void* vkDeviceMemory; // VkDeviceMemory
    void* vkBuffer;       // VkBuffer
} JuneResourceVkBufferResultInfo;

// can be chained with JuneResourceDescriptor
typedef struct JuneResourceVkBufferDescriptor
{
    JuneChainedStruct chain;
    const JuneResourceVkBufferCreateInfo* vkBufferCreateInfo;
    JuneResourceVkBufferResultInfo* vkBufferResultInfo;
} JuneResourceVkBufferDescriptor;

typedef struct JuneResourceCLMemCreateInfo
{
    JuneChainedStruct const* nextInChain;
} JuneResourceCLMemCreateInfo;

typedef struct JuneResourceCLMemResultInfo
{
    JuneChainedStruct const* nextInChain;
    void* clMem; // cl_mem
} JuneResourceCLMemResultInfo;

// can be chained with JuneResourceDescriptor
typedef struct JuneResourceCLMemDescriptor
{
    JuneChainedStruct chain;
    const JuneResourceCLMemCreateInfo* clMemCreateInfo;
    JuneResourceCLMemResultInfo* clMemResultInfo;
} JuneResourceCLMemDescriptor;

typedef struct JuneResourceVkImageCreateInfo
{
    JuneChainedStruct const* nextInChain;
    const void* vkImageCreateInfo; // VkImageCreateInfo
} JuneResourceVkImageCreateInfo;

typedef struct JuneResourceVkImageResultInfo
{
    JuneChainedStruct const* nextInChain;
    void* vkDeviceMemory; // VkDeviceMemory
    void* vkImage;        // VkImage
} JuneResourceVkImageResultInfo;

// can be chained with JuneResourceDescriptor
typedef struct JuneResourceVkImageDescriptor
{
    JuneChainedStruct chain;
    const JuneResourceVkImageCreateInfo* createInfo;
    JuneResourceVkImageResultInfo* resultInfo;
} JuneResourceVkImageDescriptor;

typedef struct JuneResourceEGLImageCreateInfo
{
    JuneChainedStruct const* nextInChain;
} JuneResourceEGLImageCreateInfo;

typedef struct JuneResourceEGLImageResultInfo
{
    JuneChainedStruct const* nextInChain;
    void* eglClientBuffer; // EGLClientBuffer
    void* eglImage;        // EGLImageKHR
} JuneResourceEGLImageResultInfo;

typedef struct JuneResourceEGLImageDescriptor
{
    JuneChainedStruct chain;
    const JuneResourceEGLImageCreateInfo* eglImageCreateInfo;
    JuneResourceEGLImageResultInfo* eglImageResultInfo;
} JuneResourceEGLImageDescriptor;

typedef struct JuneResourceDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    JuneSharedMemory sharedMemory;
} JuneResourceDescriptor;

typedef struct JuneFenceDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
} JuneFenceDescriptor;

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
    typedef void (*JuneProcApiContextCreateResource)(JuneApiContext apiContext, JuneResourceDescriptor const* descriptor);
    typedef JuneFence (*JuneProcApiContextCreateFence)(JuneApiContext apiContext, JuneFenceDescriptor const* descriptor);
    typedef void (*JuneProcApiContextBeginMemoryAccess)(JuneApiContext apiContext, JuneApiContextBeginMemoryAccessDescriptor const* descriptor);
    typedef void (*JuneProcApiContextEndMemoryAccess)(JuneApiContext apiContext, JuneApiContextEndMemoryAccessDescriptor const* descriptor);
    typedef void (*JuneProcApiContextDestroy)(JuneApiContext apiContext);

    // for Shared Memory
    typedef void (*JuneProcSharedMemoryDestroy)(JuneSharedMemory memory);

    // for Fence
    typedef void (*JuneProcFenceDestroy)(JuneFence fence);

#if !defined(JUNE_SKIP_DECLARATIONS)

    JUNE_EXPORT JuneProc juneGetProcAddress(StringView name);
    JUNE_EXPORT JuneInstance juneCreateInstance(JuneInstanceDescriptor const* desc);

    JUNE_EXPORT JuneSharedMemory juneInstanceCreateSharedMemory(JuneInstance instance, JuneSharedMemoryDescriptor const* descriptor);
    JUNE_EXPORT JuneApiContext juneInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc);
    JUNE_EXPORT void juneInstanceDestroy(JuneInstance instance);

    JUNE_EXPORT void juneApiContextCreateResource(JuneApiContext apiContext, JuneResourceDescriptor const* descriptor);
    JUNE_EXPORT JuneFence juneApiContextCreateFence(JuneApiContext apiContext, JuneFenceDescriptor const* descriptor);
    JUNE_EXPORT void juneApiContextDestroy(JuneApiContext apiContext);

    JUNE_EXPORT void juneApiContextBeginMemoryAccess(JuneApiContext apiContext, JuneApiContextBeginMemoryAccessDescriptor const* descriptor);
    JUNE_EXPORT void juneApiContextEndMemoryAccess(JuneApiContext apiContext, JuneApiContextEndMemoryAccessDescriptor const* descriptor);
    JUNE_EXPORT void juneSharedMemoryDestroy(JuneSharedMemory memory);

    JUNE_EXPORT void juneFenceDestroy(JuneFence fence);

#endif // !defined(JUNE_SKIP_DECLARATIONS)

#ifdef __cplusplus
} // extern "C"
#endif

#endif // JUNE_H_