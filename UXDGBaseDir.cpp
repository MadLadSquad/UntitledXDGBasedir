#include "UXDGBaseDir.hpp"
#include <filesystem>

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
    return HOME() +  "/" + val;
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

void handleDir(std::string& dir, std::filesystem::perm_options permsOpt = std::filesystem::perm_options::add)
{
#ifdef UXDG_CREATE_DIRS
    std::filesystem::path f(dir);

    try
    {
        if (!exists(f))
        {
            create_directory(f);
            std::filesystem::permissions(f, std::filesystem::perms::owner_all, permsOpt);
        }
    }
    catch (std::filesystem::filesystem_error&)
    {

    }
#endif

#ifdef UXDG_DO_NOT_IGNORE_RELATIVE_DIRS
    if (!f.is_absolute())
        dir = absolute(f).string();
#endif
}

void handleDirs(std::vector<std::string>& dirs) noexcept
{
    for (auto& a : dirs)
        handleDir(a);
}

std::string UXDG::XDG_DATA_HOME() noexcept
{
    auto result = getEnv(XDG_DATA_HOME_DEFAULT);
    handleDir(result);
    return result;
}

std::string UXDG::XDG_CONFIG_HOME() noexcept
{
    auto result = getEnv(XDG_CONFIG_HOME_DEFAULT);
    handleDir(result);
    return result;
}

std::string UXDG::XDG_STATE_HOME() noexcept
{
    auto result = getEnv(XDG_STATE_HOME_DEFAULT);
    handleDir(result);
    return result;
}

std::string UXDG::XDG_CACHE_HOME() noexcept
{
    auto result = getEnv(XDG_CACHE_HOME_DEFAULT);
    handleDir(result);
    return result;
}

std::string UXDG::XDG_RUNTIME_DIR() noexcept
{
    auto result = getEnv(XDG_RUNTIME_DIR_DEFAULT);

    // XDG Basedir spec: ... The directory MUST be owned by the user, and he MUST be the only one having
    // read and write access to it. Its Unix access mode MUST be 0700.
    handleDir(result, std::filesystem::perm_options::replace);
    return result;
}

std::string UXDG::HOME() noexcept
{
    auto result = std::getenv("HOME");
    if (result != nullptr)
        return result;
    return "";
}

std::string UXDG::XDG_BIN_PATH_HOME() noexcept
{
    std::string result = HOME() + "/" + XDG_BIN_PATH_HOME_DEFAULT;
    handleDir(result);
    return result;
}

std::vector<std::string> UXDG::XDG_DATA_DIRS() noexcept
{
    auto result = splitEnv(getEnv(XDG_DATA_DIRS_DEFAULT));
    handleDirs(result);
    return result;
}

std::vector<std::string> UXDG::XDG_CONFIG_DIRS() noexcept
{
    auto result = splitEnv(getEnv(XDG_CONFIG_DIRS_DEFAULT));
    handleDirs(result);
    return result;
}

void UXDG::setStickyBit(const std::string& location) noexcept
{
    std::filesystem::path p(location);
    try {
        if (exists(p))
            permissions(p, std::filesystem::perms::sticky_bit, std::filesystem::perm_options::add);
    }
    catch (std::filesystem::filesystem_error&)
    {

    }
}