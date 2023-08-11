cmake_minimum_required(VERSION 3.21)

project(UntitledXDGBasedir VERSION 3.0.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

include_directories(.)

option(CREATE_DIRS "Creates missing directories automatically." ON)
option(NO_IGNORE_RELATIVE_DIRS "Does not ignore relative paths." ON)

set(UXDG_LIBRARY_TYPE "SHARED")
if (BUILD_VARIANT_STATIC)
    set(UXDG_LIBRARY_TYPE "STATIC")
endif()

set(UXDG_HEADERS "Common.h" "CUXDGBaseDir.h" "UXDGBaseDir.hpp")

add_library(UntitledXDGBasedir ${UXDG_LIBRARY_TYPE} CUXDGBaseDir.cpp UXDGBaseDir.cpp ${UXDG_HEADERS})

target_compile_definitions(UntitledXDGBasedir PRIVATE UVK_LOG_EXPORT_FROM_LIBRARY UVK_LIB_COMPILE)
target_compile_definitions(UntitledXDGBasedir PUBLIC UIMGUI_XDG_BASEDIR_SUBMODULE_ENABLED)
list(APPEND compile_defs "UIMGUI_XDG_BASEDIR_SUBMODULE_ENABLED")
if (CREATE_DIRS)
    target_compile_definitions(UntitledXDGBasedir PUBLIC UXDG_CREATE_DIRS)
    list(APPEND compile_defs "UXDG_CREATE_DIRS")
endif()

if (NO_IGNORE_RELATIVE_DIRS)
    target_compile_definitions(UntitledXDGBasedir PUBLIC UXDG_DO_NOT_IGNORE_RELATIVE_DIRS)
    list(APPEND compile_defs "UXDG_DO_NOT_IGNORE_RELATIVE_DIRS")
endif()

configure_file(UntitledXDGBasedir.pc.in UntitledXDGBasedir.pc @ONLY)

if (UIMGUI_INSTALL)
    install(TARGETS UntitledXDGBasedir
            LIBRARY DESTINATION lib/
            ARCHIVE DESTINATION lib/)
    install(FILES ${UXDG_HEADERS} DESTINATION include/UntitledXDGBasedir)
    install(FILES ${PROJECT_BINARY_DIR}/UntitledXDGBasedir.pc
            DESTINATION lib/pkgconfig/)
endif()
