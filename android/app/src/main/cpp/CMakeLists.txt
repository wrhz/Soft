cmake_minimum_required(VERSION 3.22.1)

project("softapplication")

add_library(${CMAKE_PROJECT_NAME} SHARED native-lib.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} android log)

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/extension/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extension/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/soft/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/soft/*.cpp"
)

add_library(init_extension SHARED "${CMAKE_CURRENT_SOURCE_DIR}/../python/init_extension.cpp" ${SOURCES})

target_include_directories(init_extension PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(init_extension PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/python")
target_include_directories(init_extension PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/extension")

target_link_libraries(init_extension PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../lib/${ANDROID_ABI}/libpython3.12.so")

set_target_properties(init_extension PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../python")

set_target_properties(init_extension PROPERTIES PREFIX "")

set_target_properties(init_extension PROPERTIES SUFFIX ".cpython-312.so")