#pragma once
#include "gtest/gtest.h"
#include "NelderMeadTestFixture.h"
#include "ParserTestFixture.h"
//#include <iostream>
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

//------------NELDER_MEAD_TESTS---------------------------

TEST_F(NelderMeadTestFixture, CreateAndDestroySolver) {
    NelderMeadHandle solver = CreateNelderMead("x^2 + y^2");
    ASSERT_NE(solver, nullptr);
    DestroyNelderMead(solver);  
}

TEST_F(NelderMeadTestFixture, Function1) {
    NelderMeadHandle solver = CreateNelderMead("x1^2 + x1*x2 + x2^2 -6*x1 - 9*x2 ");
    double output[2];
    SolveBasic(solver, output);
    EXPECT_NEAR(output[0], 1, 1e-3);
    EXPECT_NEAR(output[1], 4, 1e-3);
}

// Тест на функцию Розенброка
TEST_F(NelderMeadTestFixture, RozenbrockFunction) {
    NelderMeadHandle solver = CreateNelderMead("100*(x1 - x2^2)^2 + (1 - x2)^2");
    if (!solver) {
        std::cerr << "Failed to create solver!" << std::endl;
        return;
    }
    double output[2];
    SolveBasic(solver, output);
    EXPECT_NEAR(output[0], 1, 1e-3);
    EXPECT_NEAR(output[1], 1, 1e-3);

    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, HimmelblauFunction1) {
    NelderMeadHandle solver = CreateNelderMead("(x1^2 + x2 - 11)^2 + (x1 + x2^2 - 7)^2");
    double initial[2] = {0, 0};
    double output[2];
    SolveInit(solver, initial, output);
    EXPECT_NEAR(output[0], 3.0, 1e-1);
    EXPECT_NEAR(output[1], 2.0, 1e-1);
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, RastriginFunction2D) {
    NelderMeadHandle solver = CreateNelderMead("20 + (x1^2 - 10*cos(2*pi*x1)) + (x2^2 - 10*cos(2*pi*x2))");
    double initial[2] = {2.5, 2.5};
    double output[2];
    SolveInit(solver, initial, output);
    EXPECT_NEAR(output[0], 0, 1e-1);
    EXPECT_NEAR(output[1], 0, 1e-1);
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, BukinFunction) {
    NelderMeadHandle solver = CreateNelderMead("100*sqrt(abs(x2 - 0.01*x1^2)) + 0.01*abs(x1 + 10)");
    double initial[2] = {-8, 2};
    double output[2];
    SolveInit(solver, initial, output);
    EXPECT_NEAR(output[0], -10, 1e-1);
    EXPECT_NEAR(output[1], 1, 1e-1);
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, SquareFunction) {
    NelderMeadHandle solver = CreateNelderMead("x1^2 + 2*x2^2 +3*x3^2");
    double output[3];
    SolveBasic(solver, output);
    EXPECT_NEAR(output[0], 0, 1e-3);
    EXPECT_NEAR(output[1], 0, 1e-3);
    EXPECT_NEAR(output[2], 0, 1e-3);
}

TEST_F(NelderMeadTestFixture, LinearFunction) {
    NelderMeadHandle solver = CreateNelderMead("2*x1 + 3*x2");
    double output[2];
    SolveBasic(solver, output);
    // Линейная функция не имеет минимума, но алгоритм должен сойтись к какой-то точке
    EXPECT_TRUE(std::isfinite(output[0]) && std::isfinite(output[1]));
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, QuadraticFunction3D) {
    NelderMeadHandle solver = CreateNelderMead("x1^2 + x2^2 + x3^2");
    double output[3];
    SolveBasic(solver, output);
    EXPECT_NEAR(output[0], 0, 1e-3);
    EXPECT_NEAR(output[1], 0, 1e-3);
    EXPECT_NEAR(output[2], 0, 1e-3);
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, 4DFunction) {
    NelderMeadHandle solver = CreateNelderMead("x1^2 + x2^2 + x3^2 + x4^2");
    double output[4];
    SolveBasic(solver, output);
    for (int i = 0; i < 4; ++i) {
        EXPECT_NEAR(output[i], 0, 1e-3);
    }
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, 5DFunction) {
    NelderMeadHandle solver = CreateNelderMead("x1^2 + x2^2 + x3^2 + x4^2 + x5^2");
    double output[5];
    SolveBasic(solver, output);
    for (int i = 0; i < 5; ++i) {
        EXPECT_NEAR(output[i], 0, 1e-3);
    }
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, ExponentialFunction) {
    NelderMeadHandle solver = CreateNelderMead("exp(x1) + exp(x2)");
    double output[2];
    SolveBasic(solver, output);
    // Минимум в (-∞, -∞), но алгоритм сойдется к некоторым большим отрицательным значениям
    EXPECT_LT(output[0], -10);
    EXPECT_LT(output[1], -10);
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, TrigonometricFunction) {
    NelderMeadHandle solver = CreateNelderMead("sin(x1)^2 + cos(x2)^2");
    double output[2], value;
    SolveWithValue(solver, output, &value);
    EXPECT_NEAR(value, 0, 1e-3);
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, FunctionValueCheck) {
    NelderMeadHandle solver = CreateNelderMead("x1^2 + x2^2");
    double output[2], value;
    SolveWithValue(solver, output, &value);
    EXPECT_NEAR(value, 0, 1e-6);
    EXPECT_NEAR(output[0], 0, 1e-3);
    EXPECT_NEAR(output[1], 0, 1e-3);
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, RosenbrockWithValue) {
    NelderMeadHandle solver = CreateNelderMead("100*(x2 - x1^2)^2 + (1 - x1)^2");
    double output[2], value;
    SolveWithValue(solver, output, &value);
    EXPECT_NEAR(value, 0, 1e-4);
    EXPECT_NEAR(output[0], 1, 1e-3);
    EXPECT_NEAR(output[1], 1, 1e-3);
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, LargeCoefficients) {
    NelderMeadHandle solver = CreateNelderMead("1e12*x1^2 + 1e12*x2^2");
    double output[2];
    SolveBasic(solver, output);
    EXPECT_NEAR(output[0], 0, 1e-3);
    EXPECT_NEAR(output[1], 0, 1e-3);
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, WithInitialPoint) {
    NelderMeadHandle solver = CreateNelderMead("(x1-3)^2 + (x2-4)^2");
    double initial[2] = {10, 10};
    double output[2];
    SolveInit(solver, initial, output);
    EXPECT_NEAR(output[0], 3, 1e-3);
    EXPECT_NEAR(output[1], 4, 1e-3);
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, FarInitialPoint) {
    NelderMeadHandle solver = CreateNelderMead("sin(x1) + cos(x2)");
    double initial[2] = {100, 100};
    double output[2], value;
    SolveFull(solver, initial, output, &value);
    EXPECT_NEAR(value, -2.0, 1e-2); // Минимум для sin(x)+cos(y)
    DestroyNelderMead(solver);
}


// TEST_F(NelderMeadTestFixture, DISmallCoefficients) {
//     NelderMeadHandle solver = CreateNelderMead("1e-12*x1^2 + 1e-12*x2^2");
//     double output[2];
//     SolveBasic(solver, output);
//     EXPECT_NEAR(output[0], 0, 1e-3);
//     EXPECT_NEAR(output[1], 0, 1e-3);
//     DestroyNelderMead(solver);
// }

// Тесты на особые случаи
// TEST_F(ParserExceptionsTest, DivisionByZero) {
//     Parser parser("x1 / x2");
//     SetVariable(parser, "x1", 1.0);
//     SetVariable(parser, "x2", 0.0);

//     EXPECT_THROW({
//         try {
//             EvaluateExpression(parser, variables);
//         }catch (const runtime_error& e) {
//         EXPECT_STREQ(e.what(), "Division by zero");
//         throw;
//         }
//     }, runtime_error);
//   
// }

// TEST_F(ParserExceptionsTest, InvalidCharacter) {
//     EXPECT_THROW({
//         try {
//             Parser("x1 @ x2");
//         }catch (const runtime_error& e) {
//         EXPECT_STREQ(e.what(), "Invalid character");
//         throw;
//         }
//     }, runtime_error);
//}


