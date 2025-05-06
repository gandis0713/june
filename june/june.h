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
    JuneApiType_NoApi = 0x00000006,
} JuneApiType;

typedef enum JuneMemoryType
{
    JuneMemoryType_Undefined = 0x00000000,
    JuneMemoryType_AHardwareBuffer = 0x00000001,
    JuneMemoryType_DMABuf = 0x00000002,
} JuneMemoryType;

typedef enum JuneSType
{
    JuneSType_VulkanContext = 0x00000000,
    JuneSType_D3D11ApiContext = 0x00000001,
    JuneSType_D3D12ApiContext = 0x00000002,
    JuneSType_OpenGLApiContext = 0x00000003,
    JuneSType_GLESContext = 0x00000004,
    JuneSType_NoApiContext = 0x00000005,

    JuneSType_SharedMemoryEGLImageImportDescriptor = 0x00000010,
    JuneSType_SharedMemoryAHardwareBufferImportDescriptor = 0x00000011,

    JuneSType_SharedMemoryEGLImageCreateDescriptor = 0x00000020,
    JuneSType_SharedMemoryAHardwareBufferCreateDescriptor = 0x00000021,

    JuneSType_ResourceEGLImageCreateDescriptor = 0x00000030,
    JuneSType_ResourceVkImageCreateDescriptor = 0x00000031,
    JuneSType_ResourceVkBufferCreateDescriptor = 0x00000032,

    JuneSType_FenceEGLSyncExportDescriptor = 0x000000040,
    JuneSType_FenceVkSemaphoreExportDescriptor = 0x000000041,
    JuneSType_FenceSyncFDExportDescriptor = 0x000000042,
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
typedef struct JuneNoApiContextDescriptor
{
    JuneChainedStruct chain;
} JuneNoApiContextDescriptor;

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

// can be chained with JuneSharedMemoryImportDescriptor
typedef struct JuneSharedMemoryDXGISharedHandleImportDescriptor
{
    JuneChainedStruct chain;
    void* handle;
} JuneSharedMemoryDXGISharedHandleImportDescriptor;

// can be chained with JuneSharedMemoryImportDescriptor
typedef struct JuneSharedMemoryEGLImageImportDescriptor
{
    JuneChainedStruct chain;
    void* eglImage;
} JuneSharedMemoryEGLImageImportDescriptor;

// can be chained with JuneSharedMemoryImportDescriptor
typedef struct JuneSharedMemoryAHardwareBufferImportDescriptor
{
    JuneChainedStruct chain;
    void* aHardwareBuffer; // AHardwareBuffer
} JuneSharedMemoryAhardwareBufferImportDescriptor;

typedef struct JuneSharedMemoryImportDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
} JuneSharedMemoryImportDescriptor;

// can be chained with JuneSharedMemoryCreateDescriptor
typedef struct JuneSharedMemoryEGLImageCreateDescriptor
{
    JuneChainedStruct chain;
} JuneSharedMemoryEGLImageCreateDescriptor;

// can be chained with JuneSharedMemoryCreateDescriptor
typedef struct JuneSharedMemoryAHardwareBufferCreateDescriptor
{
    JuneChainedStruct chain;
    void* aHardwareBufferDesc; // AHardwareBuffer_Desc
} JuneSharedMemoryAHardwareBufferCreateDescriptor;

typedef struct JuneSharedMemoryCreateDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
} JuneSharedMemoryCreateDescriptor;

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

// can be chained with JuneResourceCreateDescriptor
typedef struct JuneResourceVkBufferCreateDescriptor
{
    JuneChainedStruct chain;
    const JuneResourceVkBufferCreateInfo* vkBufferCreateInfo;
    JuneResourceVkBufferResultInfo* vkBufferResultInfo;
} JuneResourceVkBufferCreateDescriptor;

typedef struct JuneResourceCLMemCreateInfo
{
    JuneChainedStruct const* nextInChain;
} JuneResourceCLMemCreateInfo;

typedef struct JuneResourceCLMemResultInfo
{
    JuneChainedStruct const* nextInChain;
    void* clMem; // cl_mem
} JuneResourceCLMemResultInfo;

// can be chained with JuneResourceCreateDescriptor
typedef struct JuneResourceCLMemCreateDescriptor
{
    JuneChainedStruct chain;
    const JuneResourceCLMemCreateInfo* clMemCreateInfo;
    JuneResourceCLMemResultInfo* clMemResultInfo;
} JuneResourceCLMemCreateDescriptor;

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

// can be chained with JuneResourceCreateDescriptor
typedef struct JuneResourceVkImageCreateDescriptor
{
    JuneChainedStruct chain;
    const JuneResourceVkImageCreateInfo* createInfo;
    JuneResourceVkImageResultInfo* resultInfo;
} JuneResourceVkImageCreateDescriptor;

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

// can be chained with JuneResourceCreateDescriptor
typedef struct JuneResourceEGLImageCreateDescriptor
{
    JuneChainedStruct chain;
    const JuneResourceEGLImageCreateInfo* eglImageCreateInfo;
    JuneResourceEGLImageResultInfo* eglImageResultInfo;
} JuneResourceEGLImageCreateDescriptor;

typedef struct JuneResourceCreateDescriptor
{
    JuneChainedStruct const* nextInChain;
    JuneSharedMemory sharedMemory;
} JuneResourceCreateDescriptor;

typedef struct JuneFenceResetDescriptor
{
    JuneChainedStruct const* nextInChain;
} JuneFenceResetDescriptor;

// can be chained with JuneFenceExportDescriptor
typedef struct JuneFenceSyncFDExportDescriptor
{
    JuneChainedStruct chain;
    int syncFD;
} JuneFenceSyncFDExportDescriptor;

// can be chained with JuneFenceExportDescriptor
typedef struct JuneFenceEGLSyncExportDescriptor
{
    JuneChainedStruct chain;
    void* eglSync;
} JuneFenceEGLSyncExportDescriptor;

// can be chained with JuneFenceExportDescriptor
typedef struct JuneFenceVkSemaphoreExportDescriptor
{
    JuneChainedStruct chain;
    void* vkSemaphore; // VkSemaphore
} JuneFenceVkSemaphoreExportDescriptor;

typedef struct JuneFenceExportDescriptor
{
    JuneChainedStruct* nextInChain;
} JuneFenceExportDescriptor;

typedef struct JuneFenceWaitDescriptor
{
    JuneChainedStruct* nextInChain;
} JuneFenceWaitDescriptor;

typedef struct JuneFenceCreateDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
} JuneFenceCreateDescriptor;

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void (*JuneProc)(void);
    typedef JuneProc (*JuneProcGetProcAddress)(StringView name);
    typedef JuneInstance (*JuneProcCreateInstance)(JuneInstanceDescriptor const* desc);

    // for Instance
    typedef JuneSharedMemory (*JuneProcInstanceImportSharedMemory)(JuneInstance instance, JuneSharedMemoryImportDescriptor const* descriptor);
    typedef JuneSharedMemory (*JuneProcInstanceCreateSharedMemory)(JuneInstance instance, JuneSharedMemoryCreateDescriptor const* descriptor);
    typedef JuneApiContext (*JuneProcInstanceCreateApiContext)(JuneInstance instance, JuneApiContextDescriptor const* desc);
    typedef void (*JuneProcInstanceDestroy)(JuneInstance instance);

    // for Api Context
    typedef void (*JuneProcApiContextCreateResource)(JuneApiContext apiContext, JuneResourceCreateDescriptor const* descriptor);
    typedef JuneFence (*JuneProcApiContextCreateFence)(JuneApiContext apiContext, JuneFenceCreateDescriptor const* descriptor);
    typedef void (*JuneProcApiContextDestroy)(JuneApiContext apiContext);

    // for Shared Memory
    typedef void (*JuneProcSharedMemoryDestroy)(JuneSharedMemory memory);

    // for Fence
    typedef void (*JuneProcFenceReset)(JuneFence fence, JuneFenceResetDescriptor const* descriptor);
    typedef void (*JuneProcFenceExport)(JuneFence fence, JuneFenceExportDescriptor const* descriptor);
    typedef void (*JuneProcFenceDestroy)(JuneFence fence);

#if !defined(JUNE_SKIP_DECLARATIONS)

    JUNE_EXPORT JuneProc juneGetProcAddress(StringView name);
    JUNE_EXPORT JuneInstance juneCreateInstance(JuneInstanceDescriptor const* desc);

    JUNE_EXPORT JuneSharedMemory juneInstanceImportSharedMemory(JuneInstance instance, JuneSharedMemoryImportDescriptor const* descriptor);
    JUNE_EXPORT JuneSharedMemory juneInstanceCreateSharedMemory(JuneInstance instance, JuneSharedMemoryCreateDescriptor const* descriptor);
    JUNE_EXPORT JuneApiContext juneInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc);
    JUNE_EXPORT void juneInstanceDestroy(JuneInstance instance);

    JUNE_EXPORT void juneApiContextCreateResource(JuneApiContext apiContext, JuneResourceCreateDescriptor const* descriptor);
    JUNE_EXPORT JuneFence juneApiContextCreateFence(JuneApiContext apiContext, JuneFenceCreateDescriptor const* descriptor);
    JUNE_EXPORT void juneApiContextDestroy(JuneApiContext apiContext);

    JUNE_EXPORT void juneSharedMemoryDestroy(JuneSharedMemory memory);

    JUNE_EXPORT void juneFenceReset(JuneFence fence, JuneFenceResetDescriptor const* descriptor);
    JUNE_EXPORT void juneFenceExport(JuneFence fence, JuneFenceExportDescriptor const* descriptor);
    JUNE_EXPORT void juneFenceDestroy(JuneFence fence);

#endif // !defined(JUNE_SKIP_DECLARATIONS)

#ifdef __cplusplus
} // extern "C"
#endif

#endif // JUNE_H_