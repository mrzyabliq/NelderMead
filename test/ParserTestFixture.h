#pragma once
#include <gtest/gtest.h>
#include <windows.h>
#include <vector>
#include <unordered_map>
#include <string>

class ParserTestFixture : public ::testing::Test {
protected:
    // Типы для C-интерфейса парсера
    typedef void* ParserHandle;
    typedef ParserHandle (*CreateParserFunc)(const char*);
    typedef double (*ParserCalcFunc)(ParserHandle, const char*[], double[], int);
    typedef void (*DestroyParserFunc)(ParserHandle);

    static HMODULE hDll;
    static CreateParserFunc CreateParser;
    static ParserCalcFunc Calc;
    static DestroyParserFunc DestroyParser;

    static void SetUpTestSuite() {
        hDll = LoadLibraryW(L"NelderMead.dll");
        ASSERT_TRUE(hDll != nullptr) 
            << "DLL load failed. Error: " << GetLastError();

        CreateParser = reinterpret_cast<CreateParserFunc>(
            GetProcAddress(hDll, "CreateParser"));
        Calc = reinterpret_cast<ParserCalcFunc>(
            GetProcAddress(hDll, "ParserCalc"));
        DestroyParser = reinterpret_cast<DestroyParserFunc>(
            GetProcAddress(hDll, "DestroyParser"));

        ASSERT_TRUE(CreateParser) << "CreateParser not found in DLL";
        ASSERT_TRUE(Calc) << "ParserCalc not found in DLL";
        ASSERT_TRUE(DestroyParser) << "DestroyParser not found in DLL";
    }

    static void TearDownTestSuite() {
        if (hDll) {
            FreeLibrary(hDll);
            hDll = nullptr;
        }
    }

    double EvaluateExpression(
        ParserHandle parser,
        const std::unordered_map<std::string, double>& vars
    ) {
        std::vector<const char*> names;
        std::vector<double> values;
        
        for (const auto& [name, val] : vars) {
            names.push_back(name.c_str());
            values.push_back(val);
        }
        
        return Calc(parser, names.data(), values.data(), static_cast<int>(vars.size()));
    }
};

HMODULE ParserTestFixture::hDll = nullptr;
ParserTestFixture::CreateParserFunc ParserTestFixture::CreateParser = nullptr;
ParserTestFixture::ParserCalcFunc ParserTestFixture::Calc = nullptr;
ParserTestFixture::DestroyParserFunc ParserTestFixture::DestroyParser = nullptr;