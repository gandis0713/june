#ifndef JUNE_H_
#define JUNE_H_

#if defined(JUNE_SHARED_LIBRARY)
#    if defined(_WIN32)
#        if defined(JUNE_IMPLEMENTATION)
#            define JUNE_EXPORT __declspec(dllexport)
#        else
#            define JUNE_EXPORT __declspec(dllimport)
#        endif
#    else  // defined(_WIN32)
#        if defined(JUNE_IMPLEMENTATION)
#            define JUNE_EXPORT __attribute__((visibility("default")))
#        else
#            define JUNE_EXPORT
#        endif
#    endif  // defined(_WIN32)
#else       // defined(JUNE_SHARED_LIBRARY)
#    define JUNE_EXPORT
#endif  // defined(JUNE_SHARED_LIBRARY)

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

#endif // JUNE_H_