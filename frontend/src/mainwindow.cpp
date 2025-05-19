#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include <QDateTime>
#include <QProcess>    // Для запуска блокнота
#include <QFile>

#include <NelderMead.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->calcBut, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(ui->clearBut, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);
    connect(ui->helpBut, &QPushButton::clicked, this, &MainWindow::onHelpButtonClicked);
    connect(ui->showLogsButton, &QPushButton::clicked, this, &MainWindow::onShowLogsClicked);

    connect(ui->historyListWidget, &QListWidget::itemClicked, this, &MainWindow::onHistoryItemClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onSendButtonClicked()
{
    QString inputText = ui->inputLineEdit->text().trimmed();

    if (inputText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Поле ввода пустое!");
        return;
    }

    // Проверяем, что используются только x1, x2, ..., xn
    QRegularExpression varRegex(R"(\b(?!x\d+\b)[a-zA-Z_]\w*\b)"); // Ищет другие переменные
    if (varRegex.match(inputText).hasMatch()) {
        QMessageBox::warning(
            this,
            "Ошибка",
            "Используйте только переменные вида x1, x2, ..., xn!\n"
            "Пример: x1^2 + sin(x2) + 3*x3"
            );
        return;
    }

    try {
        NelderMead solver(inputText.toStdString());
        X result = solver.Solver();

        QString resultStr;
        for (size_t i = 0; i < result.coordinates.size(); ++i) {
            if (i > 0) resultStr += ", ";
            resultStr += QString("x%1 = %2").arg(i + 1).arg(result.coordinates[i]);
        }
        resultStr += QString("\nЗначение функции: %1").arg(result.value);

        ui->outputLineEdit->setText(resultStr);
        addToHistory(inputText + " → " + resultStr);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка вычисления: %1").arg(e.what()));
    }
}
void MainWindow::onShowLogsClicked() {
    // Путь к файлу логов (в папке build/bin/Release)
    QString logPath = QCoreApplication::applicationDirPath() + "/logfile.txt";

    // Проверяем, существует ли файл
    if (!QFile::exists(logPath)) {
        QMessageBox::warning(this, "Ошибка",
                             "Файл логов не найден:\n" + logPath +
                                 "\nУбедитесь, что программа уже сохраняла логи.");
        return;
    }

    // Открываем файл в блокноте Windows
    if (!QProcess::startDetached("notepad.exe", {logPath})) {
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось открыть блокнот.\nПопробуйте открыть файл вручную:\n" + logPath);
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
    QString fullText = item->text();

    // Разделяем строку по стрелке "→" чтобы получить только выражение
    QStringList parts = fullText.split("→");

    if (!parts.isEmpty()) {
        // Берем первую часть (выражение) и убираем возможные пробелы по краям
        QString expression = parts[0].trimmed();

        // Переносим только выражение в поле ввода
        ui->inputLineEdit->setText(expression);
    }
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
