#pragma once
#include <gtest/gtest.h>
#include <windows.h>

typedef struct NelderMeadHandleT* NelderMeadHandle;

typedef NelderMeadHandle (*CreateNelderMeadFunc)(const char*);
typedef void (*SolveFunc)(NelderMeadHandle, double*);
typedef void (*SolveWithValueFunc)(NelderMeadHandle, double*, double*);
typedef void (*SolveInitFunc)(NelderMeadHandle, double*, double*);
typedef void (*SolveFullFunc)(NelderMeadHandle, double*, double*, double*);
typedef void (*DestroyNelderMeadFunc)(NelderMeadHandle);

class NelderMeadTestFixture : public ::testing::Test {
protected:
    static HMODULE hDll;
    static CreateNelderMeadFunc CreateNelderMead;
    static SolveFunc SolveBasic;
    static SolveWithValueFunc SolveWithValue;
    static SolveInitFunc SolveInit;
    static SolveFullFunc SolveFull;
    static DestroyNelderMeadFunc DestroyNelderMead;

    static void SetUpTestSuite() {
        hDll = LoadLibraryW(L"NelderMeadBackend.dll");
        ASSERT_TRUE(hDll != nullptr) << "DLL не загружена! Ошибка: " << GetLastError();

        CreateNelderMead = (CreateNelderMeadFunc)GetProcAddress(hDll, "CreateNelderMead");
        SolveBasic = (SolveFunc)GetProcAddress(hDll, "SolveBasic");
        SolveWithValue = (SolveWithValueFunc)GetProcAddress(hDll, "SolveWithValue");
        SolveInit = (SolveInitFunc)GetProcAddress(hDll, "SolveInit");
        SolveFull = (SolveFullFunc)GetProcAddress(hDll, "SolveFull");
        DestroyNelderMead = (DestroyNelderMeadFunc)GetProcAddress(hDll, "DestroyNelderMead");

        ASSERT_TRUE(CreateNelderMead != nullptr) << "Функция CreateNelderMead не найдена!";
        ASSERT_TRUE(SolveBasic != nullptr) << "Функция SolveBasic не найдена!";
        ASSERT_TRUE(SolveWithValue != nullptr) << "Функция SolveWithValue не найдена!";
        ASSERT_TRUE(SolveInit != nullptr) << "Функция SolveInit не найдена!";
        ASSERT_TRUE(SolveFull != nullptr) << "Функция SolveFull не найдена!";
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
SolveFunc NelderMeadTestFixture::SolveBasic = nullptr;
SolveWithValueFunc NelderMeadTestFixture::SolveWithValue = nullptr;
SolveInitFunc NelderMeadTestFixture::SolveInit= nullptr;
SolveFullFunc NelderMeadTestFixture::SolveFull = nullptr;
DestroyNelderMeadFunc NelderMeadTestFixture::DestroyNelderMead = nullptr;