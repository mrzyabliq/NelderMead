#pragma once
#ifdef _WIN32
    #ifdef NELDERMEAD_EXPORTS
        #define NELDERMEAD_API __declspec(dllexport)
    #else
        #define NELDERMEAD_API __declspec(dllimport)
    #endif
#else
    #define NELDERMEAD_API  // Пустой макрос для Linux
#endif

#include <string>
extern "C" {
    // Handle для работы с парсером
    struct ParserHandle;

    // Создание парсера
    NELDERMEAD_API ParserHandle* CreateParser(const char* expression);

    // Установка переменной
    NELDERMEAD_API void SetVariable(ParserHandle* handle, const char* name, double value);

    // Вычисление выражения
    NELDERMEAD_API double Evaluate(ParserHandle* handle);

    // Уничтожение парсера
    NELDERMEAD_API void DestroyParser(ParserHandle* handle);
}