set(APP_SOURCES "")

file(GLOB_RECURSE SRC_CPP "${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/*.c")
list(APPEND APP_SOURCES ${SRC_CPP})

file(GLOB_RECURSE EXT_CPP "${CMAKE_CURRENT_SOURCE_DIR}/extension/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/extension/*.c")
list(APPEND APP_SOURCES ${EXT_CPP})

file(GLOB_RECURSE WIN_CPP "${CMAKE_CURRENT_SOURCE_DIR}/windows/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/windows/*.c")
list(APPEND APP_SOURCES ${WIN_CPP})

list(REMOVE_DUPLICATES APP_SOURCES)

add_executable(app WIN32 ${APP_SOURCES})

target_include_directories(app
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/build/windows/python/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/private"
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/public"
    "${CMAKE_CURRENT_SOURCE_DIR}/extension"
)
target_link_directories(app PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/build/windows/python/libs")
target_link_libraries(app PRIVATE "user32.lib")
target_link_libraries(app PRIVATE "gdi32.lib")
target_link_libraries(app PRIVATE "shlwapi.lib")
target_link_libraries(app PRIVATE "python312.lib")
