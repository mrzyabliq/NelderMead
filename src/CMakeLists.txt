# Сборка основной библиотеки
add_library(NelderMead SHARED 
    NelderMead.cpp
    Parser.cpp
    library_init.cpp
    pch.cpp
)

target_include_directories(NelderMead
    PRIVATE .
    PUBLIC ../include
)

if(UNIX)
    target_link_libraries(NelderMead PRIVATE m)
endif()

# Установка runtime библиотек
set_target_properties(NelderMead PROPERTIES
    MSVC_RUNTIME_LIBRARY "${CMAKE_MSVC_RUNTIME_LIBRARY}"
)

# Установка библиотеки (опционально)
install(TARGETS NelderMead
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY ../include/ DESTINATION include)