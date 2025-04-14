cmake_minimum_required(VERSION 3.15)
set(CMAKE_SYSTEM_PROCESSOR "x64")
project(NelderMeadProject LANGUAGES CXX)

# Установка политик CMake
cmake_policy(SET CMP0091 NEW) # Для контроля runtime библиотек

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Единая настройка runtime библиотек для всех конфигураций
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<NOT:$<CONFIG:Debug>>:>DLL")

# Настройки экспорта символов
if(WIN32)
    add_definitions(-DNELDERMEAD_EXPORTS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
else()
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
endif()

# Настройка Google Test с правильными флагами
option(INSTALL_GTEST "Disable install targets" OFF)
option(BUILD_GMOCK "Disable GMock" OFF)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Сборка основной библиотеки
add_library(NelderMead SHARED 
    src/NelderMead.cpp
    src/Parser.cpp
    src/library_init.cpp
    src/pch.cpp
)

target_include_directories(NelderMead
    PRIVATE src
    PUBLIC include
)

if(UNIX)
    target_link_libraries(NelderMead PRIVATE m)
endif()

# Сборка тестов
enable_testing()
add_executable(NelderMeadTests
    test/AllTest.cpp
    test/tests.cpp
)

target_include_directories(NelderMeadTests PRIVATE include)
target_link_libraries(NelderMeadTests
    PRIVATE
        NelderMead
        gtest_main
)

# Явное указание runtime библиотек для всех целей
foreach(target IN ITEMS NelderMead NelderMeadTests)
    set_target_properties(${target} PROPERTIES
        MSVC_RUNTIME_LIBRARY "${CMAKE_MSVC_RUNTIME_LIBRARY}"
    )
endforeach()

add_test(NAME NelderMeadTests COMMAND NelderMeadTests)