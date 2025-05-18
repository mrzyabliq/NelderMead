#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include <QDateTime>

#include <NelderMead.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->calcBut, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(ui->clearBut, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);
    connect(ui->helpBut, &QPushButton::clicked, this, &MainWindow::onHelpButtonClicked);

    connect(ui->historyListWidget, &QListWidget::itemClicked, this, &MainWindow::onHistoryItemClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onSendButtonClicked()
{
    QString inputText = ui->inputLineEdit->text();

    if (inputText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Поле ввода пустое!");
        return;
    }

    try {
        // 1. Создаём экземпляр NelderMead
        NelderMead solver(inputText.toStdString());

        // 2. Запускаем оптимизацию
        X result = solver.Solver();

        // 3. Формируем результат
        QString resultStr;
        for (size_t i = 0; i < result.coordinates.size(); ++i) {
            if (i > 0) resultStr += ", ";
            resultStr += QString("x%1 = %2").arg(i + 1).arg(result.coordinates[i]);
        }

        // 4. Добавляем значение функции
        resultStr += QString("\nЗначение функции: %1").arg(result.value);

        // 5. Выводим результат
        ui->outputLineEdit->setText(resultStr);
        addToHistory(inputText + " → " + resultStr);
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка вычисления: %1").arg(e.what()));
    }
}

// Слот для кнопки "Очистить"
void MainWindow::onClearButtonClicked()
{
    ui->inputLineEdit->clear(); // Очищаем поле ввода
    ui->outputLineEdit->clear(); // Очищаем поле вывода
}

void MainWindow::addToHistory(const QString &text)
{
    // Проверяем, чтобы не добавлять пустые строки
    if (text.isEmpty()) {
        return;
    }

    // Добавляем текст в список истории
    ui->historyListWidget->addItem(text);

    // Прокручиваем список вниз, чтобы новый элемент был виден
    ui->historyListWidget->scrollToBottom();
}
// Слот для выбора элемента из списка
void MainWindow::onHistoryItemClicked(QListWidgetItem *item)
{
    // Получаем текст выбранного элемента
    QString selectedText = item->text();

    // Переносим текст в поле ввода
    ui->inputLineEdit->setText(selectedText);
}

void MainWindow::onHelpButtonClicked()
{
    // Текст сообщения
    QString helpText = R"(
Правила ввода функций:
1. Все переменные выражаются через x1, x2 и т.д.
2. Все математические операции выражаются через общепринятые символы +, -, *, /, ^
3. Корень квадратный √¯ ≡ sqrt, степень корня ≡ x^(2/3), (x-1)^(1/3)
   число π ≡ pi, число e ≡ exp(1), ∞ ≡ infinity
   ex = exp(x), log5(x) ≡ log(x,5)
4. Тригонометрические функции: cos(x), sin(x), tg(x), ctg(x), arccos(x), arcsin(x), arctg(x), arcctg(x)
)";

    // Отображаем сообщение
    QMessageBox::information(this, "Справка", helpText);
}
