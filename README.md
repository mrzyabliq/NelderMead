# NelderMead
Кроссплатформенный проект динамической библиотеки с реализацией метода Нелдера - Мида.

# Инструкция
Сборка осуществляется с помощью CMake.
## Сборка проекта на Windows и Linux
-Windows: запустите ```buildme.bat``` для сборки проекта
-Linux: (добавить)
## Запуск тестов и расположение библиотеки (Windows)
- Запуск тестов: ```runtests.bat``` или ```ctest -C Debug --output-on-failure```
- Расположениее dll ```build/Debug/NelderMead.dll```
## Запуск тестов и расположение библиотеки (Linux)
- Запуск тестов: ```./NelderMeadTests``` или ```ctest -C Debug --output-on-failure```
- Расположениее dll ```build/libNelderMead.so```
