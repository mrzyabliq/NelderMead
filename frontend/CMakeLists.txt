cmake_minimum_required(VERSION 3.15)
project(NelderMeadFrontend LANGUAGES CXX)

# 1. Настройки Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 2. Исходные файлы
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.h
    src/*.ui
)

# 3. Исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# 4. Зависимости
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
    NelderMeadBackend
)

# 5. Развертывание (Windows)
if(WIN32)
    # Автоматическое копирование DLL
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${Qt6_DIR}/../../../bin/Qt6Core.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${Qt6_DIR}/../../../bin/Qt6Gui.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${Qt6_DIR}/../../../bin/Qt6Widgets.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${Qt6_DIR}/../../../plugins/platforms/qwindows.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
        COMMENT "Копирование Qt DLL..."
    )
endif()

# 6. Установка иконки (опционально)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.ico")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE_ICON_FILE icon.ico
    )
endif()