#pragma once
#include <gtest/gtest.h>
#include <windows.h>

typedef void* NelderMeadHandle;
typedef NelderMeadHandle (*CreateNelderMeadFunc)(const char*);
typedef void (*SolveFunc)(NelderMeadHandle, double*);
typedef void (*DestroyNelderMeadFunc)(NelderMeadHandle);

class NelderMeadTestFixture : public ::testing::Test {
protected:
    static HMODULE hDll;
    static CreateNelderMeadFunc CreateNelderMead;
    static SolveFunc Solve;
    static DestroyNelderMeadFunc DestroyNelderMead;

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

    static void TearDownTestSuite() {
        if (hDll) {
            FreeLibrary(hDll);
            hDll = nullptr;
        }
    }

    NelderMeadHandle solver = nullptr;
};

HMODULE NelderMeadTestFixture::hDll = nullptr;
CreateNelderMeadFunc NelderMeadTestFixture::CreateNelderMead = nullptr;
SolveFunc NelderMeadTestFixture::Solve = nullptr;
DestroyNelderMeadFunc NelderMeadTestFixture::DestroyNelderMead = nullptr;