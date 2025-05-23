project(NelderMeadBackend LANGUAGES CXX)

# 1. Исходные файлы
file(GLOB SRC_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# 2. Создание библиотеки
add_library(NelderMeadBackend SHARED ${SRC_FILES})

# 3. Настройки для Windows
if(WIN32)
    target_compile_definitions(NelderMeadBackend PRIVATE NELDERMEAD_EXPORTS)
    set_target_properties(NelderMeadBackend PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
        SUFFIX ".dll"
    )
else()
    # Настройки для Unix
    target_compile_definitions(NelderMeadBackend PRIVATE NELDERMEAD_API_EXPORTS)
    find_library(M_LIB m)
    if(M_LIB)
        target_link_libraries(NelderMeadBackend PRIVATE ${M_LIB})
    endif()
endif()

# 4. Подключение заголовков
target_include_directories(NelderMeadBackend PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 5. Установка для Windows
if(WIN32)
    install(TARGETS NelderMeadBackend
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
    )
endif()