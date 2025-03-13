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

#if !defined(JUNE_OBJECT_ATTRIBUTE)
#define JUNE_OBJECT_ATTRIBUTE
#endif
#if !defined(JUNE_ENUM_ATTRIBUTE)
#define JUNE_ENUM_ATTRIBUTE
#endif
#if !defined(JUNE_STRUCTURE_ATTRIBUTE)
#define JUNE_STRUCTURE_ATTRIBUTE
#endif
#if !defined(JUNE_FUNCTION_ATTRIBUTE)
#define JUNE_FUNCTION_ATTRIBUTE
#endif
#if !defined(JUNE_NULLABLE)
#define JUNE_NULLABLE
#endif

typedef struct JuneInstance_T* JuneInstance;     // Opaque handle for an API instance
typedef struct JuneApiContext_T* JuneApiContext; // Opaque handle for an API context
typedef struct JuneBuffer_T* JuneBuffer;         // Opaque handle for a shared buffer
typedef struct JuneTexture_T* JuneTexture;       // Opaque handle for a shared texture
typedef struct JuneSemaphore_T* JuneSemaphore;   // Opaque cross-API semaphore
typedef struct JuneFence_T* JuneFence;           // Opaque cross-API fence

struct JuneApiContextDescriptor;

typedef enum JuneApiType
{
    JUNE_VULKAN,
    JUNE_D3D11,
    JUNE_D3D12,
    JUNE_OPENGL,
    JUNE_OPENGL_ES
} JuneApiType;

typedef struct JuneInstanceDescriptor
{
} JuneInstanceDescriptor;

typedef struct JuneApiContextDescriptor
{
} JuneApiContextDescriptor;

typedef struct JuneVulkanApiContextDescriptor
{
} JuneApiContextDescriptor;

typedef struct JuneD3D12ApiContextDescriptor
{
} JuneApiContextDescriptor;

typedef struct JuneD3D11ApiContextDescriptor
{
} JuneApiContextDescriptor;

typedef struct JuneOpenGLApiContextDescriptor
{
} JuneApiContextDescriptor;

typedef struct JuneGLESApiContextDescriptor
{
} JuneApiContextDescriptor;

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*JuneProcCreateInstance)(JuneInstanceDescriptor const* desc);
typedef void (*JuneProcDestroyInstance)(JuneInstance instance);
typedef void (*JuneProcCreateApiContext)(JuneInstance instance, JuneApiContextDescriptor const* desc);
typedef void (*JuneProcDestroyApiContext)(JuneApiContext context);

#if !defined(JUNE_SKIP_DECLARATIONS)

JUNE_EXPORT JuneInstance JuneCreateInstance(JuneInstanceDescriptor const* desc);
JUNE_EXPORT JuneApiContext JuneCreateApiContext(JuneInstance innstance, JuneApiContextDescriptor const* desc);
JUNE_EXPORT void JuneDestroyInstance(JuneInstance instance);
JUNE_EXPORT void JuneDestroyApiContext(JuneApiContext context);

#endif // !defined(JUNE_SKIP_DECLARATIONS)

#ifdef __cplusplus
} // extern "C"
#endif

#endif // JUNE_H_