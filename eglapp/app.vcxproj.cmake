
# https://github.com/juj/MathGeoLib/blob/master/CommonOptions.cmake
# Add the global _DEBUG flag from WIN32 platform to all others, which is universally used in MGL to
# perform debug-mode-specific compilation.
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")


# set(MSVC_CFG_Debug_Win32 OFF CACHE INTERNAL "")
option(MSVC_CFG_Debug_Win32 "config from vcxproj file." OFF)
if (MSVC_CFG_Debug_Win32)
    add_definitions(-D_CONSOLE)
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../irrlicht/irrlicht-ogles/include/khronos)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../irrlicht/irrlicht-ogles/include)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../irrlicht/irrlicht-ogles/lib/Win32-visualstudio)
    set(MSVC_APP_VCXPROJ_LIBS fakedriver.lib)
endif()


# set(MSVC_CFG_Debug_x64 OFF CACHE INTERNAL "")
option(MSVC_CFG_Debug_x64 "config from vcxproj file." OFF)
if (MSVC_CFG_Debug_x64)
    add_definitions(-D_CONSOLE)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../opengl-3rd/egl/powervr-v5.6/include)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../opengl-3rd/egl/powervr-v5.6/lib/Windows_x86_64)
    set(MSVC_APP_VCXPROJ_LIBS libGLESv2.lib libEGL.lib)
endif()


# set(MSVC_CFG_Release_Win32 OFF CACHE INTERNAL "")
option(MSVC_CFG_Release_Win32 "config from vcxproj file." OFF)
if (MSVC_CFG_Release_Win32)
    add_definitions(-D_CONSOLE)
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
endif()


# set(MSVC_CFG_Release_x64 OFF CACHE INTERNAL "")
option(MSVC_CFG_Release_x64 "config from vcxproj file." OFF)
if (MSVC_CFG_Release_x64)
    add_definitions(-D_CONSOLE)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
    set(MSVC_APP_VCXPROJ_LIBS libGLESv2.lib libEGL.lib)
endif()


set(MSVC_APP_VCXPROJ_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/cube.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/window.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cube.c
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix.c
    ${CMAKE_CURRENT_SOURCE_DIR}/shader.c
    ${CMAKE_CURRENT_SOURCE_DIR}/window.c
)
