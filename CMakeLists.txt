cmake_minimum_required(VERSION 3.15)
project(NelderMeadProject LANGUAGES CXX)

# Общие настройки
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка runtime библиотек
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<NOT:$<CONFIG:Debug>>:>DLL")

# Настройки экспорта символов
if(WIN32)
    add_definitions(-DNELDERMEAD_EXPORTS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
else()
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
endif()

# Добавляем поддиректорию src
add_subdirectory(src)

# Включаем тесты, если нужно
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()