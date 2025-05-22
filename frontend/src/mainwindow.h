#pragma once

#include <QListWidgetItem>
#include <QMainWindow>
#include <QMessageBox>
#include <QRegularExpression>
#include <memory>
#include <vector>

#include "NelderMead.h"
#include "graphwindow.h"
#include "parametersdialog.h"
#include "ui_mainwindow.h"

extern "C" {
typedef struct NelderMeadHandle NelderMeadHandle;
NELDERMEAD_API NelderMeadHandle* CreateNelderMead(const char* expr);
NELDERMEAD_API void SolveBasic(NelderMeadHandle* handle, double* output);
NELDERMEAD_API void SolveWithValue(NelderMeadHandle* handle, double* output,
                                   double* value);
NELDERMEAD_API void SolveInit(NelderMeadHandle* handle, double* coordinates,
                              double* output);
NELDERMEAD_API void SolveWithKoefs(NelderMeadHandle* handle, double alpha, double beta,
                                   double gamma, double sigma, double* output, double* value);
NELDERMEAD_API void SolveFull(NelderMeadHandle* handle, double* coordinates,
                              double* output, double* value);
NELDERMEAD_API void SolveFullKoefs(NelderMeadHandle* handle, double* coordinates, double alpha,
                                   double beta, double gamma, double sigma, double* output,
                                   double* value);
NELDERMEAD_API void DestroyNelderMead(NelderMeadHandle* handle);
NELDERMEAD_API void GetPointsForGraph(NelderMeadHandle* handle, double* output,
                                      int maxSize);
NELDERMEAD_API int getDims(NelderMeadHandle* handle);
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
  void onParamsButtonClicked();

 private:
  Ui::MainWindow* ui;
  NelderMeadHandle* solverHandle;
  bool needGraphUpdate;
  void addToHistory(const QString& text);
  void cleanupSolver();

  ParametersDialog *paramsDialog = nullptr;
  // Параметры по умолчанию
  double alpha = 1.0;
  double beta = 0.5;
  double gamma = 2.0;
  double sigma = 1.0;
  double tolerance = 0.000001;
  QString startPoint;
};
