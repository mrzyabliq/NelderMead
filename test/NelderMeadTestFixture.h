#pragma once
#include <gtest/gtest.h>
#include <windows.h>

// Объявление типов функций DLL
typedef void* NelderMeadHandle;
typedef NelderMeadHandle (*CreateNelderMeadFunc)(const char*);
typedef void (*SolveFunc)(NelderMeadHandle, double*);
typedef void (*DestroyNelderMeadFunc)(NelderMeadHandle);

class NelderMeadTestFixture : public ::testing::Test {
protected:
    static HMODULE hDll;          // Хэндл DLL
    static CreateNelderMeadFunc CreateNelderMead;
    static SolveFunc Solve;
    static DestroyNelderMeadFunc DestroyNelderMead;

    // Загружаем DLL перед всеми тестами
    static void SetUpTestSuite() {
        hDll = LoadLibraryW(L"NelderMead.dll");
        ASSERT_TRUE(hDll != nullptr) << "DLL не загружена! Ошибка: " << GetLastError();

        CreateNelderMead = (CreateNelderMeadFunc)GetProcAddress(hDll, "CreateNelderMead");
        Solve = (SolveFunc)GetProcAddress(hDll, "Solve");
        DestroyNelderMead = (DestroyNelderMeadFunc)GetProcAddress(hDll, "DestroyNelderMead");

        ASSERT_TRUE(CreateNelderMead != nullptr) << "Функция CreateNelderMead не найдена!";
        ASSERT_TRUE(Solve != nullptr) << "Функция Solve не найдена!";
        ASSERT_TRUE(DestroyNelderMead != nullptr) << "Функция DestroyNelderMead не найдена!";
    }

    // Выгружаем DLL после всех тестов
    static void TearDownTestSuite() {
        if (hDll) {
            FreeLibrary(hDll);
            hDll = nullptr;
        }
    }

    // Общие данные для тестов (если нужны)
    NelderMeadHandle solver = nullptr;
};

// Инициализация статических полей
HMODULE NelderMeadTestFixture::hDll = nullptr;
CreateNelderMeadFunc NelderMeadTestFixture::CreateNelderMead = nullptr;
SolveFunc NelderMeadTestFixture::Solve = nullptr;
DestroyNelderMeadFunc NelderMeadTestFixture::DestroyNelderMead = nullptr;