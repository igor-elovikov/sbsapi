
#ifndef SBSAPI_EXPORT_H
#define SBSAPI_EXPORT_H

#ifdef _WIN32

#ifdef SBSAPI_STATIC_DEFINE
#  define SBSAPI_EXPORT
#  define SBSAPI_NO_EXPORT
#else
#  ifndef SBSAPI_EXPORT
#    ifdef sbsapi_EXPORTS
        /* We are building this library */
#      define SBSAPI_EXPORT [[maybe_unused]] __declspec(dllexport)
#    else
        /* We are using this library */
#      define SBSAPI_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef SBSAPI_NO_EXPORT
#    define SBSAPI_NO_EXPORT 
#  endif
#endif

#ifndef SBSAPI_DEPRECATED
#  define SBSAPI_DEPRECATED __declspec(deprecated)
#endif

#ifndef SBSAPI_DEPRECATED_EXPORT
#  define SBSAPI_DEPRECATED_EXPORT SBSAPI_EXPORT SBSAPI_DEPRECATED
#endif

#ifndef SBSAPI_DEPRECATED_NO_EXPORT
#  define SBSAPI_DEPRECATED_NO_EXPORT SBSAPI_NO_EXPORT SBSAPI_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef SBSAPI_NO_DEPRECATED
#    define SBSAPI_NO_DEPRECATED
#  endif
#endif

#else

#define SBSAPI_EXPORT

#endif

#endif /* SBSAPI_EXPORT_H */
