cmake_minimum_required(VERSION 3.10)
project(NelderMeadProject LANGUAGES CXX)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ищем GoogleTest (через find_package)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)  # GTest может зависеть от потоков

# --- Сборка библиотеки NelderMead ---
add_library(NelderMead SHARED 
    src/NelderMead.cpp
    src/library_init.cpp
    src/pch.cpp
)

# Подключаем заголовки (публичные и приватные)
target_include_directories(NelderMead
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Прекомпилированный заголовок (pch.h)
target_precompile_headers(NelderMead PRIVATE include/pch.h)

# --- Сборка тестов с GTest ---
enable_testing()

add_executable(NelderMeadTests
    test/AllTest.cpp
    test/tests.cpp
)

# Линкуем тесты с библиотекой NelderMead и GTest
target_include_directories(NelderMeadTests PRIVATE include)
target_link_libraries(NelderMeadTests 
    PRIVATE 
        NelderMead
        GTest::GTest 
        GTest::Main 
        Threads::Threads
)

# Добавляем тест в CTest
add_test(NAME NelderMeadTests COMMAND NelderMeadTests)