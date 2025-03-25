#include "pch.h"
#include "NelderMead.h"

TEST(ParserSimpleTest, CreateAndDestroy) {
    ParserHandle* parser = CreateParser("x1+1");
    ASSERT_NE(parser, nullptr);
    DestroyParser(parser);
}

TEST(ParserSimpleTest, SimpleEvaluation1) {
    ParserHandle *parser = CreateParser("x1-x2+x3");
    SetVariable(parser, "x1", 50);
    SetVariable(parser, "x2", 20);
    SetVariable(parser, "x3", 70);
    EXPECT_DOUBLE_EQ(Evaluate(parser), 100.0);
    DestroyParser(parser);
}

TEST(ParserSimpleTest, SimpleEvaluation2) {
    ParserHandle* parser = CreateParser("x1+50");
    SetVariable(parser, "x1", 10.0);
    EXPECT_DOUBLE_EQ(Evaluate(parser), 60.0);
    DestroyParser(parser);
}

TEST(ParserSimpleTest, SimpleEvaluation3) {
    ParserHandle* parser = CreateParser("x1-x2");
    SetVariable(parser, "x1", 10);
    SetVariable(parser, "x2", 30);
    EXPECT_DOUBLE_EQ(Evaluate(parser), -20.0);
    DestroyParser(parser);
}

TEST(ParserSimpleTest, BasicMathOperations) {
    ParserHandle* parser = CreateParser("(x1 + x2) * (x3 - x4) / x5");
    SetVariable(parser, "x1", 3.0);
    SetVariable(parser, "x2", 2.0);  // 3+2=5
    SetVariable(parser, "x3", 7.0);
    SetVariable(parser, "x4", 2.0);  // 7-2=5
    SetVariable(parser, "x5", 2.5);  // 5*5=25 /2.5=10
    EXPECT_DOUBLE_EQ(Evaluate(parser), 10.0);
}

TEST(ParserSimpleTest, UnaryMinus) {
    ParserHandle* parser = CreateParser("-x1 + x2");
    SetVariable(parser, "x1", 5.0);
    SetVariable(parser, "x2", 10.0);
    EXPECT_DOUBLE_EQ(Evaluate(parser), 5.0);
    DestroyParser(parser);
}

// Тест на константные выражения
TEST(ParserSimpleTest, ConstantExpressions) {
    ParserHandle* parser = CreateParser("2 + 3 * 5");
    EXPECT_DOUBLE_EQ(Evaluate(parser), 17.0);

    DestroyParser(parser);
}

// Тесты на математические функции
TEST(ParserSimpleTest, MathFunctions) {
    ParserHandle* parser = CreateParser("sin(x1) + cos(x2) + exp(x3) + abs(x4)");
    SetVariable(parser, "x1", 0.0);       // sin(0)=0
    SetVariable(parser, "x2", 0.0);       // cos(0)=1
    SetVariable(parser, "x3", 0.0);       // exp(0)=1
    SetVariable(parser, "x4", -2.0);       // abs(-2)=2
    EXPECT_DOUBLE_EQ(Evaluate(parser), 4.0);
    DestroyParser(parser);
}

TEST(ParserSimpleTest, Sqrt) {
    ParserHandle* parser = CreateParser("sqrt(x1)");
    SetVariable(parser, "x1", 9.0);
    cout<< Evaluate(parser);
    EXPECT_DOUBLE_EQ(Evaluate(parser), 3.0);
    DestroyParser(parser);
}

// Тест на базовый приоритет операций (умножение перед сложением)
TEST(ParserPrecedenceTest, MultiplicationBeforeAddition) {
    ParserHandle* parser = CreateParser("x1 + x2 * x3");
    SetVariable(parser, "x1", 1.0);
    SetVariable(parser, "x2", 2.0);
    SetVariable(parser, "x3", 3.0);
    EXPECT_DOUBLE_EQ(Evaluate(parser), 7.0); // 1 + (2*3) = 7, а не (1+2)*3=9
    DestroyParser(parser);
}

// Тест на приоритет деления перед сложением
TEST(ParserPrecedenceTest, DivisionBeforeAddition) {
    ParserHandle* parser = CreateParser("x1 + x2 / x3");
    SetVariable(parser, "x1", 1.0);
    SetVariable(parser, "x2", 6.0);
    SetVariable(parser, "x3", 2.0);
    EXPECT_DOUBLE_EQ(Evaluate(parser), 4.0); // 1 + (6/2) = 4, а не (1+6)/2=3.5
    DestroyParser(parser);
}

// Тест на влияние скобок на приоритет
TEST(ParserPrecedenceTest, ParenthesesOverride) {
    ParserHandle* parser = CreateParser("(x1 + x2) * x3");
    SetVariable(parser, "x1", 1.0);
    SetVariable(parser, "x2", 2.0);
    SetVariable(parser, "x3", 3.0);
    EXPECT_DOUBLE_EQ(Evaluate(parser), 9.0); // (1+2)*3 = 9, а не 1+(2*3)=7
    DestroyParser(parser);
}

// Тест на вложенные скобки
TEST(ParserPrecedenceTest, NestedParentheses) {
    ParserHandle* parser = CreateParser("(x1 + (x2 * x3)) / (x4 - x5)");
    SetVariable(parser, "x1", 5.0);
    SetVariable(parser, "x2", 3.0);
    SetVariable(parser, "x3", 4.0); // 3*4=12 → 5+12=17
    SetVariable(parser, "x4", 7.0);
    SetVariable(parser, "x5", 2.0); // 7-2=5 → 17/5=3.4
    EXPECT_DOUBLE_EQ(Evaluate(parser), 3.4);
    DestroyParser(parser);
}

// Тест на приоритет степени
TEST(ParserPrecedenceTest, ExponentiationPrecedence) {
    ParserHandle* parser = CreateParser("x1 * x2 ^ x3");
    SetVariable(parser, "x1", 2.0);
    SetVariable(parser, "x2", 3.0);
    SetVariable(parser, "x3", 2.0);
    EXPECT_DOUBLE_EQ(Evaluate(parser), 18.0); // 2 * (3^2) = 18, а не (2*3)^2=36
    DestroyParser(parser);
}

// Тест на сочетание разных операций
TEST(ParserPrecedenceTest, MixedOperations) {
    ParserHandle* parser = CreateParser("x1 + x2 * x3 ^ x4");
    SetVariable(parser, "x1", 1.0);
    SetVariable(parser, "x2", 2.0);
    SetVariable(parser, "x3", 3.0);
    SetVariable(parser, "x4", 2.0);
    EXPECT_DOUBLE_EQ(Evaluate(parser), 19.0); // 1 + (2*(3^2)) = 19
    DestroyParser(parser);
}

// Тест на унарный минус и приоритет
TEST(ParserPrecedenceTest, UnaryMinusPrecedence) {
    ParserHandle* parser = CreateParser("(-x1) ^ 2");
    SetVariable(parser, "x1", 3.0);
    EXPECT_DOUBLE_EQ(Evaluate(parser), 9.0);
    DestroyParser(parser);

    parser = CreateParser("-x1 ^ 2");
    SetVariable(parser, "x1", 3.0);
    EXPECT_DOUBLE_EQ(Evaluate(parser), -9.0); // -(3^2) = -9, а не (-3)^2=9
    DestroyParser(parser);
}

// Тест на приоритет функций
TEST(ParserPrecedenceTest, FunctionPrecedence) {
    ParserHandle* parser = CreateParser("sin(x1 + x2) * cos(x3 - x4)");
    SetVariable(parser, "x1", 0.5);
    SetVariable(parser, "x2", 0.5); // sin(1.0) ≈ 0.8415
    SetVariable(parser, "x3", 1.0);
    SetVariable(parser, "x4", 0.5); // cos(0.5) ≈ 0.8776
    EXPECT_NEAR(Evaluate(parser), 0.8415 * 0.8776, 1e-4);
    DestroyParser(parser);
}

// Тест на сложное сочетание операций
TEST(ParserPrecedenceTest, ComplexExpression) {
    ParserHandle* parser = CreateParser("(x1 + x2 * x3) ^ (x4 / x5) - x6");
    SetVariable(parser, "x1", 1.0);
    SetVariable(parser, "x2", 2.0);
    SetVariable(parser, "x3", 3.0); // 1 + 2*3 = 7
    SetVariable(parser, "x4", 4.0);
    SetVariable(parser, "x5", 2.0); // 4/2 = 2 → 7^2=49
    SetVariable(parser, "x6", 9.0); // 49-9=40
    EXPECT_DOUBLE_EQ(Evaluate(parser), 40.0);
    DestroyParser(parser);
}

// Тесты на особые случаи
TEST(ParserExceptionsTest, DivisionByZero) {
    ParserHandle* parser = CreateParser("x1 / x2");
    SetVariable(parser, "x1", 1.0);
    SetVariable(parser, "x2", 0.0);

    EXPECT_THROW({
        try {
            Evaluate(parser);
        }catch (const runtime_error& e) {
        EXPECT_STREQ(e.what(), "Division by zero");
        throw;
        }
    }, runtime_error);
    DestroyParser(parser);
}

TEST(ParserExceptionsTest, InvalidCharacter) {
    EXPECT_THROW({
        try {
            CreateParser("x1 @ x2");
        }catch (const runtime_error& e) {
        EXPECT_STREQ(e.what(), "Invalid character");
        throw;
        }
    }, runtime_error);
}

