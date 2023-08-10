cmake_minimum_required(VERSION 3.21)

project(UntitledXDGBasedir)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

include_directories(.)

option(UXDG_INSTALL "Install the UntitledXDGBasedir library" OFF)

set(UXDG_LIBRARY_TYPE "SHARED")
if (BUILD_VARIANT_STATIC)
    set(UXDG_LIBRARY_TYPE "STATIC")
endif()

set(UXDG_HEADERS "Common.h" "CUXDGBaseDir.h" "UXDGBaseDir.hpp")

add_library(UntitledXDGBasedir ${UXDG_LIBRARY_TYPE} CUXDGBaseDir.cpp UXDGBaseDir.cpp ${UXDG_HEADERS})

target_compile_definitions(UntitledXDGBasedir PUBLIC USING_XDG)

if (UXDG_INSTALL)
    install(TARGETS UntitledXDGBasedir
            LIBRARY DESTINATION lib/
            ARCHIVE DESTINATION lib/)
    install(FILES ${UXDG_HEADERS} DESTINATION include/UntitledXDGBasedir)
endif()
