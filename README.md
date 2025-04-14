# NelderMead
Кроссплатформенный проект динамической библиотеки с реализацией метода Нелдера - Мида.

# Инструкция
Сборка осуществляется с помощью CMake.
## Сборка проекта на Windows и Linux
1. В корневой директории создайте папку для сборки:
```mkdir build```
2. Перейдите в папку сборки:
```cd build```
3. Сконфигурируйте проект:
```cmake ..```
4. Соберите проект:
```cmake --build .```

## Запуск тестов и расположение библиотеки (Windows)
- Запуск тестов: ```./Debug/NelderMeadTests.exe``` или ```ctest -C Debug --output-on-failure```
- Расположениее dll ```build/Debug/NelderMead.dll```
## Запуск тестов и расположение библиотеки (Linux)
- Запуск тестов: ```./NelderMeadTests``` или ```ctest -C Debug --output-on-failure```
- Расположениее dll ```build/libNelderMead.so```
