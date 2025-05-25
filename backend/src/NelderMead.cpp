#include "../include/NelderMead.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Класс решателя Нелдера-Мида

NelderMead::NelderMead(string init_expression): expression(init_expression) {
  symplex = Symplex(expression, {0, 0, 0, 0});
};

X NelderMead::Solver(double tolerance = 0.000001, double alpha = 1,
                     double beta = 0.5, double gamma = 2, double sigma = 1){
  symplex = Symplex(expression, {alpha, beta, gamma, sigma});
  return solverProcessing(tolerance);
}
X NelderMead::Solver(vector<double> init_point, double tolerance = 0.000001,
                     double alpha = 1, double beta = 0.5, double gamma = 2,
                     double sigma = 1) {
  symplex = Symplex(init_point, expression, {alpha, beta, gamma, sigma});
  return solverProcessing(tolerance);
}
X NelderMead::solverProcessing(double tolerance = 0.000001) {
  clearHistory();
  symplex.sort();
  startLogFile();
  savePointsToLog(0);
  saveHistory();
  while (condition(tolerance)) {
    chooseBest();
    symplex.sort();
    saveHistory();
    savePointsToLog(history.size());
  }
  finishLog();
  return symplex.getTop();
}
bool NelderMead::condition(double tolerance) {
  if(history.size()>=100000) return false;
  if (abs(symplex.getTop().value - symplex.getBad().value) <= tolerance) return false;
  return true;
}
void NelderMead::chooseBest() {
  X centroid = symplex.computeCentroid();
  X reflected = symplex.reflection(centroid, symplex.getBad());
  X best = symplex.getTop();

  if (reflected < best) {
    X expanded = symplex.expansion(centroid, reflected);
    if (expanded < reflected) {
      symplex.removeBad(expanded);
    } else {
      symplex.removeBad(reflected);
    }
  } else if (reflected < symplex.getTop(1)) {
    symplex.removeBad(reflected);
  } else {
    X contracted = symplex.contraction(centroid, symplex.getBad());
    if (contracted < symplex.getBad()) {
      symplex.removeBad(contracted);
    } else {
      symplex.reduction();
    }
  }
}

int NelderMead::getDims() { return symplex.getDims(); }

void NelderMead::startLogFile() {
  outfile.open("logfile.txt", ios::out | ios::trunc);
  outfile << "-------Started method solving-------" << endl;
  outfile << "Calculated dims: " << symplex.getDims() << endl;
}
void NelderMead::finishLog() {
  outfile << "-------Finished solving-------" << endl;
  outfile << "Calculated solution:" << endl << "(";
  X best = symplex.getTop();
  for (int j = 0; j < best.coordinates.size(); j++) {
    outfile << best.coordinates[j] << "; ";
  }
  outfile << ") " << endl;
  outfile << "Value: " << best.value;
  outfile.close();
}
void NelderMead::savePointsToLog(int iteration_num) {
  outfile << iteration_num << " iteration" << endl;
  vector<X> symplex_points = symplex.getSymplex();
  for (auto i : symplex_points) {
    outfile << "    (";
    for (int j = 0; j < i.coordinates.size(); j++) {
      outfile << i.coordinates[j] << "; ";
    }
    outfile << ") ";
    outfile << "Calculated value: " << i.value << endl;
  }
}
void NelderMead::saveHistory() { history.push_back(symplex.getTop().value); }
vector<double> NelderMead::getHistory() {
  if (history.size() == 0)
    throw runtime_error("History is empty");
  else
    return history;
}
void NelderMead::clearHistory() { history.clear(); }

int NelderMead::getIterations() { return history.size(); }

// C-интерфейс для NelderMead
#ifdef __cplusplus
extern "C" {
#endif
struct NelderMeadHandle {
  NelderMead* solver;
  NelderMeadHandle(const char* expr) : solver(new NelderMead(expr)) {}
  ~NelderMeadHandle() { delete solver; }
};

NelderMeadHandle* CreateNelderMead(const char* expr) {
  return new NelderMeadHandle(expr);
}
int getDims(NelderMeadHandle* handle) { return handle->solver->getDims(); }
void SolveBasic(NelderMeadHandle* handle, double* output) {
  if (!handle || !output) return;
  X result = handle->solver->Solver();
  copy(result.coordinates.begin(), result.coordinates.end(), output);
}
void SolveWithValue(NelderMeadHandle* handle, double* output, double* value) {
  if (!handle || !output) return;
  X result = handle->solver->Solver();
  copy(result.coordinates.begin(), result.coordinates.end(), output);
  *value = result.value;
}
void SolveInit(NelderMeadHandle* handle, double* coordinates, double* output) {
  if (!handle || !output) return;
  vector<double> input;
  for (int i = 0; i < handle->solver->getDims(); i++) {
    input.push_back(coordinates[i]);
  }
  X result = handle->solver->Solver(input);
  copy(result.coordinates.begin(), result.coordinates.end(), output);
}
void SolveFull(NelderMeadHandle* handle, double* coordinates, double* output,
               double* value) {
  if (!handle || !output) return;
  vector<double> input;
  for (int i = 0; i < handle->solver->getDims(); i++) {
    input.push_back(coordinates[i]);
  }
  X result = handle->solver->Solver(input);
  copy(result.coordinates.begin(), result.coordinates.end(), output);
  *value = result.value;
}
void SolveWithKoefs(NelderMeadHandle* handle, double tolerance, double alpha,
                    double beta, double gamma, double sigma, double* output,
                    double* value) {
  if (!handle || !output) return;
  X result = handle->solver->Solver(tolerance, alpha, beta, gamma, sigma);
  copy(result.coordinates.begin(), result.coordinates.end(), output);
  *value = result.value;
}
void SolveFullKoefs(NelderMeadHandle* handle, double* coordinates,
                    double tolerance, double alpha, double beta, double gamma,
                    double sigma, double* output, double* value) {
  if (!handle || !output) return;
  vector<double> input;
  for (int i = 0; i < handle->solver->getDims(); i++) {
    input.push_back(coordinates[i]);
  }
  X result =
      handle->solver->Solver(input, tolerance, alpha, beta, gamma, sigma);
  copy(result.coordinates.begin(), result.coordinates.end(), output);
  *value = result.value;
}
void GetPointsForGraph(NelderMeadHandle* handle, double* output, int maxSize) {
  if (!handle || !output) return;

  vector<double> history = handle->solver->getHistory();
  copy(history.begin(), history.end(), output);
}
int getIterations(NelderMeadHandle* handle) {
  return handle->solver->getIterations();
}

void DestroyNelderMead(NelderMeadHandle* handle) { delete handle; }

#ifdef __cplusplus
}
#endif
