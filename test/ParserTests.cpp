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

TEST_F(ParserTestFixture, CreateAndDestroy) {
    ParserHandle parser = CreateParser("x1+1");
    ASSERT_NE(parser, nullptr);
    DestroyParser(parser);
}

TEST_F(ParserTestFixture, SimpleEvaluation1) {
    ParserHandle parser = CreateParser("x1-x2+x3");
    std::unordered_map<std::string, double> variables = {{"x1", 50}, {"x2", 20}, {"x3", 70}};
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), 100.0);
}

TEST_F(ParserTestFixture, SimpleEvaluation2) {
    ParserHandle parser = CreateParser("x1+50");
    std::unordered_map<std::string, double> variables = {{"x1", 10.0}};
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), 60.0);
}

TEST_F(ParserTestFixture, SimpleEvaluation3) {
    ParserHandle parser = CreateParser("x1-x2");
    std::unordered_map<std::string, double> variables = {{"x1", 10}, {"x2", 30}};
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), -20.0);
}

TEST_F(ParserTestFixture, BasicMathOperations) {
    ParserHandle parser = CreateParser("(x1 + x2) * (x3 - x4) / x5");
    std::unordered_map<std::string, double> variables = {{"x1", 3.0}, {"x2", 2.0}, {"x3", 7.0}, {"x4", 2.0}, {"x5", 2.5}};
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), 10.0);
}

TEST_F(ParserTestFixture, UnaryMinus) {
    ParserHandle parser = CreateParser("-x1 + x2");
    std::unordered_map<std::string, double> variables = {{"x1", 5.0}, {"x2", 10.0}};
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), 5.0);
}

// Тест на константные выражения
TEST_F(ParserTestFixture, ConstantExpressions) {
    ParserHandle parser = CreateParser("2 + 3 * 5");
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, {}), 17.0);

}

// Тесты на математические функции
TEST_F(ParserTestFixture, MathFunctions) {
    ParserHandle parser = CreateParser("sin(x1) + cos(x2) + exp(x3) + abs(x4)");
    std::unordered_map<std::string, double> variables = {{"x1", 0.0}, {"x2", 0.0}, {"x3", 0.0}, {"x4", -2.0}};
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), 4.0);
}

TEST_F(ParserTestFixture, Sqrt) {
    ParserHandle parser = CreateParser("sqrt(x1)");
    std::unordered_map<std::string, double> variables = {{"x1", 9.0}};
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), 3.0);
}

// Тест на базовый приоритет операций (умножение перед сложением)
TEST_F(ParserTestFixture, MultiplicationBeforeAddition) {
    ParserHandle parser = CreateParser("x1 + x2 * x3");
    std::unordered_map<std::string, double> variables = {{"x1", 1.0}, {"x2", 2.0}, {"x3", 3.0}};
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), 7.0); // 1 + (2*3) = 7, а не (1+2)*3=9
}

// Тест на приоритет деления перед сложением
TEST_F(ParserTestFixture, DivisionBeforeAddition) {
    ParserHandle parser = CreateParser("x1 + x2 / x3");
    std::unordered_map<std::string, double> variables = {{"x1", 1.0}, {"x2", 6.0}, {"x3", 2.0}};
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), 4.0); // 1 + (6/2) = 4, а не (1+6)/2=3.5
}

// Тест на влияние скобок на приоритет
TEST_F(ParserTestFixture, ParenthesesOverride) {
    ParserHandle parser = CreateParser("(x1 + x2) * x3");
    std::unordered_map<std::string, double> variables = {{"x1", 1.0}, {"x2", 2.0}, {"x3", 3.0}};
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), 9.0); // (1+2)*3 = 9, а не 1+(2*3)=7
}

// Тест на вложенные скобки
TEST_F(ParserTestFixture, NestedParentheses) {
    ParserHandle parser = CreateParser("(x1 + (x2 * x3)) / (x4 - x5)");
    std::unordered_map<std::string, double> variables = {{"x1", 5.0}, {"x2", 3.0}, {"x3", 4.0}, {"x4", 7.0}, {"x5", 2.0}};
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), 3.4);
}

// Тест на приоритет степени
TEST_F(ParserTestFixture, ExponentiationPrecedence) {
    ParserHandle parser = CreateParser("x1 * x2 ^ x3");
    std::unordered_map<std::string, double> variables = {{"x1", 2.0}, {"x2", 3.0}, {"x3", 2.0}};
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), 18.0); // 2 * (3^2) = 18, а не (2*3)^2=36
}

// Тест на сочетание разных операций
TEST_F(ParserTestFixture, MixedOperations) {
    ParserHandle parser = CreateParser("x1 + x2 * x3 ^ x4");
    std::unordered_map<std::string, double> variables = {{"x1", 1.0}, {"x2", 2.0}, {"x3", 3.0}, {"x4", 2.0}};
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), 19.0); // 1 + (2*(3^2)) = 19
}

// Тест на унарный минус и приоритет
TEST_F(ParserTestFixture, UnaryMinusPrecedence) {
    std::unordered_map<std::string, double> variables = {{"x1", 3.0}};

    ParserHandle parser = CreateParser("(-x1) ^ 2");
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), 9.0);

    ParserHandle parserTwo = CreateParser("-x1 ^ 2");
    EXPECT_DOUBLE_EQ(EvaluateExpression(parserTwo, variables), -9.0); // -(3^2) = -9, а не (-3)^2=9
}

// Тест на приоритет функций
TEST_F(ParserTestFixture, FunctionPrecedence) {
    ParserHandle parser = CreateParser("sin(x1 + x2) * cos(x3 - x4)");
    std::unordered_map<std::string, double> variables = {{"x1", 0.5}, {"x2", 0.5}, {"x3", 1.0}, {"x4", 0.5}};
    EXPECT_NEAR(EvaluateExpression(parser, variables), 0.8415 * 0.8776, 1e-4);
}

// Тест на сложное сочетание операций
TEST_F(ParserTestFixture, ComplexExpression) {
    ParserHandle parser = CreateParser("(x1 + x2 * x3) ^ (x4 / x5) - x6");
    std::unordered_map<std::string, double> variables = {{"x1", 1.0}, {"x2", 2.0}, {"x3", 3.0}, {"x4", 4.0}, {"x5", 2.0}, {"x6", 9.0}};
    EXPECT_DOUBLE_EQ(EvaluateExpression(parser, variables), 40.0);
}
