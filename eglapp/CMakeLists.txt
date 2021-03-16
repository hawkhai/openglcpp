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
)

set(MSVC_CFG_Debug_Win32 OFF CACHE INTERNAL "")
include(app.vcxproj.cmake)
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

add_executable(
    appx
    app.vcxproj.cmake
    ${MSVC_APP_VCXPROJ_SRCS}
)
target_link_libraries(
    appx
    fakedriver
)
set_target_properties(appx PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/product")
set_target_properties(appx PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/product")
