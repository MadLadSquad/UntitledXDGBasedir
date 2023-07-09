#pragma once
#ifdef UVK_LOG_EXPORT_FROM_LIBRARY
    #ifdef _WIN32
            #ifdef UVK_LIB_COMPILE
                #define UVK_PUBLIC_API __declspec(dllexport)
                #define UVK_PUBLIC_TMPL_API __declspec(dllexport)
            #else
                #define UVK_PUBLIC_API __declspec(dllimport)
                #define UVK_PUBLIC_TMPL_API
            #endif
        #else
            #define UVK_PUBLIC_API
            #define UVK_PUBLIC_TMPL_API
        #endif
#else
    #define UVK_PUBLIC_API
    #define UVK_PUBLIC_TMPL_API
#endif