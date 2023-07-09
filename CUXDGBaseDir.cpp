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