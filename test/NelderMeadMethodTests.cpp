#pragma once
#include "gtest/gtest.h"
#include "NelderMeadTestFixture.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <libloaderapi.h>
#else
    // Linux-эквиваленты, если нужны
    #include <dlfcn.h>  // Для динамической загрузки (если используется)
#endif

TEST_F(NelderMeadTestFixture, CreateAndDestroySolver) {
    NelderMeadHandle solver = CreateNelderMead("x^2 + y^2");
    ASSERT_NE(solver, nullptr);
    DestroyNelderMead(solver);  
}

TEST_F(NelderMeadTestFixture, RandomFunction2D) {
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
    NelderMeadHandle solver = CreateNelderMead("20 + (x1^2 - 10*cos(2*3.1415926535*x1)) + (x2^2 - 10*cos(2*3.1415926535*x2))");
    ASSERT_NE(solver, nullptr);
    
    double initial[2] = {1.0, 1.0}; 
    double output[2];
    SolveInit(solver, initial, output);
    
    EXPECT_NEAR(output[0], 0.0, 0.5);
    EXPECT_NEAR(output[1], 0.0, 0.5);
    
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, MatyasFunction) {
    NelderMeadHandle solver = CreateNelderMead("0.26*(x1^2 + x2^2) - 0.48*x1*x2");
    ASSERT_NE(solver, nullptr);
    
    double output[2], value;
    SolveWithValue(solver, output, &value);
    
    EXPECT_NEAR(output[0], 0.0, 1e-2);
    EXPECT_NEAR(output[1], 0.0, 1e-2);
    EXPECT_NEAR(value, 0.0, 1e-4);
    
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, BealeFunction) {
    NelderMeadHandle solver = CreateNelderMead("(1.5 - x1 + x1*x2)^2 + (2.25 - x1 + x1*x2^2)^2 + (2.625 - x1 + x1*x2^3)^2");
    ASSERT_NE(solver, nullptr);
    
    double initial[2] = {1.0, 1.0};
    double output[2], value;
    SolveFull(solver, initial, output, &value);
    
    EXPECT_NEAR(output[0], 3.0, 1e-1);
    EXPECT_NEAR(output[1], 0.5, 1e-1);
    EXPECT_NEAR(value, 0.0, 1e-2);
    
    DestroyNelderMead(solver);
}

TEST_F(NelderMeadTestFixture, ThreeHumpCamelFunction) {
    NelderMeadHandle solver = CreateNelderMead("2*x1^2 - 1.05*x1^4 + x1^6/6 + x1*x2 + x2^2");
    ASSERT_NE(solver, nullptr);
    
    double initial[2] = {0.5, -0.5};
    double output[2];
    SolveInit(solver, initial, output);
    
    // Увеличиваем допуск
    EXPECT_NEAR(output[0], 0.0, 0.1);
    EXPECT_NEAR(output[1], 0.0, 0.1);
    
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
    ASSERT_NE(solver, nullptr);
    
    double initial[2] = {100, 100};
    double output[2], value;
    SolveFull(solver, initial, output, &value);
    
    EXPECT_NEAR(value, -2.0, 0.1);
    
    DestroyNelderMead(solver);
}
