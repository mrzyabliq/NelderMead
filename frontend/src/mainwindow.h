#pragma once

#include <QListWidgetItem>
#include <QMainWindow>
#include <QMessageBox>
#include <QRegularExpression>
#include <memory>
#include <vector>

#include "NelderMead.h"
#include "graphwindow.h"
#include "ui_mainwindow.h"

extern "C" {
typedef struct NelderMeadHandle NelderMeadHandle;
NELDERMEAD_API NelderMeadHandle* CreateNelderMead(const char* expr);
NELDERMEAD_API void SolveBasic(NelderMeadHandle* handle, double* output);
NELDERMEAD_API void SolveWithValue(NelderMeadHandle* handle, double* output,
                                   double* value);
NELDERMEAD_API void DestroyNelderMead(NelderMeadHandle* handle);
NELDERMEAD_API void GetPointsForGraph(NelderMeadHandle* handle, double* output,
                                      int maxSize);
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  void onSendButtonClicked();
  void onClearButtonClicked();
  void onHelpButtonClicked();
  void onShowLogsClicked();
  void onShowGraphClicked();
  void onHistoryItemClicked(QListWidgetItem* item);

 private:
  Ui::MainWindow* ui;
  NelderMeadHandle* solverHandle;
  bool needGraphUpdate;
  void addToHistory(const QString& text);
  void cleanupSolver();
};
