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

typedef struct JuneInstance_T* JuneInstance;         // Opaque handle for an API instance
typedef struct JuneApiContext_T* JuneApiContext;     // Opaque handle for an API context
typedef struct JuneSharedMemory_T* JuneSharedMemory; // Opaque handle for a shared memory
typedef struct JuneBuffer_T* JuneBuffer;             // Opaque handle for a buffer with shared memory
typedef struct JuneTexture_T* JuneTexture;           // Opaque handle for a texture with shared memory
typedef struct JuneSemaphore_T* JuneSemaphore;       // Opaque cross-API semaphore
typedef struct JuneFence_T* JuneFence;               // Opaque cross-API fence

struct JuneApiContextDescriptor;

typedef enum JuneApiType
{
    JUNE_VULKAN,
    JUNE_D3D11,
    JUNE_D3D12,
    JUNE_OPENGL,
    JUNE_GLES
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
    JuneTextureFormat_R8Unorm = 0x00000001,
    JuneTextureFormat_R8Snorm = 0x00000002,
    JuneTextureFormat_R8Uint = 0x00000003,
    JuneTextureFormat_R8Sint = 0x00000004,
    JuneTextureFormat_R16Uint = 0x00000005,
    JuneTextureFormat_R16Sint = 0x00000006,
    JuneTextureFormat_R16Float = 0x00000007,
    JuneTextureFormat_RG8Unorm = 0x00000008,
    JuneTextureFormat_RG8Snorm = 0x00000009,
    JuneTextureFormat_RG8Uint = 0x0000000A,
    JuneTextureFormat_RG8Sint = 0x0000000B,
    JuneTextureFormat_R32Float = 0x0000000C,
    JuneTextureFormat_R32Uint = 0x0000000D,
    JuneTextureFormat_R32Sint = 0x0000000E,
    JuneTextureFormat_RG16Uint = 0x0000000F,
    JuneTextureFormat_RG16Sint = 0x00000010,
    JuneTextureFormat_RG16Float = 0x00000011,
    JuneTextureFormat_RGBA8Unorm = 0x00000012,
    JuneTextureFormat_RGBA8UnormSrgb = 0x00000013,
    JuneTextureFormat_RGBA8Snorm = 0x00000014,
    JuneTextureFormat_RGBA8Uint = 0x00000015,
    JuneTextureFormat_RGBA8Sint = 0x00000016,
    JuneTextureFormat_BGRA8Unorm = 0x00000017,
    JuneTextureFormat_BGRA8UnormSrgb = 0x00000018,
    JuneTextureFormat_RGB10A2Uint = 0x00000019,
    JuneTextureFormat_RGB10A2Unorm = 0x0000001A,
    JuneTextureFormat_RG11B10Ufloat = 0x0000001B,
    JuneTextureFormat_RGB9E5Ufloat = 0x0000001C,
    JuneTextureFormat_RG32Float = 0x0000001D,
    JuneTextureFormat_RG32Uint = 0x0000001E,
    JuneTextureFormat_RG32Sint = 0x0000001F,
    JuneTextureFormat_RGBA16Uint = 0x00000020,
    JuneTextureFormat_RGBA16Sint = 0x00000021,
    JuneTextureFormat_RGBA16Float = 0x00000022,
    JuneTextureFormat_RGBA32Float = 0x00000023,
    JuneTextureFormat_RGBA32Uint = 0x00000024,
    JuneTextureFormat_RGBA32Sint = 0x00000025,
    JuneTextureFormat_Stencil8 = 0x00000026,
    JuneTextureFormat_Depth16Unorm = 0x00000027,
    JuneTextureFormat_Depth24Plus = 0x00000028,
    JuneTextureFormat_Depth24PlusStencil8 = 0x00000029,
    JuneTextureFormat_Depth32Float = 0x0000002A,
    JuneTextureFormat_Depth32FloatStencil8 = 0x0000002B,
    JuneTextureFormat_BC1RGBAUnorm = 0x0000002C,
    JuneTextureFormat_BC1RGBAUnormSrgb = 0x0000002D,
    JuneTextureFormat_BC2RGBAUnorm = 0x0000002E,
    JuneTextureFormat_BC2RGBAUnormSrgb = 0x0000002F,
    JuneTextureFormat_BC3RGBAUnorm = 0x00000030,
    JuneTextureFormat_BC3RGBAUnormSrgb = 0x00000031,
    JuneTextureFormat_BC4RUnorm = 0x00000032,
    JuneTextureFormat_BC4RSnorm = 0x00000033,
    JuneTextureFormat_BC5RGUnorm = 0x00000034,
    JuneTextureFormat_BC5RGSnorm = 0x00000035,
    JuneTextureFormat_BC6HRGBUfloat = 0x00000036,
    JuneTextureFormat_BC6HRGBFloat = 0x00000037,
    JuneTextureFormat_BC7RGBAUnorm = 0x00000038,
    JuneTextureFormat_BC7RGBAUnormSrgb = 0x00000039,
    JuneTextureFormat_ETC2RGB8Unorm = 0x0000003A,
    JuneTextureFormat_ETC2RGB8UnormSrgb = 0x0000003B,
    JuneTextureFormat_ETC2RGB8A1Unorm = 0x0000003C,
    JuneTextureFormat_ETC2RGB8A1UnormSrgb = 0x0000003D,
    JuneTextureFormat_ETC2RGBA8Unorm = 0x0000003E,
    JuneTextureFormat_ETC2RGBA8UnormSrgb = 0x0000003F,
    JuneTextureFormat_EACR11Unorm = 0x00000040,
    JuneTextureFormat_EACR11Snorm = 0x00000041,
    JuneTextureFormat_EACRG11Unorm = 0x00000042,
    JuneTextureFormat_EACRG11Snorm = 0x00000043,
    JuneTextureFormat_ASTC4x4Unorm = 0x00000044,
    JuneTextureFormat_ASTC4x4UnormSrgb = 0x00000045,
    JuneTextureFormat_ASTC5x4Unorm = 0x00000046,
    JuneTextureFormat_ASTC5x4UnormSrgb = 0x00000047,
    JuneTextureFormat_ASTC5x5Unorm = 0x00000048,
    JuneTextureFormat_ASTC5x5UnormSrgb = 0x00000049,
    JuneTextureFormat_ASTC6x5Unorm = 0x0000004A,
    JuneTextureFormat_ASTC6x5UnormSrgb = 0x0000004B,
    JuneTextureFormat_ASTC6x6Unorm = 0x0000004C,
    JuneTextureFormat_ASTC6x6UnormSrgb = 0x0000004D,
    JuneTextureFormat_ASTC8x5Unorm = 0x0000004E,
    JuneTextureFormat_ASTC8x5UnormSrgb = 0x0000004F,
    JuneTextureFormat_ASTC8x6Unorm = 0x00000050,
    JuneTextureFormat_ASTC8x6UnormSrgb = 0x00000051,
    JuneTextureFormat_ASTC8x8Unorm = 0x00000052,
    JuneTextureFormat_ASTC8x8UnormSrgb = 0x00000053,
    JuneTextureFormat_ASTC10x5Unorm = 0x00000054,
    JuneTextureFormat_ASTC10x5UnormSrgb = 0x00000055,
    JuneTextureFormat_ASTC10x6Unorm = 0x00000056,
    JuneTextureFormat_ASTC10x6UnormSrgb = 0x00000057,
    JuneTextureFormat_ASTC10x8Unorm = 0x00000058,
    JuneTextureFormat_ASTC10x8UnormSrgb = 0x00000059,
    JuneTextureFormat_ASTC10x10Unorm = 0x0000005A,
    JuneTextureFormat_ASTC10x10UnormSrgb = 0x0000005B,
    JuneTextureFormat_ASTC12x10Unorm = 0x0000005C,
    JuneTextureFormat_ASTC12x10UnormSrgb = 0x0000005D,
    JuneTextureFormat_ASTC12x12Unorm = 0x0000005E,
    JuneTextureFormat_ASTC12x12UnormSrgb = 0x0000005F,
    JuneTextureFormat_R16Unorm = 0x00050000,
    JuneTextureFormat_RG16Unorm = 0x00050001,
    JuneTextureFormat_RGBA16Unorm = 0x00050002,
    JuneTextureFormat_R16Snorm = 0x00050003,
    JuneTextureFormat_RG16Snorm = 0x00050004,
    JuneTextureFormat_RGBA16Snorm = 0x00050005,
    JuneTextureFormat_R8BG8Biplanar420Unorm = 0x00050006,
    JuneTextureFormat_R10X6BG10X6Biplanar420Unorm = 0x00050007,
    JuneTextureFormat_R8BG8A8Triplanar420Unorm = 0x00050008,
    JuneTextureFormat_R8BG8Biplanar422Unorm = 0x00050009,
    JuneTextureFormat_R8BG8Biplanar444Unorm = 0x0005000A,
    JuneTextureFormat_R10X6BG10X6Biplanar422Unorm = 0x0005000B,
    JuneTextureFormat_R10X6BG10X6Biplanar444Unorm = 0x0005000C
} JuneTextureFormat;

typedef JuneFlags JuneBufferUsage;
static const JuneBufferUsage JuneBufferUsage_None = 0x0000000000000000;
static const JuneBufferUsage JuneBufferUsage_MapRead = 0x0000000000000001;
static const JuneBufferUsage JuneBufferUsage_MapWrite = 0x0000000000000002;
static const JuneBufferUsage JuneBufferUsage_CopySrc = 0x0000000000000004;
static const JuneBufferUsage JuneBufferUsage_CopyDst = 0x0000000000000008;
static const JuneBufferUsage JuneBufferUsage_Index = 0x0000000000000010;
static const JuneBufferUsage JuneBufferUsage_Vertex = 0x0000000000000020;
static const JuneBufferUsage JuneBufferUsage_Uniform = 0x0000000000000040;
static const JuneBufferUsage JuneBufferUsage_Storage = 0x0000000000000080;
static const JuneBufferUsage JuneBufferUsage_Indirect = 0x0000000000000100;

typedef JuneFlags JuneTextureUsage;
static const JuneTextureUsage JuneTextureUsage_None = 0x0000000000000000;
static const JuneTextureUsage JuneTextureUsage_CopySrc = 0x0000000000000001;
static const JuneTextureUsage JuneTextureUsage_CopyDst = 0x0000000000000002;
static const JuneTextureUsage JuneTextureUsage_TextureBinding = 0x0000000000000004;
static const JuneTextureUsage JuneTextureUsage_StorageBinding = 0x0000000000000008;
static const JuneTextureUsage JuneTextureUsage_RenderAttachment = 0x0000000000000010;
static const JuneTextureUsage JuneTextureUsage_TransientAttachment = 0x0000000000000020;
static const JuneTextureUsage JuneTextureUsage_StorageAttachment = 0x0000000000000040;

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
} JuneInstanceDescriptor;

typedef struct JuneApiContextDescriptor
{
    JuneChainedStruct const* nextInChain;
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
    void* handle;
    // bool useKeyedMutex;
} JuneSharedMemoryDXGISharedHandleDescriptor;

typedef struct JuneTextureMemoeyEGLImageDescriptor
{
    JuneChainedStruct chain;
    void* eglImage;
} JuneTextureMemoeyEGLDescriptor;

typedef struct JuneSharedMemoryAHardwareBufferDescriptor
{
    JuneChainedStruct chain;
    void* handle;
    // bool useExternalFormat;
} JuneSharedMemoryAhardwareBufferDescriptor;

typedef struct JuneSharedMemoryDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
} JuneSharedMemoryDescriptor;

typedef struct JuneBufferDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    size_t size;
    JuneBufferUsage usage;
} JuneBufferDescriptor;

typedef struct JuneTextureDescriptor
{
    JuneChainedStruct const* nextInChain;
    StringView label;
    JuneTextureUsage usage;
    JuneTextureDimension dimension;
    JuneExtent3D size;
    JuneTextureFormat format;
    uint32_t mipLevelCount;
    uint32_t sampleCount;
} JuneTextureDescriptor;

typedef struct JuneFenceDescriptor
{
    JuneChainedStruct const* nextInChain;
} JuneFenceDescriptor;

typedef struct JuneBeginAccessDescriptor
{
    JuneChainedStruct const* nextInChain;
    JuneFence* fences;
    uint32_t fenceCount;
} JuneBeginAccessDescriptor;

typedef JuneBeginAccessDescriptor JuneEndAccessDescriptor;
typedef JuneBeginAccessDescriptor JuneBeginTextureAccessDescriptor;
typedef JuneBeginAccessDescriptor JuneEndTextureAccessDescriptor;

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
    typedef void (*JuneProcApiContextDestroy)(JuneApiContext context);

    typedef JuneBuffer (*JuneProcSharedMemoryCreateBuffer)(JuneSharedMemory memory, JuneBufferDescriptor const* descriptor);
    typedef JuneTexture (*JuneProcSharedMemoryCreateTexture)(JuneSharedMemory memory, JuneTextureDescriptor const* descriptor);
    typedef void (*JuneProcSharedMemoryBeginAccess)(JuneSharedMemory memory, JuneBeginAccessDescriptor const* descriptor);
    typedef void (*JuneProcSharedMemoryEndAccess)(JuneSharedMemory memory, JuneEndAccessDescriptor const* descriptor);
    typedef void (*JuneProcSharedMemoryDestroy)(JuneSharedMemory memory);

    typedef void (*JuneProcBufferDestroy)(JuneBuffer buffer);
    typedef JuneFence (*JuneProcBufferCreateFence)(JuneBuffer buffer, JuneFenceDescriptor const* descriptor);

    typedef JuneFence (*JuneProcTextureCreateFence)(JuneTexture texture, JuneFenceDescriptor const* descriptor);
    typedef void (*JuneProcTextureDestroy)(JuneTexture texture);

    typedef void (*JuneProcFenceDestroy)(JuneFence fence);

#if !defined(JUNE_SKIP_DECLARATIONS)

    JUNE_EXPORT JuneProc juneGetProcAddress(StringView name);
    JUNE_EXPORT JuneInstance juneCreateInstance(JuneInstanceDescriptor const* desc);

    JUNE_EXPORT JuneApiContext juneInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc);
    JUNE_EXPORT void juneInstanceDestroy(JuneInstance instance);

    JUNE_EXPORT JuneSharedMemory juneApiContextCreateSharedMemory(JuneApiContext context, JuneSharedMemoryDescriptor const* descriptor);
    JUNE_EXPORT void juneApiContextDestroy(JuneApiContext context);

    JUNE_EXPORT JuneBuffer juneSharedMemoryCreateBuffer(JuneSharedMemory memory, JuneBufferDescriptor const* descriptor);
    JUNE_EXPORT JuneTexture juneSharedMemoryCreateTexture(JuneSharedMemory memory, JuneTextureDescriptor const* descriptor);
    JUNE_EXPORT void juneSharedMemoryBeginAccess(JuneSharedMemory memory, JuneBeginAccessDescriptor const* descriptor);
    JUNE_EXPORT void juneSharedMemoryEndAccess(JuneSharedMemory memory, JuneEndAccessDescriptor const* descriptor);
    JUNE_EXPORT void juneSharedMemoryDestroy(JuneSharedMemory memory);

    JUNE_EXPORT JuneFence juneBufferCreateFence(JuneBuffer buffer, JuneFenceDescriptor const* descriptor);
    JUNE_EXPORT void juneBufferDestroy(JuneBuffer buffer);

    JUNE_EXPORT JuneFence juneTextureCreateFence(JuneTexture texture, JuneFenceDescriptor const* descriptor);
    JUNE_EXPORT void juneTextureDestroy(JuneTexture texture);

    JUNE_EXPORT void juneFenceDestroy(JuneFence fence);

#endif // !defined(JUNE_SKIP_DECLARATIONS)

#ifdef __cplusplus
} // extern "C"
#endif

#endif // JUNE_H_