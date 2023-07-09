#include "UXDGBaseDir.hpp"

#define XDG_DATA_HOME_DEFAULT "XDG_DATA_HOME", ".local/share"
#define XDG_CONFIG_HOME_DEFAULT "XDG_CONFIG_HOME", ".config"
#define XDG_STATE_HOME_DEFAULT "XDG_STATE_HOME", ".local/state"
#define XDG_CACHE_HOME_DEFAULT "XDG_CACHE_HOME", ".cache"

#define XDG_BIN_PATH_HOME_DEFAULT ".local/bin"

#define XDG_RUNTIME_DIR_DEFAULT "XDG_RUNTIME_DIR", ""

#define XDG_DATA_DIRS_DEFAULT "XDG_DATA_DIRS", "/usr/local/share:/usr/share"
#define XDG_CONFIG_DIRS_DEFAULT "XDG_CONFIG_DIRS", "/etc/xdg"

std::string UXDG::getEnv(const char* name, const char* val) noexcept
{
    auto result = std::getenv(name);
    if (result != nullptr)
        return result;
    return val;
}

std::vector<std::string> UXDG::splitEnv(const std::string& str) noexcept
{
    std::vector<std::string> result;
    size_t lastPos = 0;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == ':')
        {
            if (i == 0)
                result.emplace_back();
            else
                result.push_back(str.substr(lastPos, i - lastPos));
            ++i;
            lastPos = i;
        }
    }
    result.push_back(str.substr(lastPos, str.size()));

    return result;
}

std::string UXDG::XDG_DATA_HOME() noexcept
{
    return getEnv(XDG_DATA_HOME_DEFAULT);
}

std::string UXDG::XDG_CONFIG_HOME() noexcept
{
    return getEnv(XDG_CONFIG_HOME_DEFAULT);
}

std::string UXDG::XDG_STATE_HOME() noexcept
{
    return getEnv(XDG_STATE_HOME_DEFAULT);
}

std::string UXDG::XDG_CACHE_HOME() noexcept
{
    return getEnv(XDG_CACHE_HOME_DEFAULT);
}

std::string UXDG::XDG_RUNTIME_DIR() noexcept
{
    return getEnv(XDG_RUNTIME_DIR_DEFAULT);
}

std::string UXDG::HOME() noexcept
{
    return getEnv("HOME", "");
}

std::string UXDG::XDG_BIN_PATH_HOME() noexcept
{
    return XDG_BIN_PATH_HOME_DEFAULT;
}

std::vector<std::string> UXDG::XDG_DATA_DIRS() noexcept
{
    return splitEnv(getEnv(XDG_DATA_DIRS_DEFAULT));
}

std::vector<std::string> UXDG::XDG_CONFIG_DIRS() noexcept
{
    return splitEnv(getEnv(XDG_CONFIG_DIRS_DEFAULT));
}
