cmake_minimum_required(VERSION 3.10)

set(OGL3RD_DIR      "E:/kSource/pythonx/shader/opengl-3rd")
set(FAKEDRIVER_DIR  "E:/cfwpe/irrlicht/fakedriver")

# fakedriver fakedriverAdapter
add_subdirectory(${FAKEDRIVER_DIR}/fakedriverAdapter fakedriverAdapter.build)
add_subdirectory(${FAKEDRIVER_DIR}/fakedriver fakedriver.build)
# fakelib
add_subdirectory(${FAKEDRIVER_DIR}/fakelib fakelib.build)

include_directories(
    ${FAKEDRIVER_DIR}/fakelib/include
    ${FAKEDRIVER_DIR}/include/khronos
    ${FAKEDRIVER_DIR}/include
    ${OGL3RD_DIR}/SOIL/include/SOIL
    ${OGL3RD_DIR}/glfw-3.3.2.bin.WIN32/glfw-3.3.2.bin.WIN32/include
    ${OGL3RD_DIR}/glad/include
)
link_directories(
    ${OGL3RD_DIR}/SOIL/lib/vs2017/Release
    ${OGL3RD_DIR}/glfw-3.3.2.bin.WIN32/glfw-3.3.2.bin.WIN32/lib-vc2017
    ${OGL3RD_DIR}/glad/lib/vs2017_win32/Release
)

set(MSVC_CFG_Debug_Win32 OFF CACHE INTERNAL "")
include(app.vcxproj.cmake)
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

add_executable(
    appx
    app.vcxproj.cmake
    ${MSVC_APP_VCXPROJ_SRCS}
    shader/shader.frag
    shader/shader.vert
    test_gles2.0.c
    test_opengl.c
)
target_link_libraries(
    appx
    SOIL
    fakedriver glfw3 glad
)
set_target_properties(appx PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/product")
set_target_properties(appx PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/product")
