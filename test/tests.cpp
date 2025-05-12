#include "gtest/gtest.h"
#include "../include/NelderMead.h"
#include "../include/Parser.h"
//#include <iostream>
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <libloaderapi.h>
#else
    // Linux-эквиваленты, если нужны
    #include <dlfcn.h>  // Для динамической загрузки (если используется)
#endif


TEST(ParserSimpleTest, CreateAndDestroy) {
    Parser parser("x1+1");
    ASSERT_NE(parser.num_of_variables, 0);
}

TEST(ParserSimpleTest, SimpleEvaluation1) {
    Parser parser("x1-x2+x3");
    std::unordered_map<std::string, double> variables = {{"x1", 50}, {"x2", 20}, {"x3", 70}};
    EXPECT_DOUBLE_EQ(parser.calc(variables), 100.0);
}

TEST(ParserSimpleTest, SimpleEvaluation2) {
    Parser parser("x1+50");
    std::unordered_map<std::string, double> variables = {{"x1", 10.0}};
    EXPECT_DOUBLE_EQ(parser.calc(variables), 60.0);
}

TEST(ParserSimpleTest, SimpleEvaluation3) {
    Parser parser("x1-x2");
    std::unordered_map<std::string, double> variables = {{"x1", 10}, {"x2", 30}};
    EXPECT_DOUBLE_EQ(parser.calc(variables), -20.0);
}

TEST(ParserSimpleTest, BasicMathOperations) {
    Parser parser("(x1 + x2) * (x3 - x4) / x5");
    std::unordered_map<std::string, double> variables = {{"x1", 3.0}, {"x2", 2.0}, {"x3", 7.0}, {"x4", 2.0}, {"x5", 2.5}};
    EXPECT_DOUBLE_EQ(parser.calc(variables), 10.0);
}

TEST(ParserSimpleTest, UnaryMinus) {
    Parser parser("-x1 + x2");
    std::unordered_map<std::string, double> variables = {{"x1", 5.0}, {"x2", 10.0}};
    EXPECT_DOUBLE_EQ(parser.calc(variables), 5.0);
}

// Тест на константные выражения
TEST(ParserSimpleTest, ConstantExpressions) {
    Parser parser("2 + 3 * 5");
    EXPECT_DOUBLE_EQ(parser.calc({}), 17.0);

}

// Тесты на математические функции
TEST(ParserSimpleTest, MathFunctions) {
    Parser parser("sin(x1) + cos(x2) + exp(x3) + abs(x4)");
    std::unordered_map<std::string, double> variables = {{"x1", 0.0}, {"x2", 0.0}, {"x3", 0.0}, {"x4", -2.0}};
    EXPECT_DOUBLE_EQ(parser.calc(variables), 4.0);
}

TEST(ParserSimpleTest, Sqrt) {
    Parser parser("sqrt(x1)");
    std::unordered_map<std::string, double> variables = {{"x1", 9.0}};
    EXPECT_DOUBLE_EQ(parser.calc(variables), 3.0);
}

// Тест на базовый приоритет операций (умножение перед сложением)
TEST(ParserPrecedenceTest, MultiplicationBeforeAddition) {
    Parser parser("x1 + x2 * x3");
    std::unordered_map<std::string, double> variables = {{"x1", 1.0}, {"x2", 2.0}, {"x3", 3.0}};
    EXPECT_DOUBLE_EQ(parser.calc(variables), 7.0); // 1 + (2*3) = 7, а не (1+2)*3=9
}

// Тест на приоритет деления перед сложением
TEST(ParserPrecedenceTest, DivisionBeforeAddition) {
    Parser parser("x1 + x2 / x3");
    std::unordered_map<std::string, double> variables = {{"x1", 1.0}, {"x2", 6.0}, {"x3", 2.0}};
    EXPECT_DOUBLE_EQ(parser.calc(variables), 4.0); // 1 + (6/2) = 4, а не (1+6)/2=3.5
}

// Тест на влияние скобок на приоритет
TEST(ParserPrecedenceTest, ParenthesesOverride) {
    Parser parser("(x1 + x2) * x3");
    std::unordered_map<std::string, double> variables = {{"x1", 1.0}, {"x2", 2.0}, {"x3", 3.0}};
    EXPECT_DOUBLE_EQ(parser.calc(variables), 9.0); // (1+2)*3 = 9, а не 1+(2*3)=7
}

// Тест на вложенные скобки
TEST(ParserPrecedenceTest, NestedParentheses) {
    Parser parser("(x1 + (x2 * x3)) / (x4 - x5)");
    std::unordered_map<std::string, double> variables = {{"x1", 5.0}, {"x2", 3.0}, {"x3", 4.0}, {"x4", 7.0}, {"x5", 2.0}};
    EXPECT_DOUBLE_EQ(parser.calc(variables), 3.4);
}

// Тест на приоритет степени
TEST(ParserPrecedenceTest, ExponentiationPrecedence) {
    Parser parser("x1 * x2 ^ x3");
    std::unordered_map<std::string, double> variables = {{"x1", 2.0}, {"x2", 3.0}, {"x3", 2.0}};
    EXPECT_DOUBLE_EQ(parser.calc(variables), 18.0); // 2 * (3^2) = 18, а не (2*3)^2=36
}

// Тест на сочетание разных операций
TEST(ParserPrecedenceTest, MixedOperations) {
    Parser parser("x1 + x2 * x3 ^ x4");
    std::unordered_map<std::string, double> variables = {{"x1", 1.0}, {"x2", 2.0}, {"x3", 3.0}, {"x4", 2.0}};
    EXPECT_DOUBLE_EQ(parser.calc(variables), 19.0); // 1 + (2*(3^2)) = 19
}

// Тест на унарный минус и приоритет
TEST(ParserPrecedenceTest, UnaryMinusPrecedence) {
    std::unordered_map<std::string, double> variables = {{"x1", 3.0}};

    Parser parser("(-x1) ^ 2");
    EXPECT_DOUBLE_EQ(parser.calc(variables), 9.0);

    parser = Parser("-x1 ^ 2");
    EXPECT_DOUBLE_EQ(parser.calc(variables), -9.0); // -(3^2) = -9, а не (-3)^2=9
}

// Тест на приоритет функций
TEST(ParserPrecedenceTest, FunctionPrecedence) {
    Parser parser("sin(x1 + x2) * cos(x3 - x4)");
    std::unordered_map<std::string, double> variables = {{"x1", 0.5}, {"x2", 0.5}, {"x3", 1.0}, {"x4", 0.5}};
    EXPECT_NEAR(parser.calc(variables), 0.8415 * 0.8776, 1e-4);
}

// Тест на сложное сочетание операций
TEST(ParserPrecedenceTest, ComplexExpression) {
    Parser parser("(x1 + x2 * x3) ^ (x4 / x5) - x6");
    std::unordered_map<std::string, double> variables = {{"x1", 1.0}, {"x2", 2.0}, {"x3", 3.0}, {"x4", 4.0}, {"x5", 2.0}, {"x6", 9.0}};
    EXPECT_DOUBLE_EQ(parser.calc(variables), 40.0);
}

TEST(NelderMeadSimpleTest, Function1) {
    //HMODULE hlib = LoadLibrary(TEXT("../build/Debug/NelderMead.dll"));
    NelderMeadHandle* solver = CreateNelderMead("x1^2 + x1*x2 + x2^2 -6*x1 - 9*x2 ");
    double output[2];
    Solve(solver, output);
    EXPECT_DOUBLE_EQ(output[0], 1);
    EXPECT_DOUBLE_EQ(output[1], 4);
}

TEST(NelderMeadSimpleTest, RozenbrockFunction) {
    //HMODULE hlib = LoadLibrary(TEXT("NelderMead.dll"));
    NelderMeadHandle* solver = CreateNelderMead("100*(x2 - x1^2)^2 + (1 - x1)^2");
    double output[2];
    Solve(solver, output);
    EXPECT_DOUBLE_EQ(output[0], 1.0);
    EXPECT_DOUBLE_EQ(output[1], 1.0);
}

TEST(NelderMeadSimpleTest, SquareFunction) {
    //HMODULE hlib = LoadLibrary(TEXT("../build/Debug/NelderMead.dll"));
    NelderMeadHandle* solver = CreateNelderMead("x1^2 + 2*x2^2 +3*x3^2");
    double output[3];
    Solve(solver, output);
    EXPECT_DOUBLE_EQ(output[0], 0);
    EXPECT_DOUBLE_EQ(output[1], 0);
    EXPECT_DOUBLE_EQ(output[2], 0);
}

// Тест на функцию Розенброка (известный сложный случай)
TEST(NelderMead, Rosenbrock) {
    NelderMeadHandle* handle = CreateNelderMead("100*(y - x^2)^2 + (1 - x)^2");
    ASSERT_NE(handle, nullptr);

    double result[2];
    Solve(handle, result);

    EXPECT_NEAR(result[0], 1.0, 1e-3);
    EXPECT_NEAR(result[1], 1.0, 1e-3);

    DestroyNelderMead(handle);
}

TEST(NelderMead, TetsDllImport)
{
    
}
// Тесты на особые случаи
// TEST(ParserExceptionsTest, DivisionByZero) {
//     Parser parser("x1 / x2");
//     SetVariable(parser, "x1", 1.0);
//     SetVariable(parser, "x2", 0.0);

//     EXPECT_THROW({
//         try {
//             parser.calc(variables);
//         }catch (const runtime_error& e) {
//         EXPECT_STREQ(e.what(), "Division by zero");
//         throw;
//         }
//     }, runtime_error);
//   
// }

// TEST(ParserExceptionsTest, InvalidCharacter) {
//     EXPECT_THROW({
//         try {
//             Parser("x1 @ x2");
//         }catch (const runtime_error& e) {
//         EXPECT_STREQ(e.what(), "Invalid character");
//         throw;
//         }
//     }, runtime_error);
//}


