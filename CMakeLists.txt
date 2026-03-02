cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_executable(app WIN32 "windows/main.cpp")

    include(cmake/windows.cmake)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "cmake/windows.cmake")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_executable(app "linux/main.cpp")

    include(cmake/linux.cmake)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "cmake/linux.cmake")

    target_compile_options(app PRIVATE -Wno-attributes)
endif()

target_sources(app PRIVATE ${APP_SOURCES})