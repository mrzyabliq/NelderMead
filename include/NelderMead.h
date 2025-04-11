#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// ”ниверсальный макрос дл€ экспорта/импорта
#if defined(_WIN32)
    #if defined(NELDERMEAD_EXPORTS)
        #define NELDERMEAD_API __declspec(dllexport)
    #else
        #define NELDERMEAD_API __declspec(dllimport)
    #endif
#else
    #define NELDERMEAD_API __attribute__((visibility("default")))
#endif

typedef struct ParserHandle ParserHandle;

NELDERMEAD_API ParserHandle* CreateParser(const char* expression);
NELDERMEAD_API void SetVariable(ParserHandle* handle, const char* name, double value);
NELDERMEAD_API double Evaluate(ParserHandle* handle);
NELDERMEAD_API void DestroyParser(ParserHandle* handle);

#ifdef __cplusplus
}
#endif