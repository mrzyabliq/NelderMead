#pragma once
#ifdef _WIN32
    #ifdef NELDERMEAD_EXPORTS
        #define NELDERMEAD_API __declspec(dllexport)
    #else
        #define NELDERMEAD_API __declspec(dllimport)
    #endif
#else
    #define NELDERMEAD_API  // ������ ������ ��� Linux
#endif

#include <string>
extern "C" {
    // Handle ��� ������ � ��������
    struct ParserHandle;

    // �������� �������
    NELDERMEAD_API ParserHandle* CreateParser(const char* expression);

    // ��������� ����������
    NELDERMEAD_API void SetVariable(ParserHandle* handle, const char* name, double value);

    // ���������� ���������
    NELDERMEAD_API double Evaluate(ParserHandle* handle);

    // ����������� �������
    NELDERMEAD_API void DestroyParser(ParserHandle* handle);
}