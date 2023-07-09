#pragma once
#include <string>
#include <vector>
#include "Common.h"

namespace UXDG
{
    // XDG Basedir spec: $XDG_DATA_HOME defines the base directory relative to which user-specific data files should be
    // stored. If $XDG_DATA_HOME is either not set or empty, a default equal to $HOME/.local/share should be used.
    UVK_PUBLIC_API std::string XDG_DATA_HOME() noexcept;

    // XDG Basedir spec: $XDG_CONFIG_HOME defines the base directory relative to which user-specific configuration
    // files should be stored. If $XDG_CONFIG_HOME is either not set or empty, a default equal to $HOME/.config should
    // be used.
    UVK_PUBLIC_API std::string XDG_CONFIG_HOME() noexcept;

    // XDG Basedir spec: The $XDG_STATE_HOME contains state data that should persist between (application) restarts,
    // but that is not important or portable enough to the user that it should be stored in $XDG_DATA_HOME.
    // It may contain:
    // - actions history (logs, history, recently used files, …)
    // - current state of the application that can be reused on a restart (view, layout, open files, undo history, …)
    UVK_PUBLIC_API std::string XDG_STATE_HOME() noexcept;

    // XDG Basedir spec: $XDG_CACHE_HOME defines the base directory relative to which user-specific non-essential
    // data files should be stored. If $XDG_CACHE_HOME is either not set or empty, a default equal to
    // $HOME/.cache should be used.
    UVK_PUBLIC_API std::string XDG_CACHE_HOME() noexcept;

    // XDG Basedir spec: $XDG_RUNTIME_DIR defines the base directory relative to which user-specific non-essential
    // runtime files and other file objects (such as sockets, named pipes, ...) should be stored.
    // The directory MUST be owned by the user, and he MUST be the only one having read and write access to it.
    // Its Unix access mode MUST be 0700.
    //
    // The lifetime of the directory MUST be bound to the user being logged in. It MUST be created when the user first
    // logs in and if the user fully logs out the directory MUST be removed. If the user logs in more than once he
    // should get pointed to the same directory, and it is mandatory that the directory continues to exist from his
    // first login to his last logout on the system, and not removed in between. Files in the directory MUST not
    // survive reboot or a full logout/login cycle.
    //
    // The directory MUST be on a local file system and not shared with any other system. The directory MUST be
    // fully-featured by the standards of the operating system. More specifically, on Unix-like operating systems
    // AF_UNIX sockets, symbolic links, hard links, proper permissions, file locking, sparse files, memory mapping,
    // file change notifications, a reliable hard link count must be supported, and no restrictions on the file
    // name character set should be imposed. Files in this directory MAY be subjected to periodic clean-up.
    // To ensure that your files are not removed, they should have their access time timestamp modified at least once
    // every 6 hours of monotonic time or the 'sticky' bit should be set on the file.
    //
    // If $XDG_RUNTIME_DIR is not set applications should fall back to a replacement directory with similar
    // capabilities and print a warning message. Applications should use this directory for communication and
    // synchronization purposes and should not place larger files in it, since it might reside in runtime
    // memory and cannot necessarily be swapped out to disk.
    UVK_PUBLIC_API std::string XDG_RUNTIME_DIR() noexcept;

    // Returns the home directory for the current user
    UVK_PUBLIC_API std::string HOME() noexcept;

    // XDG Basedir spec: User-specific executable files may be stored in $HOME/.local/bin. Distributions should ensure
    // this directory shows up in the UNIX $PATH environment variable, at an appropriate place.
    //
    // Since $HOME might be shared between systems of different architectures, installing compiled binaries to
    // $HOME/.local/bin could cause problems when used on systems of differing architectures. This is often not a
    // problem, but the fact that $HOME becomes partially architecture-specific if compiled binaries are placed in it
    // should be kept in mind.
    UVK_PUBLIC_API std::string XDG_BIN_PATH_HOME() noexcept;

    // XDG Basedir spec: $XDG_DATA_DIRS defines the preference-ordered set of base directories to search for data
    // files in addition to the $XDG_DATA_HOME base directory. The directories in $XDG_DATA_DIRS should be separated
    // with a colon ':'.
    //
    // If $XDG_DATA_DIRS is either not set or empty, a value equal to /usr/local/share/:/usr/share/ should be used.
    //
    // The order of base directories denotes their importance; the first directory listed is the most important.
    UVK_PUBLIC_API std::vector<std::string> XDG_DATA_DIRS() noexcept;

    // XDG Basedir spec: $XDG_CONFIG_DIRS defines the preference-ordered set of base directories to search for
    // configuration files in addition to the $XDG_CONFIG_HOME base directory. The directories in $XDG_CONFIG_DIRS
    // should be separated with a colon ':'.
    //
    // If $XDG_CONFIG_DIRS is either not set or empty, a value equal to /etc/xdg should be used.
    //
    // The order of base directories denotes their importance; the first directory listed is the most important.
    UVK_PUBLIC_API std::vector<std::string> XDG_CONFIG_DIRS() noexcept;

    // Get an environment variable where the first argument is the name and the second is the default value if it does
    // not exist
    UVK_PUBLIC_API std::string getEnv(const char* name, const char* val) noexcept;

    // Splits a list in environment variable format "a:b:c:d:e:f" into an array of strings
    UVK_PUBLIC_API std::vector<std::string> splitEnv(const std::string& str) noexcept;
}