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

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum UXDG_XDG_USER_DIR_STANDARD_TYPE
{
    XDG_USER_DIR_STANDARD_TYPE_CUSTOM,
    XDG_USER_DIR_STANDARD_TYPE_DESKTOP_DIR_DEFAULT,
    XDG_USER_DIR_STANDARD_TYPE_DOWNLOAD_DIR_DEFAULT,
    XDG_USER_DIR_STANDARD_TYPE_TEMPLATES_DIR_DEFAULT,
    XDG_USER_DIR_STANDARD_TYPE_PUBLICSHARE_DIR_DEFAULT,
    XDG_USER_DIR_STANDARD_TYPE_DOCUMENTS_DIR_DEFAULT,
    XDG_USER_DIR_STANDARD_TYPE_MUSIC_DIR_DEFAULT,
    XDG_USER_DIR_STANDARD_TYPE_PICTURES_DIR_DEFAULT,
    XDG_USER_DIR_STANDARD_TYPE_VIDEOS_DIR_DEFAULT,
    XDG_USER_DIR_STANDARD_TYPE_SIZE,
} UXDG_XDG_USER_DIR_STANDARD_TYPE;

#ifdef __cplusplus
}
#endif