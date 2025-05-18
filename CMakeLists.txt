cmake_minimum_required(VERSION 3.15)
project(NelderMead LANGUAGES CXX)

# 1. Функция для показа сообщения об ошибке Qt
function(qt_not_found_error)
    message(FATAL_ERROR "\n"
        "=============================================================\n"
        "Qt 6.8.2 не найден!\n\n"
        "1. Скачайте Qt 6.8.2 для вашей системы по ссылке:\n"
        "   https://disk.yandex.ru/d/_aF-pyYLWEEWRQ\n\n"
        "2. Установите в один из стандартных путей:\n"
        "   - Windows: C:/Qt/qtkit/6.8.2-msvc2022\n"
        "   - Linux: ~/Qt/6.8.2\n\n"
        "3. ИЛИ задайте переменную окружения QTDIR:\n"
        "   setx QTDIR \"C:\\path\\to\\qt\" (Windows)\n"
        "   export QTDIR=/path/to/qt (Linux/macOS)\n\n"
        "4. Удалите папку build и пересоберите проект:\n"
        "   rm -rf build && mkdir build && cd build && cmake ..\n"
        "=============================================================\n"
    )
endfunction()

# 2. Автоматический поиск Qt
list(APPEND QT_PATHS
    "$ENV{QTDIR}"
    "C:/Qt"
    "C:/Qt/qtkit/6.8.2-msvc2022"
    "$ENV{HOME}/Qt/6.8.2"
    [HKEY_CURRENT_USER\\SOFTWARE\\QtProject\\Qt6.8.2;InstallDir]
)

find_package(Qt6 6.8.2 COMPONENTS Widgets Gui Core
    PATHS ${QT_PATHS}
    PATH_SUFFIXES "lib/cmake/Qt6" "msvc2022_64/lib/cmake/Qt6"
)

if(NOT Qt6_FOUND)
    qt_not_found_error()
endif()

# 3. Общие настройки
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# 4. Настройка выходных директорий
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 5. Подпроекты
add_subdirectory(backend)
add_subdirectory(frontend)

# 6. Дополнительные цели
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E echo "Запуск приложения..."
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/NelderMeadFrontend$<$<CONFIG:Debug>:_debug>
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    DEPENDS NelderMeadFrontend
)