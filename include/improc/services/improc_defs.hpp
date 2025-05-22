#ifndef IMPROC_SERVICES_DEFINITIONS_HPP
#define IMPROC_SERVICES_DEFINITIONS_HPP

#if defined(_MSC_VER) && !defined(__MINGW32__) && !defined(__MINGW64__)
    // Pure MSVC + Clang with MSVC libraries
    #define IMPROC_EXPORT_TEMPLATE_DECLARATION_SERVICES
    #define IMPROC_EXPORT_TEMPLATE_DEFINITION_SERVICES IMPROC_API
#elif defined(__clang__)
    // Clang (non-Windows) + LLVM-MinGW
    #define IMPROC_EXPORT_TEMPLATE_DECLARATION_SERVICES IMPROC_API
    #define IMPROC_EXPORT_TEMPLATE_DEFINITION_SERVICES
#else
    // Fallback for other compilers/platforms
    // GCC (non-Windows) + GCC-MinGW + other
    #define IMPROC_EXPORT_TEMPLATE_DECLARATION_SERVICES
    #define IMPROC_EXPORT_TEMPLATE_DEFINITION_SERVICES
#endif

#endif