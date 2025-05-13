#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onSendButtonClicked();  // Слот для кнопки "Отправить"
    void onClearButtonClicked(); // Слот для кнопки "Очистить"
    void onHistoryItemClicked(QListWidgetItem *item); // Слот для выбора элемента из списка
    void onHelpButtonClicked(); // Слот для кнопки "Справка"
private:
    Ui::MainWindow *ui;
    void addToHistory(const QString &text); // Функция для логирования
};
#endif // MAINWINDOW_H
