#include "UXDGBaseDir.hpp"
#include <filesystem>
#include <fstream>

#define XDG_DATA_HOME_DEFAULT "XDG_DATA_HOME", ".local/share"
#define XDG_CONFIG_HOME_DEFAULT "XDG_CONFIG_HOME", ".config"
#define XDG_STATE_HOME_DEFAULT "XDG_STATE_HOME", ".local/state"
#define XDG_CACHE_HOME_DEFAULT "XDG_CACHE_HOME", ".cache"

#define XDG_BIN_PATH_HOME_DEFAULT ".local/bin"

#define XDG_RUNTIME_DIR_DEFAULT "XDG_RUNTIME_DIR", ""

#define XDG_DATA_DIRS_DEFAULT "XDG_DATA_DIRS", "/usr/local/share:/usr/share"
#define XDG_CONFIG_DIRS_DEFAULT "XDG_CONFIG_DIRS", "/etc/xdg"

uxdgstring UXDG::getEnv(const char* name, const char* val) noexcept
{
    auto result = std::getenv(name);
    if (result != nullptr)
        return result;
    return HOME() +  "/" + val;
}

std::vector<uxdgstring> UXDG::splitEnv(const uxdgstring& str) noexcept
{
    std::vector<uxdgstring> result;
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

void handleDir(uxdgstring& dir, std::filesystem::perm_options permsOpt = std::filesystem::perm_options::add)
{
    std::filesystem::path f(dir);
#ifdef UXDG_CREATE_DIRS
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

void handleDirs(std::vector<uxdgstring>& dirs) noexcept
{
    for (auto& a : dirs)
        handleDir(a);
}

uxdgstring UXDG::XDG_DATA_HOME() noexcept
{
    auto result = getEnv(XDG_DATA_HOME_DEFAULT);
    handleDir(result);
    return result;
}

uxdgstring UXDG::XDG_CONFIG_HOME() noexcept
{
    auto result = getEnv(XDG_CONFIG_HOME_DEFAULT);
    handleDir(result);
    return result;
}

uxdgstring UXDG::XDG_STATE_HOME() noexcept
{
    auto result = getEnv(XDG_STATE_HOME_DEFAULT);
    handleDir(result);
    return result;
}

uxdgstring UXDG::XDG_CACHE_HOME() noexcept
{
    auto result = getEnv(XDG_CACHE_HOME_DEFAULT);
    handleDir(result);
    return result;
}

uxdgstring UXDG::XDG_RUNTIME_DIR() noexcept
{
    auto result = getEnv(XDG_RUNTIME_DIR_DEFAULT);

    // XDG Basedir spec: ... The directory MUST be owned by the user, and he MUST be the only one having
    // read and write access to it. Its Unix access mode MUST be 0700.
    if (!result.empty()) handleDir(result, std::filesystem::perm_options::replace);
    return result;
}

uxdgstring UXDG::HOME() noexcept
{
    auto result = std::getenv("HOME");
    if (result != nullptr)
        return result;
    return "";
}

uxdgstring UXDG::XDG_BIN_PATH_HOME() noexcept
{
    uxdgstring result = HOME() + "/" + XDG_BIN_PATH_HOME_DEFAULT;
    handleDir(result);
    return result;
}

std::vector<uxdgstring> UXDG::XDG_DATA_DIRS() noexcept
{
    auto result = splitEnv(getEnv(XDG_DATA_DIRS_DEFAULT));
    handleDirs(result);
    return result;
}

std::vector<uxdgstring> UXDG::XDG_CONFIG_DIRS() noexcept
{
    auto result = splitEnv(getEnv(XDG_CONFIG_DIRS_DEFAULT));
    handleDirs(result);
    return result;
}

void UXDG::setStickyBit(const uxdgstring& location) noexcept
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

// Replaces $HOME with home directory in strings
void replaceHome(uxdgstring& str) noexcept
{
    static const uxdgstring home = "$HOME";
    if (str.starts_with(home))
        str.replace(0, home.length(), UXDG::HOME());
}

void loadDataFromUserDirFile(const uxdgstring& location,
                             std::array<uxdgstring, XDG_USER_DIR_STANDARD_TYPE_SIZE>& strings,
                             std::vector<std::pair<uxdgstring, uxdgstring>>& customDirs) noexcept
{
    static constexpr const char* predefinedNames[] = {
            "", // This one is here so that we are aligned to the XDG_USER_DIR_STANDARD_TYPE enum
        "XDG_DESKTOP_DIR",
        "XDG_DOWNLOAD_DIR",
        "XDG_TEMPLATES_DIR",
        "XDG_PUBLICSHARE_DIR",
        "XDG_DOCUMENTS_DIR",
        "XDG_MUSIC_DIR",
        "XDG_PICTURES_DIR",
        "XDG_VIDEOS_DIR",
    };

    std::ifstream in(location);
    uxdgstring line;

    while (std::getline(in, line))
    {
        bool bFoundOne = false;
        // Starting from 1 since 0 is the custom type which we don't want
        for (size_t i = 1; i < XDG_USER_DIR_STANDARD_TYPE_SIZE; i++)
        {
            uxdgstring it = predefinedNames[i];

// strlen("=\"")
#define MAGIC_NUMBER_EQUAL_QUOTE 2
            // The magic number
            if (line.starts_with(it + "=\""))
            {
                bFoundOne = true;
                strings[i] = line.substr(
                        it.size() + MAGIC_NUMBER_EQUAL_QUOTE,
                        line.size() - it.size() - MAGIC_NUMBER_EQUAL_QUOTE - 1 // -1 here because of trailing "
                );
            }
        }
        // If no strings were found this means it's custom
        if (!bFoundOne && line.starts_with("XDG_"))
        {
            size_t result = line.find_first_of("=\"");
            if (result != uxdgstring::npos)
            {
                customDirs.emplace_back(std::make_pair<uxdgstring, uxdgstring>(
                    line.substr(0, result),
                    line.substr(
                        result + MAGIC_NUMBER_EQUAL_QUOTE,
                        line.length() - result - MAGIC_NUMBER_EQUAL_QUOTE - 1 // -1 here because of trailing "
                    )
                ));
            }
        }
    }

    // Replace $HOME with directory to home
    for (auto& a : strings)
        replaceHome(a);
    for (auto& a : customDirs)
        replaceHome(a.second);
}

uxdgstring UXDG::getXDGUserDir(const char* dir, UXDG_XDG_USER_DIR_STANDARD_TYPE type) noexcept
{
    static std::array<uxdgstring, XDG_USER_DIR_STANDARD_TYPE_SIZE> strings;
    static std::vector<std::pair<uxdgstring, uxdgstring>> customDirs;

    // This is made so that the file is read only once
    static bool bFirst = true;
    if (bFirst)
    {
        // Set data from file
        loadDataFromUserDirFile(XDG_CONFIG_HOME() + "/user-dirs.dirs", strings, customDirs);

        // From here set the default value if it is somehow not set
        static constexpr const char* predefinedNames[] =
        {
            "", // This one is here so that we are aligned to the XDG_USER_DIR_STANDARD_TYPE enum
            "$HOME/Desktop",
            "$HOME/Downloads",
            "$HOME/Templates",
            "$HOME/Public",
            "$HOME/Documents",
            "$HOME/Music",
            "$HOME/Pictures",
            "$HOME/Videos",
        };

        // i = 1 because 0 is a custom argument
        for (size_t i = 1; i < XDG_USER_DIR_STANDARD_TYPE_SIZE; i++)
        {
            if (strings[i].empty())
            {
                strings[i] = predefinedNames[i];
                replaceHome(strings[i]);
            }
        }
        bFirst = false;
    }

    // If predefined type
    if (type > 0 && type <= XDG_USER_DIR_STANDARD_TYPE_SIZE)
        return strings[type];

    // If custom type
    for (auto& a : customDirs)
        if (a.first == dir)
            return a.second;

    // If it's not found
    return "";
}

uxdgstring UXDG::XDG_DESKTOP_DIR() noexcept
{
    static uxdgstring str = getXDGUserDir("XDG_DESKTOP_DIR", XDG_USER_DIR_STANDARD_TYPE_DESKTOP_DIR_DEFAULT);
    return str;
}

uxdgstring UXDG::XDG_DOWNLOAD_DIR() noexcept
{
    static uxdgstring str = getXDGUserDir("XDG_DOWNLOAD_DIR", XDG_USER_DIR_STANDARD_TYPE_DOWNLOAD_DIR_DEFAULT);
    return str;
}

uxdgstring UXDG::XDG_TEMPLATES_DIR() noexcept
{
    static uxdgstring str = getXDGUserDir("XDG_TEMPLATES_DIR", XDG_USER_DIR_STANDARD_TYPE_TEMPLATES_DIR_DEFAULT);
    return str;
}

uxdgstring UXDG::XDG_PUBLICSHARE_DIR() noexcept
{
    static uxdgstring str = getXDGUserDir("XDG_PUBLICSHARE_DIR", XDG_USER_DIR_STANDARD_TYPE_PUBLICSHARE_DIR_DEFAULT);
    return str;
}

uxdgstring UXDG::XDG_DOCUMENTS_DIR() noexcept
{
    static uxdgstring str = getXDGUserDir("XDG_DOCUMENTS_DIR", XDG_USER_DIR_STANDARD_TYPE_DOCUMENTS_DIR_DEFAULT);
    return str;
}

uxdgstring UXDG::XDG_MUSIC_DIR() noexcept
{
    static uxdgstring str = getXDGUserDir("XDG_MUSIC_DIR", XDG_USER_DIR_STANDARD_TYPE_MUSIC_DIR_DEFAULT);
    return str;
}

uxdgstring UXDG::XDG_PICTURES_DIR() noexcept
{
    static uxdgstring str = getXDGUserDir("XDG_PICTURES_DIR", XDG_USER_DIR_STANDARD_TYPE_PICTURES_DIR_DEFAULT);
    return str;
}

uxdgstring UXDG::XDG_VIDEOS_DIR() noexcept
{
    static uxdgstring str = getXDGUserDir("XDG_VIDEOS_DIR", XDG_USER_DIR_STANDARD_TYPE_VIDEOS_DIR_DEFAULT);
    return str;
}

uxdgstring UXDG::legacyUserIconsDir() noexcept
{
    return HOME() + "/.icons";
}

uxdgstring UXDG::legacyUserThemesDir() noexcept
{
    return HOME() + "/.themes";
}

uxdgstring UXDG::legacyUserFontsDir() noexcept
{
    return HOME() + "/.fonts";
}
