set(APP_SOURCES
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/debug/Log.cpp"
    "D:/Projects/Soft/Soft-main/extension/register_modules.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/algorithm/Cache.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/debug/AssertFatal.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/YGNodeStyle.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/config/Config.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/YGConfig.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/YGNode.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/soft/types.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/node/LayoutResults.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/algorithm/PixelGrid.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/YGPixelGrid.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/soft/style.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/YGNodeLayout.cpp"
    "D:/Projects/Soft/Soft-main/extension/example_module.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/YGEnums.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/public/soft/classdb.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/event/event.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/algorithm/FlexLine.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/node/Node.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/soft/get_json.cpp"
    "D:/Projects/Soft/Soft-main/windows/utils.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/algorithm/AbsoluteLayout.cpp"
    "D:/Projects/Soft/Soft-main/windows/element.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/YGValue.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/algorithm/Baseline.cpp"
    "D:/Projects/Soft/Soft-main/src/cpp/private/yoga/algorithm/CalculateLayout.cpp"
)
target_include_directories(app
    PRIVATE
    "D:/Projects/Soft/Soft-main/build/windows/python/include"
    "D:/Projects/Soft/Soft-main/src/cpp/private"
    PUBLIC
    "D:/Projects/Soft/Soft-main/src/cpp/public"
    "D:/Projects/Soft/Soft-main/extension"
)
target_link_directories(app PRIVATE "D:/Projects/Soft/Soft-main/build/windows/python/libs")
target_link_libraries(app PRIVATE "user32.lib")
target_link_libraries(app PRIVATE "gdi32.lib")
target_link_libraries(app PRIVATE "shlwapi.lib")
target_link_libraries(app PRIVATE "python312.lib")
