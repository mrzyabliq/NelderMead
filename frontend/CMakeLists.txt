cmake_minimum_required(VERSION 3.15)
project(NelderMeadFrontend LANGUAGES CXX)

# Настройки Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Исходные файлы
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.h
    src/*.ui
)

# Исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Зависимости
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
    NelderMeadBackend
)

# Развертывание (Windows)
if(WIN32)
    # Определяем путь к Qt в зависимости от конфигурации (Debug/Release)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(QT_DLL_SUFFIX "d")  # Для Debug-версий DLL 
    else()
        set(QT_DLL_SUFFIX "")    # Для Release-версий 
    endif()

    # Автоматическое копирование DLL
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms"
        # Копируем основные Qt DLL с учётом конфигурации
        COMMAND ${CMAKE_COMMAND} -E copy
            "${Qt6_DIR}/../../../bin/Qt6Core${QT_DLL_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${Qt6_DIR}/../../../bin/Qt6Gui${QT_DLL_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${Qt6_DIR}/../../../bin/Qt6Widgets${QT_DLL_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
        # Копируем qwindows.dll (плагин платформы)
        COMMAND ${CMAKE_COMMAND} -E copy
            "${Qt6_DIR}/../../../plugins/platforms/qwindows${QT_DLL_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
        COMMENT "Копирование Qt DLL (${CMAKE_BUILD_TYPE})..."
    )
endif()