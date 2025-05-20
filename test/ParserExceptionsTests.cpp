#pragma once
#include "gtest/gtest.h"
#include "ParserTestFixture.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <libloaderapi.h>
#else
    // Linux-эквиваленты, если нужны
    #include <dlfcn.h>  // Для динамической загрузки (если используется)
#endif

TEST_F(ParserTestFixture, DivisionByZero) {
    ParserHandle parser = CreateParser("x1 / x2");
    ASSERT_NE(parser, nullptr);
    
    std::unordered_map<std::string, double> variables = {{"x1", 5.0}, {"x2", 0.0}};
    
    EXPECT_THROW({
        try {
            EvaluateExpression(parser, variables);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Division by zero"), std::string::npos);
            throw; 
        }
    }, std::runtime_error);
    
    DestroyParser(parser);
}

// Тест на некорректные числа
TEST_F(ParserTestFixture, InvalidNumberFormat) {
    // Две точки в числе

    std::unordered_map<std::string, double> variables = {{"x1", 5.0}};

    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("3.14.15 + x1");
            EvaluateExpression(parser, variables);
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Invalid number"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
    
    // Точка без цифр после
    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("123. + x1");
            EvaluateExpression(parser, variables);
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Invalid number"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
    
    // Точка без цифр перед
    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser(".5 + x1");
            EvaluateExpression(parser, variables);
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Invalid character"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
}

// Тест на неожиданные токены
TEST_F(ParserTestFixture, UnexpectedTokens) {
    // Недопустимый символ
    std::unordered_map<std::string, double> variables = {{"x1", 5.0}, {"x2", 0.0}};
    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("x1 @ x2");
            EvaluateExpression(parser, variables);
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Invalid character"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
    
    // Несбалансированные скобки
    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("(x1 + x2");
            EvaluateExpression(parser, variables);
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Missing closing parenthesis"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
    
    
    // Пустые скобки
    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("()");
            EvaluateExpression(parser, variables);
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Invalid input"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
}

// Тест на некорректные имена переменных
TEST_F(ParserTestFixture, InvalidVariableNames) {

    std::unordered_map<std::string, double> variables = {{"x1", 5.0}};
    
    // Недопустимые символы в имени
    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("x@1 + 2");
            EvaluateExpression(parser, variables);
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Invalid"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
}

// Тест на некорректные количество переменных
TEST_F(ParserTestFixture, InvalidCountVariables) {

    std::unordered_map<std::string, double> variables = {{"x1", 5.0}};
    
    // Недопустимые символы в имени
    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("x1 + x2");
            EvaluateExpression(parser, variables);
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Invalid input"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
}

// Тест на синтаксические ошибки
TEST_F(ParserTestFixture, SyntaxErrors) {
    // Два оператора подряд

    std::unordered_map<std::string, double> variables = {{"x1", 5.0}, {"x2", 0.0}};

    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("x1 + * x2");
            EvaluateExpression(parser, variables);
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Unexpected token"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
    
    // Оператор в начале выражения
    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("* x1 + x2");
            EvaluateExpression(parser, variables);
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Unexpected token"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
    
    // Оператор в конце выражения
    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("x1 + x2 *");
            EvaluateExpression(parser, variables);
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Unexpected token"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
}

// Тест на обработку пустого выражения
TEST_F(ParserTestFixture, EmptyExpression) {
    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("");
            EvaluateExpression(parser, {});
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Empty expression"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
    
    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("      ");
            EvaluateExpression(parser, {});
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Empty expression"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
}

// Тест на некорректные константы
TEST_F(ParserTestFixture, InvalidConstants) {
    // Некорректное имя константы
     std::unordered_map<std::string, double> variables = {{"x1", 5.0}};

    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("x1 + test");
            EvaluateExpression(parser, variables);
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Unexpected token"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
}

// Тест на некорректное возведение в степень
TEST_F(ParserTestFixture, InvalidExponentiation) {
    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("2 ^ ^ 3");
            EvaluateExpression(parser, {});
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Unexpected token"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
    
    EXPECT_THROW({
        try {
            ParserHandle parser = CreateParser("^3");
            EvaluateExpression(parser, {});
            DestroyParser(parser);
        }
        catch (const std::runtime_error& e) {
            EXPECT_NE(std::string(e.what()).find("Unexpected token"), std::string::npos);
            throw;
        }
    }, std::runtime_error);
}