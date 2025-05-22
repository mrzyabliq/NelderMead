#include "mainwindow.h"

#include <QDateTime>
#include <QFile>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QProcess>  // Для запуска блокнота
#include <algorithm>
#include <stdexcept>
#include <vector>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      solverHandle(nullptr),
      needGraphUpdate(false)
      {
  ui->setupUi(this);

    alpha = 1.0;
    beta = 0.5;
    gamma = 2.0;
    sigma = 1.0;
    tolerance = 0.000001;
    startPoint = "";

  connect(ui->calcBut, &QPushButton::clicked, this,
          &MainWindow::onSendButtonClicked);
  connect(ui->clearBut, &QPushButton::clicked, this,
          &MainWindow::onClearButtonClicked);
  connect(ui->helpBut, &QPushButton::clicked, this,
          &MainWindow::onHelpButtonClicked);
  connect(ui->showLogsButton, &QPushButton::clicked, this,
          &MainWindow::onShowLogsClicked);
  connect(ui->showGraphButton, &QPushButton::clicked, this,
          &MainWindow::onShowGraphClicked);
  connect(ui->historyListWidget, &QListWidget::itemClicked, this,
          &MainWindow::onHistoryItemClicked);
  connect(ui->paramsButton, &QPushButton::clicked,
          this, &MainWindow::onParamsButtonClicked);
}

MainWindow::~MainWindow() {
  cleanupSolver();
  delete ui;
}
void MainWindow::cleanupSolver() {
  if (solverHandle) {
    DestroyNelderMead(solverHandle);
    solverHandle = nullptr;
  }
  needGraphUpdate = false;
}

void MainWindow::onSendButtonClicked() {
    QString inputText = ui->inputLineEdit->text().trimmed();
    if (inputText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите функцию");
        return;
    }

    cleanupSolver();
    needGraphUpdate = true;

    try {
        solverHandle = CreateNelderMead(inputText.toStdString().c_str());
        int dims = getDims(solverHandle);
        std::vector<double> result(dims);
        double funcValue = 0.0;

        if (!startPoint.isEmpty()) {
            // Разбиваем строку точки на координаты
            QStringList coords = startPoint.split(" ", Qt::SkipEmptyParts);
            if (coords.size() != dims) {
                throw std::runtime_error(
                    QString("Требуется %1 координат").arg(dims).toStdString()
                    );
            }

            std::vector<double> initPoint;
            for (const QString& coord : coords) {
                initPoint.push_back(coord.toDouble());
            }

            SolveFullKoefs(solverHandle, initPoint.data(), alpha, beta,
                           gamma, sigma, result.data(), &funcValue);
        } else {
            SolveWithKoefs(solverHandle, alpha, beta, gamma, sigma,
                           result.data(), &funcValue);
        }

        // Формирование результата
        QString resultStr;
        for (size_t i = 0; i < result.size() && result[i] != 0; ++i) {
            if (i > 0) resultStr += ", ";
            resultStr += QString("x%1 = %2").arg(i+1).arg(result[i]);
        }
        resultStr += QString("\nf(x) = %1").arg(funcValue);

        ui->outputLineEdit->setText(resultStr);
        addToHistory(inputText + " → " + resultStr);

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка: %1").arg(e.what()));
        cleanupSolver();
    }
}

void MainWindow::onParamsButtonClicked() {
    if (!paramsDialog) {
        paramsDialog = new ParametersDialog(this);
    }

    // Установка текущих значений
    paramsDialog->setAlpha(alpha);
    paramsDialog->setBeta(beta);
    paramsDialog->setGamma(gamma);
    paramsDialog->setSigma(sigma);
    paramsDialog->setStartPoint(startPoint);

    if (paramsDialog->exec() == QDialog::Accepted) {
        alpha = paramsDialog->getAlpha();
        beta = paramsDialog->getBeta();
        gamma = paramsDialog->getGamma();
        sigma = paramsDialog->getSigma();
        startPoint = paramsDialog->getStartPoint();
    }
}

void MainWindow::onShowGraphClicked() {
  if (!solverHandle) {
    QMessageBox::warning(this, "Ошибка", "Сначала выполните оптимизацию");
    return;
  }

  try {
    const int MAX_POINTS = 1000;
    double output[MAX_POINTS] = {0};

    // Передаём output и MAX_POINTS в функцию
    GetPointsForGraph(solverHandle, output, MAX_POINTS);

    // Собираем данные для графика (игнорируем нули)
    std::vector<double> history;
    for (int i = 0; i < MAX_POINTS && output[i] != 0; ++i) {
      history.push_back(output[i]);
    }

    if (history.empty()) {
      QMessageBox::information(
          this, "Информация",
          "Нет данных для графика. Возможно:\n"
          "1. Оптимизация сошлась за 1 итерацию\n"
          "2. История не записывалась (проверьте NelderMead.cpp)");
      return;
    }

    // Создаём и настраиваем окно графика
    GraphWindow* graphWindow = new GraphWindow();
    graphWindow->setAttribute(Qt::WA_DeleteOnClose);
    graphWindow->setHistoryData(history);
    graphWindow->setWindowTitle("График для: " + ui->inputLineEdit->text());
    graphWindow->show();

    needGraphUpdate = false;

  } catch (const std::exception& e) {
    QMessageBox::critical(
        this, "Ошибка",
        QString("Ошибка при построении графика:\n%1").arg(e.what()));
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
    QMessageBox::critical(
        this, "Ошибка",
        "Не удалось открыть блокнот.\nПопробуйте открыть файл вручную:\n" +
            logPath);
  }
}

// Слот для кнопки "Очистить"
void MainWindow::onClearButtonClicked() {
  ui->inputLineEdit->clear();   // Очищаем поле ввода
  ui->outputLineEdit->clear();  // Очищаем поле вывода
}

void MainWindow::addToHistory(const QString& text) {
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
void MainWindow::onHistoryItemClicked(QListWidgetItem* item) {
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

void MainWindow::onHelpButtonClicked() {
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
