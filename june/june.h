/*
 * June is a cross-platform library that unifies external GPU memory and
 * fence synchronization across Vulkan, D3D11, D3D12, OpenGL, GLES and
 * Metal.
 *
 * The API is intentionally small and mirrors the style of Vulkan. Opaque
 * handles are used for instances, API contexts, shared memory objects and
 * fences. Descriptor structures contain a `JuneChainedStruct` header so
 * extensions can be chained together when additional information is
 * required.
 *
 * Supported functionality
 * -----------------------
 * - Import shared memory from platform handles. The library does not
 *   allocate GPU memory on its own.
 * - Create fences using an imported synchronization handle such as
 *   EGLSync or SyncFD, reset them with a new handle and export them
 *   to other APIs.
 * - Imported memory can then be used to create API specific resources
 *   such as Vulkan images or EGLImages.
 * - Platform memory interfaces handle AHardwareBuffer, DMABuf and IOSurface
 *   objects.
 * - Synchronization primitives cover SyncFD, EGLSync, Vulkan semaphores and
 *   MTLSharedFence.
 * - Create API contexts for the following backends: Vulkan, D3D11, D3D12,
 *   OpenGL, GLES, Metal or a "no API" stub implementation.
 * - Retrieve entry points dynamically through `juneGetProcAddress` or link
 *   them directly when building as a shared library.
 *
 * Using June allows applications to share resources between APIs or
 * devices without dealing with platform specific extensions directly.
 */
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

// Graphics API backends that can be used when creating a JuneApiContext. Metal
// is also supported.
typedef enum JuneApiType
{
    JuneApiType_Undefined = 0x00000000,
    JuneApiType_Vulkan = 0x00000001,
    JuneApiType_D3D11 = 0x00000002,
    JuneApiType_D3D12 = 0x00000003,
    JuneApiType_OpenGL = 0x00000004,
    JuneApiType_GLES = 0x00000005,
    JuneApiType_Metal = 0x00000006,
    JuneApiType_NoApi = 0x00000007,
} JuneApiType;

// Platform specific memory handle types that can be wrapped by JuneSharedMemory.
typedef enum JuneMemoryType
{
    JuneMemoryType_Undefined = 0x00000000,
    JuneMemoryType_AHardwareBuffer = 0x00000001,
    JuneMemoryType_DMABuf = 0x00000002,
    JuneMemoryType_IOSurface = 0x00000003,
} JuneMemoryType;

// Supported fence object types for cross-API synchronization.
typedef enum JuneFenceType
{
    JuneFenceType_Undefined = 0x00000000,
    JuneFenceType_SyncFD = 0x00000001,
    JuneFenceType_EGLSync = 0x00000002,
    JuneFenceType_VkSemaphoreOpaqueFD = 0x00000003,
    JuneFenceType_MTLSharedFence = 0x00000004,
} JuneFenceType;

// Structure type values used by all chained descriptors. This is similar to
// Vulkan's `sType` field and allows the implementation to identify the
// contents of a descriptor passed through a `next` pointer chain.
typedef enum JuneSType
{
    JuneSType_VulkanContext = 0x00000000,
    JuneSType_D3D11ApiContext = 0x00000001,
    JuneSType_D3D12ApiContext = 0x00000002,
    JuneSType_OpenGLApiContext = 0x00000003,
    JuneSType_GLESContext = 0x00000004,
    JuneSType_MetalContext = 0x00000005,
    JuneSType_NoApiContext = 0x00000006,

    JuneSType_SharedMemoryEGLImageImportDescriptor = 0x00000010,
    JuneSType_SharedMemoryAHardwareBufferImportDescriptor = 0x00000011,
    JuneSType_SharedMemoryIOSurfaceImportDescriptor = 0x00000012,


    JuneSType_ResourceEGLImageCreateDescriptor = 0x00000030,
    JuneSType_ResourceVkImageCreateDescriptor = 0x00000031,
    JuneSType_ResourceVkBufferCreateDescriptor = 0x00000032,

    // Handle descriptors used with JuneFenceCreateDescriptor and
    // JuneFenceResetDescriptor to import synchronization primitives.
    JuneSType_FenceVkSemaphoreOpaqueFDImportDescriptor = 0x000000040,
    JuneSType_FenceSyncFDImportDescriptor = 0x000000041,
    JuneSType_FenceMTLSharedFenceImportDescriptor = 0x000000042,
    JuneSType_FenceEGLSyncImportDescriptor = 0x000000043,

    JuneSType_FenceEGLSyncExportDescriptor = 0x000000050,
    JuneSType_FenceVkSemaphoreExportDescriptor = 0x000000051,
    JuneSType_FenceSyncFDExportDescriptor = 0x000000052,
    JuneSType_FenceMTLSharedFenceExportDescriptor = 0x000000053,

    JuneSType_FenceEGLSyncResetDescriptor = 0x000000060,
    JuneSType_FenceVkSemaphoreOpaqueFDResetDescriptor = 0x000000061,
    JuneSType_FenceSyncFDResetDescriptor = 0x000000062,
    JuneSType_FenceMTLSharedFenceResetDescriptor = 0x000000063,
} JuneSType;

// Bitmask specifying the intended usage of a shared memory allocation. Multiple
// usages can be ORed together when creating or importing memory.
typedef JuneFlags JuneSharedMemoryUsage;
static const JuneSharedMemoryUsage JuneSharedMemoryUsage_None = 0x0000000000000000;
static const JuneSharedMemoryUsage JuneSharedMemoryUsage_GPUSampledImage = 0x0000000000000001;
static const JuneSharedMemoryUsage JuneSharedMemoryUsage_GPUFramebuffer = 0x0000000000000002;
static const JuneSharedMemoryUsage JuneSharedMemoryUsage_GPUBuffer = 0x0000000000000004;
static const JuneSharedMemoryUsage JuneSharedMemoryUsage_CPURead = 0x0000000000000008;
static const JuneSharedMemoryUsage JuneSharedMemoryUsage_CPUWrite = 0x0000000000000010;

// Base structure placed at the beginning of all descriptor structs. The
// implementation walks the `next` chain to process additional extensions.
typedef struct JuneChainedStruct
{
    struct JuneChainedStruct* next;
    JuneSType sType;
} JuneChainedStruct;

// Simple read-only string representation used throughout the API.
typedef struct StringView
{
    char const* data;
    size_t length;
} StringView;

// 3D extent describing the dimensions of a resource.
typedef struct JuneExtent3D
{
    uint32_t width;
    uint32_t height;
    uint32_t depthOrArrayLayers;
} JuneExtent3D;

// Passed to `juneCreateInstance` to configure global behaviour. Additional
// implementation specific options can be chained via `nextInChain`.
typedef struct JuneInstanceDescriptor
{
    JuneChainedStruct* nextInChain;
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

// can be chained with JuneApiContextDescriptor
typedef struct JuneMetalApiContextDescriptor
{
    JuneChainedStruct chain;
    void* mtlDevice; // MTLDevice
} JuneMetalApiContextDescriptor;

// Descriptor passed to `juneInstanceCreateApiContext` describing the API
// backend that should be initialized. Backend specific parameters can be
// chained via `nextInChain`.
typedef struct JuneApiContextDescriptor
{
    JuneChainedStruct* nextInChain;
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

// can be chained with JuneSharedMemoryImportDescriptor
typedef struct JuneSharedMemoryIOSurfaceImportDescriptor
{
    JuneChainedStruct chain;
    void* ioSurface; // IOSurfaceRef
} JuneSharedMemoryIOSurfaceImportDescriptor;

// Generic descriptor used when importing an existing platform memory handle.
// Additional platform specific information is provided via structures chained
// to `nextInChain`.
typedef struct JuneSharedMemoryImportDescriptor
{
    JuneChainedStruct* nextInChain;
    StringView label;
} JuneSharedMemoryImportDescriptor;


typedef struct JuneResourceVkBufferCreateInfo
{
    JuneChainedStruct* nextInChain;
    const void* vkBufferCreateInfo; // VkBufferCreateInfo
} JuneResourceVkBufferCreateInfo;

typedef struct JuneResourceVkBufferResultInfo
{
    JuneChainedStruct* nextInChain;
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
    JuneChainedStruct* nextInChain;
} JuneResourceCLMemCreateInfo;

typedef struct JuneResourceCLMemResultInfo
{
    JuneChainedStruct* nextInChain;
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
    JuneChainedStruct* nextInChain;
    const void* vkImageCreateInfo; // VkImageCreateInfo
} JuneResourceVkImageCreateInfo;

typedef struct JuneResourceVkImageResultInfo
{
    JuneChainedStruct* nextInChain;
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
    JuneChainedStruct* nextInChain;
} JuneResourceEGLImageCreateInfo;

typedef struct JuneResourceEGLImageResultInfo
{
    JuneChainedStruct* nextInChain;
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

// Used with `juneApiContextCreateResource` to create API specific objects such
// as images or buffers backed by a previously imported/shared memory block. The
// details of the object to create are described by the chained structures.
typedef struct JuneResourceCreateDescriptor
{
    JuneChainedStruct* nextInChain;
    JuneSharedMemory sharedMemory;
} JuneResourceCreateDescriptor;

// can be chained with JuneFenceCreateDescriptor or JuneFenceResetDescriptor
typedef struct JuneFenceVkSemaphoreOpaqueFDDescriptor
{
    JuneChainedStruct chain;
    int opaqueFD;
} JuneFenceVkSemaphoreOpaqueFDDescriptor;

// can be chained with JuneFenceCreateDescriptor or JuneFenceResetDescriptor
typedef struct JuneFenceSyncFDImportDescriptor
{
    JuneChainedStruct chain;
    int syncFD;
} JuneFenceSyncFDImportDescriptor;

// can be chained with JuneFenceCreateDescriptor or JuneFenceResetDescriptor
typedef struct JuneFenceMTLSharedFenceImportDescriptor
{
    JuneChainedStruct chain;
    void* mtlSharedFence; // MTLSharedFence
} JuneFenceMTLSharedFenceImportDescriptor;

// can be chained with JuneFenceCreateDescriptor
typedef struct JuneFenceEGLSyncImportDescriptor
{
    JuneChainedStruct chain;
    void* eglSync; // EGLSyncKHR
} JuneFenceEGLSyncImportDescriptor;

// can be chained with JuneFenceResetDescriptor
typedef struct JuneFenceEGLSyncResetDescriptor
{
    JuneChainedStruct chain;
    void* eglSync; // EGLSyncKHR
} JuneFenceEGLSyncResetDescriptor;

// can be chained with JuneFenceResetDescriptor
typedef struct JuneFenceVkSemaphoreOpaqueFDResetDescriptor
{
    JuneChainedStruct chain;
    int opaqueFD;
} JuneFenceVkSemaphoreOpaqueFDResetDescriptor;

// can be chained with JuneFenceResetDescriptor
typedef struct JuneFenceSyncFDResetDescriptor
{
    JuneChainedStruct chain;
    int syncFD;
} JuneFenceSyncFDResetDescriptor;

// can be chained with JuneFenceResetDescriptor
typedef struct JuneFenceMTLSharedFenceResetDescriptor
{
    JuneChainedStruct chain;
    void* mtlSharedFence; // MTLSharedFence
} JuneFenceMTLSharedFenceResetDescriptor;

// Base descriptor used with `juneFenceReset` indicating which fence is being
// reset. Specific reset operations (for example providing a new file
// descriptor) can be chained via `nextInChain`.
typedef struct JuneFenceResetDescriptor
{
    JuneChainedStruct* nextInChain;
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

// can be chained with JuneFenceExportDescriptor
typedef struct JuneFenceMTLSharedFenceExportDescriptor
{
    JuneChainedStruct chain;
    void* mtlSharedFence; // MTLSharedFence
} JuneFenceMTLSharedFenceExportDescriptor;

// Used with `juneApiContextExportFence` to obtain a platform specific handle
// from an existing fence object. The returned handle is written into the
// structure chained via `nextInChain`.
typedef struct JuneFenceExportDescriptor
{
    JuneChainedStruct* nextInChain;
    JuneFence fence;
} JuneFenceExportDescriptor;

// Descriptor passed to `juneInstanceCreateFence` describing the type of fence
// to create. Additional platform specific parameters such as the handle to
// import can be chained via `nextInChain`.
typedef struct JuneFenceCreateDescriptor
{
    JuneChainedStruct* nextInChain;
    StringView label;
    JuneFenceType type;
} JuneFenceCreateDescriptor;

#ifdef __cplusplus
extern "C"
{
#endif

    // Function pointer typedefs for dynamically loading the API. Applications
    // can query addresses via `juneGetProcAddress` and call the retrieved
    // functions using these signatures.

    typedef void (*JuneProc)(void);
    typedef JuneProc (*JuneProcGetProcAddress)(StringView name);
    typedef JuneInstance (*JuneProcCreateInstance)(JuneInstanceDescriptor const* desc);

    // for Instance
    typedef JuneSharedMemory (*JuneProcInstanceImportSharedMemory)(JuneInstance instance, JuneSharedMemoryImportDescriptor const* descriptor);
    typedef JuneFence (*JuneProcInstanceCreateFence)(JuneInstance instance, JuneFenceCreateDescriptor const* descriptor);
    typedef JuneApiContext (*JuneProcInstanceCreateApiContext)(JuneInstance instance, JuneApiContextDescriptor const* desc);
    typedef void (*JuneProcInstanceDestroy)(JuneInstance instance);

    // for Api Context
    typedef void (*JuneProcApiContextCreateResource)(JuneApiContext apiContext, JuneResourceCreateDescriptor const* descriptor);
    typedef void (*JuneProcApiContextExportFence)(JuneApiContext apiContext, JuneFenceExportDescriptor const* descriptor);
    typedef void (*JuneProcApiContextDestroy)(JuneApiContext apiContext);

    // for Shared Memory
    typedef void (*JuneProcSharedMemoryDestroy)(JuneSharedMemory memory);

    // for Fence
    typedef void (*JuneProcFenceReset)(JuneFence fence, JuneFenceResetDescriptor const* descriptor);
    typedef void (*JuneProcFenceDestroy)(JuneFence fence);

#if !defined(JUNE_SKIP_DECLARATIONS)

    // Retrieve a function pointer for one of the API entry points.
    JUNE_EXPORT JuneProc juneGetProcAddress(StringView name);
    // Create and destroy the top level instance which owns all other objects.
    JUNE_EXPORT JuneInstance juneCreateInstance(JuneInstanceDescriptor const* desc);

    // Import shared memory objects on the instance.
    JUNE_EXPORT JuneSharedMemory juneInstanceImportSharedMemory(JuneInstance instance, JuneSharedMemoryImportDescriptor const* descriptor);
    // Create fence objects using an imported synchronization handle.
    JUNE_EXPORT JuneFence juneInstanceCreateFence(JuneInstance instance, JuneFenceCreateDescriptor const* descriptor);
    // Create an API context for a specific backend and destroy the instance when finished.
    JUNE_EXPORT JuneApiContext juneInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc);
    JUNE_EXPORT void juneInstanceDestroy(JuneInstance instance);

    // Operations performed on an API context.
    JUNE_EXPORT void juneApiContextCreateResource(JuneApiContext apiContext, JuneResourceCreateDescriptor const* descriptor);
    JUNE_EXPORT void juneApiContextExportFence(JuneApiContext apiContext, JuneFenceExportDescriptor const* descriptor);
    JUNE_EXPORT void juneApiContextDestroy(JuneApiContext apiContext);

    // Destroy shared memory and fence objects when no longer needed.
    JUNE_EXPORT void juneSharedMemoryDestroy(JuneSharedMemory memory);

    JUNE_EXPORT void juneFenceReset(JuneFence fence, JuneFenceResetDescriptor const* descriptor);
    JUNE_EXPORT void juneFenceDestroy(JuneFence fence);

#endif // !defined(JUNE_SKIP_DECLARATIONS)

#ifdef __cplusplus
} // extern "C"
#endif

#endif // JUNE_H_