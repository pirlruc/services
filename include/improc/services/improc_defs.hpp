#ifndef IMPROC_SERVICES_DEFINITIONS_HPP
#define IMPROC_SERVICES_DEFINITIONS_HPP

#if defined(IMPROC_STATIC)
    #define IMPROC_API
#elif defined(_WIN32) || defined(_WIN64) || defined(WINCE)
    #if defined(IMPROC_EXPORTS)     // Building the DLL
        #define IMPROC_API __declspec(dllexport)
    #else                           // Using the DLL
        #define IMPROC_API __declspec(dllimport)
    #endif
#elif (defined(__GNUC__) && __GNUC__ >= 4) || defined(__clang__)
    #define IMPROC_API __attribute__ ((visibility ("default")))
#else
    #define IMPROC_API
#endif

#if defined(_MSC_VER) && !defined(__MINGW32__) && !defined(__MINGW64__)
    // Pure MSVC and Clang with MSVC libraries
    #define IMPROC_EXPORT_TEMPLATE_DECLARATION
    #define IMPROC_EXPORT_TEMPLATE_DEFINITION IMPROC_API
#elif (defined(__MINGW32__) || defined(__MINGW64__))
    // LLVM-MinGW + GCC-MinGW
    #define IMPROC_EXPORT_TEMPLATE_DECLARATION IMPROC_API
    #define IMPROC_EXPORT_TEMPLATE_DEFINITION
#elif defined(__clang__)
    // Other Clang (non-Windows, or not MinGW)
    #define IMPROC_EXPORT_TEMPLATE_DECLARATION IMPROC_API
    #define IMPROC_EXPORT_TEMPLATE_DEFINITION
#elif defined(__GNUC__) && __GNUC__ >= 4
    // GCC (non-Windows)
    #define IMPROC_EXPORT_TEMPLATE_DECLARATION IMPROC_API
    #define IMPROC_EXPORT_TEMPLATE_DEFINITION
#else
    // Fallback for other compilers/platforms
    #define IMPROC_EXPORT_TEMPLATE_DECLARATION
    #define IMPROC_EXPORT_TEMPLATE_DEFINITION
#endif

#define IMPROC_ENUM_KEY_TYPE unsigned int
#define IMPROC_FWD(value) std::forward<decltype(value)>(value)

#endif