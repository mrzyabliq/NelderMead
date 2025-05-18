#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

// Предварительные объявления
namespace Ui {
class MainWindow;
}
class LogViewerDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSendButtonClicked();
    void onClearButtonClicked();
    void onHelpButtonClicked();
    void onHistoryItemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;

    void addToHistory(const QString &text);
};

#endif // MAINWINDOW_H
