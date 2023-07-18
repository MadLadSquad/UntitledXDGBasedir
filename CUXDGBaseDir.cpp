#include "CUXDGBaseDir.h"
#include "UXDGBaseDir.hpp"
#include <cstring>

#define str(x) strdup(x.c_str())

char** copyArray(const std::vector<std::string>& arr, size_t* sz)
{
    if (arr.empty())
        return nullptr;

    *sz = arr.size();
    auto result = (char**)malloc(arr.size() * sizeof(char*));
    for (size_t i = 0; i < arr.size(); i++)
        result[i] = str(arr[i]);
    return result; 
}

char* UXDG_XDG_DATA_HOME()
{
    return str(UXDG::XDG_DATA_HOME());
}

char* UXDG_XDG_CONFIG_HOME()
{
    return str(UXDG::XDG_CONFIG_HOME());
}

char* UXDG_XDG_STATE_HOME()
{
    return str(UXDG::XDG_STATE_HOME());
}

char* UXDG_XDG_CACHE_HOME()
{
    return str(UXDG::XDG_CACHE_HOME());
}

char* UXDG_XDG_RUNTIME_DIR()
{
    return str(UXDG::XDG_RUNTIME_DIR());
}

char* UXDG_HOME()
{
    return str(UXDG::HOME());
}

char* UXDG_XDG_BIN_PATH_HOME()
{
    return str(UXDG::XDG_BIN_PATH_HOME());
}

char** UXDG_XDG_DATA_DIRS(size_t* size)
{
    return copyArray(UXDG::XDG_DATA_DIRS(), size);
}

char** UXDG_XDG_CONFIG_DIRS(size_t* size)
{
    return copyArray(UXDG::XDG_CONFIG_DIRS(), size);
}

char* UXDG_getEnv(const char* name, const char* val)
{
    return str(UXDG::getEnv(name, val));
}

void UXDG_FreeResult_Str(char* data)
{
    free(data);
}

void UXDG_FreeResult_Arr(char** data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        UXDG_FreeResult_Str(data[i]);
    free(data);
}

char** UXDG_splitEnv(const char* str, size_t* size)
{
    return copyArray(UXDG::splitEnv(str), size);
}

void UXDG_setStickyBit(const char* location)
{
    UXDG::setStickyBit(location);
}

char* UXDG_XDG_DESKTOP_DIR()
{
    return str(UXDG::XDG_DESKTOP_DIR());
}

char* UXDG_XDG_DOWNLOAD_DIR()
{
    return str(UXDG::XDG_DOWNLOAD_DIR());
}

char* UXDG_XDG_TEMPLATES_DIR()
{
    return str(UXDG::XDG_TEMPLATES_DIR());
}

char* UXDG_XDG_PUBLICSHARE_DIR()
{
    return str(UXDG::XDG_PUBLICSHARE_DIR());
}

char* UXDG_XDG_DOCUMENTS_DIR()
{
    return str(UXDG::XDG_DOCUMENTS_DIR());
}

char* UXDG_XDG_MUSIC_DIR()
{
    return str(UXDG::XDG_MUSIC_DIR());
}

char* UXDG_XDG_PICTURES_DIR()
{
    return str(UXDG::XDG_PICTURES_DIR());
}

char* UXDG_XDG_VIDEOS_DIR()
{
    return str(UXDG::XDG_VIDEOS_DIR());
}

char* UXDG_getXDGUserDir(const char* dir, UXDG_XDG_USER_DIR_STANDARD_TYPE type)
{
    return str(UXDG::getXDGUserDir(dir, type));
}

char* UXDG_legacyUserIconsDir()
{
    return str(UXDG::legacyUserIconsDir());
}

char* UXDG_legacyUserThemesDir()
{
    return str(UXDG::legacyUserThemesDir());
}

char* UXDG_legacyUserFontsDir()
{
    return str(UXDG::legacyUserFontsDir());
}