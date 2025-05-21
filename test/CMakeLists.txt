# Настройка Google Test
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

# Сборка тестов
add_executable(NelderMeadTests
    AllTest.cpp
    ParserTests.cpp
    ParserExceptionsTests.cpp
    NelderMeadMethodTests.cpp
    NelderMeadTestFixture.h
    ParserTestFixture.h
    ParserTestFixture.cpp
)

target_include_directories(NelderMeadTests 
    PRIVATE ../include
)

target_link_libraries(NelderMeadTests
    PRIVATE
        NelderMeadBackend
        gtest_main
)

# Установка runtime библиотек
set_target_properties(NelderMeadTests PROPERTIES
    MSVC_RUNTIME_LIBRARY "${CMAKE_MSVC_RUNTIME_LIBRARY}"
)

# Добавление тестов
add_test(NAME NelderMeadTests COMMAND NelderMeadTests)

# Копирование DLL в папку с тестами (для Windows)
if(WIN32)
    add_custom_command(TARGET NelderMeadTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:NelderMeadBackend>
            $<TARGET_FILE_DIR:NelderMeadTests>
    )
endif()